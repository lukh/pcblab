#include "excellonparser.h"
#include "tools/stringoperations.h"

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
                if(!parseGCode(inStream)){
                    return false;
                }
                break;

            case 'T':
                if(!parseTCode(inStream)){
                    return false;
                }
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



            case 'X': //no breaks
            case 'Y':{
                if(!parseCoordinates(inStream)){
                    return false;
                }
                break;}



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
    bool status = true;
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
                status = false;
        }
    }

    return status;
}

bool ExcellonParser::parseGCode(istream &inStream)
{
    bool status = true;
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
            status = false;
            break;

        default:
            err_printf("ExcellonParser::parseGCode: (WARNING): G code not implemented");
            status = false;
            break;
    }

    return status;
}

bool ExcellonParser::parseTCode(istream &inStream)
{
    bool isToolChange = true; //should be useless in most of the cases, because there are no params after a tool change
    bool status = true;
    uint8_t tool_idx = 0xFF;

    double diameter = 0;
    double infeed_rate = 0, spindle_rate = 0;

    // to get the char and be sure it is the right code
    if(inStream.get() != 'T'){
        err_printf("ERROR(ExcellonParser::parseTCode): wrong first char, expecting T");
        return false;
    }

    string line = getLine(inStream);
    stringstream ssline(line);

    // tool idx
    tool_idx = getInteger(ssline);

    char read = 0;

    while((read = ssline.get()) != EOF){
        switch(read){
            // diameter
            case 'C':{
                string str_num;
                while((ssline.peek() >= '0' && ssline.peek() <= '9') || ssline.peek() == '.' || ssline.peek() == ','){
                    str_num.push_back(ssline.get());
                }
                try{
                    diameter = stringToDouble(str_num);
                }
                catch(...){
                    err_printf("ERROR(ExcellonParser::parseTCode): Impossible to convert the tool diameter");
                    status = false;
                }

                isToolChange = false;

                break;}

            // infeed rate
            case 'F':{
                // here, leading zero on 3 digits
                string str_num;
                while(ssline.peek() >= '0' && ssline.peek() <= '9'){
                    str_num.push_back(ssline.get()); // fill the str
                }
                //complete with zeros
                for(uint8_t i = 0; i < (3-str_num.size()); i ++){ str_num.push_back('0'); }

                try{
                    infeed_rate = stringToDouble(str_num);
                }
                catch(...){
                    err_printf("ERROR(ExcellonParser::parseTCode): Impossible to convert the tool infeed rate");
                    status = false;
                }

                isToolChange = false;

                break;}
            // spindle rate
            case 'S':{
                string str_num;
                while((ssline.peek() >= '0' && ssline.peek() <= '9') || ssline.peek() == '.' || ssline.peek() == ','){
                    str_num.push_back(ssline.get());
                }
                try{
                    spindle_rate = stringToDouble(str_num) * 1000.0;
                }
                catch(...){
                    err_printf("ERROR(ExcellonParser::parseTCode): Impossible to convert the tool spindle rate");
                    status = false;
                }

                isToolChange = false;

                break;}
            default:
                break;
        }
        if(! status) { break; }
    }

    if(status){
        if(isToolChange){
            setCurrentTool(tool_idx);
        }
        else{
            addTool(tool_idx, diameter, infeed_rate, spindle_rate);
        }
    }

    return status;
}

bool ExcellonParser::parseCoordinates(istream &inStream)
{
    char read = 0;
    string xy[2];
    uint8_t idx = 0;
    bool active = false;

    string line = getLine(inStream);
    stringstream ssline(line);


    while((read = ssline.get()) != EOF){
        switch(read){
            case 'X':
                active = true;
                idx = 0;
                break;
            case 'Y':
                active = true;
                idx = 1;
                break;

            default:
                if(active && read >= '0' && read <= '9'){
                    xy[idx].push_back(read);
                }
                else{
                    return false;
                }
                break;
        }
    }

    addHole(xy[0], xy[1]);
    return true;
}


uint8_t ExcellonParser::getInteger(istream &inStream) const{
    string str;
    char read;

    uint8_t val = 0;

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
        err_printf("ERROR(ExcellonParser::getInteger): Impossible to convert the string");
    }


    return val;
}


