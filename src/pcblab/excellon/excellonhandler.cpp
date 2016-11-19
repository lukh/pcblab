#include "excellonhandler.h"

ExcellonHandler::ExcellonHandler()
{

}

ExcellonHandler::~ExcellonHandler()
{

}

bool ExcellonHandler::openFolder(const string &inFolderName){
    bool open_status = false;
    // list all files in dir
    vector<string> files;
    if(! listFilesInDirectory(inFolderName, files)){
        //error listing the dir
        err_printf("ERROR(ExcellonHandler::openFolder): impossible to list the folder given");
        return false;
    }


    // scan the existing files
    for(vector<string>::iterator it = files.begin(); it != files.end(); ++it){
        string file = *it;

        string ext;
        if(getExtensionFromFilename(file, ext)){
            transform(ext.begin(), ext.end(),ext.begin(), ::toupper);
        }

        // it is a text file
        if(ext.find("TXT") == 0){
            string filename = inFolderName + "/" + file;

            ifstream fs;

            fs.open(filename);
            if(! fs.is_open()){
                err_printf("ERROR(ExcellonHandler::openFolder): Impossible to open the file "+ filename);
                return false;
            }

            //read the line
            string line;
            bool fileFound = false;
            while(getline(fs, line)){
                if(line.find("M48") == 0){
                    fileFound = true;
                    break;
                }
            }

            fs.close();


            if (fileFound) {
                open_status = openExcellonLayer(filename);
                break;
            }
        }
    }

    return open_status;
}

bool ExcellonHandler::openExcellonLayer(const string &inFileName)
{
    ifstream fs;
    close();

    fs.open(inFileName);
    if(! fs.is_open()){
        err_printf("ERROR(ExcellonHandler::openExcellonLayer): Impossible to open the file "+ inFileName);
        return false;
    }

    bool parse_res = parse(fs);

    fs.close();

    return parse_res;
}

void ExcellonHandler::close()
{
    mState = ExcellonState();

    //TODO: RESET TOOLS AND HOLES list
    mTools.clear();
}

void ExcellonHandler::addTool(uint8_t inIdx, double inHoleSize, double inInfeedRate, double inSpindleRate)
{
    if(mState.getSection() == ExcellonState::eSectionHeader){
        mTools[inIdx] = ExcellonHandler::Tool(inIdx, inHoleSize, inSpindleRate, inInfeedRate);
    }
    else{
        err_printf("ERROR(ExcellonHandler::addTool): Trying to add a tool when not in header mode");
    }
}

void ExcellonHandler::addHole(const string &inRawX, const string &inRawY)
{
    string raw_x = inRawX;
    string raw_y = inRawY;

    if(mState.getSection() != ExcellonState::eSectionBody){
        err_printf("ERROR(ExcellonHandler::addHole): Trying to add a hole when not in body mode");
        return;
    }

    // checks coords are ok
    if(raw_x.size() > 6 || raw_y.size() > 6){
        err_printf("ERROR(ExcellonHandler::addHole): number format is bad, expecting 4:2(metric) or 2:4(metric) (6digits)");
        return;
    }

    //check that the Current tool is actually saved
    uint8_t tool_idx = mState.getCurrentToolIdx();
    map<uint8_t, Tool>::const_iterator it;
    it = mTools.find(tool_idx);
    if (it == mTools.end()){
        err_printf("ERROR(ExcellonHandler::addHole): Bad tool");
        return;
    }


    double power = 0;
    switch(mState.getUnit()){
        case ExcellonState::eUnitInch:
            power = 4;
            break;
        case ExcellonState::eUnitMetric:
            power = 2;
            break;
        default:
            err_printf("ERROR(ExcellonHandler::addHole): Bad Unit !");
            break;
    }

    double x=0, y=0, scaleX=0, scaleY=0;

    switch(mState.getNumberFormat()){
        case ExcellonState::eNumForLeadingZeros:{
            //10^(6-len - power)
            scaleX = pow(10, 6-raw_x.size()-power);
            //10^(6-len - power)
            scaleY = pow(10, 6-raw_y.size()-power);
            break;}

        case ExcellonState::eNumForTrailingZeros:{
            scaleX = pow(10, power);
            scaleY = pow(10, power);
            break;}

        default:
            err_printf("ERROR(ExcellonHandler::addHole): Bad Number format !");
            break;
    }

    if(raw_x.size() != 0){
        try{
            x = stringToDouble(raw_x);
            x *= scaleX;
        }
        catch(...){
            x=0;
            err_printf("ERROR(ExcellonHandler::addHole): Bad Number: " + raw_x);
        }
    }
    else{
        x = (mState.getCoordMode() == ExcellonState::eCoordModeAbsolute) ? mState.getCurrentPoint().mX : 0.0;
    }

    if(raw_y.size() != 0){
        try{
            y = stringToDouble(raw_y);
            y *= scaleY;
        }
        catch(...){
            y=0;
            err_printf("ERROR(ExcellonHandler::addHole): Bad Number: " + raw_x);
        }
    }
    else{
        y = (mState.getCoordMode() == ExcellonState::eCoordModeAbsolute) ? mState.getCurrentPoint().mY : 0.0;
    }



    switch(mState.getCoordMode()){
        case ExcellonState::eCoordModeAbsolute:
            mState.setCurrentPoint(plPoint(x,y));
            break;
        case ExcellonState::eCoordModeIncremental:
            x += mState.getCurrentPoint().mX;
            y += mState.getCurrentPoint().mY;
            mState.setCurrentPoint(plPoint(x,y));
            break;
        default:
            break;
    }


    plPoint position(x, y);
    Tool *tool = &mTools[tool_idx];

    mHoles.push_back(Hole(tool, position));
}

void ExcellonHandler::setSection(ExcellonState::eSection inSection)
{
    mState.setSection(inSection);
}

void ExcellonHandler::setUnit(ExcellonState::eUnit inUnit)
{
    if(mState.getUnit() != ExcellonState::eUnitUndefined || mState.getSection() != ExcellonState::eSectionHeader){
        err_printf("ERROR(ExcellonHandler::setUnit): Trying to update unit type when not in header or when already set");
        return;
    }

    mState.setUnit(inUnit);
}

void ExcellonHandler::setNumberFormat(ExcellonState::eNumberFormat inFormat)
{
    if(mState.getNumberFormat() != ExcellonState::eNumForUndefined || mState.getSection() != ExcellonState::eSectionHeader){
        err_printf("ERROR(ExcellonHandler::setNumberFormat): Trying to update num format type when not in header or when already set");
        return;
    }

    mState.setNumberFormat(inFormat);
}

void ExcellonHandler::setCoordMode(ExcellonState::eCoordinatesMode inCoordMode)
{
    if(mState.getSection() != ExcellonState::eSectionHeader){
        err_printf("ERROR(ExcellonHandler::setCoordMode): Trying to update coord type when not in header");
        return;
    }

    mState.setCoordMode(inCoordMode);
}


void ExcellonHandler::setOriginPoint(plPoint inPoint)
{
    //TODO: ???
}

void ExcellonHandler::setCurrentTool(uint8_t inToolIdx)
{
    if(mState.getSection() != ExcellonState::eSectionBody){
        err_printf("ERROR(ExcellonHandler::setCurrentTool): Trying to update coord type when not in body");
        return;
    }

    mState.setCurrentToolIdx(inToolIdx);
}
