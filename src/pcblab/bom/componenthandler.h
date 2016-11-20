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

        bool openBomLayer(const string &inFileName, Components &outComponents);
        bool openPnpLayer(const string &inFileName, Components &outComponents);

        void close();

        const Components &getComponents() const { return mComponents; }

        void getDesignatorsList(vector <string> & outDesList) const;

        void getSortedAndGroupedDesignatorsList(vector<string> inDesPrefix, vector <string> & outDesList) const;

    private:
        BomParser mBOM;
        PnpParser mPNP;


        Components mComponents;
};

#endif // BOMHANDLER_H