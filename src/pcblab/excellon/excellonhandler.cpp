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
}

void ExcellonHandler::addTool(uint8_t inIdx, double inHoleSize, double inInfeedRate, double inSpindleRate)
{

}

void ExcellonHandler::addHole(const string &inRawX, const string &inRawY)
{

}

void ExcellonHandler::setSection(ExcellonState::eSection inSection)
{
    mState.setSection(inSection);
}

void ExcellonHandler::setUnit(ExcellonState::eUnit inUnit)
{

}

void ExcellonHandler::setNumberFormat(ExcellonState::eNumberFormat inFormat)
{

}

void ExcellonHandler::setCoordMode(ExcellonState::eCoordinatesMode inCoordMode)
{

}

void ExcellonHandler::setCurrentPoint(plPoint inPoint)
{

}

void ExcellonHandler::setOriginPoint(plPoint inPoint)
{

}

void ExcellonHandler::setCurrentTool(uint8_t inToolIdx)
{
}
