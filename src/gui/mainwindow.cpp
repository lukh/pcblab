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

    mPcb.openFolder("../../data", list);

    mGerberViewer = new CairoGerberViewer();


    mProcessor = new ViewProcessor(mPcb, ui->openCVViewer, mGerberViewer);
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
