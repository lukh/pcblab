#ifndef ICOMPONENTRENDERER_H
#define ICOMPONENTRENDERER_H

#include <vector>

#include "pcblab/common.h"

#include "componenthandler.h"

using namespace std;

class IComponentRenderer
{
    public:
        IComponentRenderer(): mColor(0, 230, 230), mTransparency(255), mAllComponents(false) {}
        virtual ~IComponentRenderer() {}

        //<<< ---
        /// draws the ExcellonHandler given
        virtual void draw(const ComponentHandler &inComponentHandler) = 0;

        // draw one component
        virtual void drawComponent(const Component &inCompo) = 0;
        //--- >>>

        /// changes the color
        void setColor(Color inColor){ mColor = inColor; }
        Color getColor() const { return mColor; }

        /// changes transparency
        void setTransparency(uint8_t inTransp){ mTransparency = inTransp; }
        uint8_t getTransparency() const { return mTransparency; }


        void clearActiveComponents();
        void addComponent(const string & inDesignator);
        void activeAllComponents() { mAllComponents = true; }

        void setActiveComponent(const string & inDesignator);
        void setActiveComponents(vector <string> inDesignators);

    protected:
        Color mColor;
        uint8_t mTransparency;

        vector <string> mActiveComponents;
        bool mAllComponents;
};

#endif // ICOMPONENTRENDERER_H
