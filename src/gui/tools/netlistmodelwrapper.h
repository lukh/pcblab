#ifndef NETLISTMODELWRAPPER_H
#define NETLISTMODELWRAPPER_H

#include <vector>
#include <string>

#include <QAbstractItemModel>

#include "netlisthandler.h"

using namespace std;

class NetlistModelWrapper : public QAbstractItemModel{
    Q_OBJECT
    public:
        NetlistModelWrapper(QObject *parent = NULL);

        //<<< --- QAbstractItemModel implementation
        QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
        Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

        QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
        QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
        int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
        int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
        //--- >>>



        void setHandler(NetlistHandler *inHandler);

    private:
        NetlistHandler *mHandler;

        vector <string> mNetsName;
};

#endif // NETLISTMODELWRAPPER_H
