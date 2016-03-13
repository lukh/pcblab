/*******************************************************************************
* Module: SyntaxParser
*
* Project: PcbLab
*
* Author: Vivien HENRY
*
* Date: 2016/03/13
*******************************************************************************/

#include <stdint.h>
#include <string.h>
#include <iostream.h>

#include "common.h"

class SyntaxParser {
    public:
        SyntaxParser ();
        virtual ~SyntaxParser ();


        void parse(char inChar);

    private:
};
