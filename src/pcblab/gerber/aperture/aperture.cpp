#include "aperture.h"

Aperture::~Aperture(){
    for(vector<IApertureParam *>::iterator it = mParameters.begin(); it != mParameters.end(); ++it){
        delete (*it);
    }
}

void Aperture::addParameter(double inDouble){
    string pname;
    IApertureParam::eDataType type;

    if(mTemplate == NULL){
        err_printf("ERROR(Aperture::addParameter): Bad template");
        return;
    }

    // extract from the template the kind of param we should get
    if(! mTemplate->getParameterInfos(mParameters.size(), pname, type)){
        err_printf("ERROR(Aperture::addParameter): impossible to get correct informations about the parameter from template");
        return;
    }

    if(type != IApertureParam::eDouble){
        err_printf("ERROR(Aperture::addParameter): expected double");
        return;
    }

    ApertureParam<double> *p = new ApertureParam<double>(pname, IApertureParam::eDouble, inDouble);

    mParameters.push_back(p);
}

void Aperture::addParameter(int inInt){
    string pname;
    IApertureParam::eDataType type;

    if(mTemplate == NULL){
        err_printf("ERROR(Aperture::addParameter): Bad template");
        return;
    }

    // extract from the template the kind of param we should get
    if(! mTemplate->getParameterInfos(mParameters.size(), pname, type)){
        err_printf("ERROR(Aperture::addParameter): impossible to get correct informations about the parameter from template");
        return;
    }

    if(type != IApertureParam::eInt){
        err_printf("ERROR(Aperture::addParameter): expected int");
        return;
    }

    ApertureParam<int> *p = new ApertureParam<int>(pname, IApertureParam::eInt, inInt);

    mParameters.push_back(p);
}


bool Aperture::isValid(){
    return true;
}
