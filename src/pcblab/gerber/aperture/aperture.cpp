#include "aperture.h"

Aperture::Aperture(uint32_t inDCode, ApertureTemplate &inTemplate): mDCode(inDCode), mTemplate(inTemplate){
    d_printf("%%%Creating Aperture", 4, 0, false);
}

Aperture::~Aperture(){
    for(vector<IApertureParam *>::iterator it = mParameters.begin(); it != mParameters.end(); ++it){
        delete (*it);
    }

    d_printf("%%% Deleting Aperture", 4, 0, false);
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

    d_printf("APERTURE D"+ to_string(mDCode) +": parameter " + infos->getName() + " = " + to_string(inDouble) + " added", 1, 1);
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

    d_printf("APERTURE D"+ to_string(mDCode) +": parameter " + infos->getName() + " = " + to_string(inInt) + " added", 1, 1);
}



const IApertureParam *Aperture::getParameter(string &inName){
    IApertureParam *p = NULL;
    for(vector<IApertureParam *>::iterator it = mParameters.begin(); it != mParameters.end(); ++it){
        p = *it;
        if(p->getInfos().getName().compare(inName) == 0){
            return p;
        }
    }

    return NULL;
}


bool Aperture::isValid(){
    return true;
}
