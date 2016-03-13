#ifndef GERBERLAYER_H
#define GERBERLAYER_H

#include <stdint.h>
#include <vector>
#include <string.h>
#include "common.h"
#include "graphicstate.h"

using namespace std;

/// Describes Gerber Layer, defines by its file
/// Parses the file and generate Graphic Object.
class GerberLayer {
    public:

        /// GerberLevel is a container holding all the graphic object with the same polarity
        class GerberLevel{
            private:
                GraphicState::eLevelPolarity mPolarity;
                vector <GraphicObject *> mObjects;
        };


        GerberLayer ();
        virtual ~GerberLayer ();


        // --- I/O methods. ---

        /// open from the file given
        bool open(string &inFileName);

    private:

        /// Handles the different GraphicObjects in a GerberLevel container.
        /// the 0 level is the bottom one, it can be overriden by upper layers.
        vector<GerberLevel> mLevels;
};


#endif
