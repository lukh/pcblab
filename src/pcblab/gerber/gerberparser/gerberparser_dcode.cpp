#include "../gerberparser.h"

bool GerberParser::parseDCode(istream &inStream){
    int op_code = getOpCode(inStream);

    //we should reach a * here
    if(inStream.get() != '*'){
       err_printf("ERROR (SyntaxParser::parseDCode): No * at the end of the DCode" );
        return false;
    }

    switch(op_code){
        case 00:
            err_printf("ERROR (SyntaxParser::parseDCode): D00 is invalid" );
            break;

        case 01:
#ifdef DEBUG_PRINT
            d_printf("SyntaxParser(DCode) > Interpolate(D01)", 2, 1);
#endif
            interpolate(mXY, mIJ);
            break;

        case 02:
#ifdef DEBUG_PRINT
            d_printf("SyntaxParser(DCode) > Move(D02)", 2, 1);
#endif
            move(mXY);
            break;
        case 03:
#ifdef DEBUG_PRINT
            d_printf("SyntaxParser(DCode) > Flash(D03)", 2, 1);
#endif
            flash(mXY);
            break;

        default:
#ifdef DEBUG_PRINT
            d_printf("SyntaxParser(DCode) > Change Aperture(Dnn)", 2, 1);
#endif
            if(op_code >= 10){
                setCurrentAperture(op_code);
            }
            else{
               err_printf("ERROR (SyntaxParser::parseDCode): DCode out of bounds" );
                return false;
            }
            break;
    }

    return true;
}
