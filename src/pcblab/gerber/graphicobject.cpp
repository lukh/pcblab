#include "graphicobject.h"

/////////////////////////// Draws ///////////////////////////
plRectangle GraphicObjectDraw::getBoundingBox() const
{
    if(!isValid()) {
        err_printf("ERROR (GraphicObjectDraw::getBoundingBox): Invalid GraphicObject");
        return plRectangle();
    }

    // points of the draw
    plPoint p1 = getStartPoint();
    plPoint p2 = getEndPoint();

    // Bounding box of the aperture
    const Aperture *ap = getAperture();
    plRectangle ap_bb;
    if(ap != NULL){
        ap_bb = ap->getBoundingBox();
    }

    plRectangle bb(p1, p2);

    //enlarge
    //this one works if ap_bb is centered !!! (???)
    p2.mX = bb.getTopRight().mX + ap_bb.getTopRight().mX;
    p2.mY = bb.getTopRight().mY + ap_bb.getTopRight().mY;

    p1.mX = bb.getBottomLeft().mX + ap_bb.getBottomLeft().mX;
    p1.mY = bb.getBottomLeft().mY + ap_bb.getBottomLeft().mY;

    return plRectangle(p1, p2);
}



/////////////////////////// Arcs ///////////////////////////

GraphicObjectArc::GraphicObjectArc(plPoint inStartPoint, plPoint inEndPoint, plPoint inCenterOffset, GraphicState::eQuadrantMode inQuadrantMode, GraphicState::eInterpolationMode inInterpolationMode, Aperture *inAperture):
    IGraphicObject(IGraphicObject::eTypeArc, inAperture),
    IGraphicObjectTrack(inStartPoint, inEndPoint),
    mQuadrantMode(inQuadrantMode), mInterpolationMode(inInterpolationMode)
{
    mValid = false;
    if(mInterpolationMode == GraphicState::eInterpolLinear){
        err_printf("ERROR (GraphicObjectArc::GraphicObjectArc): Can't instanciate an arc with an interpolation set to linear...");
    }

    else{

        if(mQuadrantMode == GraphicState::eQuadrantMulti){
            //it's easy here, since the center offset is signed
            mCenter = plPoint(mStartPoint.mX + inCenterOffset.mX, mStartPoint.mY + inCenterOffset.mY);
            mValid = true;
        }

        else if(mQuadrantMode == GraphicState::eQuadrantSingle){
            double signs[2] = {1.0, -1.0};
            vector<plPoint> candidates, winner;

            // get the candidates for center
            for(int i = 0; i < 4; i ++){
                //determine the Center (c) point
                plPoint c( inStartPoint.mX + signs[i&0x1] * std::abs(inCenterOffset.mX), inStartPoint.mY + signs[(i>>1)&0x1] * std::abs(inCenterOffset.mY)  );

                double Rs2c = sqrt( pow(inStartPoint.mX - c.mX, 2) + pow(inStartPoint.mY - c.mY, 2) );
                double Re2c = sqrt( pow(inEndPoint.mX - c.mX, 2) + pow(inEndPoint.mY - c.mY, 2) );

                if(Rs2c == Re2c){

                    //check if the point is not in the candidates list already
                    bool added = false;
                    for(vector<plPoint>::iterator it_p = candidates.begin(); it_p != candidates.end(); ++it_p){
                        plPoint temp = *it_p;
                        if(temp.mX == c.mX && temp.mY == c.mY) {added = true; }
                    }
                    if(!added) { candidates.push_back(c); }
                }
            }

            if(candidates.size() == 0){
                err_printf("GraphicObjectArc/GraphicObjectArc: no candidates founds for the center");
            }

            else{
                // check if the candidates allows the arc to go in the right direction CCW/CW, and if the angle is smaller than 90deg
                for (vector<plPoint>::iterator it_p = candidates.begin(); it_p != candidates.end(); ++it_p){

                    plPoint c = *it_p; //this is the candidate

                    double v0_x = inStartPoint.mX - c.mX;
                    double v0_y = inStartPoint.mY - c.mY;
                    double v1_x = inEndPoint.mX - c.mX;
                    double v1_y = inEndPoint.mY - c.mY;

                    //find angle
                    double dot = v0_x*v1_x + v0_y*v1_y;
                    double cross = v0_x*v1_y - v0_y*v1_x;

                    double angle = std::atan2(cross, dot);

                    if(
                            std::abs(angle) <= M_PI/2 &&
                                ((angle > 0 && mInterpolationMode == GraphicState::eInterpolCCWCircular) ||
                                (angle < 0 && mInterpolationMode == GraphicState::eInterpolCWCircular))
                            )
                    {
                        winner.push_back(c);
                    }
                }


                if (winner.size() == 1){
                    mValid = true;
                    mCenter = winner.at(0);
                }
            }
        }

        else{
            err_printf("GraphicObjectArc/GraphicObjectArc: quadrant mode is undefined or unknown");
        }

    }
#ifdef DEBUG_PRINT
    d_printf("Creating GraphicObjectArc: start =(" + to_string(inStartPoint.mX) + ", " +to_string(inStartPoint.mY) + ") end =(" + to_string(inEndPoint.mX) + ", " +to_string(inEndPoint.mY) + ")", 4, 0, false);
#endif
}

