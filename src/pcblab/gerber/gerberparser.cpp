#include "gerberparser.h"


GerberParser::GerberParser(){
    resetPointsAttributes();
}


bool GerberParser::parse(istream &inStream){
    char read;
    bool status = true, eof_cmd = false;

    while((read = inStream.get()) != EOF){
        switch(read){
            case ' ': //pass...
            case '\r':
            case '\n':
                break;

            case 'M': // end of file ?
                if(getOpCode(inStream) == 2 && inStream.get() == '*'){
                    //this is the end of the file, what should we do ???
                    eof_cmd = true;
#ifdef DEBUG_PRINT
                    d_printf("SYNTAXPARSER > EndOfFile(M02)");
#endif
                }
                break;

            case '%':
#ifdef DEBUG_PRINT
                d_printf("SYNTAXPARSER > XCode");
#endif
                parseXCode(inStream);
                break;

            case '*': //end command, should not append since it's handle in the D/G command
                break;

            case 'D': //DCode
#ifdef DEBUG_PRINT
                d_printf("SYNTAXPARSER > DCode:");
#endif
                parseDCode(inStream);
                resetPointsAttributes();

                break;

            case 'G': //GCode
#ifdef DEBUG_PRINT
                d_printf("SYNTAXPARSER > GCode");
#endif
                parseGCode(inStream);
                resetPointsAttributes();
                break;

            case 'X':
#ifdef DEBUG_PRINT
                d_printf("SYNTAXPARSER > X");
#endif
                mXY.mIsXOmitted = false;
                mXY.mX = convertCoordinate(getRawCoord(inStream));
                break;
            case 'Y':
#ifdef DEBUG_PRINT
                d_printf("SYNTAXPARSER > Y");
#endif
                mXY.mIsYOmitted = false;
                mXY.mY = convertCoordinate(getRawCoord(inStream));
                break;
            case 'I':
#ifdef DEBUG_PRINT
                d_printf("SYNTAXPARSER > I");
#endif
                mIJ.mIsXOmitted = false;
                mIJ.mX = convertCoordinate(getRawCoord(inStream));
                break;
            case 'J':
#ifdef DEBUG_PRINT
                d_printf("SYNTAXPARSER > J");
#endif
                mIJ.mIsYOmitted = false;
                mIJ.mY = convertCoordinate(getRawCoord(inStream));

            default:
                break;
        }
    }

    return eof_cmd && status;
}





uint8_t GerberParser::getOpCode(istream &inStream){
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
        val = stringToInt(str);
    }
    catch(...){
        val=0;
        err_printf("ERROR (SyntaxParser::getOpCode): Couldn't convert string to int" );
    }

    return val;
}



GerberParser::eXCode GerberParser::getXCode(char ch1, char ch2){
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

int32_t GerberParser::getRawCoord(istream &inStream){
    string str;
    char read;
    bool neg = false;
    int32_t val;

    while((read = inStream.peek()) != EOF){
        if(read == '-'){
            neg=true;
            //extract the char
            inStream.get();
            continue;
        }

        if((read < '0' || read > '9'))
            break;

        //extract the char
        inStream.get();

        str.push_back(read);
    }

    try{
        val = stringToInt(str);
    }
    catch(...){
        val=0;
        err_printf("ERROR (SyntaxParser::getRawCoord): Couldn't convert string to int" );
    }

    return neg ? -val : val;
}
