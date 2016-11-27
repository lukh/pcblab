#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

GerberHandler::ExtensionOrderList sExtList;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mProcessor(NULL)
{
    ui->setupUi(this);


    QObject::connect(ui->cairoWidget, SIGNAL(moved(double, double)), this, SLOT(updateMove(double, double)));
    QObject::connect(ui->cairoWidget, SIGNAL(zoomed(bool, plPoint)), this, SLOT(updateZoom(bool, plPoint)));
    QObject::connect(ui->cairoWidget, SIGNAL(cursor(plPoint)), this, SLOT(updateCursor(plPoint)));

    QObject::connect(ui->componentDisplayer, SIGNAL(componentUpdated(string)), this, SLOT(updateCurrentComponent(string)));


    QObject::connect(ui->layersList, SIGNAL(colorUpdated(string, Color)), this, SLOT(updateColor(string,Color)));
    QObject::connect(ui->layersList, SIGNAL(transparencyUpdated(string, uint8_t)), this, SLOT(updateTransparency(string,uint8_t)));

    sExtList = GerberHandler::createExtensionOrderMap_Altium();
}

MainWindow::~MainWindow()
{
    delete ui;

    if(mProcessor != NULL){
        delete mProcessor;
    }
}

void MainWindow::on_actionOpenFolder_triggered()
{
    //DO NOT COMMIT
    QString dir("D:\\documents\\projets\\dev\\pcblab\\data\\Factory"); // = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "D:/documents/projets/dev/pcblab/data", 0);

    string std_dir = dir.toStdString();

    mPcb.openFolder(std_dir, sExtList);

    init();
}



void MainWindow::init()
{
    if(mProcessor != NULL){
        delete mProcessor;
    }

    mProcessor = new DisplayViewProcessor(mPcb, ui->cairoWidget);
    mProcessor->init(ui->cairoWidget->width(), ui->cairoWidget->height());

    mProcessor->refresh();

    updateLayersList();

    vector <string> des_list;
    //inPcb.getComponents().getDesignatorsList(des_list);
    vector <string> prefix;
    prefix.push_back("C");
    prefix.push_back("R");
    prefix.push_back("U");
    prefix.push_back("X");
    prefix.push_back("L");
    prefix.push_back("P");
    prefix.push_back("J");
    mPcb.getComponents().getSortedAndGroupedDesignatorsList(prefix, "Value", des_list);
    ui->componentDisplayer->setDesignatorList(des_list);
}



void MainWindow::updateZoom(bool inZoomIn, plPoint inPoint)
{
    if(mProcessor != NULL){
        mProcessor->zoom(inZoomIn, inPoint);
    }
}

void MainWindow::updateMove(double inDx, double inDy)
{
    if(mProcessor != NULL){
        mProcessor->move(inDx, inDy);
    }
}

void MainWindow::updateCursor(plPoint inPoint)
{
    if(mProcessor != NULL){
        plPoint pos = mProcessor->convertCoordsFromImageToReal(inPoint);
        QString str("Mouse: (" + QString::number(pos.mX) + ", " + QString::number(pos.mY) + ")");
        ui->statusbar->showMessage(str);
    }
}

void MainWindow::updateColor(string inIdentifier, Color inColor)
{
    mProcessor->updateLayerColor(inIdentifier, inColor);
}

void MainWindow::updateTransparency(string inIdentifier, uint8_t inTransp)
{
    mProcessor->updateLayerTransparency(inIdentifier, inTransp);
}

void MainWindow::updateCurrentComponent(string inDes)
{
    Component compo;
    if(mPcb.getComponents().getComponent(inDes, compo)){
        ui->componentDisplayer->displayComponent(compo);
        mProcessor->displayComponent(inDes);
    }
}

void MainWindow::updateLayersList()
{
    ui->layersList->updateLayersList(mPcb, mProcessor);
}



void MainWindow::clearLayersList()
{
    ui->layersList->clearLayersList();
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
    if(mProcessor != NULL){
        mProcessor->init(ui->cairoWidget->width(), ui->cairoWidget->height());
        mProcessor->refresh();
    }
}





