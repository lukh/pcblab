#ifndef APERTUREPRIMITIVE_H
#define APERTUREPRIMITIVE_H


#include <vector>

#include "../../common.h"

#include "aperturemodifier.h"


using namespace std;


/// Used to describe a AD primitive (it s values are fixed then).
/// the interface contains the modifiers, and it s type
class IAperturePrimitive{
    public:
        /// describes the type of primitive
        enum eType{
            eCircle,
            eVectorLine,
            eCenterLine,
            eOutLine,
            ePolygon,
            eMoire,
            eThermal,
            eInvalid
        };


        enum eExposure{
            eExposureOn,
            eExposureOff
        };

        ///
        IAperturePrimitive(eType inType);
        virtual ~IAperturePrimitive() { d_printf("%%% Deleting IAperturePrimitive", 4, 0, false); }


        /// add a modifier
        void addModifier(ApertureModifier inModifier){
            mModifiers.push_back(inModifier);
        }


        // exposure is alway the first modifier
        eExposure getExposure() {
            if(mModifiers.size() == 0){
                return eExposureOff;
            }

            return (mModifiers.at(0) == 1.0) ? eExposureOn : eExposureOff;
        }


        /// abstract method used to define if the primitive is valid.
        /// implementation can count the number of modifiers for instance
        virtual bool isValid() = 0;


    protected:
        vector <ApertureModifier> mModifiers;

    private:
        eType mType;

};




class APrimCircle: public IAperturePrimitive{
    public:
        APrimCircle();
        virtual bool isValid();
};

class APrimVectorLine: public IAperturePrimitive{
    public:
        APrimVectorLine();
        virtual bool isValid();
};

class APrimCenterLine: public IAperturePrimitive{
    public:
        APrimCenterLine();
        virtual bool isValid();
};

class APrimOutline: public IAperturePrimitive{
    public:
        APrimOutline();
        virtual bool isValid();
};

class APrimPolygon: public IAperturePrimitive{
    public:
        APrimPolygon();
        virtual bool isValid();
};

class APrimMoire: public IAperturePrimitive{
    public:
        APrimMoire();
        virtual bool isValid();
};

class APrimThermal: public IAperturePrimitive{
    public:
        APrimThermal();
        virtual bool isValid();
};


#endif // APERTUREPRIMITIVE_H
