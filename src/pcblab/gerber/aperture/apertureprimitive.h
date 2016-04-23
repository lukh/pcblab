#ifndef APERTUREPRIMITIVE_H
#define APERTUREPRIMITIVE_H


#include <vector>
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
            eThermal
        };


        enum eExposure{
            eExposureOn,
            eExposureOff
        };

        ///
        IAperturePrimitive(eType inType): mType(inType) {}


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
        virtual void isValid() = 0;


    protected:
        vector <ApertureModifier> mModifiers;

    private:
        eType mType;

};


#endif // APERTUREPRIMITIVE_H
