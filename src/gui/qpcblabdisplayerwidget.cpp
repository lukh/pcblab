#include "qpcblabdisplayerwidget.h"
#include "ui_qpcblabdisplayerwidget.h"

QPcbLabDisplayerWidget::QPcbLabDisplayerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QPcbLabDisplayerWidget)
{
    ui->setupUi(this);
}

QPcbLabDisplayerWidget::~QPcbLabDisplayerWidget()
{
    delete ui;
}
