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

void GerberLayer::GerberLevel::makeGraphicObjectFlash(Aperture *inAperture){}

void GerberLayer::GerberLevel::makeGraphicObjectRegion(Aperture *inAperture){}




// ---------------------------------- GerberLayer --------------------------------
GerberLayer::~GerberLayer(){
    for(vector<ApertureTemplate*>::iterator it = mApertureTemplates.begin(); it != mApertureTemplates.end(); ++it){
        delete (*it);
    }
}


void GerberLayer::addNewLevel(GraphicState::eLevelPolarity inPolarity){
    mLevels.push_back(GerberLevel(inPolarity));
}



void GerberLayer::defineAperture(/*  */){

}

void GerberLayer::defineApertureTemplate(/*  */){

}


void GerberLayer::setCurrentAperture(uint32_t inDCode){
    Aperture *curr_aperture = NULL;

    if(inDCode >= 10){
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



void GerberLayer::makeGraphicObjectDraw(Point inStop){
    if(mCurrentLevel == NULL){
        cerr << "ERROR(GerberLayer::makeGraphicObjectDraw): level doesn't exist" << endl;
        return;
    }

    mCurrentLevel->makeGraphicObjectDraw(mState.getCurrentPoint(), inStop, mState.getCurrentAperture());
}

void GerberLayer::makeGraphicObjectArc(Point inStop, Point inCenterOffset){
    if(mCurrentLevel == NULL){
        cerr << "ERROR(GerberLayer::makeGraphicObjectDraw): level doesn't exist" << endl;
        return;
    }

    mCurrentLevel->makeGraphicObjectArc(mState.getCurrentPoint(), inStop, inCenterOffset, mState.getQuadrantMode(), mState.getInterpolationMode(), mState.getCurrentAperture());
}

void GerberLayer::makeGraphicObjectFlash(){

}

void GerberLayer::makeGraphicObjectRegion(){

}
