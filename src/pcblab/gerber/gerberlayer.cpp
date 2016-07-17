#include "gerberlayer.h"

#include <fstream>

// --------------------------------- GerberLevel ---------------------------------
GerberLayer::GerberLevel::GerberLevel(GraphicState::eLevelPolarity inPolarity): mPolarity(inPolarity){
    d_printf("%%% Creating GerberLevel", 4, 0, false);
}

GerberLayer::GerberLevel::~GerberLevel(){
    for(vector<IGraphicObject *>::iterator it = mObjects.begin(); it != mObjects.end(); ++it){
        delete (*it);
    }

    d_printf("%%% Deleting GerberLevel", 4, 0, false);
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
GerberLayer::GerberLayer(const string &inName): SyntaxParser(),
    mName(inName), mCurrentLevel(NULL)
{
    d_printf("%%% Creating GerberLevel", 4, 0, false);

    //adding default aperture templates
    IApertureTemplate *t;

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

    for(vector<Aperture*>::iterator it = mApertures.begin(); it != mApertures.end(); ++it){
        delete (*it);
    }

    for(vector<IApertureTemplate*>::iterator it = mApertureTemplates.begin(); it != mApertureTemplates.end(); ++it){
        delete (*it);
    }

    GraphicObjectRegion::flushContoursPool();

    d_printf("%%% Deleting GerberLayer", 4, 0, false);
}


void GerberLayer::setRegionMode(GraphicState::eRegionMode inRegMode) {
    //update the graphic state
    mState.setRegMode(inRegMode);

    switch(inRegMode){
        case GraphicState::eRegModeOn:
            //check pool is clean
            if(! GraphicObjectRegion::isPoolCleaned()){
                GraphicObjectRegion::flushContoursPool();
            }
            break;
        case GraphicState::eRegModeOff:{
            // close contour if not properly closed with D02
            GraphicObjectRegion::closeContour();

            //create regions from pool
            mCurrentLevel->makeGraphicObjectRegions(mState.getCurrentAperture());
            break;}
        default:
            err_printf("ERROR(GerberLayer::setRegionMode): GraphicState::RegionMode is undefined !");
            break;
    }

    d_printf("GERBERLAYER: setRegionMode",1,0);
}

void GerberLayer::addNewLevel(GraphicState::eLevelPolarity inPolarity){
    mCurrentLevel = new GerberLevel(inPolarity);
    mLevels.push_back(mCurrentLevel);

    d_printf("GERBERLAYER: addNewLevel" ,1,0);
}



void GerberLayer::addAperture(uint32_t inDCode, string inTemplateName, const vector<ApertureModifier> &inModifiers){
    //find the template
    IApertureTemplate *aper_temp = getApertureTemplateByName(inTemplateName);
    if(aper_temp == NULL){
        err_printf("ERROR(GerberLayer::addAperture): Template not found");
        return;
    }

    Aperture *ap = new Aperture(inDCode, *aper_temp);
    mApertures.push_back(ap);
    d_printf("GERBERLAYER: Aperture D" + to_string(inDCode) + "(" + inTemplateName + ") Added", 1, 0);

    ap->build(inModifiers);
}




void GerberLayer::defineApertureTemplate(string &inName, const vector<string> &inRawCmds)
{
    MacroApertureTemplate *at = new MacroApertureTemplate(inName);

    mApertureTemplates.push_back(at);
    d_printf("GERBERLAYER: ApertureTemplate " + inName + " added", 1, 0);


    for(vector<string>::const_iterator it = inRawCmds.begin(); it != inRawCmds.end(); ++it){
        if(!at->addCommand(*it)){
            err_printf("ERROR(GerberLayer::defineApertureTemplate): unrecognized command");
        }
    }
}




void GerberLayer::setCurrentAperture(uint32_t inDCode){
    Aperture *curr_aperture = getApertureByDCode(inDCode);

    if(curr_aperture == NULL){
        err_printf("ERROR(GerberLayer::SetCurrentAperture: DCode(" + to_string(inDCode) + ") not found in existing aperture dictionary");
        return;
    }


    mState.setCurrentAperture(curr_aperture);
    d_printf("GERBERLAYER: setCurrentAperture D" + to_string(inDCode), 1, 0);
}



void GerberLayer::interpolate(Point inPointXY, Point inPointIJ){
    // save the current point as start point
    Point startPoint = mState.getCurrentPoint();
    // defines the end point, modal mode (keeps the coord of the previous if the new is not provided)
    Point endPoint = startPoint;
    endPoint.updateCoordinates(inPointXY);

    d_printf("GERBERLAYER: interpolate",1,0);

    if(mCurrentLevel == NULL){
        // error.
        err_printf("ERROR(GerberLayer::interpolate): No existing level");
        return;
    }

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

                default:
                    err_printf("ERROR(GerberLayer::interpolate): GraphicState::InterpolationMode is undefined !");
                    break;

            }
            break;

        case GraphicState::eRegModeOn:
            switch(mState.getInterpolationMode()){
                case GraphicState::eInterpolLinear:
                    GraphicObjectRegion::addSegment(startPoint, endPoint);
                    break;
                case GraphicState::eInterpolCWCircular:
                case GraphicState::eInterpolCCWCircular:
                    GraphicObjectRegion::addSegment(startPoint, endPoint, inPointIJ, mState.getQuadrantMode(), mState.getInterpolationMode());
                    break;
                default:
                    err_printf("ERROR(GerberLayer::interpolate): GraphicState::InterpolationMode is undefined !");
                    break;
            }
            break;

        default:
            err_printf("ERROR(GerberLayer::interpolate): GraphicState::RegionMode is undefined !");
            break;
    }


    // update the current point
    mState.setCurrentPoint(inPointXY);
}


void GerberLayer::move(Point inPointXY){
    // move the current point
    mState.setCurrentPoint(inPointXY);

    if(mState.getRegMode() == GraphicState::eRegModeOn){
        GraphicObjectRegion::closeContour();
    }

    d_printf("GERBERLAYER: move",1,0);
}


void GerberLayer::flash(Point inPointXY){
    // set the current point
    mState.setCurrentPoint(inPointXY);

    d_printf("GERBERLAYER: flash",1,0);

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



double GerberLayer::convertCoordinate(int32_t inRaw){
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
    for (std::vector<Aperture *>::iterator it = mApertures.begin() ; it != mApertures.end(); ++it){
        if((*it)->getDCode() == inDCode){
            return (*it);
        }
    }

    err_printf("ERROR(GerberLayer::getApertureByDCode): DCode(" + to_string(inDCode) + ") not found in existing aperture dictionary");
    return NULL;
}







IApertureTemplate *GerberLayer::getApertureTemplateByName(string &inTemplateName){
    IApertureTemplate *aper_temp = NULL;

    //scan the template list
    for(vector<IApertureTemplate *>::iterator it = mApertureTemplates.begin(); it != mApertureTemplates.end(); ++it){
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
