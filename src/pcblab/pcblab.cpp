#include "pcblab.h"

PcbLab::PcbLab()
{

}

void PcbLab::openFolder(const string &inFolderName, GerberHandler::ExtensionOrderList inOrder)
{
    mGerber.openFolder(inFolderName, inOrder);
    mExcellon.openFolder(inFolderName);
    bool compo_status = mComponent.openFolder(inFolderName);
    bool nets_status = mNetlist.openFolder(inFolderName);

    if(compo_status && nets_status){
        linkNetsToComponents();
    }
}

void PcbLab::linkNetsToComponents()
{
    vector <string> net_list;
    mNetlist.getNetlistList(net_list);
    for (vector<string>::iterator nn_it = net_list.begin(); nn_it != net_list.end(); ++nn_it){
        string &netname = *nn_it;
        Net *n;
        if(mNetlist.getNet(netname, &n)){
            for(vector<NetEntry>::const_iterator ne_it = n->getEntries().begin(); ne_it != n->getEntries().end(); ++ne_it){
                const NetEntry &ne = *ne_it;

                Component *c;
                if(mComponent.getComponent(ne.getDesignator(), &c)){
                    c->addNet(netname);
                }
            }
        }
    }
}
