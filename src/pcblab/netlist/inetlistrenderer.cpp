#include "inetlistrenderer.h"


void INetlistRenderer::clearActiveNets() {
    mActiveNets.clear();
}

void INetlistRenderer::addNet(const string &inNet) {
    mActiveNets.push_back(inNet);
}

void INetlistRenderer::setActiveNet(const string &inNet)
{
    mActiveNets.clear();
    mActiveNets.push_back(inNet);
}

void INetlistRenderer::setActiveNets(vector<string> inNets)
{
    mActiveNets.clear();
    mActiveNets = inNets;
}
