#ifndef NETLISTHANDLER_h
#define NETLISTHANDLER_h

#include <stdint.h>

#include <string>
#include <fstream>
#include <iostream>

#include "pcblab/common.h"

using namespace std;

/// this class manages the Net list, from IPC-d-356 format
/// 
class NetlistHandler{
    public:
        NetlistHandler();
        virtual ~NetlistHandler();

        /// Opens a folder containing all gerber files, based on extensions
        bool openFolder(const string &inFolderName);

        /// clean existing layers
        void close();

};

#endif