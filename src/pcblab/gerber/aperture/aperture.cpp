#include "aperture.h"

Aperture::~Aperture(){
    for(vector<IApertureParam *>::iterator it = mParameters.begin(); it != mParameters.end(); ++it){
        delete (*it);
    }
}

void Aperture::addParameter(double inDouble){
    ATParameterInfos *infos;


    // extract from the template the kind of param we should get
    if(!mTemplate.getParameterInfos(mParameters.size(), &infos)){
        err_printf("ERROR(Aperture::addParameter): impossible to get correct informations about the parameter from template");
        return;
    }

    if(infos->getType() != ATParameterInfos::eDouble){
        err_printf("ERROR(Aperture::addParameter): expected double");
        return;
    }

    ApertureParam<double> *p = new ApertureParam<double>(*infos, inDouble);

    mParameters.push_back(p);

    d_printf("        parameter <" + infos->getName() + "> = " + to_string(inDouble) + " added");
}

void Aperture::addParameter(int inInt){
    ATParameterInfos *infos;


    // extract from the template the kind of param we should get
    if(!mTemplate.getParameterInfos(mParameters.size(), &infos)){
        err_printf("ERROR(Aperture::addParameter): impossible to get correct informations about the parameter from template");
        return;
    }

    if(infos->getType() != ATParameterInfos::eInt){
        err_printf("ERROR(Aperture::addParameter): expected int");
        return;
    }

    ApertureParam<int> *p = new ApertureParam<int>(*infos, inInt);

    mParameters.push_back(p);

    d_printf("        parameter <" + infos->getName() + "> = " + to_string(inInt) + " added");
}


bool Aperture::isValid(){
    return true;
}
