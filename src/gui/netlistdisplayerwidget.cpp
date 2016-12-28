#include "netlistdisplayerwidget.h"
#include "ui_netlistdisplayerwidget.h"

NetlistDisplayerWidget::NetlistDisplayerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetlistDisplayerWidget)
{
    ui->setupUi(this);

    QObject::connect(ui->netlistView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(on_itemSelectedInTree(const QModelIndex&)));
}

NetlistDisplayerWidget::~NetlistDisplayerWidget()
{
    delete ui;
}

void NetlistDisplayerWidget::setModel(NetlistModelWrapper *inModel)
{
    ui->netlistView->setModel(inModel);
}

void NetlistDisplayerWidget::on_itemSelectedInTree(const QModelIndex &inIndex)
{
    //if it is a component...

    // it is a net
    if(!inIndex.parent().isValid()){
        QVariant var = inIndex.data();
        Q_EMIT(netUpdated(var.toString().toStdString()));
    }

    else if(!inIndex.parent().parent().isValid()){
        QVariant var = inIndex.data();
        Q_EMIT(componentUpdated(var.toString().toStdString()));
    }
}