plRectangle GraphicObjectArc::getBoundingBox() const
{
    return plRectangle(); //TODO

}



GraphicState::eQuadrantMode GraphicObjectArc::getQuadrantMode() const
{
    return mQuadrantMode;
}


GraphicState::eInterpolationMode GraphicObjectArc::getInterpolationMode() const
{
    return mInterpolationMode;
}








/////////////////////////// Flashs ///////////////////////////
plRectangle GraphicObjectFlash::getBoundingBox() const
{
    if(!isValid()) {
        err_printf("ERROR (GraphicObjectFlash::getBoundingBox): Invalid GraphicObject");
        return plRectangle();
    }

    plRectangle bb = getAperture()->getBoundingBox();

    plRectangle r(plPoint(mPoint.mX + bb.getBottomLeft().mX, mPoint.mY + bb.getBottomLeft().mY), plPoint(mPoint.mX + bb.getTopRight().mX, mPoint.mY + bb.getTopRight().mY));

    return r;
}



/////////////////////////// Contours ///////////////////////////


/// Adds a linear segment
void GraphicObjectRegion::Contour::addSegment(plPoint inStart, plPoint inStop){
    GraphicObjectDraw *draw = new GraphicObjectDraw(inStart, inStop, NULL);

    mSegments.push_back(draw);
}

/// Adds an arc segment
void GraphicObjectRegion::Contour::addSegment(plPoint inStart, plPoint inStop, plPoint inCenterOffset, GraphicState::eQuadrantMode inQuadrantMode, GraphicState::eInterpolationMode inInterpolationMode){
    GraphicObjectArc *arc = new GraphicObjectArc(inStart, inStop, inCenterOffset, inQuadrantMode, inInterpolationMode, NULL);

    mSegments.push_back(arc);
}

/// checks if the contour is closed
bool GraphicObjectRegion::Contour::isClosed() const{
    IGraphicObjectTrack *track, *last_track = NULL;
    for(vector<IGraphicObject*>::const_iterator it = mSegments.begin(); it != mSegments.end(); ++it){
        //extract the track information
        track = convert2Track((*it));


        if(last_track != NULL){
            if(track->getStartPoint() != last_track->getEndPoint()){
                return false;
            }
        }

        // save previous track
        last_track = track;
    }


    //check the first segment and the last one
    track = convert2Track(mSegments.front());
    if(track->getStartPoint() != last_track->getEndPoint()){
        return false;
    }

    return true;
}

void GraphicObjectRegion::Contour::close()
{
    if(isClosed() || mSegments.size() < 1){
        return;
    }

    IGraphicObject * first = mSegments.front();
    IGraphicObject * last = mSegments.back();

    // check data type
    if(first->getType() != IGraphicObject::eTypeArc && first->getType() != IGraphicObject::eTypeLine && last->getType() != IGraphicObject::eTypeArc && last->getType() != IGraphicObject::eTypeLine){
        err_printf("ERROR: (GraphicObjectRegion::Contour::addClosingSegment): Unexpected GraphicObject in the contour");
        return;
    }

    IGraphicObjectTrack *t_first = convert2Track(first);
    IGraphicObjectTrack *t_last = convert2Track(last);

    // adding the segment
    addSegment(t_last->getEndPoint(), t_first->getStartPoint());
}

