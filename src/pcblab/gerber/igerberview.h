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
        class ColorList{
            public:
                ColorList(): mCurrentColorIdx(0){
                    mColorList.push_back(Color(255,0,0));
                    mColorList.push_back(Color(255,255,0));
                    mColorList.push_back(Color(255,0,255));
                    mColorList.push_back(Color(0,255,0));
                    mColorList.push_back(Color(0,255,255));
                    mColorList.push_back(Color(0,0,255));
                }

                /// get the current color
                const Color &getCurrentColor() const { return mColorList[mCurrentColorIdx]; }

                /// increments the current color, and restart to the first if the end is reached
                void increment() { mCurrentColorIdx = (mCurrentColorIdx+1) % mColorList.size(); }

            private:
                vector<Color> mColorList;
                uint32_t mCurrentColorIdx;
        };


    public:
        IGerberView() /*, mPixOffsetX(0), mPixOffsetY(0) */{}
        virtual ~IGerberView() {}

        virtual void drawAll(const GerberHandler &inGerber) = 0;
        virtual void drawLayer(const GerberLayer *inLayer) = 0;

        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;

    protected:

        /// Converts a point from real to the surface
        inline uint32_t r2p_X(double inX) const{
            return inX;
        }

        inline uint32_t r2p_Y(double inY) const {
            return (getHeight() - inY);
        }

        inline double r2p_angle(double inAngle) const{
            return -inAngle;
        }


        /*uint32_t r2p(double inReal) { return (uint32_t)(inReal*mPixPerUnit); }
        double p2r(uint32_t inPix) { return (double)inPix/mPixPerUnit; }

        double mPixPerUnit;*/

        //uint32_t mPixOffsetX;
        //uint32_t mPixOffsetY;
};

#endif
