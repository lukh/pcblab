#include "aperturetemplate.h"


ApertureTemplate::ApertureTemplate(const string &inName): mName(inName){
    d_printf("%%% Creating ApertureTemplate", 4, 0, false);
}

ApertureTemplate::~ApertureTemplate(){
    d_printf("%%% Deleting ApertureTemplate", 4, 0, false);
}

bool ApertureTemplate::addCommand(const string &inCmd)
{
    d_printf("GERBERLAYER/ApertureTemplate "+ mName +": adding " + inCmd, 1, 0);

    //time to define what we have...
}


bool ApertureTemplate::buildAperturePrimitives(const vector<ApertureModifier> &inModifiers,vector<IAperturePrimitive *> &outPrimitives){
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


void ApertureTemplate::buildVarsFromModifiers(const vector<ApertureModifier> &inModifiers, vector<ApertureVariable> &outVariables){
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
