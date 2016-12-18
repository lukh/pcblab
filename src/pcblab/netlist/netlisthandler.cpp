#include "netlisthandler.h"


NetlistHandler::NetlistHandler(): mUnit(NetlistParser::eUnitNotSet)
{

}

NetlistHandler::~NetlistHandler()
{

}

bool NetlistHandler::openFolder(const string &inFolderName)
{
    close();

    bool open_status = false;
    // list all files in dir
    vector<string> files;
    if(! listFilesInDirectory(inFolderName, files)){
        //error listing the dir
        err_printf("ERROR(NetlistHandler::openFolder): impossible to list the folder given");
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
        if(ext.compare("IPC") == 0){
            string filename = inFolderName + "/" + file;

            open_status = openNetlist(filename);
            break;
        }
    }

    return open_status;
}

bool NetlistHandler::openNetlist(const string &inFileName)
{
    ifstream fs;

    fs.open(inFileName);
    if(! fs.is_open()){
        err_printf("ERROR(NetlistHandler::openNetlist): Impossible to open the file "+ inFileName);
        return false;
    }

    bool parse_res = parse(fs);

    fs.close();

    return parse_res;
}

void NetlistHandler::close()
{
    mNetlist.clear();
}

void NetlistHandler::setUnit(eUnit inUnit)
{
    mUnit = inUnit;
}

void NetlistHandler::setJobName()
{

}

bool NetlistHandler::isUnitMm()
{
    return mUnit == NetlistParser::eUnitMmDeg;
}

void NetlistHandler::addNetEntry(string inNetName, NetlistParser::NetEntry inEntry)
{
    mNetlist[inNetName].push_back(inEntry);
}
