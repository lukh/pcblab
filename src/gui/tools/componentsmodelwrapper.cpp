#include "componentsmodelwrapper.h"





ComponentModelWrapper::ComponentModelWrapper(QObject *parent):
    mHandler(NULL)
{
    //default param
    mParameters.push_back("PartNumber");
    mParameters.push_back("Value");
    mParameters.push_back("Footprint");
    mParameters.push_back("Description");
}

int ComponentModelWrapper::rowCount(const QModelIndex &parent) const
{
    return mDesList.size();
}

int ComponentModelWrapper::columnCount(const QModelIndex &parent) const
{
    return mParameters.size()+3;
}

QVariant ComponentModelWrapper::data(const QModelIndex &index, int role) const
{
    Component c;

    string des = mDesList[index.row()];
    bool status = mHandler->getComponent(des, c);

    if ((role == Qt::DisplayRole || role == Qt::EditRole) && status)
    {
        switch (index.column()) {
            case 0:
                return QString::fromStdString(des);

            case 1:
                return QString ("(%1, %2)")
                        .arg(c.getPosition().mX)
                        .arg(c.getPosition().mY);

            case 2:
                return QString::number(c.getRotation());

            default:
                if(mParameters.size() >= index.column()-3){
                    string param = mParameters[index.column()-3];
                    string value;
                    if(c.getParameter(param, value)){
                        return QString::fromStdString(value);
                    }
                }
                break;
        }

    }
    return QVariant();
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

void ComponentModelWrapper::setHandler(ComponentHandler *inHandler)
{
    if(inHandler == NULL) { return; }

    mHandler = inHandler;
    mHandler->getDesignatorsList(mDesList);

    vector <string> prefix;
    prefix.push_back("C");
    prefix.push_back("R");
    prefix.push_back("U");
    prefix.push_back("X");
    prefix.push_back("L");
    prefix.push_back("P");
    prefix.push_back("J");

    inHandler->getSortedAndGroupedDesignatorsList(prefix, "Value", mDesList);
}
