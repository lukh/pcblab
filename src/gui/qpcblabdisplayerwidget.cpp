#include "qpcblabdisplayerwidget.h"
#include "ui_qpcblabdisplayerwidget.h"

QPcbLabDisplayerWidget::QPcbLabDisplayerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QPcbLabDisplayerWidget),
    mProcessor(NULL)
{
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout(ui->layerConfigList);

    ui->mousePosDisplayer->setName("Mouse position:");

    QObject::connect(ui->cairoWidget, SIGNAL(moved(double, double)), this, SLOT(updateMove(double, double)));
    QObject::connect(ui->cairoWidget, SIGNAL(zoomed(bool, plPoint)), this, SLOT(updateZoom(bool, plPoint)));
    QObject::connect(ui->cairoWidget, SIGNAL(cursor(plPoint)), this, SLOT(updateCursor(plPoint)));

    QObject::connect(ui->componentDisplayer, SIGNAL(componentUpdated(string)), this, SLOT(updateCurrentComponent(string)));
}

QPcbLabDisplayerWidget::~QPcbLabDisplayerWidget()
{
    clearLayersList();

    delete ui;

    if(mProcessor != NULL){
        delete mProcessor;
    }
}

void QPcbLabDisplayerWidget::init(PcbLab &inPcb)
{
    if(mProcessor != NULL){
        delete mProcessor;
    }

    mProcessor = new DisplayViewProcessor(inPcb, ui->cairoWidget);
    mProcessor->init(ui->cairoWidget->width(), ui->cairoWidget->height());

    mProcessor->refresh();

    updateLayersList(inPcb);

    vector <string> des_list;
    //inPcb.getComponents().getDesignatorsList(des_list);
    vector <string> prefix;
    prefix.push_back("C");
    prefix.push_back("R");
    prefix.push_back("U");
    inPcb.getComponents().getSortedAndGroupedDesignatorsList(prefix, "Value", des_list);
    ui->componentDisplayer->setDesignatorList(des_list);
}


void QPcbLabDisplayerWidget::updateZoom(bool inZoomIn, plPoint inPoint)
{
    if(mProcessor != NULL){
        mProcessor->zoom(inZoomIn, inPoint);
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

void QPcbLabDisplayerWidget::updateColor(string inIdentifier, Color inColor)
{
    mProcessor->updateLayerColor(inIdentifier, inColor);
}

void QPcbLabDisplayerWidget::updateTransparency(string inIdentifier, uint8_t inTransp)
{
    mProcessor->updateLayerTransparency(inIdentifier, inTransp);
}

void QPcbLabDisplayerWidget::updateCurrentComponent(string inDes)
{
    Component compo;
    if(mProcessor->getComponent(inDes, compo)){
        ui->componentDisplayer->displayComponent(compo);
        mProcessor->displayComponent(inDes);
    }
}

void QPcbLabDisplayerWidget::updateLayersList(PcbLab &inPcb)
{
    clearLayersList();

    // gerber layers
    const GerberHandler &gerber = inPcb.getGerber();
    const CairoGerberRenderer &renderer = mProcessor->getGerberRenderer();

    int32_t layers_count = gerber.getLayersCount();

    for(int32_t idx = layers_count-1; idx >= 0; idx--){
        bool status = false;
        string identifier = gerber.getLayerIdentifier(idx);
        IGerberRenderer::GraphicSettings gs = renderer.getGraphicSettings(identifier, status);

        QLayerConfigWidget *conf = new QLayerConfigWidget(identifier);
        mLayersList.append(conf);

        ui->layerConfigList->layout()->addWidget(conf);

        conf->updateTransparency(gs.mTransparency);
        conf->updateColor(gs.mColor);

        QObject::connect(conf, SIGNAL(transparencyUpdated(string, uint8_t)), this, SLOT(updateTransparency(string, uint8_t)));
        QObject::connect(conf, SIGNAL(colorUpdated(string, Color)), this, SLOT(updateColor(string, Color)));
    }


    //nc drill
    const CairoExcellonRenderer &exc_renderer = mProcessor->getExcellonRenderer();
    QLayerConfigWidget *conf = new QLayerConfigWidget("NC Drill");
    mLayersList.append(conf);

    ui->layerConfigList->layout()->addWidget(conf);

    conf->updateTransparency(exc_renderer.getTransparency());
    conf->updateColor(exc_renderer.getColor());

    QObject::connect(conf, SIGNAL(transparencyUpdated(string, uint8_t)), this, SLOT(updateTransparency(string, uint8_t)));
    QObject::connect(conf, SIGNAL(colorUpdated(string, Color)), this, SLOT(updateColor(string, Color)));

    //Components
    const CairoComponentRenderer &compo_renderer = mProcessor->getComponentRenderer();
    conf = new QLayerConfigWidget("Components");
    mLayersList.append(conf);

    ui->layerConfigList->layout()->addWidget(conf);

    conf->updateTransparency(compo_renderer.getTransparency());
    conf->updateColor(compo_renderer.getColor());

    QObject::connect(conf, SIGNAL(transparencyUpdated(string, uint8_t)), this, SLOT(updateTransparency(string, uint8_t)));
    QObject::connect(conf, SIGNAL(colorUpdated(string, Color)), this, SLOT(updateColor(string, Color)));

    // spacer

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->layerConfigList->layout()->addItem(verticalSpacer);
}

void QPcbLabDisplayerWidget::clearLayersList()
{
    for(int i = 0; i < mLayersList.size(); ++i){
        delete mLayersList.at(i);
    }

    delete ui->layerConfigList->layout();
    QVBoxLayout *layout = new QVBoxLayout(ui->layerConfigList);


    mLayersList.clear();
}

void QPcbLabDisplayerWidget::resizeEvent(QResizeEvent * event)
{
    if(mProcessor != NULL){
        mProcessor->init(ui->cairoWidget->width(), ui->cairoWidget->height());
        mProcessor->refresh();
    }
}



