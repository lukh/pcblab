#include "netlistparser.h"



const NetlistParser::Column NetlistParser::sColumnsDescription[] = {
    NetlistParser::Column(0,0),      //eColEntry
    NetlistParser::Column(1,1),      //eColType
    NetlistParser::Column(3,16),     //eColNetName
    NetlistParser::Column(20,25),    //eColRefDesID
    NetlistParser::Column(27,30),    //eColRefDesAlpha
    NetlistParser::Column(31,31),    //eColRefDesM
    NetlistParser::Column(32,32),    //eColIsDrilled
    NetlistParser::Column(33,36),    //eColDiameter
    NetlistParser::Column(37,37),    //eColPlated
    NetlistParser::Column(38,40),    //eColAccessSide
    NetlistParser::Column(42,42),    //eColSignX
    NetlistParser::Column(43,48),    //eColCoordsX
    NetlistParser::Column(50,50),    //eColSignY
    NetlistParser::Column(51,56),    //eColCoordY
    NetlistParser::Column(57,57),    //eColHasDataX
    NetlistParser::Column(58,61),    //eColRectDataX
    NetlistParser::Column(62,62),    //eColHasData
    NetlistParser::Column(63,66),    //eColRectDataY
    NetlistParser::Column(67,67),    //eColRectDataIsCCW
    NetlistParser::Column(68,70),    //eColRectDataRot
    NetlistParser::Column(72,73)     //eColSolderMask
};


NetlistParser::NetlistParser(){


}

bool NetlistParser::parse(istream &inStream){
    //scan the file
    string line;
    while(getline(inStream, line)){
        switch(line[0]){
            //comment
            case 'C':
                break;

            case 'P':
                if(line.find("P  UNITS CUST ") == 0 && line.size() >= 15){
                    switch(line[14]){
                        case '0':
                            setUnit(eUnitInchDeg);
                            break;
                        case '1':
                            setUnit(eUnitMmDeg);
                            break;
                        case '2':
                            setUnit(eUnitInchRad);
                            break;
                        default:
                            setUnit(eUnitNotSet);
                            break;
                    }

                }
                else if(line.compare("P  UNITS CUST") == 0){
                    setUnit(eUnitInchDeg);
                }
                break;

            default:
                parseOperation(line);
                break;
        }
    }

    return true;
}


