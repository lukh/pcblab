/*******************************************************************************
* Module: GraphicObject
*
* Project: PcbLab
*
* Author: Vivien HENRY
*
* Date: 2016/03/10
*******************************************************************************/

#ifndef GRAPHICOBJECT_H
#define GRAPHICOBJECT_H


#include <stdint.h>
#include "../common.h"
#include "aperture/aperture.h"
#include "igerberview.h"


/// Interface for GraphicObject.
/// Informations such as polarity and levels are stored in their containers
class IGraphicObject{
    public:
        enum eType{
            eTypeLine,
            eTypeArc,
            eTypeFlash,
            eTypeRegion,

            eTypeNone
        };


        IGraphicObject(): mType(eTypeNone) {}
        IGraphicObject(eType inType): mType(inType) {}

        virtual void draw(IGerberView *inView) = 0;

    protected:
        Point mStartPoint;
        Aperture *mAperture; //should it be DCode ?



    private:
        eType mType;
};


class GraphicObjectDraw: public IGraphicObject{
    public:
        GraphicObjectDraw(): IGraphicObject(IGraphicObject::eTypeLine) {}

        virtual void draw(IGerberView *inView);

    private:
        Point mEndPoint;
};



class GraphicObjectArc: public IGraphicObject{
    public:
        GraphicObjectArc(): IGraphicObject(IGraphicObject::eTypeLine) {}

        virtual void draw(IGerberView *inView);

    private:
        Point mEndPoint;
};

class GraphicObjectFlash: public IGraphicObject{
    public:
        GraphicObjectFlash(): IGraphicObject(IGraphicObject::eTypeFlash) {}

        virtual void draw(IGerberView *inView);
};

class GraphicObjectRegion: public IGraphicObject{
    public:
        GraphicObjectRegion(): IGraphicObject(IGraphicObject::eTypeRegion) {}


        virtual void draw(IGerberView *inView);
};

#endif
