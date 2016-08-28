#include "cairogerberviewer.h"

#include <math.h>



void CairoGerberViewer::drawRegion(GraphicObjectRegion *inRegion)
{
    Point p0,p1;

    if(!isViewerReady()){ return; }

    if (inRegion == NULL){ return; }

    if(!inRegion->isValid()) {
        err_printf("ERROR(CairoGerberViewer::drawRegion): The region is invalid");
        return;
    }


    vector<GraphicObjectRegion::Contour *> contours = inRegion->getContours();

    for(vector<GraphicObjectRegion::Contour *>::iterator cit = contours.begin(); cit != contours.end(); ++cit){
        GraphicObjectRegion::Contour *c = *cit;
        vector<IGraphicObject *> segments = c->getSegments();

        if(segments.size() < 2){
            err_printf("ERROR(CairoGerberViewer::drawRegion): The contour doesn't have enough tracks");
            continue;
        }


        IGraphicObject* first_seg = segments.at(0);
        if(first_seg->getType() != IGraphicObject::eTypeLine && first_seg->getType() != IGraphicObject::eTypeArc){
            err_printf("ERROR(CairoGerberViewer::drawRegion): The first seg is bad");
            continue;
        }

        IGraphicObjectTrack *first_track = GraphicObjectRegion::Contour::convert2Track(first_seg);
        p0 = first_track->getStartPoint();
        p1 = first_track->getStartPoint(); //allows to pass the first test of connection
        cairo_move_to(mContext, r2p_X(p0.mX), r2p_Y(p0.mY));

        for(vector<IGraphicObject *>::iterator sit = segments.begin(); sit != segments.end(); ++sit){
            IGraphicObject *igo = *sit;

            switch(igo->getType()){
                case IGraphicObject::eTypeArc:{

                    break;}
                case IGraphicObject::eTypeLine:{
                    GraphicObjectDraw *seq_draw = static_cast<GraphicObjectDraw *>(igo);

                    p0 = seq_draw->getStartPoint();

                    if(p1.mX != p0.mX || p1.mY != p0.mY){
                        err_printf("ERROR(CairoGerberViewer::drawRegion): The tracks are not connected");
                    }

                    p1 = seq_draw->getEndPoint();


                    cairo_line_to(mContext, r2p_X(p1.mX), r2p_Y(p1.mY));

                    break;}
                default:
                    err_printf("ERROR(CairoGerberViewer::drawRegion): the graphic object in the contour is not a draw or an arc");
                    break;
            }
        }


        cairo_fill(mContext);
    }

}
