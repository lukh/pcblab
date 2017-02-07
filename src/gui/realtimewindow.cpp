#include "realtimewindow.h"
#include "ui_realtimewindow.h"

ProjProcessorWrapper::ProjProcessorWrapper(ProjectorViewProcessor *inPVP):
    mProcessor(inPVP)
{
    qRegisterMetaType<Mat>("Mat");
}

ProjProcessorWrapper::~ProjProcessorWrapper()
{
}

void ProjProcessorWrapper::show()
{
    Mat img;
    mProcessor->show(img);

    Q_EMIT(imageReady(img));
}




RealTimeWindow::RealTimeWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RealTimeWindow),
    mWrapper(NULL),
    mPcb(NULL),
    mProcessor(NULL)
{
    ui->setupUi(this);
}

RealTimeWindow::~RealTimeWindow()
{
    mTimer.stop();
    workerThread.quit();
    workerThread.wait();

    delete ui;

    //if(mWrapper)
    //    delete mWrapper;//handled by deleteLater Slot
    if(mProcessor)
        delete mProcessor;
}

void RealTimeWindow::init(PcbLab *inPcb)
{
    mPcb = inPcb;

    mTimer.stop();
    workerThread.quit();
    workerThread.wait();


    if(mProcessor != NULL){
        delete mProcessor;
    }

    mProcessor = new ProjectorViewProcessor(mPcb);
    mProcessor->init(ui->openCVWidget->width(), ui->openCVWidget->height());

    mProcessor->refresh();


    mWrapper = new ProjProcessorWrapper(mProcessor);
    mWrapper->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, mWrapper, &QObject::deleteLater);

    QObject::connect(mWrapper, SIGNAL(imageReady(Mat)), this, SLOT(refreshWidget(Mat)));

    workerThread.start();

    QObject::connect(&mTimer, SIGNAL(timeout()), mWrapper, SLOT(show()));
    mTimer.start(100);
}



void RealTimeWindow::refreshWidget(Mat img)
{
    ui->openCVWidget->showImage(img);
}


