#include "netlistmodelwrapper.h"

NetlistModelWrapper::NetlistModelWrapper(QObject *parent):
    QAbstractTableModel(parent),
    mHandler(NULL)
{

}

void NetlistModelWrapper::setHandler(NetlistHandler *inHandler)
{
    if(inHandler == NULL) { return; }

    mHandler = inHandler;

    mHandler->getNetlistList(mNetsName);
}


int NetlistModelWrapper::rowCount(const QModelIndex &parent) const
{
    return mNetsName.size();
}

int NetlistModelWrapper::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant NetlistModelWrapper::data(const QModelIndex &index, int role) const
{
    Net net;

    string name = mNetsName[index.row()];
    if(!mHandler->getNet(name, net)){
        return QVariant();
    }

    if ((role == Qt::DisplayRole || role == Qt::EditRole) && index.column() == 0)
    {
        return QString::fromStdString(net.getName());
    }
    return QVariant();
}

QVariant NetlistModelWrapper::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal && section == 0){
            return QString("Designator");
        }
    }
    return QVariant();
}
