#include "componentdisplayerwidget.h"
#include "ui_componentdisplayerwidget.h"

ComponentDisplayerWidget::ComponentDisplayerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComponentDisplayerWidget)
{
    ui->setupUi(this);

    mDesListDialog = new DesListDialog(this);
    mDesListDialog->hide();
    QObject::connect(mDesListDialog, SIGNAL(itemSelected(string)), this, SLOT(on_itemSelectedInList(string)));

    //default params list
    mParametersList.push_back("PartNumber");
    mParametersList.push_back("Value");
    mParametersList.push_back("Footprint");
    mParametersList.push_back("Description");

}

ComponentDisplayerWidget::~ComponentDisplayerWidget()
{
    delete ui;
}

void ComponentDisplayerWidget::setParametersList(vector<string> inParams)
{
    mParametersList = inParams;
}

void ComponentDisplayerWidget::setDesignatorList(vector<string> inList){
    mDesList = inList;
    mDesIterator = mDesList.begin();

    mDesListDialog->update(inList);

    Q_EMIT(componentUpdated(*mDesIterator));
    mDesListDialog->setCurrent(*mDesIterator);
}

void ComponentDisplayerWidget::displayComponent(Component &inComponent)
{
    for(vector<QDisplayer *>::iterator it = mDisplayerList.begin(); it != mDisplayerList.end(); ++it){
        delete *it;
    }
    mDisplayerList.clear();

    if (ui->paramWidget->layout() != NULL){
        delete ui->paramWidget->layout();
    }
    QHBoxLayout *layout = new QHBoxLayout();

    QDisplayer *disp= new QDisplayer("Name");
    disp->update(QString::fromStdString(inComponent.getDesignator()));
    layout->addWidget(disp);
    mDisplayerList.push_back(disp);

    disp= new QDisplayer("Position", ui->paramWidget);
    layout->addWidget(disp);
    disp->update(inComponent.getPosition());
    mDisplayerList.push_back(disp);

    disp= new QDisplayer("Rotation", ui->paramWidget);
    layout->addWidget(disp);
    disp->update(inComponent.getRotation());
    mDisplayerList.push_back(disp);


    for(vector<string>::iterator p = mParametersList.begin(); p != mParametersList.end(); ++p){
        string param;
        if(inComponent.getParameter(*p, param)){
            QDisplayer *disp= new QDisplayer(QString::fromStdString(*p));
            disp->update(QString::fromStdString(param));
            layout->addWidget(disp);
            mDisplayerList.push_back(disp);
        }
    }

    ui->paramWidget->setLayout(layout);
}

void ComponentDisplayerWidget::on_previousCompoButton_clicked()
{
    if(mDesList.size() == 0){
        return;
    }

    if(mDesIterator != mDesList.begin()){
        mDesIterator--;
    }

    Q_EMIT(componentUpdated(*mDesIterator));
    mDesListDialog->setCurrent(*mDesIterator);
}

void ComponentDisplayerWidget::on_nextCompoButton_clicked()
{
    if(mDesList.size() == 0){
        return;
    }

    if(mDesIterator != mDesList.end()){
        mDesIterator++;
    }

    Q_EMIT(componentUpdated(*mDesIterator));
    mDesListDialog->setCurrent(*mDesIterator);
}

void ComponentDisplayerWidget::on_showListButton_clicked()
{
    mDesListDialog->show();
}

void ComponentDisplayerWidget::on_itemSelectedInList(string inDes)
{
    mDesIterator = std::find(mDesList.begin(), mDesList.end(), inDes);

    Q_EMIT(componentUpdated(inDes));
}
