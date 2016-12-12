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
        enum eColumnName{
            eColEntry,
            eColType,
            eColNetName,
            eColRefDesID,
            eColRefDesAlpha,
            eColRefDesM,
            eColIsDrilled,
            eColDiameter,
            eColPlated,
            eColAccessSide,
            eColCoordsX,
            eColCoordY,
            eColRectDataX,
            eColRectDataY,
            eColRectDataRot,

            eColCount
        };

        class Column{
            public:
                Column(uint16_t inBegin, uint16_t inEnd): mBegin(inBegin), mEnd(inEnd) {}
                uint16_t mBegin;
                uint16_t mEnd;
        };


        class NetEntry{
            public:
                enum eEntryType{
                    eThroughHole,
                    eSurfaceMount
                };

                eEntryType getType() const;
                void setType(const eEntryType &inType);

                string getDesignator() const;
                void setDesignator(const string &inDes);

                uint32_t getPin() const;
                void setPin(uint32_t inPin);

                bool getMidPoint() const;
                void setMidPoint(bool midPoint);

                bool getIsDrilled() const;
                void setIsDrilled(bool isDrilled);

                double getHoleSize() const;
                void setHoleSize(double inHoleSize);

                plPoint getPosition() const;
                void setPosition(const plPoint &inPosition);

                uint32_t getAccessSide() const;
                void setAccessSide(uint32_t accessSide);

                bool getPlated() const;
                void setPlated(bool plated);

        private:
                eEntryType mType;

                string mDesignator;
                uint32_t mPin;

                bool mIsDrilled;
                double mHoleSize;


                bool mPlated;

                plPoint mPosition;

                uint32_t mAccessSide;

                bool mMidPoint;
        };

    public:
        NetlistParser();
        virtual ~NetlistParser() {}

    protected:
        /// parses the stream given and generate the net list
        bool parse(istream &inStream);


        //<<< --- Interface for NetListParser (if implemented in that way !)
        virtual void setUnit() = 0;
        virtual void setJobName() = 0;

        virtual bool isUnitMm() = 0;

        virtual void addNetEntry(string inNetName, const NetEntry &inEntry) = 0;
        //--- >>>

    private:
        void parseOperation(const string &inString);



    private:
        static const Column sColumnsDescription[eColCount];
};

#endif
