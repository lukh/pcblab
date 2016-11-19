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


static const uint8_t kDefaultColorListSize = 9;
static const Color kDefaultColorList[kDefaultColorListSize] = {
    //GBO
    Color(127,127,0),

    Color(0,255,0),
    Color(0,127,0),

    Color(0,0,255),
    Color(255,0,0),

    Color(255,0,255),
    Color(127,0,127),

    Color(255,255,0),

    Color(255,255,255),
};

class IGerberRenderer{
    public:
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
        IGerberRenderer() {}
        virtual ~IGerberRenderer() {}

        virtual void drawAll(const GerberHandler &inGerber) = 0;
        virtual void drawLayer(string inIdentifier, const GerberLayer *inLayer) = 0;


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

        //others
        vector<IGraphicObject *> mHilightedObjects;
};


#endif
