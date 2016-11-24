#ifndef IEXCELLONRENDERER_H
#define IEXCELLONRENDERER_H

#include "excellonhandler.h"
#include "pcblab/common.h"

class IExcellonRenderer{
    public:
        IExcellonRenderer(): mColor(255,255,255), mTransparency(255){}
        virtual ~IExcellonRenderer() {}

        /// draws the ExcellonHandler given
        virtual void draw(ExcellonHandler &inExcellonHandler) = 0;


        /// changes the color
        void setColor(Color inColor){ mColor = inColor; }

        /// changes transparency
        void setTransparency(uint8_t inTransp){ mTransparency = inTransp;}



        Color getColor() const { return mColor; }

        uint8_t getTransparency() const { return mTransparency; }

    protected:
        Color mColor;
        uint8_t mTransparency;
};

#endif // IEXCELLONRENDERER_H


