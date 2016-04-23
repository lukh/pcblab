/*******************************************************************************
* Module: GerberLayer
*
* Project: PcbLab
*
* Author: Vivien HENRY
*
* Date: 2016/03/08
*******************************************************************************/

#ifndef GERBERLAYER_H
#define GERBERLAYER_H

#include <stdint.h>
#include <vector>
#include <string.h>

#include "../common.h"
#include "graphicstate.h"
#include "syntaxparser.h"
#include "graphicobject.h"
#include "aperture/aperture.h"

using namespace std;

/// Describes Gerber Layer, defines by its file
/// Parses the file and generate Graphic Object.
class GerberLayer: public SyntaxParser {
    public:

        /// GerberLevel is a container holding all the graphic object with the same polarity
        class GerberLevel{
            public:
                GerberLevel(GraphicState::eLevelPolarity inPolarity);
                ~GerberLevel();


                // --- MakeGraphicObjects ---
                void makeGraphicObjectDraw(Point inStart, Point inStop, Aperture *inAperture);
                void makeGraphicObjectArc(Point inStart, Point inStop, Point inCenterOffset, GraphicState::eQuadrantMode inQuadrantMode, GraphicState::eInterpolationMode inInterpolationMode, Aperture *inAperture);
                void makeGraphicObjectFlash(Point inPoint, Aperture *inAperture);
                void makeGraphicObjectRegions(Aperture *inAperture);





            private:
                const GraphicState::eLevelPolarity mPolarity;
                vector <IGraphicObject *> mObjects;
        };


        GerberLayer(const string &inName);
        GerberLayer(): GerberLayer("") {}
        virtual ~GerberLayer();

        // ------------------------------------------------------
        // ----------------- I/O methods. -----------------------
        // ------------------------------------------------------

        /// open from the file given
        bool open(const string &inFileName);





    protected:
        // ------------------------------------------------------
        // ----- parser's virtual methods implementation --------
        // ------------------------------------------------------

        /// MO cmd
        virtual void setUnit(GraphicState::eUnit inUnit){
            mState.setUnit(inUnit);
            d_printf("GERBERLAYER: setUnit", 1, 0);
        }

        /// FS cmd
        virtual void setCoordinateFormat(GraphicState::CoordinateFormat inFormat){
            mState.setCoordFormat(inFormat);
            d_printf("GERBERLAYER: setCoordinatesFormat", 1, 0);
        }

        /// G74/75 cmd
        virtual void setQuadrantMode(GraphicState::eQuadrantMode inQuadrantMode){
            mState.setQuadrantMode(inQuadrantMode);
            d_printf("GERBERLAYER: setQuadrantMode mode",1, 0);
        }

        /// G01/02/03 cmd
        virtual void setInterpolationMode(GraphicState::eInterpolationMode inInterpolationMode){
            mState.setInterpolationMode(inInterpolationMode);
            d_printf("GERBERLAYER: setInterpolation mode",1,0);
        }


        virtual void setRegionMode(GraphicState::eRegionMode inRegMode);

        virtual void addNewLevel(GraphicState::eLevelPolarity inPolarity);

        /// defines and create a standard aperture
        virtual void addAperture(uint32_t inDCode, string inTemplateName, const vector<ApertureModifier> &inModifiers);

        virtual void defineApertureTemplate(string &inName, const vector<string> &inRawCmds);

        virtual void setCurrentAperture(uint32_t inDCode);


        /// interpolate operation, D01 code, with X, Y I, J. Not used coordinates are null
        virtual void interpolate(Point inPointXY, Point inPointIJ);

        /// move operation, D02
        virtual void move(Point inPointXY);

        /// flash operation, D03
        virtual void flash(Point inPointXY);



        virtual void handleComment(string &inStr);




        /// convert coordinate from raw to a real life coord (regarding coord format and unit)
        virtual double convertCoordinate(long inRaw);







        // ------------------------------------------------------
        // ---------------- local tools -------------------------
        // ------------------------------------------------------

        Aperture *getApertureByDCode(uint32_t inDCode);

        /// returns the template regarding to the name:
        /// C,R,O,P or macro template name
        ApertureTemplate *getApertureTemplateByName(string &inTemplateName);



    private:
        /// Name of the Layer
        string mName;

        /// Store the current graphic state of the parsing.
        GraphicState mState;

        /// Handles the different GraphicObjects in a GerberLevel container.
        /// the 0 level is the bottom one, it can be overriden by upper layers.
        vector<GerberLevel *> mLevels;

        /// Store a pointer on the current level
        GerberLevel *mCurrentLevel;

        /// Aperture Dictionary.
        vector<Aperture *> mApertures;

        /// Apertures Template Dict
        vector<ApertureTemplate *> mApertureTemplates;

    protected:
        // ------------------------------------------------------
        // ---------------- getters/ setters --------------------
        // ------------------------------------------------------
        string &getName() { return mName; }


        // ------------------------------------------------------
        // ----------------- For unit test ----------------------
        // ------------------------------------------------------
        GraphicState& getState() { return mState; }

        vector<GerberLevel *> &getLevels() { return mLevels; }

        //const vector<Aperture> &getApertures() const { return mApertures; }
        //const vector<ApertureTemplate *> & getApertures() const { return mApertureTemplates; }
};


#endif
