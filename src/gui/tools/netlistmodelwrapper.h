#ifndef NETLISTMODELWRAPPER_H
#define NETLISTMODELWRAPPER_H

#include <vector>
#include <string>

#include <QAbstractItemModel>

#include "netlisthandler.h"

using namespace std;

class NetlistModelWrapper : public QAbstractTableModel{
    Q_OBJECT
    public:
        NetlistModelWrapper(QObject *parent);

        //<<< --- QAbstractTableModel implementation
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const ;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        //--- >>>

        void setHandler(NetlistHandler *inHandler);

    private:
        NetlistHandler *mHandler;

        vector <string> mNetsName;
};

#endif // NETLISTMODELWRAPPER_H
