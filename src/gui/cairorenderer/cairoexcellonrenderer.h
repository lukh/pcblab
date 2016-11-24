#ifndef CAIROEXCELLONRENDERER_H
#define CAIROEXCELLONRENDERER_H

#include "iexcellonrenderer.h"
#include "icairorenderer.h"

class CairoExcellonRenderer: public IExcellonRenderer, public ICairoRenderer
{
    public:
        CairoExcellonRenderer();
        virtual ~CairoExcellonRenderer() {}

        //<<< --- Interface of IExcellon renderer
        virtual void draw(ExcellonHandler &inExcellonHandler);
        //--- >>>
};

#endif // CAIROEXCELLONRENDERER_H
