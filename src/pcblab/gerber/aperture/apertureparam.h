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


/// ApertureTemplateParameterInfos
/// Used to describe a parameter
class ATParameterInfos{
    public:
        enum eDataType{
            eInt,
            eDouble
        };


        ATParameterInfos(const string &inName, eDataType inType, bool inOptional): mName(inName), mType(inType), mOptional(inOptional) {}


        const string &getName() const { return mName; }
        eDataType getType(){ return mType; }

    private:
        const string mName;
        const eDataType mType;
        const bool mOptional;
};







/// An Interface to handle parameters, it allows to handle all kind of types in one class
class IApertureParam{
    public:
        IApertureParam(const string &inName, const ATParameterInfos &inInfos): mName(inName), mInfos(inInfos) {
            d_printf("%%% Creating IApertureParam", 4, 0, false);
        }
        IApertureParam(ATParameterInfos &inInfos): mInfos(inInfos) {
            d_printf("%%% Creating IApertureParam", 4, 0, false);
        }
        virtual ~IApertureParam() {
            d_printf("%%% Deleting IApertureParam", 4, 0, false);
        }

        const string &getName() { return mName; }
        const ATParameterInfos &getInfos() const { return mInfos; }

    private:
        string mName;
        const ATParameterInfos &mInfos;
};



/// A Template class for Aperture Parameters
template <class T>
class ApertureParam: public IApertureParam{
    public:
        ApertureParam(string &inName, ATParameterInfos &inInfos, T inData): IApertureParam(inName, inInfos), mData(inData) {}
        ApertureParam(ATParameterInfos &inInfos, T inData): IApertureParam(inInfos), mData(inData) {}
        virtual ~ApertureParam() {}


        T getData() { return mData; }
        void setData(T inData) { mData = inData;}


    private:
        T mData;
};




#endif
