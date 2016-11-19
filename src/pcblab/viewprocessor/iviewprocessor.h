#ifndef IVIEWPROCESSOR_H
#define IVIEWPROCESSOR_H

#include "pcblab/pcblab.h"
#include "cairogerberrenderer.h" //SHOULD BE THE INTERFACE
#include "cairoexcellonrenderer.h" //SHOULD BE THE INTERFACE


class IViewProcessor
{
    public:
        IViewProcessor(PcbLab &inPcb);
        virtual ~IViewProcessor() {}

        // accessors
        const CairoGerberRenderer &getGerberRenderer() { return mGerberRenderer; }
        const CairoExcellonRenderer &getExcellonRenderer() { return mExcellonRenderer; }

        /// update layer color
        void updateLayerColor(string inIdentifier, Color inColor);

        /// update layer transparency
        void updateLayerTransparency(string inIdentifier, uint8_t inTransp);


        void setup(uint32_t inWidth, uint32_t inHeight);


        //<<< --- Interface of IViewProcessor
        /// repaint all the surface
        virtual void refresh() = 0;



        /// related to zoom
        virtual void zoom(bool inZoomIn, plPoint inPoint) = 0;

        /// move regarding the delta, in the surface coordinate
        virtual void move(double inDx, double inDy) = 0;


        /// convert coords from image (pixels) to reality (mm/in)
        virtual plPoint convertCoordsFromImageToReal(plPoint inImgCoords) = 0;
        // --- >>>


    protected:
        /// compute the new area after a zoom
        /// inPoint is the center of the zoom
        /// inRect is the original rect
        static plRectangle calculateZoom(double inZoomFactor, plPoint inPoint, plRectangle inRect);

        /// compute the new area after a move
        static plRectangle calculateMove(double inDx, double inDy, plRectangle inRect);


    protected:
        PcbLab &mPcb;

        ICairoViewer mViewer;

        CairoGerberRenderer mGerberRenderer;
        CairoExcellonRenderer mExcellonRenderer;
};

#endif // IVIEWPROCESSOR_H