bool GraphicObjectRegion::Contour::isValid() const
{
    return isClosed();
}

plRectangle GraphicObjectRegion::Contour::getBoundingBox() const
{
    if(!isValid() || mSegments.size() == 0) {
        err_printf("ERROR (GraphicObjectRegion::Contour::getBoundingBox): Invalid GraphicObject");
        return plRectangle();
    }

    plRectangle bb = mSegments.at(0)->getBoundingBox();

    for (vector<IGraphicObject *>::const_iterator it = mSegments.begin(); it != mSegments.end(); ++it){
        IGraphicObject *igo = *it;
        bb = plRectangle::getBounds(bb, igo->getBoundingBox());
    }

    return bb;
}

/// checks if the Point is in the contour (contour must be closed)
bool GraphicObjectRegion::Contour::isInside(plPoint inPoint) const{
    // a point is inside the contour if: the contour is closed, the point is on the same side of wich track
    if(!isClosed()){
        return false;
    }


    // check positition of the point regarding to the track
    double nx, ny, nxp1, nyp1; //vector coordinates normalized against inPoint
    double w = 0; //winding number
    for(vector<IGraphicObject*>::const_iterator it = mSegments.begin(); it != mSegments.end(); ++it){
        switch((*it)->getType()){
            case IGraphicObject::eTypeLine:{
                GraphicObjectDraw *draw = static_cast<GraphicObjectDraw*>((*it));

                // from https://www.engr.colostate.edu/~dga/dga/papers/point_in_polygon.pdf

                //normalize vector (origin is then inPoint
                nx = draw->getStartPoint().mX - inPoint.mX;
                ny = draw->getStartPoint().mY - inPoint.mY;
                nxp1 = draw->getEndPoint().mX - inPoint.mX;
                nyp1 = draw->getEndPoint().mY - inPoint.mY;

                if(ny*nyp1 < 0) {//vector cross the x axis
                    double r = nx + (ny*(nxp1 - nx)/(ny-nyp1)); //r is the x coord of the intersection of the vector and the x axis
                    if(r > 0){ // vector crosses positive x axis
                        if(ny < 0){w+=1;} else{ w-=1;}
                    }
                }
                else if((ny == 0) && (nx > 0)){ //vi is on the positive axis
                    if(nyp1 > 0) { w+=0.5; } else { w-=0.5; }
                }
                else if ( (nyp1 == 0) && (nxp1 > 0)){ //vi+1 is on the positive x axis
                    if(ny < 0) { w+= 0.5; } else { w-=0.5; }
                }

                break;}
            case IGraphicObject::eTypeArc:{
                /*GraphicObjectArc *arc = static_cast<GraphicObjectArc*>((*it));

                //normalize vector (origin is then inPoint
                nx = arc->getStartPoint().mX - inPoint.mX;
                ny = arc->getStartPoint().mY - inPoint.mY;
                nxp1 = arc->getEndPoint().mX - inPoint.mX;
                nyp1 = arc->getEndPoint().mY - inPoint.mY;

                Point center(arc->getCenter());
                double R = 0;

                if(ny*nyp1 < 0) {//vector cross the x axis

                    //r is the coord of the intersection of the arc and the x axis
                    double rootsign = 1.0;
                    if((ny > nyp1 && arc->getInterpolationMode() == GraphicState::eInterpolCCWCircular) || (ny < nyp1 && arc->getInterpolationMode() == GraphicState::eInterpolCWCircular)){
                        rootsign = -1;
                    }

                    double r = rootsign*sqrt( pow(R, 2) - pow((-center.mY),2) ) - xc;

                    //double r = nx + (ny*(nxp1 - nx)/(ny-nyp1)); //r is the x coord of the intersection of the vector and the x axis
                    //if(r > 0){ // vector crosses positive x axis
                    //    if(ny < 0){w+=1;} else{ w-=1;}
                    //}
                }
                else if((ny == 0) && (nx > 0)){ //vi is on the positive axis
                    if(nyp1 > 0) { w+=0.5; } else { w-=0.5; }
                }
                else if ( (nyp1 == 0) && (nxp1 > 0)){ //vi+1 is on the positive x axis
                    if(ny < 0) { w+= 0.5; } else { w-=0.5; }
                }*/


                break;}

            default:
                // there is something else in the vector, and it shouldn't be
                err_printf("ERROR: (GraphicObjectRegion::Contour::isInside): Unexpected GraphicObject in the contour");
                break;
        }

    }

    return w!=0;
}

