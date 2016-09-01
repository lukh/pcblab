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
        enum eProportionMode{
            eKeepProportion,
            eAdjustToViewer
        };


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

        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;

        virtual bool isViewerReady() const = 0;

        /// update the proportion mode keep/adjust proportion
        void setProportionMode(eProportionMode inMode) { mPropMode = inMode; }

        /// zoom to fit the area given in the real world coordinates into the surface
        void setRenderTransformation(const Rectangle &inRealWorldArea) { mRealWorldArea = inRealWorldArea; }

        /// zoom to fit the area given in the real world coordinates into the surface
        void setRenderTransformation(Point p1, Point p2) {
            Rectangle r(p1, p2);
            setRenderTransformation(r);
        }

        /// returns the (real world) view area
        const Rectangle &getRealWorldViewArea() const { return mRealWorldArea; }

        /// converts a point from the render view (the image) to the real world coord
        virtual Point getPointInRealWorldCoordinates(Point inImgCoord) const = 0;

    protected:
        ColorList mColorList;
        Rectangle mRealWorldArea;

        eProportionMode mPropMode;
};

#endif
