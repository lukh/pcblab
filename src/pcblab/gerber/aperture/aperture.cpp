#include "aperture.h"

Aperture::Aperture(uint32_t inDCode, IApertureTemplate &inTemplate): mDCode(inDCode), mTemplate(inTemplate), mValid(false){
#ifdef DEBUG_PRINT
    d_printf("%%%Creating Aperture", 4, 0, false);
#endif
}

Aperture::~Aperture(){
#ifdef DEBUG_PRINT
    d_printf("%%% Deleting Aperture", 4, 0, false);
#endif

    for (vector<IAperturePrimitive *>::iterator it = mPrimitives.begin(); it != mPrimitives.end(); ++it) {
        IAperturePrimitive *prim = *it;
        delete prim;
    }
}




void Aperture::build(const vector<ApertureModifier> &inModifiers){
#ifdef DEBUG_PRINT
    int cnt=1;
    for(vector<ApertureModifier>::const_iterator it = inModifiers.begin(); it != inModifiers.end(); ++it){
        d_printf("GERBERLAYER/Aperture (D"+ to_string(mDCode) +"): adding modifier n" + to_string(cnt++) + "  = " + to_string(*it), 1, 1);
    }

    d_printf("GERBERLAYER/Aperture (D"+ to_string(mDCode) +"): building primitives", 1, 0);
#endif

    //now it s time to build the aperture with the modifiers given
    mValid = mTemplate.buildAperturePrimitives(inModifiers, mPrimitives);
}




bool Aperture::isValid(){
    return mValid;
}

Rectangle Aperture::getBoundingBox()
{
    return Rectangle(); //TODO
}
