#include "gerberlayer.h"

#include <fstream>

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
GerberLayer::GerberLayer(): SyntaxParser() {
    //adding default aperture templates
    ApertureTemplate *t;

    t = new ApertureTemplateCircle();
    mApertureTemplates.push_back(t);

    t = new ApertureTemplateRectangle();
    mApertureTemplates.push_back(t);

    t = new ApertureTemplateObround();
    mApertureTemplates.push_back(t);

    t = new ApertureTemplateRegularPolygon();
    mApertureTemplates.push_back(t);

}


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



void GerberLayer::addAperture(uint32_t inDCode, string inTemplateName){
    //find the template
    ApertureTemplate *aper_temp = getApertureTemplateByName(inTemplateName);
    if(aper_temp == NULL){
        err_printf("ERROR(GerberLayer::addAperture): Template not found");
        return;
    }

    mApertures.push_back(Aperture(inDCode, aper_temp));
}



void GerberLayer::addApertureParam(uint32_t inDCode, double inValue){
    Aperture *aperture = getApertureByDCode(inDCode);
    if(aperture == NULL){
        err_printf("ERROR(GerberLayer::addApertureParam): Aperture not found");
        return;
    }

    aperture->addParameter(inValue);
}

void GerberLayer::addApertureParam(uint32_t inDCode, int inValue){
    Aperture *aperture = getApertureByDCode(inDCode);
    if(aperture == NULL){
        err_printf("ERROR(GerberLayer::addApertureParam): Aperture not found");
        return;
    }

    aperture->addParameter(inValue);
}



void GerberLayer::defineApertureTemplate(/*  */){

}


void GerberLayer::setCurrentAperture(uint32_t inDCode){
    Aperture *curr_aperture = getApertureByDCode(inDCode);

    if(curr_aperture == NULL){
        err_printf("ERROR(GerberLayer::SetCurrentAperture: DCode(" + to_string(inDCode) + ") not found in existing aperture dictionary");
        return;
    }

    d_printf("GERBERLAYER: setCurrentAperture D" + to_string(inDCode));
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
        err_printf("ERROR(GerberLayer::flash): Flash op (D03) is not allowed in RegionMode=On");
        return;
    }

    // create the new flash, with the current position and aperture
    mCurrentLevel->makeGraphicObjectFlash(mState.getCurrentPoint(), mState.getCurrentAperture());
}


void GerberLayer::handleComment(string &inStr){
    d_printf("COMMENT :" + inStr);
}



double GerberLayer::convertCoordinate(long inRaw){
    double out = (double) inRaw;

    for(int i = 0; i < mState.getCoordFormat().mDecimals; i ++){
        out /= 10;
    }

    return out;
}




Aperture *GerberLayer::getApertureByDCode(uint32_t inDCode){
    if(inDCode < 10){
        err_printf("ERROR(GerberLayer::getApertureByDCode): DCode(" + to_string(inDCode) + ") smaller than 10");
        return NULL;
    }

    //find the aperture corresponing to DCode
    for (std::vector<Aperture>::iterator it = mApertures.begin() ; it != mApertures.end(); ++it){
        if((*it).getDCode() == inDCode){
            return &(*it);
        }
    }

    err_printf("ERROR(GerberLayer::getApertureByDCode): DCode(" + to_string(inDCode) + ") not found in existing aperture dictionary");
    return NULL;
}







ApertureTemplate *GerberLayer::getApertureTemplateByName(string &inTemplateName){
    ApertureTemplate *aper_temp = NULL;

    //scan the template list
    for(vector<ApertureTemplate *>::iterator it = mApertureTemplates.begin(); it != mApertureTemplates.end(); ++it){
        aper_temp = *it;
        if(aper_temp->getName().compare(inTemplateName) == 0){
            return aper_temp;
        }

    }

    err_printf("ERROR(GerberLayer::getApertureTemplateByName): template not found");
    return NULL;
}


bool GerberLayer::open(const string &inFileName){
    ifstream fs;

    fs.open(inFileName);
    if(! fs.is_open()){
        err_printf("ERROR(GerberLayer::open): Impossible to open the file "+ inFileName);
        return false;
    }

    parse(fs);

    fs.close();

    return true;
}
