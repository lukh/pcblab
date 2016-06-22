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

bool GraphicObjectRegion::Contour::isCrossing(IGraphicObject *inObject)
{
    bool ret = false;

    for(vector<IGraphicObject*>::iterator it = mSegments.begin(); it != mSegments.end(); ++it){
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
    // try to find the intersection of
    // D1: y = ax+b, For E1 = [x1, x2]
    // D2: y = cx+d, For E2 = [x3, x4]
    // in E1 intersect E2

    // find xlimits
    double x1 = inDraw1->getStartPoint().mX < inDraw1->getEndPoint().mX ? inDraw1->getStartPoint().mX : inDraw1->getEndPoint().mX;
    double x2 = inDraw1->getStartPoint().mX > inDraw1->getEndPoint().mX ? inDraw1->getStartPoint().mX : inDraw1->getEndPoint().mX;
    double x3 = inDraw2->getStartPoint().mX < inDraw2->getEndPoint().mX ? inDraw2->getStartPoint().mX : inDraw2->getEndPoint().mX;
    double x4 = inDraw2->getStartPoint().mX > inDraw2->getEndPoint().mX ? inDraw2->getStartPoint().mX : inDraw2->getEndPoint().mX;

    // find intersection
    double xintmin = x1 < x3 ? x1 : x3;
    double xintmax = x2 > x4 ? x2 : x4;


    // if both lines are vertical:
    if(inDraw1->getStartPoint().mX == inDraw1->getEndPoint().mX && inDraw2->getStartPoint().mX == inDraw2->getEndPoint().mX){
        //case: x s are !=
        if(inDraw1->getStartPoint().mX != inDraw2->getStartPoint().mX){
            return false;
        }
        else{
            double minY1 = inDraw1->getStartPoint().mY < inDraw1->getEndPoint().mY ? inDraw1->getStartPoint().mY : inDraw1->getEndPoint().mY;
            double maxY1 = inDraw1->getStartPoint().mY > inDraw1->getEndPoint().mY ? inDraw1->getStartPoint().mY : inDraw1->getEndPoint().mY;

            double minY2 = inDraw2->getStartPoint().mY < inDraw2->getEndPoint().mY ? inDraw2->getStartPoint().mY : inDraw2->getEndPoint().mY;
            double maxY2 = inDraw2->getStartPoint().mY > inDraw2->getEndPoint().mY ? inDraw2->getStartPoint().mY : inDraw2->getEndPoint().mY;

            //TODO: do we have to check in the other way ???
            return (minY2 >= minY1 && minY2 <= maxY1) || (maxY2 >= minY1 && maxY2 <= maxY1);
        }
    }

    //find a,b,c,d
    // y = [(yb-ya)/(xb-xa)] * (x - xa) + ya
    // y =        m          * (x - xa) + ya
    // y =        mx - m*xa + ya

    else if(inDraw1->getStartPoint().mX != inDraw1->getEndPoint().mX && inDraw2->getStartPoint().mX != inDraw2->getEndPoint().mX){
        //D1
        double a = (inDraw1->getEndPoint().mY - inDraw1->getStartPoint().mY) / (inDraw1->getEndPoint().mX - inDraw1->getStartPoint().mX);
        double b = inDraw1->getStartPoint().mY - a * inDraw1->getStartPoint().mX;


        //D2
        double c = (inDraw2->getEndPoint().mY - inDraw2->getStartPoint().mY) / (inDraw2->getEndPoint().mX - inDraw2->getStartPoint().mX);
        double d = inDraw2->getStartPoint().mY - c * inDraw2->getStartPoint().mX;


        // CASE: D1 and D2 are not vertical
        // ax + b = cx + d
        // (a - c) x = (d - b)
        // x = (d - b) / (a - c)

        //case d1 and d2 are parallel (a = c)
        if(a == c){
            return false;
        }

        double crossX = (d-b)/(a-c);


        return crossX >= xintmin && crossX <= xintmax;
    }


    else if(inDraw1->getStartPoint().mX == inDraw1->getEndPoint().mX){
        double minY1 = inDraw1->getStartPoint().mY < inDraw1->getEndPoint().mY ? inDraw1->getStartPoint().mY : inDraw1->getEndPoint().mY;
        double maxY1 = inDraw1->getStartPoint().mY > inDraw1->getEndPoint().mY ? inDraw1->getStartPoint().mY : inDraw1->getEndPoint().mY;

        //D2
        double c = (inDraw2->getEndPoint().mY - inDraw2->getStartPoint().mY) / (inDraw2->getEndPoint().mX - inDraw2->getStartPoint().mX);
        double d = inDraw2->getStartPoint().mY - c * inDraw2->getStartPoint().mX;


        double crossY2 = c*inDraw1->getEndPoint().mX + d;
        return crossY2 >= minY1 && crossY2 <= maxY1;
    }

    else if(inDraw2->getStartPoint().mX == inDraw2->getEndPoint().mX){
        double minY2 = inDraw2->getStartPoint().mY < inDraw2->getEndPoint().mY ? inDraw2->getStartPoint().mY : inDraw2->getEndPoint().mY;
        double maxY2 = inDraw2->getStartPoint().mY > inDraw2->getEndPoint().mY ? inDraw2->getStartPoint().mY : inDraw2->getEndPoint().mY;

        //D2
        double a = (inDraw1->getEndPoint().mY - inDraw1->getStartPoint().mY) / (inDraw1->getEndPoint().mX - inDraw1->getStartPoint().mX);
        double b = inDraw1->getStartPoint().mY - a * inDraw1->getStartPoint().mX;


        double crossY1 = a*inDraw2->getEndPoint().mX + b;
        return crossY1 >= minY2 && crossY1 <= maxY2;
    }


    err_printf("ERROR: (GraphicObjectRegion::Contour::isCrossing): Unexpected case reached");
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

void GraphicObjectRegion::addSegment(Point inStart, Point inStop){
    if (GraphicObjectRegion::sContour == NULL){
        openContour();
    }

    GraphicObjectRegion::sContour->addSegment(inStart, inStop);
}

void GraphicObjectRegion::addSegment(Point inStart, Point inStop, Point inCenterOffset, GraphicState::eQuadrantMode inQuadrantMode, GraphicState::eInterpolationMode inInterpolationMode){
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

    if(! GraphicObjectRegion::sContour->isClosed()){
        err_printf("ERROR: (GraphicObjectRegion::closeContour): try to close a contour but the contour is not closed");
        return;
    }

    //the contour is finished, it has been added to the pool: The current contour pointer is resetted
    GraphicObjectRegion::sContour = NULL;
}


vector<GraphicObjectRegion *> GraphicObjectRegion::createRegionsFromContours(Aperture *inAperture){
    (void *)inAperture;

    vector <GraphicObjectRegion *> regions;
    GraphicObjectRegion *reg = NULL;

    //for now, one region is one contour
    for (vector<Contour *>::iterator it = GraphicObjectRegion::sContours.begin() ; it != GraphicObjectRegion::sContours.end(); ++it){
        reg = new GraphicObjectRegion(inAperture);
        reg->addContour(*it);

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
}



