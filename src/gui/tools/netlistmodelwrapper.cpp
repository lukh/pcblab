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

    if(index.column() == 0){
        // level 1
        if(!index.parent().isValid()){
            return QString::fromStdString(mNetsName[index.row()]);
        }

        // level 2
        Net net;
        if(mHandler->getNet(mNetsName[index.parent().row()], net)){
            return QString::fromStdString(net.getEntries()[index.row()].getDesignator());
        }
    }
    else if(index.column() == 1 && index.parent().isValid()){
        // level 2
        Net net;
        if(mHandler->getNet(mNetsName[index.parent().row()], net)){
            return net.getEntries()[index.row()].getPin();
        }
    }

    return QVariant();
}

Qt::ItemFlags NetlistModelWrapper::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled;
}

QVariant NetlistModelWrapper::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal){
            switch(section){
                case 0:
                    return QString("Net");
                case 1:
                    return QString("Component Pin");
                default:
                    break;
            }
        }
    }

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
    /*return !parent.isValid() ? mNetsName.size() : //level 1
        !parent.parent().isValid() ? 1 : 0; // level2 else 0*/

    if(!parent.isValid()){
        return mNetsName.size();
    }
    else{
        if(!parent.parent().isValid()){
            Net net;
            if(mHandler->getNet(mNetsName[parent.row()], net)){
                return net.getEntries().size();
            }
            else{
                return 0;
            }
        }
        else{
            return 0;
        }
    }
}

int NetlistModelWrapper::columnCount(const QModelIndex &parent) const
{
    return 2;
}
