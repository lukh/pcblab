#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //mQdbcerr = new Q_DebugStream(std::cerr, ui->textEdit); //Redirect Console output to QTextEdit
    //mQdbcout = new Q_DebugStream(std::cout, ui->textEdit); //Redirect Console output to QTextEdit

    GerberHandler::ExtensionOrderList list;
    list.push_back("GBL");
    list.push_back("GTL");

    mPcb.openFolder("../../data", list);

    mGerberViewer = new CairoGerberViewer();
    mGerberViewer->setRenderTransformation(plPoint(8.0,8.0), plPoint(22.0,22.0));

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

void MainWindow::on_actionStart_triggered()
{
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
