#include "aperture.h"

Aperture::Aperture(uint32_t inDCode, ApertureTemplate &inTemplate): mDCode(inDCode), mTemplate(inTemplate){
    d_printf("%%%Creating Aperture", 4, 0, false);
}

Aperture::~Aperture(){

    d_printf("%%% Deleting Aperture", 4, 0, false);
}




bool Aperture::isValid(){
    return true;
}
