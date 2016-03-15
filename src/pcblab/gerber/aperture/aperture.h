/*******************************************************************************
* Module: Aperture
*
* Project: PcbLab
*
* Author: Vivien HENRY
*
* Date: 2016/03/12
*******************************************************************************/

#include <stdint.h>
#include <string.h>

#include "common.h"
#include "aperture/aperturetemplate.h"
#include "aperture/apertureparam.h"

using namespace std;


/// Defines an Aperture.
class Aperture{
    public:
        Aperture(uint32_t inDCode, ApertureTemplate *inTemplate): mDCode(inDCode), mTemplate(inTemplate) {}

        void addParameter(IApertureParam *inParam) { mParameters.push(inParam); }
        const IApertureParam *getParameter(string &inName);

    private:
        /// Aperture number
        const uint32_t mDCode;

        /// the template used
        ApertureTemplate* mTemplate;

        /// defines its parameters
        vector<IApertureParam *> mParameters;
};
