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


        eType getType() const { return mType; }


        /// abstract method used to define if the primitive is valid.
        /// implementation can count the number of modifiers for instance
        virtual bool isValid() const = 0;

        /// This implementation of getBounding box returns the box relative to the center of the aperture !
        virtual Rectangle getBoundingBox() = 0;


        virtual string getStringInfos() const = 0;


    protected:
        /// Gets the bounding box of a rectangle inRect, rotated by a angle of inAngle (deg) regarding (0,0) point
        static Rectangle rotateBoundingBox(Rectangle inRect, double inAngle);

    protected:
        vector <ApertureModifier> mModifiers;

    private:
        eType mType;

};




class APrimCircle: public IAperturePrimitive{
    public:
        APrimCircle();

        inline eExposure getExposure() const { if(!isValid()){ return eExposureOff; } return (mModifiers.at(0) == 1.0) ? eExposureOn : eExposureOff; }
        inline double getDiameter() const { if (!isValid()){return 0.0;} return mModifiers.at(1); }
        inline double getX() const { if (!isValid()){return 0.0;} return mModifiers.at(2); }
        inline double getY() const { if (!isValid()){return 0.0;} return mModifiers.at(3); }
        inline double getRot() const { if (!isValid() || mModifiers.size() < 5){return 0.0;} return mModifiers.at(4); }

        //<<< --- Implementation of IAperturePrimitive
        virtual bool isValid() const;
        virtual string getStringInfos() const{
            if(!isValid()) { return "INVALID"; }
            return "Circle: Exp="+ to_string(int(getExposure())) +" , D="+ to_string(getDiameter()) +", X=" + to_string(getX()) + ", Y=" + to_string(getY()) + ", rot=" + to_string(getRot());
        }
        virtual Rectangle getBoundingBox();
        //--- >>>
};

class APrimVectorLine: public IAperturePrimitive{
    public:
        APrimVectorLine();

        inline eExposure getExposure() const { if(!isValid()){ return eExposureOff; } return (mModifiers.at(0) == 1.0) ? eExposureOn : eExposureOff; }
        inline double getWidth()  const { if (!isValid()){return 0.0;} return mModifiers.at(1); }
        inline double getStartX() const { if (!isValid()){return 0.0;} return mModifiers.at(2); }
        inline double getStartY() const { if (!isValid()){return 0.0;} return mModifiers.at(3); }
        inline double getEndX()   const { if (!isValid()){return 0.0;} return mModifiers.at(4); }
        inline double getEndY()   const { if (!isValid()){return 0.0;} return mModifiers.at(5); }
        inline double getRot()    const { if (!isValid()){return 0.0;} return mModifiers.at(6); }

        //<<< --- Implementation of IAperturePrimitive
        virtual bool isValid() const;
        virtual string getStringInfos() const {
            if(!isValid()) { return "INVALID"; }
            return "VectorLine: Exp="+ to_string(int(getExposure())) +" , StartX="+ to_string(getStartX()) +", StartY=" + to_string(getStartY()) + ", EndX=" + to_string(getEndX()) + ", EndY=" + to_string(getEndY()) + ", Rot=" + to_string(getRot());
        }
        virtual Rectangle getBoundingBox();
        //--- >>>
};

class APrimCenterLine: public IAperturePrimitive{
    public:
        APrimCenterLine();

        inline eExposure getExposure() const { if(!isValid()){ return eExposureOff; } return (mModifiers.at(0) == 1.0) ? eExposureOn : eExposureOff; }
        inline double getWidth()  const { if (!isValid()){return 0.0;} return mModifiers.at(1); }
        inline double getHeight() const { if (!isValid()){return 0.0;} return mModifiers.at(2); }
        inline double getX()      const { if (!isValid()){return 0.0;} return mModifiers.at(3); }
        inline double getY()      const { if (!isValid()){return 0.0;} return mModifiers.at(4); }
        inline double getRot()    const { if (!isValid()){return 0.0;} return mModifiers.at(5); }

        //<<< --- Implementation of IAperturePrimitive
        virtual bool isValid() const;
        virtual string getStringInfos() const {
            if(!isValid()) { return "INVALID"; }
            return "CenterLine: Exp="+ to_string(int(getExposure())) +" , W="+ to_string(getWidth()) +", H=" + to_string(getHeight()) + ", X=" + to_string(getX()) + ", Y=" + to_string(getY()) + ", Rot=" + to_string(getRot());
        }
        virtual Rectangle getBoundingBox();
        //--- >>>
};

class APrimOutline: public IAperturePrimitive{
    public:
        APrimOutline();

        inline eExposure getExposure() const { if(!isValid()){ return eExposureOff; } return (mModifiers.at(0) == 1.0) ? eExposureOn : eExposureOff; }

        /// returns the number of subseq points, between 1 and N
        inline uint16_t getSubSequentPointsCount() const { if (!isValid()){return 0.0;} return (uint16_t)mModifiers.at(1); }

        ///subsequent point, inN = [0; N[
        inline Point getPoint(uint16_t inN) const {
            if (!isValid()){return Point();}

            if(inN >= getSubSequentPointsCount()){return Point();}

            return Point(mModifiers.at(3+2*inN), mModifiers.at(4+2*inN));
        }

        inline double getRot() const { if (!isValid()){return 0.0;} return mModifiers.at(5+2*mModifiers.at(1)); }


