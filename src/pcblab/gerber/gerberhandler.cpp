#include "gerberhandler.h"

GerberHandler::GerberHandler()
{

}

GerberHandler::~GerberHandler()
{
    close();
}


void GerberHandler::openFolder(const string &inFolderName, ExtensionOrderList inOrder)
{
    close();

    vector<string> files;
    map<string, string> mapped_files;
    if(! listFilesInDirectory(inFolderName, files)){
        //error listing the dir
        err_printf("ERROR(GerberHandler::openFolder): impossible to list the folder given");
        return;
    }


    //cout << "--- Listing all files in the folder " << inFolderName << endl;

    // list the existing files
    for(vector<string>::iterator it = files.begin(); it != files.end(); ++it){
        string file = *it;

        string ext;
        if(getExtensionFromFilename(file, ext)){
            transform(ext.begin(), ext.end(),ext.begin(), ::toupper);

            //cout << "   > ext:" << ext << "= " << file  << ", added to mapped_files" << endl;

            mapped_files[ext] = file;
        }
    }


    cout << " --- Filtering the files" << endl;

    // add the file if in the order list and in the folder
    uint8_t idx = 0;
    for(ExtensionOrderList::iterator it = inOrder.begin(); it != inOrder.end(); ++it){
        string ext = *it;
        transform(ext.begin(), ext.end(),ext.begin(), ::toupper);

        if(mapped_files.count(ext) != 0){
            string full_path = inFolderName + "/" + mapped_files[ext];

            //cout << "   > " << ext << ": " << full_path << " added to the map" << endl;

            mMap.push_back(ext);
            openGerberLayer(ext, full_path, ext, idx++);
        }
    }

}

void GerberHandler::openFolder(const string &inFolderName, vector<string> inFileNames)
{
    close();

    uint8_t i = 0;
    for(vector<string>::iterator it = inFileNames.begin(); it != inFileNames.end(); ++it){
        string file = *it;
        string full_path = inFolderName + "/" + file;
        mMap.push_back(file);
        openGerberLayer(file, full_path, file, i++);
    }
}


void GerberHandler::openGerberLayer(const string &inName, const string &inFilename, const string &inIdentifier, uint8_t inPosition)
{
    // create the layer
    GerberLayer *layer = new GerberLayer(inName);
    layer->open(inFilename);

    // add the layer to the list
    if(mLayers.count(inIdentifier) > 0){
        // the layer already exist !
        //delete it...
        delete mLayers[inIdentifier];
    }
    mLayers[inIdentifier] = layer;
}




void GerberHandler::close()
{
    for(LayerMap::iterator it = mLayers.begin(); it != mLayers.end(); ++it){
        GerberLayer *layer = it->second;
        delete layer;
    }

    mLayers.clear();
    mMap.clear();
}