bool GraphicObjectRegion::Contour::isCrossing(IGraphicObject *inObject) const
{
    bool ret = false;

    for(vector<IGraphicObject*>::const_iterator it = mSegments.begin(); it != mSegments.end(); ++it){
        switch((*it)->getType()){
            case IGraphicObject::eTypeLine:{
                GraphicObjectDraw *draw = static_cast<GraphicObjectDraw*>((*it));

                if(inObject->getType() == IGraphicObject::eTypeLine){
                    ret = ret || isCrossing(draw, (GraphicObjectDraw*)inObject);
                }
                else if(inObject->getType() == IGraphicObject::eTypeArc){
                    ret = ret || isCrossing((GraphicObjectArc*)inObject, draw);
                }
                else{
                    err_printf("ERROR: (GraphicObjectRegion::Contour::isCrossing): Unexpected tested GraphicObject");
                    break;
                }

                break;}
            case IGraphicObject::eTypeArc:{
                GraphicObjectArc *arc = static_cast<GraphicObjectArc*>((*it));

                if(inObject->getType() == IGraphicObject::eTypeLine){
                    ret = ret || isCrossing(arc, (GraphicObjectDraw*)inObject);
                }
                else if(inObject->getType() == IGraphicObject::eTypeArc){
                    ret = ret || isCrossing(arc, (GraphicObjectArc*)inObject);
                }
                else{
                    err_printf("ERROR: (GraphicObjectRegion::Contour::isCrossing): Unexpected tested GraphicObject");
                    break;
                }


                break;}

            default:
                // there is something else in the vector, and it shouldn't be
                err_printf("ERROR: (GraphicObjectRegion::Contour::isCrossing): Unexpected GraphicObject in the contour");
                break;
        }

    }

    return ret;
}

bool GraphicObjectRegion::Contour::isCrossing(GraphicObjectDraw *inDraw1, GraphicObjectDraw *inDraw2)
{
    return false;
}

bool GraphicObjectRegion::Contour::isCrossing(GraphicObjectArc *inArc, GraphicObjectDraw *inDraw)
{
    return false;
}

bool GraphicObjectRegion::Contour::isCrossing(GraphicObjectArc *inArc1, GraphicObjectArc *inArc2)
{
    return false;
}





/// checks the connection with another contour
GraphicObjectRegion::Contour::eContoursConnection GraphicObjectRegion::Contour::getConnection(const Contour &inContour) const{
    (void)inContour;
}



IGraphicObjectTrack *GraphicObjectRegion::Contour::convert2Track(IGraphicObject *inObject){
    IGraphicObjectTrack *track = NULL;
    switch(inObject->getType()){
        case IGraphicObject::eTypeLine:{
            GraphicObjectDraw *draw = static_cast<GraphicObjectDraw*>(inObject);
            track = static_cast<IGraphicObjectTrack*>(draw);
            break;}
        case IGraphicObject::eTypeArc:{
            GraphicObjectArc *arc = static_cast<GraphicObjectArc*>(inObject);
            track = static_cast<IGraphicObjectTrack*>(arc);
            break;}

        default:
            // there is something else in the vector, and it shouldn't be
            err_printf("ERROR: (GraphicObjectRegion::Contour::isClosed): Unexpected GraphicObject in the contour");
            break;
    }

    return track;
}


GraphicObjectRegion::Contour::~Contour(){
    for (vector<IGraphicObject *>::iterator it = mSegments.begin() ; it != mSegments.end(); ++it){
        delete (*it);
    }
}



