#include "aperturetemplate.h"
#include "../../../tools/exprparser.h"
#include "../../../tools/stringsplitter.h"
// ------------------------------- CMD -----------------------------

ATCmdVarDef::ATCmdVarDef(const string &inVariableDef): ATCommand() {
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

    d_printf("ATCmdVarDef::build " + mDestVar + " = " + mExpr + " = " + to_string(val), 1, 1);

    return true;
}


ATCmdPrimitive::ATCmdPrimitive(const string &inPrimitiveDescr): ATCommand() {
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

    d_printf("ATCmdPrimitive::build", 1, 1);

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

        d_printf("ATCmdPrimitive::build, modifier " + strmod + " = " + to_string(mod), 1, 2);

        prim->addModifier(mod);
    }

    bool prim_ok = prim->isValid();

    if(prim_ok){
        d_printf("ATCmdPrimitive::build, adding: " + prim->getStringInfos(), 1, 2);
        outPrimitives.push_back(prim);
    }

    else{
        err_printf("ERROR(ATCmdPrimitive::build) the primitive build failed");
        delete prim;
    }


    return prim_ok;
}





// --------------------------- ApertureTemplate ------------------

IApertureTemplate::IApertureTemplate(const string &inName): mName(inName)
{
    d_printf("%%% Creating IApertureTemplate", 4, 0, false);
}

IApertureTemplate::~IApertureTemplate()
{
    d_printf("%%% Deleting IApertureTemplate", 4, 0, false);
}





MacroApertureTemplate::MacroApertureTemplate(const string &inName): IApertureTemplate(inName){

}



MacroApertureTemplate::~MacroApertureTemplate(){
    d_printf("%%% Deleting MacroApertureTemplate", 4, 0, false);

    for (vector<ATCommand *>::iterator it = mCommands.begin(); it != mCommands.end(); ++it){
        delete (*it);
    }
}

bool MacroApertureTemplate::addCommand(const string &inCmd)
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


