#include "netlistdisplayerwidget.h"
#include "ui_netlistdisplayerwidget.h"

NetlistDisplayerWidget::NetlistDisplayerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetlistDisplayerWidget)
{
    ui->setupUi(this);
}

NetlistDisplayerWidget::~NetlistDisplayerWidget()
{
    delete ui;
}

void NetlistDisplayerWidget::setModel(NetlistModelWrapper *inModel)
{
    ui->netlistView->setModel(inModel);
}
