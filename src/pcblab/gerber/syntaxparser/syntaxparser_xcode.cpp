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
                d_printf("SyntaxParser(XCode) > CoordinateFormat(FS)", 2, 1);
                if(!parseXCode_FS(inStream)){
                    return false;
                }

                break;

            case eXCodeMo:
                d_printf("SyntaxParser(XCode) > UnitSelection(MM)", 2, 1);
                if(!parseXCode_MO(inStream)){
                    return false;
                }

                break;


            case eXCodeLp:
                d_printf("SyntaxParser(XCode) > Add new level (LP)", 2, 1);
                if(!parseXCode_LP(inStream)){
                    return false;
                }
                break;


            case eXCodeAd:
                d_printf("SyntaxParser(XCode) > Add Aperture (AD)", 2, 1);
                if(!parseXCode_AD(inStream)){
                    return false;
                }
                break;


            case eXCodeAm:
                d_printf("SyntaxParser(XCode) > Add ApertureTemplate (AM)", 2, 1);
                if(!parseXCode_AM(inStream)){
                    return false;
                }
                break;


            default:
                //let's ignore the full unknown command
                d_printf("WARNING: unhandled XCmd:" + string(1, ch1) + string(1, ch2), 1, 1);
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
            d_printf("Leading zeros: ok", 2, 2);
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
            d_printf("Absolute coords: ok", 2, 2);
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

        d_printf("INT_X = " + to_string(format.mIntegers), 2, 2);
    } else {err_printf("ERROR(ParseXCode): Wrong char in FS cmd (Expected X Int num)" ); return false; }
    //dec
    ch = inStream.get();
    if(isNumber(ch)){
        format.mDecimals = charToNum(ch);

        d_printf("DEC_X = " + to_string(format.mDecimals), 2, 2);
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

    d_printf("addNewLevel", 2, 2);
    addNewLevel(polarity);
    return true;
}



bool SyntaxParser::parseXCode_AD(istream &inStream){
    uint32_t dcode;
    string dcode_text, name;
    char ch;

    vector<ApertureModifier> modifiers;

    // get rid of the D.
    if(inStream.get() != 'D'){
        err_printf("ERROR(ParseXCode): AD cmd: expected 'D'");
        return false;
    }

    //extract DCode
    dcode_text.clear();
    while((ch = inStream.peek()) != EOF){
        if(ch == '*'){
            err_printf("ERROR(ParseXCode): AD cmd: reach unexpected end of cmd");
            return false;
        }

        if(isNumber(ch)){
            dcode_text.push_back(inStream.get());
        }
        else{
            break;
        }
    }
    try{
        dcode = stoi(dcode_text);
    }
    catch(...){
        err_printf("ERROR (SyntaxParser::parseXCode_AD): DCode: Couldn't convert string to int" );
        return false;
    }




    // extract the name
    while((ch = inStream.get()) != EOF){
        if(ch == '*'){
            d_printf("addAperture: (" + name + ") D" + to_string(dcode), 2, 2);
            addAperture(dcode, name, modifiers); //does it have any sense ???
            return true;
        }
        else if(ch == ','){
            d_printf("addAperture: (" + name + ") D" + to_string(dcode), 2, 2);
            break;
        }
        else{
            name.push_back(ch);
        }
    }

    //extract params
    bool status = true, local_status;

    while(extractApertureModifiers(inStream, modifiers, local_status)){
        status = status && local_status;
    }

    addAperture(dcode, name, modifiers);

    return status;
}



bool SyntaxParser::extractApertureModifiers(istream &inStream, vector<ApertureModifier> &outModifiers, bool &outStatus){
    char ch;
    string param_str;

    outStatus = true;

    double mod;

    while((ch = inStream.get()) != EOF){
        bool not_done = true;

        // we should get one param
        switch(ch){
            case '*':
                not_done = false;
            case 'X':
                if(param_str.size() == 0){
                    return false;
                }

                try{
                    mod = stringToDouble(param_str);
                    outModifiers.push_back(mod);
                }
                catch(...){
                    err_printf("ERROR: extractApertureModifier: Impossible to convert to double");
                    outStatus = false;
                    return false;
                };
                d_printf("addApertureModifier: (" + to_string(mod) + ")", 2, 3);

                return not_done;

                break;

            default:
                //filling the string
                param_str.push_back(ch);

        }
    }

    outStatus = false;
    return false;
}




/// parses AM cmd
bool SyntaxParser::parseXCode_AM(istream &inStream){
    char read;
    string name;
    string content; //temp for current content

    vector<string> v_content;

    //extract name
    while((read = inStream.get()) != EOF){
        if(read == '*'){
            break;
        }
        name.push_back(read);
    }

    d_printf("adding: ApertureTemplate(" + name + ")", 2, 2);

    //extract content
    bool iscmt=false, newcmd = true;
    while((read = inStream.get()) != EOF){
        //just to handle spaces in comments
        if(read == '\n' || read == '\r'){
            continue;
        }
        if(newcmd && read == '0'){
            iscmt = true;
        }
        newcmd = false;

        if(!iscmt && read==' '){
            continue;
        }

        // start parsing...
        switch(read){
            case '\r':
            case '\n':
                break;

            case '%':
                d_printf("defineApertureTemplate(" + name + ")", 2, 2);
                defineApertureTemplate(name, v_content);
                return true;

            case '*':
                if(!extractAM_Content(content, v_content)){
                    err_printf("ERROR(parseXCode_AM): unrecognize command");
                }
                content.clear();
                newcmd=true;
                iscmt=false;
                break;

            default:
                content.push_back(read);
                break;
        }

    }

    return false;
}



bool SyntaxParser::extractAM_Content(string &inContent, vector<string> &outContent){
    //TODO validate the content
    d_printf("addAMCmd: (" + inContent + ")", 2, 3);

    if(inContent.at(0) != '0'){ //handles comments
        outContent.push_back(inContent);
    }

    return true;
}




