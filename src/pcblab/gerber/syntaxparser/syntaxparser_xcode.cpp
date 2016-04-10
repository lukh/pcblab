#include "../syntaxparser.h"

bool SyntaxParser::parseXCode(istream &inStream){
    char ch1, ch2, ch;
    eXCode code;

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
                if(!parseXCode_FS(inStream)){
                    return false;
                }

                break;

            case eXCodeMo:
                d_printf("    SyntaxParser(XCode) > UnitSelection(MM)");
                if(!parseXCode_MO(inStream)){
                    return false;
                }

                break;


            case eXCodeLp:
                d_printf("    SyntaxParser(XCode) > Add new level (LP)");
                if(!parseXCode_LP(inStream)){
                    return false;
                }
                break;


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



bool SyntaxParser::parseXCode_FS(istream &inStream){
    GraphicState::CoordinateFormat format;
    char ch;

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

    return true;
}

bool SyntaxParser::parseXCode_MO(istream &inStream){
    char ch1, ch2;
    GraphicState::eUnit unit;

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

    return true;
}

bool SyntaxParser::parseXCode_LP(istream &inStream){
    char ch;
    GraphicState::eLevelPolarity polarity;

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

    d_printf("        addNewLevel");
    addNewLevel(polarity);
    return true;
}
