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

        default:
            //let's ignore the full unknown command
            d_printf("WARNING: unhandled GCode:" + to_string(op_code));
            do{
                ch = inStream.get();
            }while(ch != '*' && ch != EOF);
            break;
    }

    return true;
}
