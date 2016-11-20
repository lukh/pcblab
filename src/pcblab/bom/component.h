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

        eUnit getUnit() const { return mUnit; }
        void setUnit(eUnit inUnit){ mUnit = inUnit; }



private:
        string mDesignator;
        plPoint mPosition;
        double mRotation;
        Parameters mParameters; // others parameters
        eUnit mUnit;
};

typedef map<string, Component> Components;


#endif // COMPONENT_H

