/*******************************************************************************
* Module: GraphicObject
*
* Project: PcbLab
*
* Author: Vivien HENRY
*
* Date: 2016/03/10
*******************************************************************************/

#include <stdint.h>
#include "commom.h"
#include "aperture/aperture.h"


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


        GraphicObject(): mType(eTypeNone) {}
        GraphicObject(eType inType): mType(inType) {}

    protected:
        Point mStartPoint;
        Aperture *mAperture; //should it be DCode ?



    private:
        eType mType;
};


class GraphicObjectDraw: public IGraphicObject{
    public:
        GraphicObjectDraw(): IGraphicObject(IGraphicObject::eTypeLine);

    private:
        Point mEndPoint;
};



class GraphicObjectArc: public IGraphicObject{
    public:
        GraphicObjectArc(): IGraphicObject(IGraphicObject::eTypeLine);

    private:
        Point mEndPoint;
};

class GraphicObjectFlash: public IGraphicObject{
    public:
        GraphicObjectFlash(): IGraphicObject()IGraphicObject::eTypeFlash);
};

class GraphicObjectRegion: public IGraphicObject{
public:
    GraphicObjectRegion(): IGraphicObject()IGraphicObject::eTypeRegion);
};
