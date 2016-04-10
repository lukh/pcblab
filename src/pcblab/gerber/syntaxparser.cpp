#include "syntaxparser.h"


SyntaxParser::SyntaxParser(){
    resetPointsAttributes();
}


bool SyntaxParser::parse(istream &inStream){
    char read;
    bool status = true, eof_cmd = false;

    while((read = inStream.get()) != EOF){
        switch(read){
            case ' ': //pass...
            case '\r':
            case '\n':
                break;

            case 'M': // end of file ?
                if(getOpCode(inStream) == 2 and inStream.get() == '*'){
                    //this is the end of the file, what should we do ???
                    eof_cmd = true;
                    d_printf("SYNTAXPARSER > EndOfFile(M02)");
                }
                break;

            case '%':
                d_printf("SYNTAXPARSER > XCode");
                parseXCode(inStream);
                break;

            case '*': //end command, should not append since it's handle in the D/G command
                break;

            case 'D': //DCode
                d_printf("SYNTAXPARSER > DCode:");
                parseDCode(inStream);
                resetPointsAttributes();

                break;

            case 'G': //GCode
                d_printf("SYNTAXPARSER > GCode");
                parseGCode(inStream);
                resetPointsAttributes();
                break;

            case 'X':
                d_printf("SYNTAXPARSER > X");
                mXY.mIsXOmitted = false;
                mXY.mX = convertCoordinate(getRawCoord(inStream));
                break;
            case 'Y':
                d_printf("SYNTAXPARSER > Y");
                mXY.mIsYOmitted = false;
                mXY.mY = convertCoordinate(getRawCoord(inStream));
                break;
            case 'I':
                d_printf("SYNTAXPARSER > I");
                mIJ.mIsXOmitted = false;
                mIJ.mX = convertCoordinate(getRawCoord(inStream));
                break;
            case 'J':
                d_printf("SYNTAXPARSER > J");
                mIJ.mIsYOmitted = false;
                mIJ.mY = convertCoordinate(getRawCoord(inStream));

            default:
                break;
        }
    }

    return eof_cmd && status;
}


bool SyntaxParser::parseDCode(istream &inStream){
    int op_code = getOpCode(inStream);

    //we should reach a * here
    if(inStream.get() != '*'){
       err_printf("ERROR (SyntaxParser::parseDCode): No * at the end of the DCode" );
        return false;
    }

    switch(op_code){
        case 00:
           err_printf("ERROR (SyntaxParser::parseDCode): D00 is invalid" );
            break;

        case 01:
            d_printf("    SyntaxParser(DCode) > Interpolate(D01)");
            interpolate(mXY, mIJ);
            break;

        case 02:
            d_printf("    SyntaxParser(DCode) > Move(D02)");
            move(mXY);
            break;
        case 03:
            d_printf("    SyntaxParser(DCode) > Flash(D03)");
            flash(mXY);
            break;

        default:
            d_printf("    SyntaxParser(DCode) > Change Aperture(Dnn)");
            if(op_code >= 10){
                setCurrentAperture(op_code);
            }
            else{
               err_printf("ERROR (SyntaxParser::parseDCode): DCode out of bounds" );
                return false;
            }
            break;
    }

    return true;
}


bool SyntaxParser::parseGCode(istream &inStream){
    return true;
}



