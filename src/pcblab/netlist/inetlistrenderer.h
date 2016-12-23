#ifndef INETLISTRENDERER_H
#define INETLISTRENDERER_H

#include <vector>

#include "pcblab/common.h"

#include "netlisthandler.h"

using namespace std;

class INetlistRenderer
{
    public:
        INetlistRenderer(): mColor(255, 255, 255), mTransparency(255) {}
        virtual ~INetlistRenderer() {}

        //<<< ---
        /// draws the netlist given
        virtual void draw(const NetlistHandler &inNetlistHandler) = 0;

        // draw one net
        virtual void drawNet(const Net &inNet) = 0;
        //--- >>>

        /// changes the color
        void setColor(Color inColor){ mColor = inColor; }
        Color getColor() const { return mColor; }

        /// changes transparency
        void setTransparency(uint8_t inTransp){ mTransparency = inTransp; }
        uint8_t getTransparency() const { return mTransparency; }


        void clearActiveNets();
        void addNet(const string & inDesignator);

        void setActiveNet(const string & inNet);
        void setActiveNets(vector <string> inNets);

    protected:
        Color mColor;
        uint8_t mTransparency;

        vector <string> mActiveNets;
};

#endif // ICOMPONENTRENDERER_H
