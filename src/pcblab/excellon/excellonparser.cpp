#include "excellonparser.h"
#include "tools/stringsplitter.h"

ExcellonParser::ExcellonParser()
{

}

bool ExcellonParser::parse(istream &inStream)
{
    char read;
    bool is_eof = false;


    while((read = inStream.peek()) != EOF && !is_eof){
        switch(read){
            case ' ': //pass...
            case '\r':
            case '\n':
                inStream.get(); // consume
                break;


            case 'M':
                if(!parseMCode(inStream, is_eof)){
                    return false;
                }
                break;

            case 'G':
                break;

            case 'I': {//ICI,ON or INCHES
                string line = getLine(inStream);

                if(line.find("ICI") == 0){
                    vector<string> splitted;
                    string delim(",");
                    tokenize(line, splitted, delim);
                    if(splitted.size() != 2){ return false;}
                    if(splitted[1].compare("ON") == 0){
                        setCoordMode(ExcellonState::eCoordModeIncremental);
                    }
                    else if(splitted[1].compare("OFF") == 0){
                        setCoordMode(ExcellonState::eCoordModeAbsolute);
                    }
                    else { return false; }

                }
                else if(line.find("INCH") == 0){
                    setUnit(ExcellonState::eUnitInch);
                    vector<string> splitted;
                    string delim(",");
                    tokenize(line, splitted, delim);
                    if(splitted.size() == 2){
                        if(splitted[1].compare("LZ") == 0){
                            setNumberFormat(ExcellonState::eNumForLeadingZeros);
                        }
                        else if(splitted[1].compare("TZ") == 0){
                            setNumberFormat(ExcellonState::eNumForTrailingZeros);
                        }
                        else { return false; }
                    }
                }
                break;
            }


            case '%':
                setSection(ExcellonState::eSectionBody);
                inStream.get();
                break;

            // unrecognized for now
            case 'R':
                err_printf("ExcellonParser::parse: (WARNING) code R not implemented");
                // no break

            default:
                //eat line
                eatLine(inStream);
                break;
        }
    }

    return true;
}

string ExcellonParser::getLine(istream &inStream)
{
    char read = 0;
    string ret;

    while((read = inStream.get()) != EOF){
        if(read == '\n' || read == '\r'){
            break;
        }
        ret.append(&read, 1);
    }
    return ret;
}

void ExcellonParser::eatLine(istream &inStream)
{
    char read = 0;

    while((read = inStream.get()) != EOF){
        if(read == '\n' || read == '\r'){
            break;
        }
    }
    return;
}

bool ExcellonParser::parseMCode(istream &inStream, bool &outEndOfProgram)
{
    // to get the char and be sure it is the right code
    if(inStream.get() != 'M'){
        return false;
    }

    string line = getLine(inStream);


    if(line.find("ETRIC") == 0){
        setUnit(ExcellonState::eUnitMetric);
        vector<string> splitted;
        string delim(",");
        tokenize(line, splitted, delim);
        if(splitted.size() == 2){
            if(splitted[1].compare("LZ") == 0){
                setNumberFormat(ExcellonState::eNumForLeadingZeros);
            }
            else if(splitted[1].compare("TZ") == 0){
                setNumberFormat(ExcellonState::eNumForTrailingZeros);
            }
            else { return false; }
        }
    }

    else{
        stringstream ssline(line);
        uint8_t code = getInteger(ssline);

        switch(code){
            /// header code
            case 48:
                setSection(ExcellonState::eSectionHeader);
                break;

            // begin of body
            case 95:
                setSection(ExcellonState::eSectionBody);
                break;


            // end rewind/ end
            case 30:
            case 00:
                outEndOfProgram = true;
                break;



            case 71:
                setUnit(ExcellonState::eUnitMetric);
                break;
            case 72:
                setUnit(ExcellonState::eUnitInch);
                break;


            default:
                err_printf("ExcellonParser::parseMCode: (WARNING): M code not implemented");
                break;
        }
    }

    return true;
}

bool ExcellonParser::parseGCode(istream &inStream)
{
    // to get the char and be sure it is the right code
    if(inStream.get() != 'M'){
        return false;
    }

    uint8_t code = getInteger(inStream);

    switch(code){
        case 90:
            setCoordMode(ExcellonState::eCoordModeAbsolute);
            break;
        case 91:
            setCoordMode(ExcellonState::eCoordModeIncremental);
            break;

        case 93:
            err_printf("ExcellonParser::parseGCode: (WARNING): G code 93 (zero set) not implemented");
            break;

        default:
            err_printf("ExcellonParser::parseGCode: (WARNING): G code not implemented");
            break;
    }

    return true;
}

bool ExcellonParser::parseTCode(istream &inStream)
{
    return true;
}

bool ExcellonParser::parseCoordinates(istream &inStream)
{
    return true;
}


uint8_t ExcellonParser::getInteger(istream &inStream){
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
        err_printf("ERROR (ExcellonParser::getOpCode): Couldn't convert string to int" );
    }

    return val;
}
