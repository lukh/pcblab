/*******************************************************************************
* Module: IGerberView
*
* Project: PcbLab
*
* Author: Vivien HENRY
*
* Date: 2016/03/13
*******************************************************************************/

#ifndef IGERBERVIEW_H
#define IGERBERVIEW_H

#include <map>
#include <vector>
#include <string>

#include "gerberhandler.h"
#include "gerberlayer.h"



using namespace std;

class IGerberView{
    public:
        /// links the transparency against the layers name
        typedef map<string, uint8_t> TransparencyMap;

        enum eProportionMode{
            eKeepProportion,
            eAdjustToViewer
        };


        class ColorList{
            public:
                ColorList(): mCurrentColorIdx(0){
                    mColorList.push_back(Color(255,0,0));
                    mColorList.push_back(Color(127,127,255));
                    mColorList.push_back(Color(255,0,255));
                    mColorList.push_back(Color(0,255,0));
                    mColorList.push_back(Color(0,255,255));
                    mColorList.push_back(Color(0,0,255));
                }

                /// get the current color
                const Color &getCurrentColor() const { return mColorList[mCurrentColorIdx]; }

                /// increments the current color, and restart to the first if the end is reached
                void increment() { mCurrentColorIdx = (mCurrentColorIdx+1) % mColorList.size(); }

                /// reset the current idx
                void reset() { mCurrentColorIdx = 0; }

            private:
                vector<Color> mColorList;
                uint32_t mCurrentColorIdx;
        };


    public:
        IGerberView(): mPropMode(eKeepProportion) {}
        virtual ~IGerberView() {}

        virtual void drawAll(const GerberHandler &inGerber) = 0;
        virtual void drawLayer(const GerberLayer *inLayer) = 0;


        /// update the proportion mode keep/adjust proportion
        void setProportionMode(eProportionMode inMode) { mPropMode = inMode; }

        /// zoom to fit the area given in the real world coordinates into the surface
        void setRenderTransformation(const plRectangle &inRealWorldArea) { mRealWorldArea = inRealWorldArea; }

        /// zoom to fit the area given in the real world coordinates into the surface
        void setRenderTransformation(plPoint p1, plPoint p2) {
            plRectangle r(p1, p2);
            setRenderTransformation(r);
        }

        /// returns the (real world) view area
        const plRectangle &getRealWorldViewArea() const { return mRealWorldArea; }

        /// converts a point from the render view (the image) to the real world coord
        virtual plPoint getPointInRealWorldCoordinates(plPoint inImgCoord) const = 0;

        /// converts a vector from the render view (the image) to the real world coord
        virtual void getVectorInRealWorldCoordinates(double *inDx, double *inDy) const = 0;



        /// set the alpha channel for a layer.
        void setAlphaChannel(string inLayerName, uint8_t inAlphaValue) { mTransparencyMap[inLayerName] = inAlphaValue; }


    private:
        virtual void setLevelPolarity(GraphicState::eLevelPolarity inPol) = 0;

        virtual void drawDraw(GraphicObjectDraw *inDraw) = 0;
        virtual void drawArc(GraphicObjectArc *inArc) = 0;
        virtual void drawRegion(GraphicObjectRegion *inRegion) = 0;

        virtual void drawFlash(GraphicObjectFlash *inFlash) = 0;
        virtual void drawAperturePrimitive(IAperturePrimitive *inPrim) = 0;

        virtual void drawAperturePrimitive_Circle(APrimCircle *inCircle) = 0;
        virtual void drawAperturePrimitive_VectorLine(APrimVectorLine *inLine) = 0;
        virtual void drawAperturePrimitive_CenterLine(APrimCenterLine *inLine) = 0;
        virtual void drawAperturePrimitive_Outline(APrimOutline *inOutline) = 0;
        virtual void drawAperturePrimitive_Polygon(APrimPolygon *inPoly) = 0;
        virtual void drawAperturePrimitive_Moire(APrimMoire *inMoire) = 0;
        virtual void drawAperturePrimitive_Thermal(APrimThermal *inThermal) = 0;


        /// changes the exposure of the primitive
        virtual void setApertureExposure(IAperturePrimitive::eExposure inExposure) = 0;

        /// update the actual context with a rotation in degree for the primitive
        virtual void setApertureRotation(double inAngle) = 0;

    protected:
        ColorList mColorList;
        TransparencyMap mTransparencyMap;

        ///defines the area rendered on the surface
        plRectangle mRealWorldArea;

        eProportionMode mPropMode;
};

#endif
