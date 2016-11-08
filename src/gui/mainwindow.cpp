#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

GerberHandler::ExtensionOrderList sExtList;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sExtList = GerberHandler::createExtensionOrderMap_Altium();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpenFolder_triggered()
{
    //DO NOT COMMIT
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "D:/documents/projets/dev/pcblab/data", 0);

    string std_dir = dir.toStdString();

    mPcb.openFolder(std_dir, sExtList);

    ui->pcbDisplayer->init(mPcb);
}

