#include "realtimewindow.h"
#include "ui_realtimewindow.h"

ProjProcessorWrapper::ProjProcessorWrapper(ProjectorViewProcessor *inPVP):
    mProcessor(inPVP)
{
    qRegisterMetaType<Mat>("Mat");
    qRegisterMetaType<plPoint>("plPoint");
}

ProjProcessorWrapper::~ProjProcessorWrapper()
{
}

void ProjProcessorWrapper::refresh()
{
    mProcessor->refresh();

    // reprocess ?
    process();
}

void ProjProcessorWrapper::process()
{
    Mat img = mProcessor->process();

    Q_EMIT(imageReady(img));
}

void ProjProcessorWrapper::setBackgroundColor(plPoint inPoint)
{
    mProcessor->setBackgroundColor(inPoint);
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
    QObject::connect(ui->openCVWidget, SIGNAL(clicked(plPoint)), mWrapper, SLOT(setBackgroundColor(plPoint)));

    workerThread.start();

    QObject::connect(&mTimer, SIGNAL(timeout()), mWrapper, SLOT(process()));
    mTimer.start(100);
}



void RealTimeWindow::refreshWidget(Mat img)
{
    ui->openCVWidget->showImage(img);
}


