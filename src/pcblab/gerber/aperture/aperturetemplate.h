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
#include <map>

#include "../../common.h"

#include "aperturemodifier.h"
#include "apertureprimitive.h"



using namespace std;


typedef double ApertureVariable;
typedef map<string, ApertureVariable> ApVarSymbolTable;



/// (virtual) describes an ApertureTemplate command.
class ATCommand{
    public:
        ATCommand(): mValid(false) { d_printf("%%% Creating ATCommand", 4, 0, false);}
        virtual ~ATCommand(){ d_printf("%%% Deleting ATCommand", 4, 0, false); }

        virtual bool build(ApVarSymbolTable &inVariables, vector<IAperturePrimitive *> &outPrimitives) = 0;


        bool isValid() { return mValid; }

    protected:
        bool mValid;
};



class ATCmdVarDef: public ATCommand{
    public:
        /// create a new variable def
        ///  \param inVariableDef: "$3=5+8x$5"
        ATCmdVarDef(const string &inVariableDef);
        virtual ~ATCmdVarDef() {}

        virtual bool build(ApVarSymbolTable &inVariables, vector<IAperturePrimitive *> &outPrimitives);


    private:
        string mDestVar;
        string mExpr;
};




class ATCmdPrimitive: public ATCommand{
    public:

        ATCmdPrimitive(const string &inPrimitiveDescr);
        virtual ~ATCmdPrimitive() {}

        virtual bool build(ApVarSymbolTable &inVariables, vector<IAperturePrimitive *> &outPrimitives);

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
        void buildVarsFromModifiers(const vector<ApertureModifier> &inModifiers, ApVarSymbolTable &outVariables);


    protected:
        static const uint16_t kMaxApertureVars = 32;

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
