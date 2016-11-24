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


/// Handles the parsing of the Netlist file and generate the net list.
/// This net list is only a net description.
class NetlistParser{
    public:
        /// describe the couple Des/PinId
        class Pin{
            public:
                string mDesignator;
                uint32_t mPin;
                plPoint mPosition;
        }

        class Via{
            public :
                plPoint mPosition;
        }

        /// defines a net
        class Net{
            private:
                string mName;
                vector<Pin> mPins
                vector<Via> mVia
        };

    public:
        NetlistParser() {};
        virtual ~NetlistParser();

    protected:
        /// parses the stream given and generate the net list
        bool parse(istream &inStream);


        //<<< --- Interface for NetListParser (if implemented in that way !)
        virtual void setUnit() = 0;
        virtual void setJobName() = 0;
        virtual void addNet(const Net &inNet) = 0;
        //--- >>>

    private:
        class Column{
            public:
                uint16_t mBegin;
                uint16_t mEnd;
        };

        enum eColumnName{
            eColEntry,
            eColType,
            eColNetName,
            eColRefDesID,
            eColRefDesAlpha,
            eColRefDesM,
            eColHoleType;
            eColDiameter,
            eColPlated,
            eColAccessSide,
            eColCoordsX,
            eColCoordY,
            eColRectDataX,
            eColRectDataY,
            eColRectDataRot

            eColCount
        };


    private:
        void parseOperation(const string &inString);



    private:
        map<string, Net> mNets;

        vector<Column> mColumnsDesc;
};

#endif