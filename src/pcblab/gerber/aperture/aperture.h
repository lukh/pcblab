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
#include "apertureparam.h"

using namespace std;


/// Defines an Aperture.
class Aperture{
    public:
        Aperture(uint32_t inDCode, ApertureTemplate *inTemplate): mDCode(inDCode), mTemplate(inTemplate) {}

        void addParameter(IApertureParam *inParam) { mParameters.push_back(inParam); }
        const IApertureParam *getParameter(string &inName);

        uint32_t getDCode() { return mDCode; }

    private:
        /// Aperture number
        const uint32_t mDCode;

        /// the template used
        ApertureTemplate* mTemplate;

        /// defines its parameters
        vector<IApertureParam *> mParameters;
};


#endif
