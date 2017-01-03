#ifndef NETLISTHANDLER_h
#define NETLISTHANDLER_h

#include <stdint.h>

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "pcblab/common.h"

#include "tools/fio.h"

#include "net.h"

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

        /// returns the map of net
        const Netlist &getNetlist() const;

        /// returns the list of net names
        void getNetlistList(vector <string> &outList) const;

        /// return the net corresponding to inName
        bool getNet(string inName, Net **outNet);

    protected:
        //<<< --- Interface for NetListParser (if implemented in that way !)
        virtual void setUnit(eUnit inUnit);

        virtual bool isUnitMm();

        virtual void addNetEntry(string inNetName, NetEntry inEntry);
        //--- >>>


    private:
        NetlistParser::eUnit mUnit;

        Netlist mNetlist;
};

#endif
