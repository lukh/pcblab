#include "gerberlayer.h"

// --------------------------------- GerberLevel ---------------------------------

GerberLayer::GerberLevel::~GerberLevel(){
    for(vector<IGraphicObject *>::iterator it = mObjects.begin(); it != mObjects.end(); ++it){
        delete (*it);
    }
}

void GerberLayer::GerberLevel::makeGraphicObjectDraw(Point inStart, Point inStop, Aperture *inAperture){
    GraphicObjectDraw *draw = new GraphicObjectDraw(inStart, inStop, inAperture);

    mObjects.push_back(draw);
}

void GerberLayer::GerberLevel::makeGraphicObjectArc(Point inStart, Point inStop, Point inCenterOffset, GraphicState::eQuadrantMode inQuadrantMode, GraphicState::eInterpolationMode inInterpolationMode, Aperture *inAperture){
    GraphicObjectArc *arc = new GraphicObjectArc(inStart, inStop, inCenterOffset, inQuadrantMode, inInterpolationMode, inAperture);

    mObjects.push_back(arc);
}

void GerberLayer::GerberLevel::makeGraphicObjectFlash(Point inPoint, Aperture *inAperture){
    GraphicObjectFlash *flash = new GraphicObjectFlash(inPoint, inAperture);

    mObjects.push_back(flash);
}

void GerberLayer::GerberLevel::makeGraphicObjectRegions(Aperture *inAperture){
    vector <GraphicObjectRegion *> regions = GraphicObjectRegion::createRegionsFromContours(inAperture);
    for(vector<GraphicObjectRegion*>::iterator it = regions.begin(); it != regions.end(); ++it){
        mObjects.push_back((*it));
    }
}



// ---------------------------------- GerberLayer --------------------------------
GerberLayer::~GerberLayer(){
    for(vector<GerberLevel*>::iterator it = mLevels.begin(); it != mLevels.end(); ++it){
        delete (*it);
    }

    for(vector<ApertureTemplate*>::iterator it = mApertureTemplates.begin(); it != mApertureTemplates.end(); ++it){
        delete (*it);
    }
}


void GerberLayer::setRegionMode(GraphicState::eRegionMode inRegMode) {
    //update the graphic state
    mState.setRegMode(inRegMode);

    switch(inRegMode){
        case GraphicState::eRegModeOn:
            break;
        case GraphicState::eRegModeOff:{
            mCurrentLevel->makeGraphicObjectRegions(mState.getCurrentAperture());
            break;}
    }
}

void GerberLayer::addNewLevel(GraphicState::eLevelPolarity inPolarity){
    mCurrentLevel = new GerberLevel(inPolarity);
    mLevels.push_back(mCurrentLevel);
}



void GerberLayer::defineAperture(/*  */){

}

void GerberLayer::defineApertureTemplate(/*  */){

}


void GerberLayer::setCurrentAperture(uint32_t inDCode){
    Aperture *curr_aperture = NULL;

    if(inDCode < 10){
        cerr << "ERROR(GerberLayer::SetCurrentAperture): DCode(" << inDCode << ") smaller than 10" << endl;
        return;
    }

    //find the aperture corresponing to DCode
    for (std::vector<Aperture>::iterator it = mApertures.begin() ; it != mApertures.end(); ++it){
        if((*it).getDCode() == inDCode){
            curr_aperture = &(*it);
            break;
        }
    }

    if(curr_aperture == NULL){
        cerr << "ERROR(GerberLayer::SetCurrentAperture): DCode(" << inDCode << ") not found in existing aperture dictionary" << endl;
        return;
    }

    mState.setCurrentAperture(curr_aperture);
}



void GerberLayer::interpolate(Point inPointXY, Point inPointIJ){
    // save the current point as start point
    Point startPoint = mState.getCurrentPoint();
    // defines the end point, modal mode (keeps the coord of the previous if the new is not provided)
    Point endPoint = startPoint;
    endPoint.updateCoordinates(inPointXY);

    switch(mState.getRegMode()){
        case GraphicState::eRegModeOff:
            switch(mState.getInterpolationMode()){
                case GraphicState::eInterpolLinear:
                    mCurrentLevel->makeGraphicObjectDraw(startPoint, endPoint, mState.getCurrentAperture());
                    break;

                case GraphicState::eInterpolCWCircular:
                case GraphicState::eInterpolCCWCircular:
                    mCurrentLevel->makeGraphicObjectArc(startPoint, endPoint, inPointIJ, mState.getQuadrantMode(), mState.getInterpolationMode(), mState.getCurrentAperture());
                    break;

            }
            break;

        case GraphicState::eRegModeOn:
            break;

        default:
            break;
    }


    // update the current point
    mState.setCurrentPoint(inPointXY);
}


void GerberLayer::move(Point inPointXY){
    // move the current point
    mState.setCurrentPoint(inPointXY);
}


void GerberLayer::flash(Point inPointXY){
    // set the current point
    mState.setCurrentPoint(inPointXY);

    //check for forbidden uses
    if(mState.getRegMode() == GraphicState::eRegModeOn){
        cerr << "ERROR(GerberLayer::flash): Flash op (D03) is not allowed in RegionMode=On" << endl;
        return;
    }

    // create the new flash, with the current position and aperture
    mCurrentLevel->makeGraphicObjectFlash(mState.getCurrentPoint(), mState.getCurrentAperture());
}



double GerberLayer::convertCoordinate(long inRaw){
    double out = (double) inRaw;

    for(int i = 0; i < mState.getCoordFormat().mDecimals; i ++){
        out /= 10;
    }

    return out;
}