void NetlistParser::parseOperation(const string &inString){
    double coeff = isUnitMm() ? 0.001 : 0.0001;


    if(inString[0] == '3'){
        // check lenght of the line, should be 80
        if(inString.size() < 73){
            err_printf("ERROR:(NetlistParser::parseOperation): the line size in not 80cols");
            return;
        }

        string records[eColCount];

        // let's split... no chance to fail (out_of_range)
        for (uint8_t col_idx = 0; col_idx < eColCount; col_idx++){
            records[col_idx] = inString.substr(sColumnsDescription[col_idx].mBegin, sColumnsDescription[col_idx].mEnd-sColumnsDescription[col_idx].mBegin + 1);
        }



        //net name
        string net_name = truncString(records[eColNetName]);


        NetEntry e;

        // through hole
        if(records[eColType].compare("1") == 0){
            // type
            e.setType(NetEntry::eThroughHole);

            // ref designator
            e.setDesignator(truncString(records[eColRefDesID]));


            // pin
            if(records[eColRefDesID].find("VIA") != 0){
                int pin = static_cast<int>(extractNumber(records[eColRefDesAlpha]));
                e.setPin(pin); //if not via
            }
            else{
                e.setPin(-1);
            }


            e.setMidPoint(records[eColRefDesM].compare("M") == 0);

            if(records[eColIsDrilled].compare("D") == 0){
                e.setIsDrilled(true);
                // if D
                e.setHoleSize(coeff * extractNumber(records[eColDiameter]));

                e.setPlated(records[eColPlated].compare("P") == 0);
            }
            else{
                e.setIsDrilled(false);
            }

            // if A
            const string &raw_acc_side = records[eColAccessSide];
            if(raw_acc_side[0] == 'A'){
                uint16_t access_side = 10*((uint16_t)(raw_acc_side[1] - '0')) + (uint16_t)(raw_acc_side[2] - '0');
                e.setAccessSide(access_side);
            }

            //X
            double x = extractNumber(records[eColCoordsX]);
            if(records[eColSignX].compare("-") == 0) { x = -x; }
            x *= coeff;


            //Y
            double y = extractNumber(records[eColCoordsY]);
            if(records[eColSignY].compare("-") == 0) {y = -y; }
            y *= coeff;

            // set pos
            e.setPosition(plPoint(x,y));


            //Feature Size
            if(records[eColRectHasDataX].compare("X") == 0){
                e.setFeatW(coeff * extractNumber(records[eColRectDataX]));
            }
            if(records[eColRectHasDataY].compare("Y") == 0){
                e.setFeatH(coeff * extractNumber(records[eColRectDataY]));
            }

            double rot = extractNumber(records[eColRectDataRot]);
            if(records[eColRectDataIsCCW].compare("R") == 0){ rot = - rot; }
            e.setFeatRot(rot);




            //Soldermask Field
            //rab

            if(net_name.size() > 0){
                addNetEntry(net_name, e);
            }
        }

        // smd
        else if(records[eColType].compare("2") == 0){
            e.setType(NetEntry::eSurfaceMount);

            // ref designator
            e.setDesignator(truncString(records[eColRefDesID]));


            // pin
            if(records[eColRefDesID].find("VIA") != 0){
                int pin = static_cast<int>(extractNumber(records[eColRefDesAlpha]));
                e.setPin(pin); //if not via
            }
            else{
                e.setPin(-1);
            }


            e.setMidPoint(records[eColRefDesM].compare("M") == 0);


            e.setIsDrilled(false);

            // if A
            const string &raw_acc_side = records[eColAccessSide];
            if(raw_acc_side[0] == 'A'){
                uint16_t access_side = 10*((uint16_t)(raw_acc_side[1] - '0')) + (uint16_t)(raw_acc_side[2] - '0');
                e.setAccessSide(access_side);
            }

            //X
            double x = extractNumber(records[eColCoordsX]);
            if(records[eColSignX].compare("-") == 0) { x = -x; }
            x *= coeff;


            //Y
            double y = extractNumber(records[eColCoordsY]);
            if(records[eColSignY].compare("-") == 0) {y = -y; }
            y *= coeff;

            // set pos
            e.setPosition(plPoint(x,y));


            //Feature Size
            if(records[eColRectHasDataX].compare("X") == 0){
                e.setFeatW(coeff * extractNumber(records[eColRectDataX]));
            }
            if(records[eColRectHasDataY].compare("Y") == 0){
                e.setFeatH(coeff * extractNumber(records[eColRectDataY]));
            }

            double rot = extractNumber(records[eColRectDataRot]);
            if(records[eColRectDataIsCCW].compare("R") == 0){ rot = - rot; }
            e.setFeatRot(rot);



            //Soldermask Field
            //rab


            if(net_name.size() > 0){
                addNetEntry(net_name, e);
            }
        }

    }



    // end of file
    else if(inString.find("999") == 0) {

    }

    // unknown
    else{
        err_printf("ERROR:(NetlistParser::parseOperation): entry unknown");
    }
}

double NetlistParser::extractNumber(const string &inStr)
{
    string checked;
    double number = 0;
    for(uint32_t i = 0; i < inStr.size() && inStr[i] >= '0' && inStr[i] <= '9'; i++) { checked += inStr[i]; }
    try{
        number = stringToDouble(checked);
    }
    catch(...){
        err_printf("ERROR:(NetlistParser::extractNumber): impossible to extract the number");
        return 0.0;
    }

    return number;
}

string NetlistParser::truncString(const string &inStr)
{
    string out;
    for (uint32_t i = 0; i < inStr.size() && inStr[i] != ' '; i++){
        out.push_back(inStr[i]);
    }
    return out;
}