        //<<< --- Implementation of IAperturePrimitive
        virtual bool isValid() const;
        virtual string getStringInfos() const {
            if(!isValid()) { return "INVALID"; }
            return "PrimitiveOutLine: Exp="+ to_string(int(getExposure())) + \
                    " , SubSeqs="+ to_string(getSubSequentPointsCount()) + \
                    ", Rot=" + to_string(getRot());
        }
        virtual Rectangle getBoundingBox();
        //--- >>>
};

class APrimPolygon: public IAperturePrimitive{
    public:
        APrimPolygon();

        inline eExposure getExposure() const { if(!isValid()){ return eExposureOff; } return (mModifiers.at(0) == 1.0) ? eExposureOn : eExposureOff; }
        inline double getVerticesCount() const { if (!isValid()){return 0.0;} return mModifiers.at(1); }
        inline double getX()             const { if (!isValid()){return 0.0;} return mModifiers.at(2); }
        inline double getY()             const { if (!isValid()){return 0.0;} return mModifiers.at(3); }
        inline double getDiameter()      const { if (!isValid()){return 0.0;} return mModifiers.at(4); }
        inline double getRot()           const { if (!isValid()){return 0.0;} return mModifiers.at(5); }

        //<<< --- Implementation of IAperturePrimitive
        virtual bool isValid() const;
        virtual string getStringInfos() const {
            if(!isValid()) { return "INVALID"; }
            return "Polygon: Exp="+ to_string(int(getExposure())) +" , Vertices="+ to_string(getVerticesCount()) +", X=" + to_string(getX()) + ", Y=" + to_string(getY()) + ", Dia=" + to_string(getDiameter()) + ", rot=" + to_string(getRot());
        }
        virtual Rectangle getBoundingBox();
        //--- >>>
};

class APrimMoire: public IAperturePrimitive{
    public:
        APrimMoire();


        //X coordinate of center point, a decimal
        inline double getX()             const { if (!isValid()){return 0.0;} return mModifiers.at(0); }

        //Y coordinate of center point, a decimal
        inline double getY()             const { if (!isValid()){return 0.0;} return mModifiers.at(1); }

        //Outer diameter of outer concentric ring, a decimal ≥ 0
        inline double getOuterDiaOfOutRing() const { if (!isValid()){return 0.0;} return mModifiers.at(2); }

        //Ring thickness, a decimal ≥ 0
        inline double getRingThickness() const { if (!isValid()) { return 0.0; } return mModifiers.at(3); }

        //Gap between rings, a decimal ≥ 0
        inline double getGapBtwRings() const { if (!isValid()) { return 0.0; } return mModifiers.at(4); }

        //Maximum number of rings, an integer ≥ 0
        inline double getMaxNumOfRings() const { if (!isValid()) { return 0.0; } return mModifiers.at(5); }

        //Cross hair thickness, a decimal ≥ 0
        inline double getCrossHaireThickness() const { if (!isValid()) { return 0.0; } return mModifiers.at(6); }

        //Cross hair length, a decimal ≥ 0
        inline double getCrossHairLength() const { if (!isValid()) { return 0.0; } return mModifiers.at(7); }

        //Rotation angle of the moiré primitive
        //The rotation angle is specified by a decimal, in degrees. The primitive is rotated around the origin of the macro definition, i.e. the (0, 0) point of macro coordinates
        //Note: Rotation is only allowed if the primitive center point coincides with the origin of the macro definition
        inline double getRot() const { if (!isValid()) { return 0.0; } return mModifiers.at(8); }


        //<<< --- Implementation of IAperturePrimitive
        virtual bool isValid() const;
        virtual string getStringInfos() const {
            if(!isValid()) { return "INVALID"; }
            return "Moire";
        }
        virtual Rectangle getBoundingBox();
        //--- >>>
};

class APrimThermal: public IAperturePrimitive{
    public:
        APrimThermal();


        //X coordinate of center point, a decimal
        inline double getX()             const { if (!isValid()){return 0.0;} return mModifiers.at(0); }

        //Y coordinate of center point, a decimal
        inline double getY()             const { if (!isValid()){return 0.0;} return mModifiers.at(1); }

        //Outer diameter, a decimal > inner diameter
        inline double getOuterDia()      const { if (!isValid()){return 0.0;} return mModifiers.at(2); }

        //Inner diameter, a decimal ≥ 0
        inline double getInnerDia()      const { if (!isValid()){return 0.0;} return mModifiers.at(3); }

        //Gap thickness, a decimal < (outer diameter)/sqrt(2)
        inline double getGapThickness()      const { if (!isValid()){return 0.0;} return mModifiers.at(4); }

        //Rotation angle of the thermal primitive
        //The rotation angle is specified by a decimal, in degrees. The primitive is rotated around the origin of the macro definition, i.e. (0, 0) point of macro coordinates. The gaps are on the X and Y axes through the center when the rotation angle is zero
        //Note: Rotation is only allowed if the pri
        inline double getRot()      const { if (!isValid()){return 0.0;} return mModifiers.at(5); }


        //<<< --- Implementation of IAperturePrimitive
        virtual bool isValid() const;
        virtual string getStringInfos() const {
            if(!isValid()) { return "INVALID"; }
            return "Thermal";
        }
        virtual Rectangle getBoundingBox();
        //--- >>>
};


#endif // APERTUREPRIMITIVE_H
