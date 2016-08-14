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
        virtual ~IAperturePrimitive() {
#ifdef DEBUG_PRINT
            d_printf("%%% Deleting IAperturePrimitive", 4, 0, false);
#endif
}


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


        eType getType() const { return mType; }


        /// abstract method used to define if the primitive is valid.
        /// implementation can count the number of modifiers for instance
        virtual bool isValid() = 0;


        virtual string getStringInfos() = 0;


    protected:
        vector <ApertureModifier> mModifiers;

    private:
        eType mType;

};




class APrimCircle: public IAperturePrimitive{
    public:
        APrimCircle();

        inline double getDiameter() { if (!isValid()){return 0.0;} return mModifiers.at(1); }
        inline double getX() { if (!isValid()){return 0.0;} return mModifiers.at(2); }
        inline double getY() { if (!isValid()){return 0.0;} return mModifiers.at(3); }
        inline double getRot() { if (!isValid()){return 0.0;} return mModifiers.at(4); }

        virtual bool isValid();
        virtual string getStringInfos() {
            if(!isValid()) { return "INVALID"; }
            return "Circle: Exp="+ to_string(int(getExposure())) +" , D="+ to_string(getDiameter()) +", X=" + to_string(getX()) + ", Y=" + to_string(getY()) + ", rot=" + to_string(getRot());
        }
};

class APrimVectorLine: public IAperturePrimitive{
    public:
        APrimVectorLine();

        inline double getWidth() { if (!isValid()){return 0.0;} return mModifiers.at(1); }
        inline double getStartX() { if (!isValid()){return 0.0;} return mModifiers.at(2); }
        inline double getStartY() { if (!isValid()){return 0.0;} return mModifiers.at(3); }
        inline double getEndX()   { if (!isValid()){return 0.0;} return mModifiers.at(4); }
        inline double getEndY()   { if (!isValid()){return 0.0;} return mModifiers.at(5); }
        inline double getRot()    { if (!isValid()){return 0.0;} return mModifiers.at(6); }

        virtual bool isValid();
        virtual string getStringInfos() {
            if(!isValid()) { return "INVALID"; }
            return "VectorLine: Exp="+ to_string(int(getExposure())) +" , StartX="+ to_string(getStartX()) +", StartY=" + to_string(getStartY()) + ", EndX=" + to_string(getEndX()) + ", EndY=" + to_string(getEndY()) + ", Rot=" + to_string(getRot());
        }
};

class APrimCenterLine: public IAperturePrimitive{
    public:
        APrimCenterLine();

        inline double getWidth() { if (!isValid()){return 0.0;} return mModifiers.at(1); }
        inline double getHeight() { if (!isValid()){return 0.0;} return mModifiers.at(2); }
        inline double getX() { if (!isValid()){return 0.0;} return mModifiers.at(3); }
        inline double getY()   { if (!isValid()){return 0.0;} return mModifiers.at(4); }
        inline double getRot()    { if (!isValid()){return 0.0;} return mModifiers.at(5); }

        virtual bool isValid();
        virtual string getStringInfos() {
            if(!isValid()) { return "INVALID"; }
            return "CenterLine: Exp="+ to_string(int(getExposure())) +" , W="+ to_string(getWidth()) +", H=" + to_string(getHeight()) + ", X=" + to_string(getX()) + ", Y=" + to_string(getY()) + ", Rot=" + to_string(getRot());
        }
};

class APrimOutline: public IAperturePrimitive{
    public:
        APrimOutline();

        inline uint16_t getSubSequentPointsCount() { if (!isValid()){return 0.0;} return (uint16_t)mModifiers.at(1); }
        inline double getStartX() { if (!isValid()){return 0.0;} return mModifiers.at(2); }
        inline double getStartY() { if (!isValid()){return 0.0;} return mModifiers.at(3); }
        ///subsequent point >= 1
        inline double getNX(uint16_t inN) { if (!isValid()){return 0.0;} if(inN < 1 || inN > 4+2*mModifiers.at(1)){return 0;} return mModifiers.at(3+2*inN); }
        ///subsequent point >= 1
        inline double getNY(uint16_t inN) { if (!isValid()){return 0.0;} if(inN < 1 || inN > 4+2*mModifiers.at(1)){return 0;} return mModifiers.at(4+2*inN); }
        inline double getRot(){ if (!isValid()){return 0.0;} return mModifiers.at(5+2*mModifiers.at(1)); }

        virtual bool isValid();
        virtual string getStringInfos() {
            if(!isValid()) { return "INVALID"; }
            return "PrimitiveOutLine: Exp="+ to_string(int(getExposure())) + \
                    " , SubSeqs="+ to_string(getSubSequentPointsCount()) + \
                    ", StartX=" + to_string(getStartX()) + \
                    ", StartY=" + to_string(getStartY()) + \
                    ", Rot=" + to_string(getRot());
        }
};

class APrimPolygon: public IAperturePrimitive{
    public:
        APrimPolygon();

        inline double getVerticesCount() { if (!isValid()){return 0.0;} return mModifiers.at(1); }
        inline double getX() { if (!isValid()){return 0.0;} return mModifiers.at(2); }
        inline double getY() { if (!isValid()){return 0.0;} return mModifiers.at(3); }
        inline double getDiameter() { if (!isValid()){return 0.0;} return mModifiers.at(4); }
        inline double getRot() { if (!isValid()){return 0.0;} return mModifiers.at(5); }

        virtual bool isValid();
        virtual string getStringInfos() {
            if(!isValid()) { return "INVALID"; }
            return "Polygon: Exp="+ to_string(int(getExposure())) +" , Vertices="+ to_string(getVerticesCount()) +", X=" + to_string(getX()) + ", Y=" + to_string(getY()) + ", Dia=" + to_string(getDiameter()) + ", rot=" + to_string(getRot());
        }
};

class APrimMoire: public IAperturePrimitive{
    public:
        APrimMoire();
        virtual bool isValid();
        virtual string getStringInfos() {
            if(!isValid()) { return "INVALID"; }
            return "Moire";
        }
};

class APrimThermal: public IAperturePrimitive{
    public:
        APrimThermal();
        virtual bool isValid();
        virtual string getStringInfos() {
            if(!isValid()) { return "INVALID"; }
            return "Thermal";
        }
};


#endif // APERTUREPRIMITIVE_H
