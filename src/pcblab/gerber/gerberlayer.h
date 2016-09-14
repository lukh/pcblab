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
/// Parses the file and generates Graphic Object.
class GerberLayer: public SyntaxParser {
    public:

        /// GerberLevel is a container holding all the graphic object with the same polarity
        class GerberLevel{
            public:
                GerberLevel(GraphicState::eLevelPolarity inPolarity);
                ~GerberLevel();


                // --- MakeGraphicObjects ---
                void makeGraphicObjectDraw(plPoint inStart, plPoint inStop, Aperture *inAperture);
                void makeGraphicObjectArc(plPoint inStart, plPoint inStop, plPoint inCenterOffset, GraphicState::eQuadrantMode inQuadrantMode, GraphicState::eInterpolationMode inInterpolationMode, Aperture *inAperture);
                void makeGraphicObjectFlash(plPoint inPoint, Aperture *inAperture);
                void makeGraphicObjectRegions(Aperture *inAperture);


                vector <IGraphicObject *> getObjects() { return mObjects; }
                GraphicState::eLevelPolarity getPolarity() const { return mPolarity; }


                plRectangle getBoundingBox() const;


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

        // ------------------------------------------------------
        // ---------------- getters/ setters --------------------
        // ------------------------------------------------------
        string getName() const { return mName; }

        uint16_t getLevelsCount() const { return mLevels.size(); }

        GerberLevel *getLevel(uint16_t inIdx) const;

        plRectangle getBoundingBox() const;





    protected:
        // ------------------------------------------------------
        // ----- parser's virtual methods implementation --------
        // ------------------------------------------------------

        /// MO cmd
        virtual void setUnit(GraphicState::eUnit inUnit){
            mState.setUnit(inUnit);
#ifdef DEBUG_PRINT
            d_printf("GERBERLAYER: setUnit", 1, 0);
#endif
        }

        /// FS cmd
        virtual void setCoordinateFormat(GraphicState::CoordinateFormat inFormat){
            if(!inFormat.isValid()){
                err_printf("WARNING: The coordinate format is not valid !");
                // Ok.. let's allow it anyway. You were warned
            }

            mState.setCoordFormat(inFormat);
#ifdef DEBUG_PRINT
            d_printf("GERBERLAYER: setCoordinatesFormat", 1, 0);
#endif
        }

        /// G74/75 cmd
        virtual void setQuadrantMode(GraphicState::eQuadrantMode inQuadrantMode){
            mState.setQuadrantMode(inQuadrantMode);
#ifdef DEBUG_PRINT
            d_printf("GERBERLAYER: setQuadrantMode mode",1, 0);
#endif
        }

        /// G01/02/03 cmd
        virtual void setInterpolationMode(GraphicState::eInterpolationMode inInterpolationMode){
            mState.setInterpolationMode(inInterpolationMode);
#ifdef DEBUG_PRINT
            d_printf("GERBERLAYER: setInterpolation mode",1,0);
#endif
        }


        virtual void setRegionMode(GraphicState::eRegionMode inRegMode);

        virtual void addNewLevel(GraphicState::eLevelPolarity inPolarity);

        /// defines and create a standard aperture
        virtual void addAperture(uint32_t inDCode, string inTemplateName, const vector<ApertureModifier> &inModifiers);

        virtual void defineApertureTemplate(string &inName, const vector<string> &inRawCmds);

        virtual void setCurrentAperture(uint32_t inDCode);


        /// interpolate operation, D01 code, with X, Y I, J. Not used coordinates are null
        virtual void interpolate(plPoint inPointXY, plPoint inPointIJ);

        /// move operation, D02
        virtual void move(plPoint inPointXY);

        /// flash operation, D03
        virtual void flash(plPoint inPointXY);



        virtual void handleComment(string &inStr);




        /// convert coordinate from raw to a real life coord (regarding coord format and unit)
        virtual double convertCoordinate(int32_t inRaw);







        // ------------------------------------------------------
        // ---------------- local tools -------------------------
        // ------------------------------------------------------

        Aperture *getApertureByDCode(uint32_t inDCode);

        /// returns the template regarding to the name:
        /// C,R,O,P or macro template name
        IApertureTemplate *getApertureTemplateByName(string &inTemplateName);



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
        vector<IApertureTemplate *> mApertureTemplates;

    protected:
        // ------------------------------------------------------
        // ----------------- For unit test ----------------------
        // ------------------------------------------------------
        GraphicState& getState() { return mState; }

        //const vector<Aperture> &getApertures() const { return mApertures; }
        //const vector<ApertureTemplate *> & getApertures() const { return mApertureTemplates; }
};


#endif
