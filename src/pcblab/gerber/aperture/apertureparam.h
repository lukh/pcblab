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

/// An Interface to handle parameters, it allows to handle all kind of types in one class
class IApertureParam{
    public:
        enum eDataType{
            eInt,
            eDouble
        };

        IApertureParam(string &inName, eDataType inType): mName(inName), mType(inType) {}

        const string &getName() { return mName; }
        eDataType getDataType() { return mType; }

    private:
        string mName;
        eDataType mType;
};

/// A Template class for Aperture Parameters
template <class T>
class ApertureParam: IApertureParam{
    public:
        ApertureParam(string &inName, eDataType inType, T inData): IApertureParam(inName, inType), mData(inData) {}


        T getData() { return mData; }
        void setData(T inData) { mData = inData;}


    private:
        T mData;
};



#endif
