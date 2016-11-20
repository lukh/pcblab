#include "componenthandler.h"

ComponentHandler::ComponentHandler()
{
    mBOM.setDesignatorString("Designator");

    mPNP.setDesignatorString("Designator");
    mPNP.setXString("Mid X");
    mPNP.setYString("Mid Y");
    mPNP.setRotString("Rotation");
}

bool ComponentHandler::openFolder(const string &inFolderName)
{
    close();

    bool open_status = true;
    // list all files in dir
    vector<string> files;
    if(! listFilesInDirectory(inFolderName, files)){
        //error listing the dir
        err_printf("ERROR(BOMHandler::openFolder): impossible to list the folder given");
        return false;
    }


    // scan the existing files (BOM)
    for(vector<string>::iterator it = files.begin(); it != files.end(); ++it){
        string file = *it;

        string ext;
        if(getExtensionFromFilename(file, ext)){
            transform(ext.begin(), ext.end(),ext.begin(), ::toupper);
        }

        // it is a text file
        if(ext.compare("CSV") == 0 && file.find("Pick Place") == string::npos){
            string filename = inFolderName + "/" + file;

            bool status = openBomLayer(filename, mComponents);
            open_status = status && open_status;
            break;
        }
    }


    // scan the existing files (PNP)
    for(vector<string>::iterator it = files.begin(); it != files.end(); ++it){
        string file = *it;

        string ext;
        if(getExtensionFromFilename(file, ext)){
            transform(ext.begin(), ext.end(),ext.begin(), ::toupper);
        }

        // it is a text file
        if(ext.compare("CSV") == 0 && file.find("Pick Place") != string::npos){
            string filename = inFolderName + "/" + file;

            open_status = openPnpLayer(filename, mComponents);
            break;
        }
    }

    return open_status;
}

bool ComponentHandler::openBomLayer(const string &inFileName, Components &outComponents)
{
    ifstream fs;
    close();

    fs.open(inFileName);
    if(! fs.is_open()){
        err_printf("ERROR(BOMHandler::openExcellonLayer): Impossible to open the file "+ inFileName);
        return false;
    }

    bool parse_res = mBOM.parse(fs, outComponents);

    fs.close();

    return parse_res;
}

bool ComponentHandler::openPnpLayer(const string &inFileName, Components &outComponents)
{
    ifstream fs;
    close();

    fs.open(inFileName);
    if(! fs.is_open()){
        err_printf("ERROR(BOMHandler::openExcellonLayer): Impossible to open the file "+ inFileName);
        return false;
    }

    bool parse_res = mPNP.parse(fs, outComponents);

    fs.close();

    return parse_res;
}

void ComponentHandler::close()
{
    mComponents.clear();
}

