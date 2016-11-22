#ifndef BOMHANDLER_H
#define BOMHANDLER_H

#include <string>
#include <fstream>
#include <map>

#include "tools/fio.h"

#include "pcblab/common.h"

#include "component.h"

#include "bomparser.h"
#include "pnpparser.h"

class ComponentHandler
{
    public:
        ComponentHandler();
        virtual ~ComponentHandler() {}

        /// search into a folder the csv file and call openFile
        bool openFolder(const string &inFolderName);

        void close();

        const Components &getComponents() const { return mComponents; }

        /// returns the list of designators
        void getDesignatorsList(vector <string> & outDesList) const;

        ///returns the list of components des, by prefix order and grouped by sorting Key
        void getSortedAndGroupedDesignatorsList(const vector<string> &inDesPrefixOrder, string inSortingKey, vector <string> & outDesList) const;

        bool getComponent(const string &inDes, Component &outComponent);

    protected:
        bool openBomLayer(const string &inFileName, Components &outComponents);
        bool openPnpLayer(const string &inFileName, Components &outComponents);

    private:
        BomParser mBOM;
        PnpParser mPNP;


        Components mComponents;
};

#endif // BOMHANDLER_H
