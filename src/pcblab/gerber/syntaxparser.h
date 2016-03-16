/*******************************************************************************
* Module: SyntaxParser
*
* Project: PcbLab
*
* Author: Vivien HENRY
*
* Date: 2016/03/13
*******************************************************************************/

#ifndef SYNTAXPARSER_H
#define SYNTAXPARSER_H

#include <stdint.h>
#include <string>
#include <iostream>

#include "../common.h"

class SyntaxParser {
    public:
        SyntaxParser ();
        virtual ~SyntaxParser ();

    protected:
        void parse(char inChar);

        virtual void makeGraphicObject();
        virtual void defineAperture();

    private:
        void parseDCode(char *inText);
        void parseGCode(char *inText);


#endif
