#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

GerberHandler::ExtensionOrderList sExtList;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->mousePosDisplayer->setName("Mouse position:");

    //mQdbcerr = new Q_DebugStream(std::cerr, ui->textEdit); //Redirect Console output to QTextEdit
    //mQdbcout = new Q_DebugStream(std::cout, ui->textEdit); //Redirect Console output to QTextEdit

    sExtList.push_back("GBL");
    sExtList.push_back("GTL");


    mProcessor = new DisplayViewProcessor(mPcb, ui->cairoWidget);

    QObject::connect(ui->cairoWidget, SIGNAL(moved(double, double)), this, SLOT(updateMove(double, double)));
    QObject::connect(ui->cairoWidget, SIGNAL(zoomed(bool, plPoint)), this, SLOT(updateZoom(bool, plPoint)));
    QObject::connect(ui->cairoWidget, SIGNAL(cursor(plPoint)), this, SLOT(updateCursor(plPoint)));
}

MainWindow::~MainWindow()
{
    //delete mQdbcout;
    //delete mQdbcerr;

    delete mProcessor;

    delete ui;
}

void MainWindow::on_actionOpenFolder_triggered()
{
    //DO NOT COMMIT
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "D:/documents/projets/dev/pcblab/data", 0);

    string std_dir = dir.toStdString();

    mPcb.openFolder(std_dir, sExtList);

    mProcessor->init(800, 600);
    mProcessor->refresh();
}

void MainWindow::updateZoom(bool inZoomIn, plPoint inPoint)
{
    mProcessor->updateZoom(inZoomIn, inPoint);
}

void MainWindow::updateMove(double inDx, double inDy)
{
    mProcessor->move(inDx, inDy);
}

void MainWindow::updateCursor(plPoint inPoint)
{
    if(mProcessor != NULL){
        ui->mousePosDisplayer->update(mProcessor->convertCoordsFromImageToReal(inPoint));
    }
}
