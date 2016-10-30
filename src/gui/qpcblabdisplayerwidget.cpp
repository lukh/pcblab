#include "qpcblabdisplayerwidget.h"
#include "ui_qpcblabdisplayerwidget.h"

QPcbLabDisplayerWidget::QPcbLabDisplayerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QPcbLabDisplayerWidget),
    mProcessor(NULL)
{
    ui->setupUi(this);

    ui->mousePosDisplayer->setName("Mouse position:");



    QObject::connect(ui->cairoWidget, SIGNAL(moved(double, double)), this, SLOT(updateMove(double, double)));
    QObject::connect(ui->cairoWidget, SIGNAL(zoomed(bool, plPoint)), this, SLOT(updateZoom(bool, plPoint)));
    QObject::connect(ui->cairoWidget, SIGNAL(cursor(plPoint)), this, SLOT(updateCursor(plPoint)));
}

QPcbLabDisplayerWidget::~QPcbLabDisplayerWidget()
{
    delete ui;

    if(mProcessor != NULL){
        delete mProcessor;
    }
}

void QPcbLabDisplayerWidget::init(PcbLab &inPcb)
{
    if(mProcessor == NULL){
        mProcessor = new DisplayViewProcessor(inPcb, ui->cairoWidget);
    }

    mProcessor->init(800, 600);
    mProcessor->refresh();
}


void QPcbLabDisplayerWidget::updateZoom(bool inZoomIn, plPoint inPoint)
{
    if(mProcessor != NULL){
        mProcessor->updateZoom(inZoomIn, inPoint);
    }
}

void QPcbLabDisplayerWidget::updateMove(double inDx, double inDy)
{
    if(mProcessor != NULL){
        mProcessor->move(inDx, inDy);
    }
}

void QPcbLabDisplayerWidget::updateCursor(plPoint inPoint)
{
    if(mProcessor != NULL){
        ui->mousePosDisplayer->update(mProcessor->convertCoordsFromImageToReal(inPoint));
    }
}

