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
#include <algorithm>

#include "gerberhandler.h"
#include "gerberlayer.h"



using namespace std;


static const uint8_t kDefaultColorListSize = 6;
static const Color kDefaultColorList[kDefaultColorListSize] = {
    Color(255,0,0),
    Color(127,127,255),
    Color(255,0,255),
    Color(0,255,0),
    Color(0,255,255),
    Color(0,0,255)
};

class IGerberView{
    public:
        enum eProportionMode{
            eKeepProportion,
            eAdjustToViewer
        };

        /// Contains the settings for a layer
        class GraphicSettings{
            public:
                typedef uint8_t Transparency;

            public:
                GraphicSettings(): mTransparency(255) {
                    mColor = kDefaultColorList[sColorIdx];
                }

                GraphicSettings(Color inColor, Transparency inTransparency = 255): mColor(inColor), mTransparency(inTransparency){}

                static void reset() { sColorIdx = 0; }
                static void increment() { sColorIdx = (sColorIdx+1) % kDefaultColorListSize; }

                Color mColor;
                Transparency mTransparency;

            private:
                static uint32_t sColorIdx;
        };

        typedef map<string, GraphicSettings> GraphicSettingsMap;


    public:
        IGerberView(): mPropMode(eKeepProportion) {}
        virtual ~IGerberView() {}

        virtual void drawAll(const GerberHandler &inGerber) = 0;
        virtual void drawLayer(string inIdentifier, const GerberLayer *inLayer) = 0;


        /// update the proportion mode keep/adjust proportion
        void setProportionMode(eProportionMode inMode) { mPropMode = inMode; }

        /// zoom to fit the area given in the real world coordinates into the surface
        void setRenderTransformation(const plRectangle &inRealWorldArea) { mRealWorldArea = inRealWorldArea; }

        /// zoom to fit the area given in the real world coordinates into the surface
        void setRenderTransformation(plPoint p1, plPoint p2);

        /// returns the (real world) view area
        const plRectangle &getRealWorldViewArea() const { return mRealWorldArea; }

        /// converts a point from the render view (the image) to the real world coord
        virtual plPoint getPointInRealWorldCoordinates(plPoint inImgCoord) const = 0;

        /// converts a vector from the render view (the image) to the real world coord
        virtual void getVectorInRealWorldCoordinates(double *inDx, double *inDy) const = 0;


        // --------------- Modification --------------

        /// set the alpha channel for a layer.
        void setAlphaChannel(string inLayerIdentifier, uint8_t inAlphaValue);

        /// set the color for a layer.
        void setColor(string inLayerIdentifier, Color inColor);

        /// update the list og hilighted objects
        void setHilightedObjects(vector<IGraphicObject *> inHilightedObjects) { mHilightedObjects = inHilightedObjects; }

        /// clear the hilighted objects list
        void clearHilightedObjects() { mHilightedObjects.clear(); }

        // ------------ Accessors -----------------
        GraphicSettings getGraphicSettings(string inLayerIdentifier, bool &outStatus) const;


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
        //globals
        GraphicSettingsMap mGraphicSettingsMap;

        ///defines the area rendered on the surface
        plRectangle mRealWorldArea;

        eProportionMode mPropMode;


        //others
        vector<IGraphicObject *> mHilightedObjects;
};


#endif
