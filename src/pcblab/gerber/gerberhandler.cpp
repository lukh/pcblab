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

#ifdef DEBUG_PRINT
    d_printf("--- Listing all files in the folder " + inFolderName, 1, 0);
#endif

    // list the existing files
    for(vector<string>::iterator it = files.begin(); it != files.end(); ++it){
        string file = *it;

        string ext;
        if(getExtensionFromFilename(file, ext)){
            transform(ext.begin(), ext.end(),ext.begin(), ::toupper);

#ifdef DEBUG_PRINT
            d_printf("   > ext:" + ext + "= " + file  + " found", 1, 0);
#endif

            mapped_files[ext] = file;
        }
    }


#ifdef DEBUG_PRINT
    d_printf("--- Adding the files to the handler ---", 1, 0);
#endif

    // add the file if in the order list and in the folder
    uint8_t idx = 0;
    for(ExtensionOrderList::iterator it = inOrder.begin(); it != inOrder.end(); ++it){
        string ext = *it;
        transform(ext.begin(), ext.end(),ext.begin(), ::toupper);

        if(mapped_files.count(ext) != 0){
            string full_path = inFolderName + "/" + mapped_files[ext];

#ifdef DEBUG_PRINT
            d_printf("   > " + ext + ": " + full_path + " added", 1, 0);
#endif

            mMap.push_back(ext);
            openGerberLayer(ext, full_path, ext, idx++);
        }
    }

}

void GerberHandler::openFolderWithList(const string &inFolderName, vector<string> inFileNames)
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

bool GerberHandler::getLayer(uint8_t inIdx, GerberLayer *&outLayer) const {
    if(inIdx >= getLayersCount()){ return false; }

    string id = mMap[inIdx];
    LayerMap::const_iterator it;
    it = mLayers.find(id);
    if (it == mLayers.end()) return false;

    outLayer = it->second;
    return true;
}

void GerberHandler::setOrderList(const GerberHandler::ExtensionOrderList &inList)
{
    mMap.clear();


    ExtensionOrderList missing;

    // add missing ext/id if linList doesn't contain all elements
    for(LayerMap::iterator it = mLayers.begin(); it != mLayers.end(); ++it){
        string id = it->first;
        //if the existing id is not in the list given
        if (find(inList.begin(), inList.end(), id) == inList.end()){
            missing.push_back(id);
        }
    }

    // add the id to the mMap if needed, other are ignored
    for(ExtensionOrderList::const_iterator it = inList.begin(); it != inList.end(); ++it){
        const string id = *it;
        if(mLayers.count(id) != 0){
            mMap.push_back(id);
        }
    }

    // add the missing ids
    for(ExtensionOrderList::iterator it = missing.begin(); it != missing.end(); ++it){
        string id = *it;
        mMap.push_back(id);
    }
}




