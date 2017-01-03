#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <vector>
#include <map>

#include "pcblab/common.h"

using namespace std;

class Component{
    public:
        enum eUnit{
            eUnitNotSet,
            eUnitMm,
            eUnitMil
        };

        typedef map<string, string> Parameters;

    public:
        Component(): mRotation(0.0), mUnit(eUnitNotSet){}
        Component(string inDesignator, plPoint inPosition, double inRotation, Parameters inParameters, eUnit inUnit):
            mDesignator(inDesignator), mPosition(inPosition), mRotation(inRotation), mParameters(inParameters), mUnit(inUnit){}

        ~Component(){}

        string getDesignator() const { return mDesignator; }
        void setDesignator(string inDes) { mDesignator = inDes; }

        plPoint getPosition() const{ return mPosition; }
        void setPosition(plPoint inPos) { mPosition = inPos; }

        double getRotation() const{ return mRotation; }
        void setRotation(double inRot) { mRotation = inRot; }

        Parameters &getParameters() { return mParameters; }
        bool getParameter(const string &inParam, string &outValue){
            Parameters::iterator it = mParameters.find(inParam);
            if(it == mParameters.end()){
                return false;
            }
            outValue = mParameters[inParam];
            return true;
        }

        eUnit getUnit() const { return mUnit; }
        void setUnit(eUnit inUnit){ mUnit = inUnit; }


        void addNet(string inNet) { mNets.push_back(inNet); }
        const vector<string> getNets() const { return mNets; }
        string getNet(uint32_t inIdx) const {
            if(inIdx < mNets.size()){
                return mNets[inIdx];
            }
            return string();
        }

    private:
        string mDesignator;
        plPoint mPosition;
        double mRotation;
        Parameters mParameters; // others parameters
        eUnit mUnit;

        vector<string>mNets;
};

typedef map<string, Component> Components;


#endif // COMPONENT_H

