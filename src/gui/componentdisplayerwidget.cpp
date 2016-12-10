#include "componentdisplayerwidget.h"
#include "ui_componentdisplayerwidget.h"

ComponentDisplayerWidget::ComponentDisplayerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComponentDisplayerWidget)
{
    ui->setupUi(this);

    mDesListDialog = new DesListDialog(this);
    mDesListDialog->hide();
    QObject::connect(mDesListDialog, SIGNAL(viewClicked(const QModelIndex &)), this, SLOT(on_itemSelectedInList(const QModelIndex&)));

    //default params list
    mParameters.push_back("PartNumber");
    mParameters.push_back("Value");
    mParameters.push_back("Footprint");
    mParameters.push_back("Description");
}

ComponentDisplayerWidget::~ComponentDisplayerWidget()
{
    delete ui;

    for(vector<QDisplayer *>::iterator it = mDisplayerList.begin(); it != mDisplayerList.end(); ++it){
        delete *it;
    }
    mDisplayerList.clear();
}

void ComponentDisplayerWidget::setModel(ComponentModelWrapper *inModel)
{
    mDesListDialog->setModel(inModel);

    mMapper.setModel(inModel);

    updateDisplayers();

    mMapper.toFirst();

    notify();
}

void ComponentDisplayerWidget::setParameters(vector<string> inParameters)
{
    mParameters = inParameters;

    updateDisplayers();

    mMapper.setCurrentIndex(mMapper.currentIndex()); //for refreshing without loosing the current pos

    notify();
}

void ComponentDisplayerWidget::setComponentByDes(string inDes)
{
    QAbstractItemModel *m = mMapper.model();

    for(uint32_t idx = 0; idx < m->rowCount(); idx++){
        QModelIndex mi = m->index(idx, 0);
        QVariant v = m->data(mi);
        string des = v.toString().toStdString();

        if(des.compare(inDes) == 0){
            mMapper.setCurrentIndex(idx);
            notify();
            break;
        }
    }
}


void ComponentDisplayerWidget::on_previousCompoButton_clicked()
{
    mMapper.toPrevious();

    notify();
}

void ComponentDisplayerWidget::on_nextCompoButton_clicked()
{
    mMapper.toNext();

    notify();
}

void ComponentDisplayerWidget::on_showListButton_clicked()
{
    mDesListDialog->show();
}

void ComponentDisplayerWidget::on_itemSelectedInList(const QModelIndex &inIndex)
{
    mMapper.setCurrentIndex(inIndex.row());

    //get the des
    QVariant var = mMapper.model()->index(inIndex.row(), 0).data();

    Q_EMIT(componentUpdated(var.toString().toStdString()));
}

void ComponentDisplayerWidget::updateDisplayers()
{
    for(vector<QDisplayer *>::iterator it = mDisplayerList.begin(); it != mDisplayerList.end(); ++it){
        delete *it;
    }
    mDisplayerList.clear();


    QHBoxLayout *layout = new QHBoxLayout();
    QDisplayer *disp= new QDisplayer("Name");
    disp->setStyleSheet("font-size: 15px");
    layout->addWidget(disp);
    mDisplayerList.push_back(disp);
    mMapper.addMapping(disp, 0, "value");

    disp= new QDisplayer("Position", ui->paramWidget);
    disp->setStyleSheet("font-size: 15px");
    layout->addWidget(disp);
    mDisplayerList.push_back(disp);
    mMapper.addMapping(disp, 1, "value");

    disp= new QDisplayer("Rotation", ui->paramWidget);
    disp->setStyleSheet("font-size: 15px");
    layout->addWidget(disp);
    mDisplayerList.push_back(disp);
    mMapper.addMapping(disp, 2, "value");

    // adding
    uint32_t idx = 3;
    for(vector<string>::iterator p = mParameters.begin(); p != mParameters.end(); ++p){
        disp= new QDisplayer(QString::fromStdString(*p), ui->paramWidget);
        disp->setStyleSheet("font-size: 15px");
        layout->addWidget(disp);
        mDisplayerList.push_back(disp);
        mMapper.addMapping(disp, idx++, "value");
    }

    if(ui->paramWidget->layout() != NULL){
        delete ui->paramWidget->layout();
    }
    ui->paramWidget->setLayout(layout);
}

void ComponentDisplayerWidget::notify()
{
    QAbstractItemModel *model = mMapper.model();
    QModelIndex idx = model->index(mMapper.currentIndex(), 0);
    QVariant var = idx.data();

    mDesListDialog->setCurrentIndex(idx);
    Q_EMIT(componentUpdated(var.toString().toStdString()));
}
