#ifndef CAIROCOMPONENTRENDERER_H
#define CAIROCOMPONENTRENDERER_H

#include "cairo/cairo.h"

#include "icairorenderer.h"
#include "icomponentrenderer.h"


class CairoComponentRenderer: public IComponentRenderer, public ICairoRenderer
{
    public:
        CairoComponentRenderer();
        virtual ~CairoComponentRenderer();

        //<<< --- Interface of IExcellon renderer
        virtual void draw(const ComponentHandler &inComponentHandler);
        virtual void drawComponent(const Component &inCompo);
        //--- >>>
};

#endif // CAIROCOMPONENTRENDERER_H
