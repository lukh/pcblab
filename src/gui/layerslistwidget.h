#ifndef LAYERSLISTWIDGET_H
#define LAYERSLISTWIDGET_H

#include <QWidget>
#include <QList>
#include <QVBoxLayout>

#include "cairogerberrenderer.h"
#include "iviewprocessor.h"
#include "qlayerconfigwidget.h"

#include "pcblab/pcblab.h"

namespace Ui {
class LayersListWidget;
}

class LayersListWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit LayersListWidget(QWidget *parent = 0);
        ~LayersListWidget();

        void updateLayersList(PcbLab &inPcb, IViewProcessor *inProcessor);
        void clearLayersList();

    Q_SIGNALS:
        void colorUpdated(string inIdentifier, Color inColor);
        void transparencyUpdated(string inIdentifier, uint8_t inTransp);

    private:
        Ui::LayersListWidget *ui;

        QList<QLayerConfigWidget *> mLayersList;
};

#endif // LAYERSLISTWIDGET_H
