#include "componentdisplayerwidget.h"
#include "ui_componentdisplayerwidget.h"

ComponentDisplayerWidget::ComponentDisplayerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComponentDisplayerWidget)
{
    ui->setupUi(this);

    setMinimumHeight(150);
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

    Component::Parameters & params = inComponent.getParameters();
    for(Component::Parameters::iterator it = params.begin(); it != params.end(); ++it){
        QDisplayer *disp= new QDisplayer(QString::fromStdString(it->first));
        disp->update(QString::fromStdString(it->second));
        layout->addWidget(disp);
        mDisplayerList.push_back(disp);
    }

    ui->paramWidget->setLayout(layout);
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
