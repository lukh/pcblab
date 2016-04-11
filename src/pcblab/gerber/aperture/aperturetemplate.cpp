#include "aperturetemplate.h"

bool ApertureTemplate::getParameterInfos (uint16_t inIdx, ATParameterInfos **outInfos){
    if(inIdx >= mParamsInfos.size()){
        err_printf("ERROR(ApertureTemplate::getParameterInfos): index out of range");
        return false;
    }

    *outInfos = &mParamsInfos[inIdx];


    return true;
}


ApertureTemplateCircle::ApertureTemplateCircle(): ApertureTemplate("C"){
    mParamsInfos.push_back(ATParameterInfos("Diameter", ATParameterInfos::eDouble, false));
    mParamsInfos.push_back(ATParameterInfos("HoleDiameter", ATParameterInfos::eDouble, true));
}

ApertureTemplateRectangle::ApertureTemplateRectangle(): ApertureTemplate("R"){
    mParamsInfos.push_back(ATParameterInfos("X", ATParameterInfos::eDouble, false));
    mParamsInfos.push_back(ATParameterInfos("Y", ATParameterInfos::eDouble, false));
    mParamsInfos.push_back(ATParameterInfos("HoleDiameter", ATParameterInfos::eDouble, true));
}

ApertureTemplateObround::ApertureTemplateObround(): ApertureTemplate("O"){
    mParamsInfos.push_back(ATParameterInfos("X", ATParameterInfos::eDouble, false));
    mParamsInfos.push_back(ATParameterInfos("Y", ATParameterInfos::eDouble, false));
    mParamsInfos.push_back(ATParameterInfos("HoleDiameter", ATParameterInfos::eDouble, true));

}

ApertureTemplateRegularPolygon::ApertureTemplateRegularPolygon(): ApertureTemplate("P"){
    mParamsInfos.push_back(ATParameterInfos("OuterDiameter", ATParameterInfos::eDouble, false));
    mParamsInfos.push_back(ATParameterInfos("VerticesNumber", ATParameterInfos::eInt, false));
    mParamsInfos.push_back(ATParameterInfos("Rotation", ATParameterInfos::eDouble, false));
    mParamsInfos.push_back(ATParameterInfos("HoleDiameter", ATParameterInfos::eDouble, true));
}
