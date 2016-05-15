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
                if(getOpCode(inStream) == 2 && inStream.get() == '*'){
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
