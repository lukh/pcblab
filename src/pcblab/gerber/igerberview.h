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

#include "gerberhandler.h"
#include "gerberlayer.h"





class IGerberView{
    public:
        IGerberView() /*, mPixOffsetX(0), mPixOffsetY(0) */{}
        virtual ~IGerberView() {}

        virtual void drawAll() = 0;
        virtual void drawLayer(GerberLayer &inLayer) = 0;

        virtual uint32_t getWidth() = 0;
        virtual uint32_t getHeight() = 0;


        /// zoom to fit the area given in the real world coordinates into the surface
        void setRenderTransformation(const Rectangle &inRealWorldArea) { mRealWorldArea = inRealWorldArea; }

    protected:

        /// Converts a point from real to the surface
        inline uint32_t r2p_X(double inX){
            return inX;
        }

        inline uint32_t r2p_Y(double inY){
            return (getHeight() - inY);
        }

        inline double r2p_angle(double inAngle){
            return -inAngle;
        }


        Rectangle mRealWorldArea;
};

#endif
