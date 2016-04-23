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

#include "aperturemodifier.h"
#include "apertureprimitive.h"



using namespace std;


typedef double ApertureVariable;




/// (virtual) describes an ApertureTemplate command
class ATCommand{
    public:
        enum eType{
            eTypePrimitive,
            eTypeVarDef
        };

        ATCommand(eType inType): mType(inType) {}
        virtual ~ATCommand(){}

        virtual bool build(vector<ApertureVariable> &inVariables, vector<IAperturePrimitive *> &outPrimitives) = 0;


        eType getType() { return mType; }

    protected:
        eType mType;
};



class ATCmdVarDef: public ATCommand{
    public:
        /// create a new variable def
        ///  \param inVariableDef: "$3=5+8x$5"
        ATCmdVarDef(string inVariableDef): ATCommand(ATCommand::eTypeVarDef) {}

        virtual bool build(vector<ApertureVariable> &inVariables, vector<IAperturePrimitive *> &outPrimitives);

    private:
        uint16_t mDestVar;
        string mExpr;
};




class ATCmdPrimitive: public ATCommand{
    public:

        ATCmdPrimitive(string inPrimitiveDescr): ATCommand(ATCommand::eTypePrimitive) {}

        virtual bool build(vector<ApertureVariable> &inVariables, vector<IAperturePrimitive *> &outPrimitives);

    private:
        /// defines the type of primitive to build
        IAperturePrimitive::eType mPrimitiveType;

        /// defines the list of the modifiers.
        /// a modifier can be a decimal, a variable or a arith expr.
        /// modifier are evaluate at the build, and sent to the primitive
        vector <string> mStrModifiers;
};




/// Virtual class to describe an aperture template, standard or macro
class ApertureTemplate{
    public:
        ApertureTemplate(const string &inName);
        virtual ~ApertureTemplate();

        const string &getName() const {return mName; }


        /// used to add a command to the template
        bool addCommand(const string &inCmd);


        /// build primitives and fill in outPrimitives.
        /// Used to define a Aperture
        /// returns true if success
        bool buildAperturePrimitives(const vector<ApertureModifier> &inModifiers,vector<IAperturePrimitive *> &outPrimitives);


    protected:
        /// set the variables to the modifiers values
        void buildVarsFromModifiers(const vector<ApertureModifier> &inModifiers, vector<ApertureVariable> &outVariables);


    protected:
        static const uint16_t kMaxApertureVars = 256;

        string mName;
        vector<ATCommand *> mCommands;
};




class ApertureTemplateCircle: public ApertureTemplate{
    public:
        ApertureTemplateCircle();
        virtual ~ApertureTemplateCircle(){}
};

class ApertureTemplateRectangle: public ApertureTemplate{
    public:
        ApertureTemplateRectangle();
        virtual ~ApertureTemplateRectangle(){}
};

class ApertureTemplateObround: public ApertureTemplate{
    public:
        ApertureTemplateObround();
        virtual ~ApertureTemplateObround(){}
};

class ApertureTemplateRegularPolygon: public ApertureTemplate{
    public:
        ApertureTemplateRegularPolygon();
        virtual ~ApertureTemplateRegularPolygon(){}
};




class MacroApertureTemplate: public ApertureTemplate{
    public:
        MacroApertureTemplate(string &inName): ApertureTemplate(inName) {}
        
    private:

};


#endif
