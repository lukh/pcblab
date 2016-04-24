#include "aperturetemplate.h"
#include "../../../tools/exprparser.h"
#include "../../../tools/stringsplitter.h"
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
    if(!mValid){
        return false;
    }

    //outPrimitive will not be used here.
    (void)outPrimitives;

    AlgorithmExpr expr(mExpr);

    double val = expr.evaluate(&inVariables);

    inVariables[mDestVar] = val;

    d_printf("ATCmdVarDef/build " + mDestVar + " = " + mExpr + " = " + to_string(val), 1, 2);

    return true;
}


ATCmdPrimitive::ATCmdPrimitive(const string &inPrimitiveDescr): ATCommand(ATCommand::eTypePrimitive) {



    size_t pos_first_del = inPrimitiveDescr.find(',');
    if(pos_first_del == string::npos){
        err_printf("ERROR (ATCmdPrimitive::ATCmdPrimitive): Invalid primitive def!" );
        return;
    }
    string prim_code_str = inPrimitiveDescr.substr(0, pos_first_del);
    string prim_mods = inPrimitiveDescr.substr(pos_first_del+1);

    //define the type of primitive
    uint32_t prim_code = stoi(prim_code_str); //extract the primirive code
    switch(prim_code){
        case 1: // circle
            mPrimitiveType = IAperturePrimitive::eCircle;
            break;
        case 20: //vector line
            mPrimitiveType = IAperturePrimitive::eVectorLine;
            break;
        case 21: // center line
            mPrimitiveType = IAperturePrimitive::eCenterLine;
            break;
        case 4://outline
            mPrimitiveType = IAperturePrimitive::eOutLine;
            break;
        case 5: //polygon
            mPrimitiveType = IAperturePrimitive::ePolygon;
            break;
        case 6: // moire
            mPrimitiveType = IAperturePrimitive::eMoire;
            break;
        case 7: //thermal
            mPrimitiveType = IAperturePrimitive::eThermal;
            break;

        default:
            mPrimitiveType = IAperturePrimitive::eInvalid;
            err_printf("ERROR (ATCmdPrimitive::ATCmdPrimitive): unrecognized primitive!" );
            return;
            break;
    }


    //extract the modifiers
    tokenize(prim_mods, mStrModifiers, ",");

    mValid = true;
}

bool ATCmdPrimitive::build(ApVarSymbolTable &inVariables, vector<IAperturePrimitive *> &outPrimitives)
{
    if(!mValid){
        return false;
    }

    d_printf("ATCmdPrimitive::build", 1, 2);

    //creating the primitive...
    IAperturePrimitive *prim;
    switch (mPrimitiveType) {
        case IAperturePrimitive::eCircle:
            prim = new APrimCircle();
            break;

        case IAperturePrimitive::eVectorLine:
            prim = new APrimVectorLine();
            break;

        case IAperturePrimitive::eCenterLine:
            prim = new APrimCenterLine();
            break;

        case IAperturePrimitive::eOutLine:
            prim = new APrimOutline();
            break;

        case IAperturePrimitive::ePolygon:
            prim = new APrimPolygon();
            break;

        case IAperturePrimitive::eMoire:
            prim = new APrimMoire();
            break;

        case IAperturePrimitive::eThermal:
            prim = new APrimThermal();
            break;

        default:
            err_printf("ERROR (ATCmdPrimitive::build): unrecognized primitive!" );
            return false;
            break;
    }


    //evaluate and add modifiers
    for (vector<string>::iterator it = mStrModifiers.begin(); it != mStrModifiers.end(); ++it) {
        string &strmod = *it;

        AlgorithmExpr expr(strmod);

        ApertureModifier mod = expr.evaluate(&inVariables);

        d_printf("ATCmdPrimitive/build, modifier " + strmod + " = " + to_string(mod), 1, 2);

        prim->addModifier(mod);
    }


    outPrimitives.push_back(prim);


    return true;
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

    else{
        cmd = new ATCmdPrimitive(inCmd);
        if(!cmd->isValid()){
            err_printf("ERROR (ApertureTemplate::addCommand): Invalid primitive def!" );
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
    for(size_t idx = 0; idx < kMaxApertureVars; idx ++){
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

