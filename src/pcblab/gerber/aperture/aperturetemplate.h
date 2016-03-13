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

    private:
        /// Defines the Aperture Type from the standard apertures
        eSAType mApertureType;

};


class MacroApertureTemplate: public ApertureTemplate{
    public:

    private:
        string mName;
};
