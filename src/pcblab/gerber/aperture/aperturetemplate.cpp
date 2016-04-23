#include "aperturetemplate.h"
#include "../../../tools/exprparser.h"

// ------------------------------- CMD -----------------------------

ATCmdVarDef::ATCmdVarDef(const string &inVariableDef): ATCommand(ATCommand::eTypeVarDef) {
    size_t pos_equal;
    if((pos_equal=inVariableDef.find('=')) == string::npos){
        err_printf("ERROR (ATCmdVarDef::ATCmdVarDef): string doesn't have = sign!" );
        return;
    }

    string dest = inVariableDef.substr(0, pos_equal);
    string op = inVariableDef.substr(pos_equal+1);

    if(dest.at(0) != '$'){
        err_printf("ERROR (ATCmdVarDef::ATCmdVarDef): first operand is not a variable!" );
        return;
    }

    mDestVar = dest;
    mExpr = op;

    mValid = true;
}

bool ATCmdVarDef::build(ApVarSymbolTable &inVariables, vector<IAperturePrimitive *> &outPrimitives)
{
    //outPrimitive will not be used here.

    AlgorithmExpr expr(mExpr);

    double val = expr.evaluate(&inVariables);

    inVariables[mDestVar] = val;

    return true;
}


ATCmdPrimitive::ATCmdPrimitive(const string &inPrimitiveDescr): ATCommand(ATCommand::eTypePrimitive) {}

bool ATCmdPrimitive::build(ApVarSymbolTable &inVariables, vector<IAperturePrimitive *> &outPrimitives)
{

}





// --------------------------- ApertureTemplate ------------------
ApertureTemplate::ApertureTemplate(const string &inName): mName(inName){
    d_printf("%%% Creating ApertureTemplate", 4, 0, false);
}

ApertureTemplate::~ApertureTemplate(){
    d_printf("%%% Deleting ApertureTemplate", 4, 0, false);

    for (vector<ATCommand *>::iterator it = mCommands.begin(); it != mCommands.end(); ++it){
        delete (*it);
    }
}

bool ApertureTemplate::addCommand(const string &inCmd)
{
    ATCommand *cmd;
    d_printf("GERBERLAYER/ApertureTemplate "+ mName +": adding " + inCmd, 1, 0);

    //time to define what we have...

    //empty string
    if(inCmd.size() < 1){
        err_printf("ERROR (ApertureTemplate::addCommand): String is empty!" );
        return false;
    }

    //comment...
    if(inCmd.at(0) == '0'){
        return true;
    }


    //it must be a variable def
    if(inCmd.find('=') != string::npos){
        cmd = new ATCmdVarDef(inCmd);
        if(!cmd->isValid()){
            err_printf("ERROR (ApertureTemplate::addCommand): Invalid variable definition!" );
            return false;
        }
        mCommands.push_back(cmd);
    }

    return true;
}


bool ApertureTemplate::buildAperturePrimitives(const vector<ApertureModifier> &inModifiers,vector<IAperturePrimitive *> &outPrimitives){
    bool status = true;

    // check if the output vector is empty
    if(outPrimitives.size() != 0){
        err_printf("ERROR (ApertureTemplate::buildAperturePrimitives): outPrimitives is not empty !" );
        return false;
    }


    // create an empty variables array, and set the variables regarding modifiers
    ApVarSymbolTable variables;
    buildVarsFromModifiers(inModifiers, variables);

    // start the build by calling each command
    for(vector<ATCommand *>::iterator it = mCommands.begin(); it != mCommands.end(); ++it){
        ATCommand *cmd = *it;

        bool ret = cmd->build(variables, outPrimitives);
        status = status && ret;
    }

    return status;
}


void ApertureTemplate::buildVarsFromModifiers(const vector<ApertureModifier> &inModifiers, ApVarSymbolTable &outVariables){
    int ind=1;
    for(int idx = 0; idx < kMaxApertureVars; idx ++){
        string varName = "$" + to_string(ind++);
        if(idx < inModifiers.size()){
            outVariables[varName] = inModifiers[idx];
        }
        else{
            outVariables[varName] = 0.0;
        }
    }
}


ApertureTemplateCircle::ApertureTemplateCircle(): ApertureTemplate("C"){
    //mParamsInfos.push_back(ATParameterInfos("Diameter", ATParameterInfos::eDouble, false));
    //mParamsInfos.push_back(ATParameterInfos("HoleDiameter", ATParameterInfos::eDouble, true));
}

ApertureTemplateRectangle::ApertureTemplateRectangle(): ApertureTemplate("R"){
    //mParamsInfos.push_back(ATParameterInfos("X", ATParameterInfos::eDouble, false));
    //mParamsInfos.push_back(ATParameterInfos("Y", ATParameterInfos::eDouble, false));
    //mParamsInfos.push_back(ATParameterInfos("HoleDiameter", ATParameterInfos::eDouble, true));
}

ApertureTemplateObround::ApertureTemplateObround(): ApertureTemplate("O"){
    //mParamsInfos.push_back(ATParameterInfos("X", ATParameterInfos::eDouble, false));
    //mParamsInfos.push_back(ATParameterInfos("Y", ATParameterInfos::eDouble, false));
    //mParamsInfos.push_back(ATParameterInfos("HoleDiameter", ATParameterInfos::eDouble, true));

}

ApertureTemplateRegularPolygon::ApertureTemplateRegularPolygon(): ApertureTemplate("P"){
    //mParamsInfos.push_back(ATParameterInfos("OuterDiameter", ATParameterInfos::eDouble, false));
    //mParamsInfos.push_back(ATParameterInfos("VerticesNumber", ATParameterInfos::eInt, false));
    //mParamsInfos.push_back(ATParameterInfos("Rotation", ATParameterInfos::eDouble, false));
    //mParamsInfos.push_back(ATParameterInfos("HoleDiameter", ATParameterInfos::eDouble, true));
}

