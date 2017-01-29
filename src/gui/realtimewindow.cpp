#include "realtimewindow.h"
#include "ui_realtimewindow.h"

RealTimeWindow::RealTimeWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RealTimeWindow)
{
    ui->setupUi(this);
}

RealTimeWindow::~RealTimeWindow()
{
    delete ui;
}

