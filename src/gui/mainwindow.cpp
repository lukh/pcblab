#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

GerberHandler::ExtensionOrderList sExtList;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //mQdbcerr = new Q_DebugStream(std::cerr, ui->textEdit); //Redirect Console output to QTextEdit
    //mQdbcout = new Q_DebugStream(std::cout, ui->textEdit); //Redirect Console output to QTextEdit

    sExtList.push_back("GBL");
    sExtList.push_back("GTL");


    mGerberViewer = new CairoGerberViewer();

    mProcessor = new ViewProcessor(mPcb, ui->openCVViewer, ui->cairoWidget, mGerberViewer);

    QObject::connect(ui->cairoWidget, SIGNAL(moved(double, double)), this, SLOT(updateMove(double, double)));
    QObject::connect(ui->cairoWidget, SIGNAL(zoomed(bool, plPoint)), this, SLOT(updateZoom(bool, plPoint)));
}

MainWindow::~MainWindow()
{
    //delete mQdbcout;
    //delete mQdbcerr;

    delete mProcessor;

    delete ui;
    delete mGerberViewer;
}

void MainWindow::on_actionOpen_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "~", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    string std_dir = dir.toStdString();

    mPcb.openFolder(std_dir, sExtList);

    mGerberViewer->setRenderTransformation(plPoint(0, 0), plPoint(20,20));
    mProcessor->update();

}

void MainWindow::updateZoom(bool inZoomIn, plPoint inPoint)
{
    mProcessor->updateZoom(inZoomIn, inPoint);
}

void MainWindow::updateMove(double inDx, double inDy)
{
    mProcessor->move(inDx, inDy);
}
