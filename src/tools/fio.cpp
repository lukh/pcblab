#include "fio.h"

#include <iostream>
using namespace std;

bool listFilesInDirectory(const string &inDir, vector<string> &outList)
{
    vector<string> ret;
    DIR *dir;
    struct stat st;

    outList.clear();

    struct dirent *ent;
    if ((dir = opendir(inDir.c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            const string file_name = ent->d_name;
            const string full_file_name = inDir + "/" + file_name;

            if (file_name[0] == '.')
                continue;

            if (stat(full_file_name.c_str(), &st) == -1)
                continue;

            const bool is_directory = (st.st_mode & S_IFDIR) != 0;

            if (is_directory)
                continue;

            outList.push_back(file_name);
        }
        closedir (dir);

        return true;
    }

    return false;
}

bool getExtensionFromFilename(const string &inFilename, string &outExt)
{
    vector<string> splitted;
    tokenize(inFilename, splitted, ".");

    if(splitted.size() != 2){
        cout << "split failed"<< endl;
        return false;
    }

    outExt = splitted[1];

    //convert to uppercase
    //TODO

    return true;
}



bool compareExtension(const string &inE1, const string &inE2)
{
    return false;
}
