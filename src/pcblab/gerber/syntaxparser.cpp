#include "syntaxparser.h"


SyntaxParser::SyntaxParser(){
    resetPointsAttributes();
}


void SyntaxParser::parse(istream &inStream){
    char read;

    while((read = inStream.get()) != EOF){
        switch(read){
            case ' ': //pass...
            case '\r':
            case '\n':
                break;

            case '*': //end command, should not append since it's handle in the D/G command
                break;

            case 'D': //DCode
                parseDCode(inStream);
                resetPointsAttributes();

                break;

            case 'G': //GCode
                parseGCode(inStream);
                resetPointsAttributes();
                break;

            case 'X':
                mXY.mIsXOmitted = false;
                mXY.mX = convertCoordinate(getRawCoord(inStream));
                break;
            case 'Y':
                mXY.mIsYOmitted = false;
                mXY.mY = convertCoordinate(getRawCoord(inStream));
                break;
            case 'I':
                mIJ.mIsXOmitted = false;
                mIJ.mX = convertCoordinate(getRawCoord(inStream));
                break;
            case 'J':
                mIJ.mIsYOmitted = false;
                mIJ.mY = convertCoordinate(getRawCoord(inStream));
        }
    }
}


void SyntaxParser::parseDCode(istream &inStream){
    int op_code = getOpCode(inStream);

    //we should reach a * here
    if(inStream.get() != '*'){
        cerr << "ERROR (SyntaxParser::parseDCode): No * at the end of the DCode" << endl;
        return;
    }

    switch(op_code){
        case 00:
            cerr << "ERROR (SyntaxParser::parseDCode): D00 is invalide" << endl;
            break;

        case 01:
            interpolate(mXY, mIJ);
            break;

        case 02:
            move(mXY);
            break;
    case ' ':
                case '\r':
                case '\n':
        case 03:
            flash(mXY);
            break;

        default:
            if(op_code >= 10){
                setCurrentAperture(op_code);
            }
            else{
                cerr << "ERROR (SyntaxParser::parseDCode): DCode out of bounds" << endl;
            }
            break;
    }
}


void SyntaxParser::parseGCode(istream &inStream){

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
        cerr << "ERROR (SyntaxParser::getOpCode): Couldn't convert string to int" << endl;
    }

    return val;
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
        cerr << "ERROR (SyntaxParser::getRawCoord): Couldn't convert string to int" << endl;
    }

    return val;
}
