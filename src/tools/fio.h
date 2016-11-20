#ifndef FIO_H
#define FIO_H

#include <vector>
#include <string>

#include <dirent.h>
#include <sys/stat.h>

#include "stringoperations.h"

using namespace std;

bool listFilesInDirectory(const string &inDir, vector<string> &outList);


bool getExtensionFromFilename(const string &inFilename, string &outExt);


bool compareExtension(const string &inE1, const string &inE2);

#endif // FIO_H
