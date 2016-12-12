#include "netlistparser.h"

NetlistParser::NetEntry::eEntryType NetlistParser::NetEntry::getType() const
{
    return mType;
}

void NetlistParser::NetEntry::setType(const eEntryType &inType)
{
    mType = inType;
}

string NetlistParser::NetEntry::getDesignator() const
{
    return mDesignator;
}

void NetlistParser::NetEntry::setDesignator(const string &inDes)
{
    mDesignator = inDes;
}

uint32_t NetlistParser::NetEntry::getPin() const
{
    return mPin;
}

void NetlistParser::NetEntry::setPin(uint32_t inPin)
{
    mPin = inPin;
}

double NetlistParser::NetEntry::getHoleSize() const
{
    return mHoleSize;
}

void NetlistParser::NetEntry::setHoleSize(double inHoleSize)
{
    mHoleSize = inHoleSize;
}

plPoint NetlistParser::NetEntry::getPosition() const
{
    return mPosition;
}

void NetlistParser::NetEntry::setPosition(const plPoint &inPosition)
{
    mPosition = inPosition;
}

uint32_t NetlistParser::NetEntry::getAccessSide() const
{
    return mAccessSide;
}

void NetlistParser::NetEntry::setAccessSide(uint32_t accessSide)
{
    mAccessSide = accessSide;
}

bool NetlistParser::NetEntry::getPlated() const
{
    return mPlated;
}

void NetlistParser::NetEntry::setPlated(bool plated)
{
    mPlated = plated;
}

bool NetlistParser::NetEntry::getIsDrilled() const
{
    return mIsDrilled;
}

void NetlistParser::NetEntry::setIsDrilled(bool isDrilled)
{
    mIsDrilled = isDrilled;
}

bool NetlistParser::NetEntry::getMidPoint() const
{
    return mMidPoint;
}

void NetlistParser::NetEntry::setMidPoint(bool midPoint)
{
    mMidPoint = midPoint;
}



const NetlistParser::Column NetlistParser::sColumnsDescription[] = {
    NetlistParser::Column(0,0),    //eColEntry
    NetlistParser::Column(1,1),    //eColType
    NetlistParser::Column(3,16),    //eColNetName
    NetlistParser::Column(20,25),    //eColRefDesID
    NetlistParser::Column(27,30),    //eColRefDesAlpha
    NetlistParser::Column(31,31),    //eColRefDesM
    NetlistParser::Column(32,32),    //eColIsDrilled
    NetlistParser::Column(33,36),    //eColDiameter
    NetlistParser::Column(37,37),    //eColPlated
    NetlistParser::Column(),    //eColAccessSide
    NetlistParser::Column(),    //eColCoordsX
    NetlistParser::Column(),    //eColCoordY
    NetlistParser::Column(),    //eColRectDataX
    NetlistParser::Column(),    //eColRectDataY
    NetlistParser::Column(),    //eColRectDataRot
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
                break;

            default:
                parseOperation(line);
                break;
        }
    }

    return true;
}


void NetlistParser::parseOperation(const string &inString){
    // check lenght of the line, should be 80
    if(inString.size() != 80){
        err_printf("ERROR:(NetlistParser::parseOperation): the line size in not 80cols");
        return;
    }

    string records[eColCount];

    // let's split... no chance to fail (out_of_range)
    for (uint8_t col_idx = 0; col_idx < eColCount; col_idx++){
        records[col_idx] = inString.substr(sColumnsDescription[col_idx].mBegin, sColumnsDescription[col_idx].mBegin-sColumnsDescription[col_idx].mBegin + 1);
    }

    NetEntry e;

    if(records[eColEntry].compare("3") == 0){
        // through hole
        if(records[eColType].compare("17") == 0){
            e.setType(NetEntry::eThroughHole);

            e.setDesignator(records[eColRefDesID]);


            const string &raw_pin = records[eColRefDesAlpha];
            string checked_pin;
            int32_t pin = -1;
            for(uint32_t i = 0; i < raw_pin.size() && raw_pin[i] >= '0' && raw_pin[i] <= '9'; i++) { checked_pin += raw_pin[i]; }
            try{
                pin = stringToInt(checked_pin);
            }
            catch(...){
                err_printf("ERROR:(NetlistParser::parseOperation): impossible to extract the pin number");
                return;
            }

            e.setPin(pin); //if not via


            e.setMidPoint(records[eColRefDesM].compare("M") == 0);

            if(records[eColIsDrilled].compare("D") == 0){
                e.setIsDrilled(true);
                // if D
                double coeff = isUnitMm() ? 0.001 : 0.0001;

                string checked_hole;
                const string &raw_hole = records[eColDiameter];
                double hole = 0;
                for(uint32_t i = 0; i < raw_hole.size() && raw_hole[i] >= '0' && raw_hole[i] <= '9'; i++) { checked_hole += raw_hole[i]; }
                try{
                    hole = stringToInt(checked_hole);
                }
                catch(...){
                    err_printf("ERROR:(NetlistParser::parseOperation): impossible to extract the hole size");
                    return;
                }
                e.setHoleSize(coeff * hole);

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


            //Y


            //Feature Size

            //Soldermask Field
        }

        // smd
        else if(records[eColType].compare("27") == 0){
            e.setType(NetEntry::eSurfaceMount);
        }

    }



    // end of file
    else if(records[eColEntry].compare("9") == 0 && records[eColType].compare("99") == 0) {

    }

    // unknown
    else{
        err_printf("ERROR:(NetlistParser::parseOperation): entry unknown");
    }
}

