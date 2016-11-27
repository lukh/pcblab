#include "layerslistwidget.h"
#include "ui_layerslistwidget.h"

LayersListWidget::LayersListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LayersListWidget)
{
    ui->setupUi(this);
}

LayersListWidget::~LayersListWidget()
{
    clearLayersList();
    delete ui;
}

void LayersListWidget::updateLayersList(PcbLab &inPcb, IViewProcessor *inProcessor)
{
    clearLayersList();

    // gerber layers
    const GerberHandler &gerber = inPcb.getGerber();
    const CairoGerberRenderer &renderer = inProcessor->getGerberRenderer();

    int32_t layers_count = gerber.getLayersCount();

    for(int32_t idx = layers_count-1; idx >= 0; idx--){
        bool status = false;
        string identifier = gerber.getLayerIdentifier(idx);
        IGerberRenderer::GraphicSettings gs = renderer.getGraphicSettings(identifier, status);

        QLayerConfigWidget *conf = new QLayerConfigWidget(identifier);
        mLayersList.append(conf);

        ui->saWidget->layout()->addWidget(conf);

        conf->updateTransparency(gs.mTransparency);
        conf->updateColor(gs.mColor);

        QObject::connect(conf, SIGNAL(transparencyUpdated(string, uint8_t)), this, SIGNAL(transparencyUpdated(string, uint8_t)));
        QObject::connect(conf, SIGNAL(colorUpdated(string, Color)), this, SIGNAL(colorUpdated(string, Color)));
    }


    //nc drill
    const CairoExcellonRenderer &exc_renderer = inProcessor->getExcellonRenderer();
    QLayerConfigWidget *conf = new QLayerConfigWidget("NC Drill");
    mLayersList.append(conf);

    ui->saWidget->layout()->addWidget(conf);

    conf->updateTransparency(exc_renderer.getTransparency());
    conf->updateColor(exc_renderer.getColor());

    QObject::connect(conf, SIGNAL(transparencyUpdated(string, uint8_t)), this, SIGNAL(transparencyUpdated(string, uint8_t)));
    QObject::connect(conf, SIGNAL(colorUpdated(string, Color)), this, SIGNAL(colorUpdated(string, Color)));

    //Components
    const CairoComponentRenderer &compo_renderer = inProcessor->getComponentRenderer();
    conf = new QLayerConfigWidget("Components");
    mLayersList.append(conf);

    ui->saWidget->layout()->addWidget(conf);

    conf->updateTransparency(compo_renderer.getTransparency());
    conf->updateColor(compo_renderer.getColor());

    QObject::connect(conf, SIGNAL(transparencyUpdated(string, uint8_t)), this, SIGNAL(transparencyUpdated(string, uint8_t)));
    QObject::connect(conf, SIGNAL(colorUpdated(string, Color)), this, SIGNAL(colorUpdated(string, Color)));

    // spacer

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->saWidget->layout()->addItem(verticalSpacer);
}

void LayersListWidget::clearLayersList()
{
    for(int i = 0; i < mLayersList.size(); ++i){
        delete mLayersList.at(i);
    }

    delete ui->saWidget->layout();
    QVBoxLayout *layout = new QVBoxLayout(ui->saWidget);


    mLayersList.clear();
}
