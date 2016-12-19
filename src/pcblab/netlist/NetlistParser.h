#ifndef NETLISTPARSER_H
#define NETLISTPARSER_H


#include <stdint.h>

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <list>

#include "pcblab/common.h"

#include "net.h"


/// Handles the parsing of the Netlist file and generate the net list.
/// This net list is only a net description.
class NetlistParser{
    public:
        enum eUnit{
            eUnitNotSet,
            eUnitInchDeg,
            eUnitMmDeg,
            eUnitInchRad
        };

        enum eColumnName{
            eColEntry,
            eColType,       //smd/through hole
            eColNetName,
            eColRefDesID,
            eColRefDesAlpha,
            eColRefDesM,
            eColIsDrilled,
            eColDiameter,
            eColPlated,
            eColAccessSide,
            eColSignX,
            eColCoordsX,
            eColSignY,
            eColCoordsY,
            eColRectHasDataX,
            eColRectDataX,
            eColRectHasDataY,
            eColRectDataY,
            eColRectDataIsCCW,
            eColRectDataRot,
            eColSolderMask,

            eColCount
        };

        class Column{
            public:
                Column(uint16_t inBegin, uint16_t inEnd): mBegin(inBegin), mEnd(inEnd) {}
                uint16_t mBegin;
                uint16_t mEnd;
        };




    public:
        NetlistParser();
        virtual ~NetlistParser() {}

    protected:
        /// parses the stream given and generate the net list
        bool parse(istream &inStream);


        //<<< --- Interface for NetListParser (if implemented in that way !)
        virtual void setUnit(eUnit inUnit) = 0;
        virtual void setJobName() = 0;

        virtual bool isUnitMm() = 0;

        virtual void addNetEntry(string inNetName, NetEntry inEntry) = 0;
        //--- >>>

    private:
        void parseOperation(const string &inString);



        double extractNumber(const string &inStr);
        string truncString(const string &inStr);

    private:
        static const Column sColumnsDescription[eColCount];
};

#endif
