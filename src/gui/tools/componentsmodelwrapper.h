#ifndef COMPONENTSMODELWRAPPER_H
#define COMPONENTSMODELWRAPPER_H

#include <vector>
#include <string>

#include <QAbstractItemModel>

#include "componenthandler.h"

using namespace std;

///an implementation of QAbstractTableModel, for displaying components
class ComponentModelWrapper : public QAbstractItemModel
{
    Q_OBJECT
    public:
        ComponentModelWrapper(QObject *parent = NULL);

        //<<< --- QAbstractItemModel implementation

        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

        virtual QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
        virtual QModelIndex parent(const QModelIndex &index) const;

        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const ;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
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