bool MacroApertureTemplate::buildAperturePrimitives(const vector<ApertureModifier> &inModifiers,vector<IAperturePrimitive *> &outPrimitives){
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


void MacroApertureTemplate::buildVarsFromModifiers(const vector<ApertureModifier> &inModifiers, ApVarSymbolTable &outVariables){
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








ApertureTemplateCircle::ApertureTemplateCircle(): IApertureTemplate("C"){
}

bool ApertureTemplateCircle::buildAperturePrimitives(const vector<ApertureModifier> &inModifiers, vector<IAperturePrimitive *> &outPrimitives)
{
    if(inModifiers.size() < 1){
        return false;
    }

    //adding a circle primitive.
    IAperturePrimitive *p = new APrimCircle();

    //adding needed modifiers
    p->addModifier(1.0); //exposure on
    p->addModifier(inModifiers.at(0)); //D
    p->addModifier(0.0); //x
    p->addModifier(0.0); //y
    p->addModifier(0.0); //rot

    if(p->isValid()){
        outPrimitives.push_back(p);
        d_printf("ApertureTemplateCircle::buildAperturePrimitives: " + p->getStringInfos(), 1, 2);
    }

    else{
        err_printf("ERROR(ApertureTemplateCircle::build) the primitive build failed");
        delete p;
        return false;
    }

    //asking for a hole
    if(inModifiers.size() > 1){
        p = new APrimCircle();

        p->addModifier(0.0); //exposure off
        p->addModifier(inModifiers.at(1));
        p->addModifier(0.0); //x
        p->addModifier(0.0); //y
        p->addModifier(0.0); //rot

        if(p->isValid()){
            outPrimitives.push_back(p);
            d_printf("ApertureTemplateCircle::buildAperturePrimitives: " + p->getStringInfos(), 1, 2);
        }

        else{
            err_printf("ERROR(ApertureTemplateCircle::build) the primitive build failed");
            delete p;
            return false;
        }
    }

    return true;
}

ApertureTemplateRectangle::ApertureTemplateRectangle(): IApertureTemplate("R"){
}

bool ApertureTemplateRectangle::buildAperturePrimitives(const vector<ApertureModifier> &inModifiers, vector<IAperturePrimitive *> &outPrimitives)
{
    if(inModifiers.size() < 2){
        return false;
    }

    //adding a circle primitive.
    IAperturePrimitive *p = new APrimCenterLine();

    //adding needed modifiers
    p->addModifier(1.0); //exposure on
    p->addModifier(inModifiers.at(0)); //w
    p->addModifier(inModifiers.at(1)); //h
    p->addModifier(0.0); // x
    p->addModifier(0.0); // y
    p->addModifier(0.0); // rot

    if(p->isValid()){
        outPrimitives.push_back(p);
        d_printf("ApertureTemplateRectangle::buildAperturePrimitives: " + p->getStringInfos(), 1, 2);d_printf("ATCmdPrimitive/build: " + p->getStringInfos(), 1, 2);
    }

    else{
        err_printf("ERROR(ApertureTemplateRectangle::build) the primitive build failed");
        delete p;
        return false;
    }


    //asking for a hole
    if(inModifiers.size() > 2){
        p = new APrimCircle();

        p->addModifier(0.0); //exposure off
        p->addModifier(inModifiers.at(2)); //D
        p->addModifier(0.0); //x
        p->addModifier(0.0); //y
        p->addModifier(0.0); //rot

        if(p->isValid()){
            outPrimitives.push_back(p);
            d_printf("ApertureTemplateRectangle::buildAperturePrimitives: " + p->getStringInfos(), 1, 2);
        }

        else{
            err_printf("ERROR(ApertureTemplateRectangle::build) the primitive build failed");
            delete p;
            return false;
        }
    }

    return true;
}

ApertureTemplateObround::ApertureTemplateObround(): IApertureTemplate("O"){
    //mParamsInfos.push_back(ATParameterInfos("X", ATParameterInfos::eDouble, false));

}

bool ApertureTemplateObround::buildAperturePrimitives(const vector<ApertureModifier> &inModifiers, vector<IAperturePrimitive *> &outPrimitives)
{
    IAperturePrimitive *p = 0;
    if(inModifiers.size() < 2){
        return false;
    }

    double X = inModifiers.at(0), Y = inModifiers.at(1);


    //horizontal
    if(X > Y){
        // rectangle
        p = new APrimCenterLine();
        p->addModifier(1.0); //exposure on
        p->addModifier( X - Y ); //Width
        p->addModifier(Y); //Height
        p->addModifier(0.0); //x
        p->addModifier(0.0); //y
        p->addModifier(0.0); //rot

        if(p->isValid()){
            outPrimitives.push_back(p);
            d_printf("ApertureTemplateObround::buildAperturePrimitives: " + p->getStringInfos(), 1, 2);
        }

        else{
            err_printf("ERROR(ApertureTemplateObround::build) the primitive build failed");
            delete p;
            return false;
        }

        //adding the two circle primitives.
        p = new APrimCircle();
        //adding needed modifiers
        p->addModifier(1.0); //exposure on
        p->addModifier(Y); //D
        p->addModifier(-(X-Y)/2.0); //x
        p->addModifier(0.0); //y
        p->addModifier(0.0); //rot

        if(p->isValid()){
            outPrimitives.push_back(p);
            d_printf("ApertureTemplateObround::buildAperturePrimitives: " + p->getStringInfos(), 1, 2);
        }

        else{
            err_printf("ERROR(ApertureTemplateObround::build) the primitive build failed");
            delete p;
            return false;
        }

        p = new APrimCircle();
        //adding needed modifiers
        p->addModifier(1.0); //exposure on
        p->addModifier(Y); //D
        p->addModifier((X-Y)/2.0); //x
        p->addModifier(0.0); //y
        p->addModifier(0.0); //rot

        if(p->isValid()){
            outPrimitives.push_back(p);
            d_printf("ApertureTemplateObround::buildAperturePrimitives: " + p->getStringInfos(), 1, 2);
        }

        else{
            err_printf("ERROR(ApertureTemplateObround::build) the primitive build failed");
            delete p;
            return false;
        }
    }

    //vertical
    else {
        // rectangle
        p = new APrimCenterLine();
        p->addModifier(1.0); //exposure on
        p->addModifier(X); //Width
        p->addModifier(Y-X); //Height
        p->addModifier(0.0); //x
        p->addModifier(0.0); //y
        p->addModifier(0.0); //rot

        if(p->isValid()){
            outPrimitives.push_back(p);
            d_printf("ApertureTemplateObround::buildAperturePrimitives: " + p->getStringInfos(), 1, 2);
        }

        else{
            err_printf("ERROR(ApertureTemplateObround::build) the primitive build failed");
            delete p;
            return false;
        }

        //adding the two circle primitives.
        p = new APrimCircle();
        //adding needed modifiers
        p->addModifier(1.0); //exposure on
        p->addModifier(X); //D
        p->addModifier(-(Y-X)/2.0); //x
        p->addModifier(0.0); //y
        p->addModifier(0.0); //rot

        if(p->isValid()){
            outPrimitives.push_back(p);
            d_printf("ApertureTemplateObround::buildAperturePrimitives: " + p->getStringInfos(), 1, 2);
        }

        else{
            err_printf("ERROR(ApertureTemplateObround::build) the primitive build failed");
            delete p;
            return false;
        }

        p = new APrimCircle();
        //adding needed modifiers
        p->addModifier(1.0); //exposure on
        p->addModifier(X); //D
        p->addModifier((Y-X)/2.0); //x
        p->addModifier(0.0); //y
        p->addModifier(0.0); //rot

        if(p->isValid()){
            outPrimitives.push_back(p);
            d_printf("ApertureTemplateObround::buildAperturePrimitives: " + p->getStringInfos(), 1, 2);
        }

        else{
            err_printf("ERROR(ApertureTemplateObround::build) the primitive build failed");
            delete p;
            return false;
        }
    }



    //asking for a hole
    if(inModifiers.size() > 2){
        p = new APrimCircle();

        p->addModifier(0.0); //exposure off
        p->addModifier(inModifiers.at(2));
        p->addModifier(0.0); //x
        p->addModifier(0.0); //y
        p->addModifier(0.0); //rot

        if(p->isValid()){
            outPrimitives.push_back(p);
            d_printf("ApertureTemplateObround::buildAperturePrimitives: " + p->getStringInfos(), 1, 2);
        }

        else{
            err_printf("ERROR(ApertureTemplateObround::build) the primitive build failed");
            delete p;
            return false;
        }
    }

    return true;
}

ApertureTemplateRegularPolygon::ApertureTemplateRegularPolygon(): IApertureTemplate("P"){

}

bool ApertureTemplateRegularPolygon::buildAperturePrimitives(const vector<ApertureModifier> &inModifiers, vector<IAperturePrimitive *> &outPrimitives)
{
    /*<Outer diameter> Diameter of the circumscribed circle, i.e. the circle through the
    polygon vertices. Must be a decimal > 0

    <Number of vertices> Number of polygon vertices, ranging from 3 to 12

    <Rotation> A decimal number specifying the rotation in degrees of the
    aperture around its center
    Without rotation one vertex is on the positive X-axis through
    the center. Rotation angle is expressed in decimal degrees;
    positive value for counterclockwise rotation, negative value for
    clockwise rotation

    <Hole diameter>
    Diameter of a round hole. If missing the aperture is solid
    See section 4.12.1.5 for more details.
    The hole modifier can be specified only after a rotation angle;
    set an angle of zero if the aperture is not rotated.*/

    if(inModifiers.size() < 3){
        return false;
    }

    double outer_dia = inModifiers.at(0);
    double vert_number = inModifiers.at(1);
    double rotation = inModifiers.at(2);

    IAperturePrimitive *p = new APrimPolygon();
    p->addModifier(1.0); //exposure on
    p->addModifier(vert_number);
    p->addModifier(0.0); //X
    p->addModifier(0.0); //Y
    p->addModifier(outer_dia); //D
    p->addModifier(rotation);

    if(p->isValid()){
        outPrimitives.push_back(p);
        d_printf("ApertureTemplateRegularPolygon::buildAperturePrimitives: " + p->getStringInfos(), 1, 2);
    }

    else{
        err_printf("ERROR(ApertureTemplateRegularPolygon::build) the primitive build failed");
        delete p;
        return false;
    }


    //asking for a hole
    if(inModifiers.size() > 3){
        p = new APrimCircle();

        p->addModifier(0.0); //exposure off
        p->addModifier(inModifiers.at(3));
        p->addModifier(0.0); //x
        p->addModifier(0.0); //y
        p->addModifier(0.0); //rot

        if(p->isValid()){
            outPrimitives.push_back(p);
            d_printf("ApertureTemplateRegularPolygon::buildAperturePrimitives: " + p->getStringInfos(), 1, 2);
        }

        else{
            err_printf("ERROR(ApertureTemplateRegularPolygon::build) the primitive build failed");
            delete p;
            return false;
        }
    }

    return true;
}


