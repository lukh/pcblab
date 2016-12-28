#ifndef IVIEWPROCESSOR_H
#define IVIEWPROCESSOR_H

#include "pcblab/pcblab.h"
#include "cairogerberrenderer.h" //SHOULD BE THE INTERFACE
#include "cairoexcellonrenderer.h" //SHOULD BE THE INTERFACE
#include "cairocomponentrenderer.h" //SHOULD BE THE INTERFACE
#include "caironetlistrenderer.h" //SHOULD BE INTERFACED

class IViewProcessor
{
    public:
        IViewProcessor(PcbLab &inPcb);
        virtual ~IViewProcessor() {}

        // accessors
        const CairoGerberRenderer &getGerberRenderer() { return mGerberRenderer; }
        const CairoExcellonRenderer &getExcellonRenderer() { return mExcellonRenderer; }
        CairoComponentRenderer &getComponentRenderer() { return mComponentRenderer; }
        const CairoNetlistRenderer &getNetlistRenderer() { return mNetlistRenderer; }

        /// update layer color
        void updateLayerColor(string inIdentifier, Color inColor);

        /// update layer transparency
        void updateLayerTransparency(string inIdentifier, uint8_t inTransp);

        /// display a component
        void displayComponent(string inDesignator);

        /// display a net
        void displayNet(string inNet);


        //<<< --- Interface of IViewProcessor
        /// repaint all the surface
        virtual void refresh() = 0;

        /// convert coords from image (pixels) to reality (mm/in)
        virtual plPoint convertCoordsFromImageToReal(plPoint inImgCoords) = 0;
        // --- >>>


    protected:
        /// Init the 2D surface and connect the renderers
        void setup(uint32_t inWidth, uint32_t inHeight);


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
        CairoComponentRenderer mComponentRenderer;
        CairoNetlistRenderer mNetlistRenderer;
};

#endif // IVIEWPROCESSOR_H
