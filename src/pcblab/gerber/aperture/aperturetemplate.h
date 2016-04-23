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

        /// build primitives and fill in outPrimitives
        /// returns true if success
        bool buildAperturePrimitives(const vector<ApertureModifier> &inModifiers,vector<IAperturePrimitive *> &outPrimitives){
            bool status = true;

            // check if the output vector is empty
            if(outPrimitives.size() != 0){
                err_printf("ERROR (ApertureTemplate::buildAperturePrimitives): outPrimitives is not empty !" );
                return false;
            }


            // create an empty variables array, and set the variables regarding modifiers
            vector<ApertureVariable> variables(kMaxApertureVars, 0.0);
            buildVarsFromModifiers(inModifiers, variables);

            // start the build by calling each command
            for(vector<ATCommand *>::iterator it = mCommands.begin(); it != mCommands.end(); ++it){
                ATCommand *cmd = *it;

                bool ret = cmd->build(variables, outPrimitives);
                status = status && ret;
            }

            return status;
        }


    protected:
        /// set the variables to the modifiers values
        void buildVarsFromModifiers(const vector<ApertureModifier> &inModifiers, vector<ApertureVariable> &outVariables){
            int inc = 0;
            int modif_size = inModifiers.size();

            for(vector<ApertureVariable>::iterator var = outVariables.begin(); var != outVariables.end(); ++var){
                if(inc < modif_size){
                    *var = inModifiers[inc++];
                }
                else{
                    *var = 0.0;
                }
            }
        }

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
