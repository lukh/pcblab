#include "aperture.h"

Aperture::Aperture(uint32_t inDCode, ApertureTemplate &inTemplate): mDCode(inDCode), mTemplate(inTemplate), mValid(false){
    d_printf("%%%Creating Aperture", 4, 0, false);
}

Aperture::~Aperture(){
    d_printf("%%% Deleting Aperture", 4, 0, false);

    for (vector<IAperturePrimitive *>::iterator it = mPrimitives.begin(); it != mPrimitives.end(); ++it) {
        IAperturePrimitive *prim = *it;
        delete prim;
    }
}




void Aperture::setModifiers(const vector<ApertureModifier> &inModifiers){
    int cnt=1;
    for(vector<ApertureModifier>::const_iterator it = inModifiers.begin(); it != inModifiers.end(); ++it){
        d_printf("GERBERLAYER/Aperture (D"+ to_string(mDCode) +"): modifier n" + to_string(cnt++) + "  = " + to_string(*it), 1, 0);
    }

    d_printf("GERBERLAYER/Aperture (D"+ to_string(mDCode) +"): building primitives", 1, 0);

    //now it s time to build the aperture with the modifiers given
    mValid = mTemplate.buildAperturePrimitives(inModifiers, mPrimitives);
}




bool Aperture::isValid(){
    return mValid;
}
