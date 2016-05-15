/*******************************************************************************
* Module: Aperture
*
* Project: PcbLab
*
* Author: Vivien HENRY
*
* Date: 2016/03/12
*******************************************************************************/

#ifndef APERTURE_H
#define APERTURE_H

#include <stdint.h>
#include <string>
#include <vector>

#include "../../common.h"
#include "apertureprimitive.h"
#include "aperturetemplate.h"
#include "aperturemodifier.h"

using namespace std;


/// Defines an Aperture.
class Aperture{
    public:
        Aperture(uint32_t inDCode, IApertureTemplate &inTemplate);
        ~Aperture();

        // build the primitives from the template and aperture modifiers
        void build(const vector<ApertureModifier> &inModifiers);

        uint32_t getDCode() { return mDCode; }

        ///defines if the aperture is complete and clean
        bool isValid();

    private:
        /// Aperture number
        const uint32_t mDCode;

        /// the template used
        IApertureTemplate &mTemplate;

        /// the calculated primitives from the template
        vector<IAperturePrimitive *> mPrimitives;

        bool mValid;
};


#endif