bool SyntaxParser::parseXCode(istream &inStream){
    char ch1, ch2, ch;
    eXCode code;
    GraphicState::CoordinateFormat format;
    GraphicState::eUnit unit;
    GraphicState::eLevelPolarity polarity;

    while ((ch1 = inStream.get()) != EOF){
        ch2 = inStream.peek();
        if(ch2 == EOF){
            break;
        }

        if(ch1 == '%' || ch2 == '%'){
            break;
        }

        code = getXCode(ch1, ch2);

        // a correct xcode cmd is received, we can pass the second char
        if(code != eXCodeNone){
            inStream.get();
        }

        switch(code){
            case eXCodeNone:
                err_printf("ERROR(ParseXCode): Impossible to find Extended code" );
                break; // it is necessary to continue to scan the Extended block

            case eXCodeFs:
                d_printf("    SyntaxParser(XCode) > CoordinateFormat(FS)");
                // here we have to read Trailing/Leading zero omission char + Absolute/Incremental char + "XnmYnm" + *
                //T/L
                switch(inStream.get()){
                    case 'T':
                        err_printf("ERROR(ParseXCode): Trailing zero omission is deprecated" );
                        break;
                    case 'L':
                        d_printf("        Leading zeros: ok");
                        break;
                    default:
                        err_printf("ERROR(ParseXCode): Wrong char in FS cmd (expected L or T)" );
                        return false;
                }

                //A/I
                switch(inStream.get()){
                    case 'I':
                       err_printf("ERROR(ParseXCode): Incremental coord is deprecated" );
                        break;
                    case 'A':
                        d_printf("        Absolute coords: ok");
                        break;
                    default:
                        err_printf("ERROR(ParseXCode): Wrong char in FS cmd (expected A or I)" );
                        return false;
                }

                //X ?
                if(inStream.get() != 'X'){
                    err_printf("ERROR(ParseXCode): Wrong char in FS cmd (expected X)" );
                    return false;
                }

                //int
                ch = inStream.get();
                if(isNumber(ch)){
                    format.mIntegers = charToNum(ch);

                    d_printf("        INT_X = " + to_string(format.mIntegers));
                } else {err_printf("ERROR(ParseXCode): Wrong char in FS cmd (Expected X Int num)" ); return false; }
                //dec
                ch = inStream.get();
                if(isNumber(ch)){
                    format.mDecimals = charToNum(ch);

                    d_printf("        DEC_X = " + to_string(format.mDecimals));
                } else {err_printf("ERROR(ParseXCode): Wrong char in FS cmd (Expected X Dec num)" ); return false; }


                //Y
                if(inStream.get() != 'Y'){
                    err_printf("ERROR(ParseXCode): Wrong char in FS cmd (expected Y)" );
                    return false;
                }

                ch = inStream.get();
                if(isNumber(ch)){
                    if(format.mIntegers != charToNum(ch)){
                       err_printf("ERROR(ParseXCode): Wrong char in FS cmd (Expected XInt FS must be the same that YInt FS)" ); return false;
                    }
                } else {err_printf("ERROR(ParseXCode): Wrong char in FS cmd (Expected Y Int num)" ); return false; }
                //dec
                ch = inStream.get();
                if(isNumber(ch)){
                    if(format.mDecimals != charToNum(ch)){
                       err_printf("ERROR(ParseXCode): Wrong char in FS cmd (Expected XDec FS must be the same that YDec FS)" ); return false;
                    }
                } else {err_printf("ERROR(ParseXCode): Wrong char in FS cmd (Expected Y Dec num)" ); return false; }

                // end of command *
                if(inStream.get() != '*'){
                    return false;
                }

                //FS Command is valid
                d_printf("        setCoordinatesFormat");
                setCoordinateFormat(format);

                break;

            case eXCodeMo:
                d_printf("    SyntaxParser(XCode) > UnitSelection(MM)");
                ch1 = inStream.get();
                ch2 = inStream.get();

                if(ch1 == 'M' && ch2 == 'M'){
                    unit = GraphicState::eUnitMm;
                }

                else if(ch1 == 'I' && ch2 == 'N'){
                    unit = GraphicState::eUnitInch;
                }

                else{
                    err_printf("ERROR(ParseXCode): Unrecognized unit");
                    return false;
                }

                // end of command *
                if(inStream.get() != '*'){
                    return false;
                }

                d_printf("        setUnit");
                setUnit(unit);

                break;


            case eXCodeLp:
                d_printf("    SyntaxParser(XCode) > Add new level (LP)");
                ch = inStream.get();
                switch(ch){
                    case 'C':
                        polarity = GraphicState::ePolClear;
                        break;

                    case 'D':
                        polarity = GraphicState::ePolClear;
                        break;

                    default:
                        err_printf("ERROR(ParseXCode): Unrecognized polarity");
                        return false;
                }

                // end of command *
                if(inStream.get() != '*'){
                    return false;
                }

                break;

                d_printf("        setAddNewLevel");
                addNewLevel(polarity);

            default:
                //let's ignore the full unknown command
                d_printf("WARNING: unhandled XCmd:" + string(1, ch1) + string(1, ch2));
                do{
                    ch = inStream.get();
                }while(ch != '*' && ch != EOF);
                break;
        }
    }

    return true;
}




uint8_t SyntaxParser::getOpCode(istream &inStream){
    string str;
    char read;

    uint8_t val;

    while((read = inStream.peek()) != EOF){
        if(read < '0' || read > '9')
            break;

        //extract the char
        inStream.get();

        str.push_back(read);
    }

    try{
        val = std::stoi(str);
    }
    catch(...){
        val=0;
        err_printf("ERROR (SyntaxParser::getOpCode): Couldn't convert string to int" );
    }

    return val;
}



SyntaxParser::eXCode SyntaxParser::getXCode(char ch1, char ch2){
    if(ch1=='F' && ch2 == 'S'){
        return eXCodeFs;
    }
    else if(ch1 == 'M' && ch2 == 'O'){
        return eXCodeMo;
    }
    else if(ch1 == 'A' && ch2 == 'D'){
        return eXCodeAd;
    }
    else if(ch1 == 'A' && ch2 == 'M'){
        return eXCodeAm;
    }
    else if(ch1 == 'S' && ch2 == 'R'){
        return eXCodeLp;
    }
    else if(ch1 == 'L' && ch2 == 'P'){
        return eXCodeLp;
    }
    else if(ch1 == 'T' && ch2 == 'F'){
        return eXCodeTf;
    }
    else if(ch1 == 'T' && ch2 == 'A'){
        return eXCodeTa;
    }
    else if(ch1 == 'T' && ch2 == 'D'){
        return eXCodeTd;
    }
    else{
        return eXCodeNone;
    }
}

uint32_t SyntaxParser::getRawCoord(istream &inStream){
    string str;
    char read;

    uint32_t val;

    while((read = inStream.peek()) != EOF){
        if(read < '0' || read > '9')
            break;

        //extract the char
        inStream.get();

        str.push_back(read);
    }

    try{
        val = std::stoi(str);
    }
    catch(...){
        val=0;
       err_printf("ERROR (SyntaxParser::getRawCoord): Couldn't convert string to int" );
    }

    return val;
}
