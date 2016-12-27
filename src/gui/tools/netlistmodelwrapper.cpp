#include "netlistmodelwrapper.h"

NetlistModelWrapper::NetlistModelWrapper(QObject *parent):
    QAbstractItemModel(parent),
    mHandler(NULL)
{

}

void NetlistModelWrapper::setHandler(NetlistHandler *inHandler)
{
    if(inHandler == NULL) { return; }

    mHandler = inHandler;

    mHandler->getNetlistList(mNetsName);
}


QVariant NetlistModelWrapper::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
            return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    if(!index.parent().isValid()){
        return QString::fromStdString(mNetsName[index.row()]);
    }

    //return QString::fromStdString(zeMap[index.parent().row()][index.row()]);
    return QString("Compo 1");
}

Qt::ItemFlags NetlistModelWrapper::flags(const QModelIndex &index) const
{
    return Qt::NoItemFlags;
}

QVariant NetlistModelWrapper::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

QModelIndex NetlistModelWrapper::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row,column, parent.isValid() ? parent.row() : -1);
}

QModelIndex NetlistModelWrapper::parent(const QModelIndex &index) const
{
    return index.internalId() >=0 ? createIndex(index.internalId(),0) : QModelIndex();
}

int NetlistModelWrapper::rowCount(const QModelIndex &parent) const
{
    return !parent.isValid() ? mNetsName.size() :
        !parent.parent().isValid() ? 1 : 0;
        //!parent.parent().isValid() ? zeMap[parent.row()].size() : 0;
}

int NetlistModelWrapper::columnCount(const QModelIndex &parent) const
{
    return 1;
}
