#include "componenthandler.h"

#include <math.h>

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

            bool status = openPnpLayer(filename, mComponents);
            open_status = status && open_status;
            break;
        }
    }

    return open_status;
}

void ComponentHandler::close()
{
    mComponents.clear();
}

void ComponentHandler::getDesignatorsList(vector<string> &outDesList) const
{
    outDesList.clear();
    for(Components::const_iterator it = mComponents.begin(); it != mComponents.end(); ++it){
        outDesList.push_back(it->first);
    }
}

void ComponentHandler::getSortedAndGroupedDesignatorsList(const vector<string> &inDesPrefixOrder, string inSortingKey, vector <string> & outDesList)
{
    // clear the output list at first
    outDesList.clear();

    //create a list of des for further analysis
    vector<string> des_list;
    getDesignatorsList(des_list);


    //sort by des
    //the map: a map[prefix][value] = list of des
    map<string, map<string, vector<string> > >temp_map;
    for (vector<string>::const_iterator prefix_it = inDesPrefixOrder.begin(); prefix_it != inDesPrefixOrder.end(); ++prefix_it){
        string prefix = *prefix_it;

        vector<string>::iterator des_it = des_list.begin();
        while(des_it != des_list.end()){
            string des = *des_it;
            if(des.find(prefix) == 0){ // if the prefix is found in the des
                // create a map for this kind of des (Cx, Ux, etc)
                map<string, map<string, vector<string> > >::iterator map_prefix_it = temp_map.find(prefix);
                if(map_prefix_it == temp_map.end()){
                    temp_map[prefix] = map<string, vector<string> >();
                }

                //get compo data:
                Component *compo;
                getComponent(des, &compo);

                string compo_param;
                if(!compo->getParameter(inSortingKey, compo_param)){
                }

                // check on values
                map<string, vector<string> >::iterator values_it = temp_map[prefix].find(compo_param);
                if(values_it == temp_map[prefix].end()){
                    temp_map[prefix][compo_param] = vector<string>();
                }

                // copy the des in the temp list and delete the one in des_list
                temp_map[prefix][compo_param].push_back(des);
                des_it = des_list.erase(des_it);
            }
            else{
                ++des_it;
            }
        }
    }


    //copies in out
    for (vector<string>::const_iterator prefix_it = inDesPrefixOrder.begin(); prefix_it != inDesPrefixOrder.end(); ++prefix_it){
        string prefix = *prefix_it;
        map<string, map<string, vector<string> > >::iterator temp_map_it = temp_map.find(prefix);
        if(temp_map_it != temp_map.end()){
            map<string, vector<string> > &sorted_list = temp_map_it->second;
            for(map<string, vector<string> >::iterator sli = sorted_list.begin(); sli != sorted_list.end(); ++sli){
                for(vector<string>::iterator curr_des = sli->second.begin(); curr_des != sli->second.end(); ++curr_des){
                    outDesList.push_back(*curr_des);
                }
            }
        }
    }





    //add other components
    for(vector<string>::iterator des_it = des_list.begin(); des_it != des_list.end(); ++des_it){
        outDesList.push_back(*des_it);
    }
}

bool ComponentHandler::getComponent(const string &inDes, Component **outComponent)
{
    Components::const_iterator it = mComponents.find(inDes);
    if(it == mComponents.end()){
        return false;
    }
    else{
        *outComponent = &mComponents[inDes];
        return true;
    }
}

string ComponentHandler::getNearestDesignator(plPoint inPoint)
{
    string nearest_des;
    double smaller_dist;

    nearest_des = mComponents.begin()->first;

    plPoint pos = mComponents.begin()->second.getPosition();
    smaller_dist = sqrt(pow(inPoint.mX - pos.mX, 2) + pow(inPoint.mY - pos.mY, 2));

    for(Components::const_iterator it = mComponents.begin(); it != mComponents.end(); ++it){
        pos = it->second.getPosition();
        double dist = sqrt(pow(inPoint.mX - pos.mX, 2) + pow(inPoint.mY - pos.mY, 2));

        if(dist < smaller_dist){
            smaller_dist = dist;
            nearest_des = it->first;
        }
    }

    return nearest_des;
}


bool ComponentHandler::openBomLayer(const string &inFileName, Components &outComponents)
{
    ifstream fs;

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

    fs.open(inFileName);
    if(! fs.is_open()){
        err_printf("ERROR(BOMHandler::openExcellonLayer): Impossible to open the file "+ inFileName);
        return false;
    }

    bool parse_res = mPNP.parse(fs, outComponents);

    fs.close();

    return parse_res;
}
