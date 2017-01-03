#include "componentsmodelwrapper.h"





ComponentModelWrapper::ComponentModelWrapper(QObject *parent):
    QAbstractItemModel(parent),
    mHandler(NULL)
{
    //default param
    mParameters.push_back("PartNumber");
    mParameters.push_back("Value");
    mParameters.push_back("Footprint");
    mParameters.push_back("Description");
}



QVariant ComponentModelWrapper::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();



    // level 1
    if(!index.parent().isValid()){
        Component *c;

        string des = mDesList[index.row()];
        if(!mHandler->getComponent(des, &c)){
            return QVariant();
        }

        switch (index.column()) {
            case 0:
                return QString::fromStdString(des);

            case 1:
                return QString ("(%1, %2)")
                        .arg(c->getPosition().mX)
                        .arg(c->getPosition().mY);

            case 2:
                return QString::number(c->getRotation());

            default:
                if(mParameters.size() >= index.column()-3){
                    string param = mParameters[index.column()-3];
                    string value;
                    if(c->getParameter(param, value)){
                        return QString::fromStdString(value);
                    }
                }
                break;
        }
    }

    else if(index.parent().isValid()){
        if(index.column() == 0){
            return QString("MyNet");
        }
    }


    return QVariant();
}

Qt::ItemFlags ComponentModelWrapper::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled;
}

QVariant ComponentModelWrapper::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal){
            switch (section){
                case 0:
                    return QString("Designator");
                case 1:
                    return QString("Position");
                case 2:
                    return QString("Rotation");
                default:
                    if(mParameters.size() >= section-3){
                        return QString::fromStdString(mParameters[section-3]);
                    }
                    else{
                        return QVariant();
                    }
            }
        }
    }
    return QVariant();
}

QModelIndex ComponentModelWrapper::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row,column, parent.isValid() ? parent.row() : -1);
}

QModelIndex ComponentModelWrapper::parent(const QModelIndex &index) const
{
    return index.internalId() >=0 ? createIndex(index.internalId(),0) : QModelIndex();
}

int ComponentModelWrapper::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid()){
        return mDesList.size();
    }
    else{
        if(!parent.parent().isValid()){
            return 1;
        }
        else{
            return 0;
        }
    }
}

int ComponentModelWrapper::columnCount(const QModelIndex &parent) const
{
    return mParameters.size()+3;
}

void ComponentModelWrapper::setHandler(ComponentHandler *inHandler)
{
    if(inHandler == NULL) { return; }

    mHandler = inHandler;
    //mHandler->getDesignatorsList(mDesList);

    // << TEMP OR DEFAULT
    vector <string> prefix;
    prefix.push_back("C");
    prefix.push_back("R");
    prefix.push_back("U");
    prefix.push_back("X");
    prefix.push_back("L");
    prefix.push_back("P");
    prefix.push_back("J");
    // >>

    mHandler->getSortedAndGroupedDesignatorsList(prefix, "Value", mDesList);
}
