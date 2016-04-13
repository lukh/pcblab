#include "mainwindow.h"
#include "ui_mainwindow.h"


#include "pcblab/gerber/gerberlayer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mQdbcerr = new Q_DebugStream(std::cerr, ui->textEdit); //Redirect Console output to QTextEdit
    mQdbcout = new Q_DebugStream(std::cout, ui->textEdit); //Redirect Console output to QTextEdit


    GerberLayer gl("Layer1");
    gl.open("../../data/example.grb");
}

MainWindow::~MainWindow()
{
    delete mQdbcout;
    delete mQdbcerr;

    delete ui;
}
