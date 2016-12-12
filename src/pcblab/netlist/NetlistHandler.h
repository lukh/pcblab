#ifndef NETLISTHANDLER_h
#define NETLISTHANDLER_h

#include <stdint.h>

#include <string>
#include <fstream>
#include <iostream>

#include "pcblab/common.h"

#include "tools/fio.h"

#include "netlistparser.h"

using namespace std;

/// this class manages the Net list, from IPC-d-356 format
/// 
class NetlistHandler: public NetlistParser{
    public:
        NetlistHandler();
        virtual ~NetlistHandler();

        /// Opens a folder and find the file
        bool openFolder(const string &inFolderName);

        bool openNetlist(const string &inFileName);

        /// clean existing
        void close();


    protected:
        //<<< --- Interface for NetListParser (if implemented in that way !)
        virtual void setUnit();
        virtual void setJobName();

        virtual bool isUnitMm();

        virtual void addNetEntry(string inNetName, const NetEntry &inEntry);
        //--- >>>

};

#endif
