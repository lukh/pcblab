/*******************************************************************************
* Module: ApertureTemplate
*
* Project: PcbLab
*
* Author: Vivien HENRY
*
* Date: 2016/03/12
*******************************************************************************/

#include <stdint.h>
#include <string.h>
#include <vector.h>

#include "common.h"



using namespace std;

/// Virtual class to describe an aperture template, standard or macro
class ApertureTemplate{
    public:
        enum eATType{
            eATTStandard,
            eATTMacro
        };


        ApertureTemplate(): mTemplateType(eATTStandard) {}
        ApertureTemplate(eATTType inType): mTemplateType(inType) {}

        eATTType getTemplateType() { return mTemplateType; }

    private:
        eATTType mTemplateType;
};


/// describes a standard aperture template
class StandardApertureTemplate: public ApertureTemplate{
    public:
        enum eSAType{
            eSACircle,
            eSARectangle,
            eSAObround,
            eSARegularPolygon
        };

        StandardApertureTemplate(eSAType inType): ApertureTemplate(ApertureTemplate::eATTStandard), mApertureType(inType) {}

    private:
        /// Defines the Aperture Type from the standard apertures
        eSAType mApertureType;

};


class MacroApertureTemplate: public ApertureTemplate{
    public:
        MacroApertureTemplate(string &inName): ApertureTemplate(ApertureTemplate::eATTMacro), mName(inName) {}
        
    private:
        string mName;
};
