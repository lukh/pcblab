/*******************************************************************************
* Module: ApertureTemplate
*
* Project: PcbLab
*
* Author: Vivien HENRY
*
* Date: 2016/03/12
*******************************************************************************/
#ifndef APERTURE_TEMPLATE_H
#define APERTURE_TEMPLATE_H

#include <stdint.h>
#include <string>
#include <vector>

#include "../../common.h"

#include "apertureparam.h"



using namespace std;

/// Virtual class to describe an aperture template, standard or macro
class ApertureTemplate{
    public:
        ApertureTemplate(string inName): mName(inName) {}
        virtual ~ApertureTemplate() = 0;

        const string &getName() const {return mName; }

        /// return informations about the parameter at inIdx
        virtual bool getParameterInfos(uint16_t inIdx, string &outName, IApertureParam::eDataType &outType) = 0;

    private:
        string mName;
};




class MacroApertureTemplate: public ApertureTemplate{
    public:
        MacroApertureTemplate(string &inName): ApertureTemplate(inName) {}
        
    private:

};


#endif
