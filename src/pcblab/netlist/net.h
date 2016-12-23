#ifndef NET_H
#define NET_H

#include <string>
#include <vector>
#include <map>

#include "pcblab/common.h"

using namespace std;



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

        int32_t getPin() const;
        void setPin(int32_t inPin);

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




class Net{
    public:
        Net() {}
        Net(string inName): mName(inName) {}

        void setName(string inName){ mName = inName; }
        const string& getName() const { return mName; }

        void addEntry(NetEntry inE){ mEntries.push_back(inE); }
        const vector<NetEntry>& getEntries() const { return mEntries; }


    private:
        string mName;

        vector <NetEntry> mEntries;
};


typedef map<string, Net> Netlist;





#endif // NET_H
