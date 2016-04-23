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

#include "../../common.h"
#include "aperturetemplate.h"
#include "aperturemodifier.h"

using namespace std;


/// Defines an Aperture.
class Aperture{
    public:
        Aperture(uint32_t inDCode, ApertureTemplate &inTemplate);
        ~Aperture();

        void addParameter(double inDouble);
        void addParameter(int inInt);

        uint32_t getDCode() { return mDCode; }

        ///defines if the aperture is complete and clean
        bool isValid();

    private:
        /// Aperture number
        const uint32_t mDCode;

        /// the template used
        ApertureTemplate &mTemplate;
};


#endif
