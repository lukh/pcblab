#ifndef CAIRONETLISTRENDERER_H
#define CAIRONETLISTRENDERER_H

#include "cairo/cairo.h"

#include "icairorenderer.h"
#include "inetlistrenderer.h"


class CairoNetlistRenderer: public INetlistRenderer, public ICairoRenderer
{
    public:
        CairoNetlistRenderer();
        virtual ~CairoNetlistRenderer();

        //<<< --- Interface of INetlistRenderer
        virtual void draw(const NetlistHandler &inNetlistHandler);
        virtual void drawNet(const Net &inNet);
        //--- >>>
};

#endif // CAIRONETLISTRENDERER_H
