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
/// The command is executed during AD, either to update variables or create primitives
class ATCommand{
    public:
        ATCommand(): mValid(false) {
#ifdef DEBUG_PRINT
            d_printf("%%% Creating ATCommand", 4, 0, false);
#endif
        }
        virtual ~ATCommand(){
#ifdef DEBUG_PRINT
            d_printf("%%% Deleting ATCommand", 4, 0, false);
#endif
        }

        virtual bool build(ApVarSymbolTable &inVariables, vector<IAperturePrimitive *> &outPrimitives) const = 0;


        bool isValid() const { return mValid; }

    protected:
        bool mValid;
};


/// Updates the variables of the aperture definition
class ATCmdVarDef: public ATCommand{
    public:
        /// create a new variable def
        ///  \param inVariableDef: "$3=5+8x$5"
        ATCmdVarDef(const string &inVariableDef);
        virtual ~ATCmdVarDef() {}

        virtual bool build(ApVarSymbolTable &inVariables, vector<IAperturePrimitive *> &outPrimitives) const;


    private:
        string mDestVar;
        string mExpr;
};



/// creates primitives
class ATCmdPrimitive: public ATCommand{
    public:

        ATCmdPrimitive(const string &inPrimitiveDescr);
        virtual ~ATCmdPrimitive() {}

        virtual bool build(ApVarSymbolTable &inVariables, vector<IAperturePrimitive *> &outPrimitives) const;

    private:
        /// defines the type of primitive to build
        IAperturePrimitive::eType mPrimitiveType;

        /// defines the list of the modifiers.
        /// a modifier can be a decimal, a variable or a arith expr.
        /// modifier are evaluate at the build, and sent to the primitive
        vector <string> mStrModifiers;
};









class IApertureTemplate{
    public:
        IApertureTemplate(const string &inName);
        virtual ~IApertureTemplate();


        const string &getName() const {return mName; }


        virtual bool buildAperturePrimitives(const vector<ApertureModifier> &inModifiers,vector<IAperturePrimitive *> &outPrimitives) = 0;

    protected:
        string mName;
};





/// Implementation of IApertureTemplate for Macros
class MacroApertureTemplate :public IApertureTemplate{
    public:
        MacroApertureTemplate(const string &inName);
        virtual ~MacroApertureTemplate();


        /// used to add a command to the template
        bool addCommand(const string &inCmd);


        /// build primitives and fill in outPrimitives.
        /// Used to define a Aperture
        /// returns true if success
        virtual bool buildAperturePrimitives(const vector<ApertureModifier> &inModifiers,vector<IAperturePrimitive *> &outPrimitives);


    protected:
        /// set the variables to the modifiers values
        void buildVarsFromModifiers(const vector<ApertureModifier> &inModifiers, ApVarSymbolTable &outVariables);


    protected:
        static const uint16_t kMaxApertureVars = 32;

        vector<ATCommand *> mCommands;
};




class ApertureTemplateCircle: public IApertureTemplate{
    public:
        ApertureTemplateCircle();
        virtual ~ApertureTemplateCircle(){}

        virtual bool buildAperturePrimitives(const vector<ApertureModifier> &inModifiers,vector<IAperturePrimitive *> &outPrimitives);
};

class ApertureTemplateRectangle: public IApertureTemplate{
    public:
        ApertureTemplateRectangle();
        virtual ~ApertureTemplateRectangle(){}

        virtual bool buildAperturePrimitives(const vector<ApertureModifier> &inModifiers,vector<IAperturePrimitive *> &outPrimitives);
};

class ApertureTemplateObround: public IApertureTemplate{
    public:
        ApertureTemplateObround();
        virtual ~ApertureTemplateObround(){}

        virtual bool buildAperturePrimitives(const vector<ApertureModifier> &inModifiers,vector<IAperturePrimitive *> &outPrimitives);
};

class ApertureTemplateRegularPolygon: public IApertureTemplate{
    public:
        ApertureTemplateRegularPolygon();
        virtual ~ApertureTemplateRegularPolygon(){}

        virtual bool buildAperturePrimitives(const vector<ApertureModifier> &inModifiers,vector<IAperturePrimitive *> &outPrimitives);
};




#endif
