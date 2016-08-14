#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //mQdbcerr = new Q_DebugStream(std::cerr, ui->textEdit); //Redirect Console output to QTextEdit
    //mQdbcout = new Q_DebugStream(std::cout, ui->textEdit); //Redirect Console output to QTextEdit


    mGerberLayer = new GerberLayer("Layer1");
    mGerberLayer->open("../../data/BassDrum.grb");

    mGerberViewer = new CairoGerberViewer();


    mProcessor = new ViewProcessor(ui->openCVViewer, mGerberViewer);


}

MainWindow::~MainWindow()
{
    //delete mQdbcout;
    //delete mQdbcerr;

    delete mProcessor;

    delete ui;
    delete mGerberViewer;

    delete mGerberLayer;
}

void MainWindow::on_actionStart_triggered()
{
    mProcessor->update(*mGerberLayer);
}
