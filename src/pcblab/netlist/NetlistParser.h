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


        class NetEntry{
            public:
                enum eEntryType{
                    eETNone,
                    eThroughHole,
                    eSurfaceMount
                };

                NetEntry(): mType(eETNone), mPin(0), mIsDrilled(false), mHoleSize(0), mPlated(false), mAccessSide(0), mMidPoint(false), mFeatW(0), mFeatH(0), mFeatRot(0) {}


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

                double getFeatW() const;
                void setFeatW(double inFW);

                double getFeatH() const;
                void setFeatH(double inFW);

                double getFeatRot() const;
                void setFeatRot(double featRot);

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

                double mFeatW;
                double mFeatH;
                double mFeatRot;
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
