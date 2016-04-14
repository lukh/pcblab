#include "../syntaxparser.h"

bool SyntaxParser::parseGCode(istream &inStream){
    char ch;
    int op_code = getOpCode(inStream);
    string comment;

    switch(op_code){
        // comment
        case 04:
            comment.clear();
            while((ch = inStream.get()) != EOF){
                if(ch == '*') { break; }
                comment.push_back(ch);
            }
            handleComment(comment);
            break;

        case 01:
            if(inStream.get() != '*'){
                return false;
            }
            setInterpolationMode(GraphicState::eInterpolLinear);
            break;

        case 02:
            if(inStream.get() != '*'){
                return false;
            }
            setInterpolationMode(GraphicState::eInterpolCWCircular);
            break;

        case 03:
            if(inStream.get() != '*'){
                return false;
            }
            setInterpolationMode(GraphicState::eInterpolCCWCircular);
            break;


        case 74:
            if(inStream.get() != '*'){
                return false;
            }
            setQuadrantMode(GraphicState::eQuadrantSingle);
            break;

        case 75:
            if(inStream.get() != '*'){
                return false;
            }
            setQuadrantMode(GraphicState::eQuadrantMulti);
            break;

        default:
            //let's ignore the full unknown command
            err_printf("WARNING: unhandled GCode:" + to_string(op_code));
            do{
                ch = inStream.get();
            }while(ch != '*' && ch != EOF);
            break;
    }

    return true;
}
