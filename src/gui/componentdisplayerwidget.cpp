#include "componentdisplayerwidget.h"
#include "ui_componentdisplayerwidget.h"

ComponentDisplayerWidget::ComponentDisplayerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComponentDisplayerWidget)
{
    ui->setupUi(this);

    ui->desisgnatorDisplayer->setName("Designator");
    ui->positionDisplayer->setName("Position");
    ui->rotationDisplayer->setName("Rotation");
}

ComponentDisplayerWidget::~ComponentDisplayerWidget()
{
    delete ui;
}

void ComponentDisplayerWidget::setDesignatorList(vector<string> inList){
    mDesList = inList;
    mDesIterator = mDesList.begin();
}

void ComponentDisplayerWidget::displayComponent(Component &inComponent)
{
    ui->desisgnatorDisplayer->update(QString::fromStdString(inComponent.getDesignator()));
    ui->positionDisplayer->update(inComponent.getPosition());
    ui->rotationDisplayer->update(inComponent.getRotation());

    ui->paramDisplayer->clear();
    ui->paramDisplayer->add(inComponent.getParameters());
}

void ComponentDisplayerWidget::on_previousCompoButton_clicked()
{
    if(mDesIterator != mDesList.begin()){
        mDesIterator--;
    }

    Q_EMIT(componentUpdated(*mDesIterator));
}

void ComponentDisplayerWidget::on_nextCompoButton_clicked()
{
    if(mDesIterator != mDesList.end()){
        mDesIterator++;
    }

    Q_EMIT(componentUpdated(*mDesIterator));
}
