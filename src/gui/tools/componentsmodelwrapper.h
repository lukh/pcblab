#ifndef COMPONENTSMODELWRAPPER_H
#define COMPONENTSMODELWRAPPER_H

#include <vector>
#include <string>

#include <QAbstractTableModel>

#include "componenthandler.h"

using namespace std;

///an implementation of QAbstractTableModel, for displaying components
class ComponentModelWrapper : public QAbstractTableModel
{
    Q_OBJECT
    public:
        ComponentModelWrapper(QObject *parent = NULL);

        //<<< --- QAbstractTableModel implementation
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const ;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        //--- >>>


        void setHandler(ComponentHandler *inHandler);

        void setParameters(vector <string> inParams) {mParameters = inParams; }

    private:
        ComponentHandler *mHandler;
        vector <string> mDesList;

        /// parameters used inside the model
        vector <string> mParameters;
};

#endif // COMPONENTSMODELWRAPPER_H
