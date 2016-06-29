#include "graphicobject.h"

/////////////////////////// Arcs ///////////////////////////

GraphicObjectArc::GraphicObjectArc(Point inStartPoint, Point inEndPoint, Point inCenterOffset, GraphicState::eQuadrantMode inQuadrantMode, GraphicState::eInterpolationMode inInterpolationMode, Aperture *inAperture):
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
            mCenter = Point(mStartPoint.mX + inCenterOffset.mX, mStartPoint.mY + inCenterOffset.mY);
            mValid = true;
        }

        else if(mQuadrantMode == GraphicState::eQuadrantSingle){
            /*double signs[2] = {1.0, -1.0};
            vector<Point> candidates;

            for(int i = 0; i < 4; i ++){
                //determine the Center (c) point
                Point c( inStartPoint.mX + signs[i&0x1] * inCenterOffset.mX, inStartPoint.mY + signs[(i>>1)&0x1] * inCenterOffset.mY  );

                double Rs2c = sqrt( pow(inStartPoint.mX - inCenterOffset.mX, 2) + pow(inStartPoint.mY - inCenterOffset.mY, 2) );
                double Re2c = sqrt( pow(inEndPoint.mX - inCenterOffset.mX, 2) + pow(inEndPoint.mY - inCenterOffset.mY, 2) );

                if(Rs2c == Re2c){
                    candidates.push_back(c);
                }
            }

            mValid = true; //maybe*/
        }

        else{
            err_printf("GraphicObjectArc/getCenter: quadrant mode is undefined or unknown");
        }

    }

    d_printf("Creating GraphicObjectArc: start =(" + to_string(inStartPoint.mX) + ", " +to_string(inStartPoint.mY) + ") end =(" + to_string(inEndPoint.mX) + ", " +to_string(inEndPoint.mY) + ")", 4, 0, false);
}



GraphicState::eQuadrantMode GraphicObjectArc::getQuadrantMode() const
{
    return mQuadrantMode;
}


GraphicState::eInterpolationMode GraphicObjectArc::getInterpolationMode() const
{
    return mInterpolationMode;
}





/////////////////////////// Contours ///////////////////////////


/// Adds a linear segment
void GraphicObjectRegion::Contour::addSegment(Point inStart, Point inStop){
    GraphicObjectDraw *draw = new GraphicObjectDraw(inStart, inStop, NULL);

    mSegments.push_back(draw);
}

/// Adds an arc segment
void GraphicObjectRegion::Contour::addSegment(Point inStart, Point inStop, Point inCenterOffset, GraphicState::eQuadrantMode inQuadrantMode, GraphicState::eInterpolationMode inInterpolationMode){
    GraphicObjectArc *arc = new GraphicObjectArc(inStart, inStop, inCenterOffset, inQuadrantMode, inInterpolationMode, NULL);

    mSegments.push_back(arc);
}

/// checks if the contour is closed
bool GraphicObjectRegion::Contour::isClosed(){
    IGraphicObjectTrack *track, *last_track = NULL;
    for(vector<IGraphicObject*>::iterator it = mSegments.begin(); it != mSegments.end(); ++it){
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

/// checks if the Point is in the contour (contour must be closed)
bool GraphicObjectRegion::Contour::isInside(Point inPoint){
    // a point is inside the contour if: the contour is closed, the point is on the same side of wich track
    if(!isClosed()){
        return false;
    }


    // check positition of the point regarding to the track
    double nx, ny, nxp1, nyp1; //vector coordinates normalized against inPoint
    double w = 0; //winding number
    for(vector<IGraphicObject*>::iterator it = mSegments.begin(); it != mSegments.end(); ++it){
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
}





/// checks the connection with another contour
GraphicObjectRegion::Contour::eContoursConnection GraphicObjectRegion::Contour::getConnection(const Contour &inContour){

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



void GraphicObjectRegion::openContour(){
    // checking the state of the previous contour if exist...
    if(GraphicObjectRegion::sContour != NULL){
        if(!GraphicObjectRegion::sContour->isClosed()){
            err_printf("ERROR: (GraphicObjectRegion::openContour): start to create a new contour but the previous one is not closed");
        }
    }

    //create a new contour in the pool
    GraphicObjectRegion::sContour = new Contour();
    GraphicObjectRegion::sContours.push_back(sContour);
}

void GraphicObjectRegion::addSegment(Point inStart, Point inStop){
    GraphicObjectRegion::sContour->addSegment(inStart, inStop);
}

void GraphicObjectRegion::addSegment(Point inStart, Point inStop, Point inCenterOffset, GraphicState::eQuadrantMode inQuadrantMode, GraphicState::eInterpolationMode inInterpolationMode){
    GraphicObjectRegion::sContour->addSegment(inStart, inStop, inCenterOffset, inQuadrantMode, inInterpolationMode);
}


vector<GraphicObjectRegion *> GraphicObjectRegion::createRegionsFromContours(Aperture *inAperture){

}


