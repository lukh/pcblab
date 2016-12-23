#include "icomponentrenderer.h"


void IComponentRenderer::clearActiveComponents() {
    mActiveComponents.clear();
    mAllComponents= false;
}

void IComponentRenderer::addComponent(const string &inDesignator) {
    mActiveComponents.push_back(inDesignator);
}

void IComponentRenderer::setActiveComponent(const string &inDesignator)
{
    mActiveComponents.clear();
    mActiveComponents.push_back(inDesignator);
}

void IComponentRenderer::setActiveComponents(vector<string> inDesignators)
{
    mActiveComponents.clear();
    mActiveComponents = inDesignators;
}
