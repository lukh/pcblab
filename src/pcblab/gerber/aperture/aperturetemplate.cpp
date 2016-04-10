#include "aperturetemplate.h"

bool ApertureTemplate::getParameterInfos(uint16_t inIdx, string &outName, IApertureParam::eDataType &outType){
    if(inIdx >= mParamsInfos.size()){
        err_printf("ERROR(ApertureTemplate::getParameterInfos): index out of range");
        return false;
    }

    ATParameterInfos &infos = mParamsInfos[inIdx];
    outName = infos.mName;
    outType = infos.mType;

    return true;
}


ApertureTemplateCircle::ApertureTemplateCircle(): ApertureTemplate("C"){

}

ApertureTemplateRectangle::ApertureTemplateRectangle(): ApertureTemplate("R"){

}

ApertureTemplateObround::ApertureTemplateObround(): ApertureTemplate("O"){

}

ApertureTemplateRegularPolygon::ApertureTemplateRegularPolygon(): ApertureTemplate("P"){

}
