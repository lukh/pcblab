#include "graphicobject.h"

/////////////////////////// Arcs ///////////////////////////

GraphicObjectArc::GraphicObjectArc(Point inStartPoint, Point inEndPoint, Point inCenterOffset, GraphicState::eQuadrantMode inQuadrantMode, GraphicState::eInterpolationMode inInterpolationMode, Aperture *inAperture):
    IGraphicObject(IGraphicObject::eTypeArc, inAperture),
    IGraphicObjectTrack(inStartPoint, inEndPoint)
  , mCenterOffset(inCenterOffset),
    mQuadrantMode(inQuadrantMode), mInterpolationMode(inInterpolationMode)
{
    if(mInterpolationMode == GraphicState::eInterpolLinear){
        err_printf("ERROR (GraphicObjectArc::GraphicObjectArc): Can't instanciate an arc with an interpolation set to linear...");
        mValid = false;
    }

    else{
        mValid = true;
    }

    d_printf("Creating GraphicObjectArc: start =(" + to_string(inStartPoint.mX) + ", " +to_string(inStartPoint.mY) + ") end =(" + to_string(inEndPoint.mX) + ", " +to_string(inEndPoint.mY) + ")", 4, 0, false);
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
                GraphicObjectArc *arc = static_cast<GraphicObjectArc*>((*it));
                (void *)arc;
                break;}

            default:
                // there is something else in the vector, and it shouldn't be
                err_printf("ERROR: (GraphicObjectRegion::Contour::isInside): Unexpected GraphicObject in the contour");
                break;
        }

    }

    return w!=0;
}





/// checks the connection with another contour
GraphicObjectRegion::Contour::eContoursConnection GraphicObjectRegion::Contour::getConnection(const Contour &inContour){
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
    (void *)inAperture;
}


