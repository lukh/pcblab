#include "NetlistParser.h"

bool NetlistParser::parse(istream &inStream){
    // clear the existing nets
    mNets.clear();


    //scan the file
    string line;
    while(getline(inStream, line)){
        // check lenght of the line, should be 80
        if(line.size() != 80){
            err_printf("ERROR:(NetlistParser::parse): the line size in not 80cols")
            continue;
        }

        switch(line[0]){
            //comment
            case 'C':
                break;

            case 'P':
                break;

            default:
                parseOperation(line);
                break;
        }
    }
}


void NetlistParser::parseOperation(const string &inString){
    // let's slit...
}