GraphicObjectRegion::Contour *GraphicObjectRegion::sContour = NULL;
vector<GraphicObjectRegion::Contour *> GraphicObjectRegion::sContours = vector<GraphicObjectRegion::Contour *> ();












/////////////////////////// Regions ///////////////////////////


GraphicObjectRegion:: ~GraphicObjectRegion(){
    for (vector<Contour *>::iterator it = mContours.begin() ; it != mContours.end(); ++it){
        delete (*it);
    }
}

plRectangle GraphicObjectRegion::getBoundingBox() const
{
    if(!isValid() || mContours.size() == 0) {
        err_printf("ERROR (GraphicObjectRegion::getBoundingBox): Invalid GraphicObject");
        return plRectangle();
    }

    plRectangle bb = mContours.at(0)->getBoundingBox();

    for (vector<Contour *>::const_iterator it = mContours.begin() ; it != mContours.end(); ++it){
        Contour *c = *it;
        bb = plRectangle::getBounds(bb, c->getBoundingBox());
    }

    return bb;
}


bool GraphicObjectRegion::isPoolCleaned(){
    return sContours.size() == 0;
}



void GraphicObjectRegion::openContour(){
    // checking the state of the previous contour if exist...
    if(GraphicObjectRegion::sContour != NULL){
        if(!GraphicObjectRegion::sContour->isClosed()){
            err_printf("ERROR: (GraphicObjectRegion::openContour): start to create a new contour but the previous one is not closed");
            return;
        }
    }

    //create a new contour in the pool
    GraphicObjectRegion::sContour = new Contour();
    GraphicObjectRegion::sContours.push_back(sContour);
}

void GraphicObjectRegion::addSegment(plPoint inStart, plPoint inStop){
    if (GraphicObjectRegion::sContour == NULL){
        openContour();
    }

    GraphicObjectRegion::sContour->addSegment(inStart, inStop);
}

void GraphicObjectRegion::addSegment(plPoint inStart, plPoint inStop, plPoint inCenterOffset, GraphicState::eQuadrantMode inQuadrantMode, GraphicState::eInterpolationMode inInterpolationMode){
    if (GraphicObjectRegion::sContour == NULL){
        openContour();
    }

    GraphicObjectRegion::sContour->addSegment(inStart, inStop, inCenterOffset, inQuadrantMode, inInterpolationMode);
}

void GraphicObjectRegion::closeContour()
{
    if (GraphicObjectRegion::sContour == NULL){
        return;
    }

    // the contour was terminated before being closed.
    // add the last segment (last point to first point)
    sContour->close();

    //the contour is finished, it has been added to the pool: The current contour pointer is resetted
    GraphicObjectRegion::sContour = NULL;
}


vector<GraphicObjectRegion *> GraphicObjectRegion::createRegionsFromContours(Aperture *inAperture){
    (void *)inAperture;

    vector <GraphicObjectRegion *> regions;
    GraphicObjectRegion *reg = NULL;

    //for now, one region is one contour
    for (vector<Contour *>::iterator it = GraphicObjectRegion::sContours.begin() ; it != GraphicObjectRegion::sContours.end(); ++it){
        Contour *c = *it;

        reg = new GraphicObjectRegion(inAperture);
        reg->addContour(c);

        regions.push_back(reg);
    }

    // when added:
    cleanContoursPool();


    return regions;
}





void GraphicObjectRegion::cleanContoursPool()
{
    GraphicObjectRegion::sContour = NULL;

    // clear the pool
    GraphicObjectRegion::sContours.clear();
}

void GraphicObjectRegion::flushContoursPool()
{
    //delete objects
    for (vector<Contour *>::iterator it = GraphicObjectRegion::sContours.begin() ; it != GraphicObjectRegion::sContours.end(); ++it){
        delete (*it);
    }

    cleanContoursPool();
}

void GraphicObjectRegion::addContour(GraphicObjectRegion::Contour *inContour)
{
    mContours.push_back(inContour);

    //udpdate the valid status, if the contour is invalid then all the region is invalid
    mValid = mValid & inContour->isValid();
}





