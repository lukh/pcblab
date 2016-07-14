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
        virtual void drawLayer(const GerberLayer &inLayer) = 0;

        virtual uint32_t getWidth() = 0;
        virtual uint32_t getHeight() = 0;

    protected:

        /// Converts a point from real to the surface
        /*uint32_t r2p_X(double inX){
            return r2p(inX) + mPixOffsetX;
        }

        uint32_t r2p_Y(double inY){
            return (getHeight() - r2p(inY)) + mPixOffsetY;
        }


        uint32_t r2p(double inReal) { return (uint32_t)(inReal*mPixPerUnit); }
        double p2r(uint32_t inPix) { return (double)inPix/mPixPerUnit; }

        double mPixPerUnit;*/

        //uint32_t mPixOffsetX;
        //uint32_t mPixOffsetY;
};

#endif
