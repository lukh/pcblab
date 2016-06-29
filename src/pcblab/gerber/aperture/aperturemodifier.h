/*******************************************************************************
* Module: ApertureTemplate
*
* Project: PcbLab
*
* Author: Vivien HENRY
*
* Date: 2016/03/12
*******************************************************************************/

#ifndef APERTUREPARAM_H
#define APERTUREPARAM_H


#include <stdint.h>
#include <string>

using namespace std;




typedef double ApertureModifier;



/// An Interface to handle parameters (= modifiers), it allows to handle all kind of types in one class
/*class ApertureModifier{
    public:
        enum eDataType{
            eInt,
            eDouble
        };

        ApertureModifier(eDataType &inType): mType(inType) {
            d_printf("%%% Creating IApertureParam", 4, 0, false);
        }
        virtual ~ApertureModifier() {
            d_printf("%%% Deleting IApertureParam", 4, 0, false);
        }

        eDataType getType() { return mType; }

        int getInt() { return (int)mData; }
        double getDouble() { return mData; }

    private:
        const eDataType mType;

        double mData;
};*/






#endif
