#ifndef PCBLAB_H
#define PCBLAB_H

#include "gerber/gerberhandler.h"

/// A manager for a full set of pcba object (gerber, nets, components)
/// Main object manipulated by the UI
class PcbLab
{
    public:
        PcbLab();


        //getters
        GerberHandler &getGerberHandler() { return mGerberHandler; }


    private:
        GerberHandler mGerber;

};

#endif // PCBLAB_H
