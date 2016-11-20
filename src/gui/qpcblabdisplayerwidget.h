#ifndef QPCBLABDISPLAYERWIDGET_H
#define QPCBLABDISPLAYERWIDGET_H

#include <QWidget>
#include <QList>

#include "qlayerconfigwidget.h"

#include "pcblab/pcblab.h"

#include "displayviewprocessor.h"

namespace Ui {
class QPcbLabDisplayerWidget;
}

/// Manages all the graphic settings, zoom and co, GUI side
class QPcbLabDisplayerWidget : public QWidget
{
    Q_OBJECT

    public:
        QPcbLabDisplayerWidget(QWidget *parent = 0);
        ~QPcbLabDisplayerWidget();

        void init(PcbLab &inPcb);

    private Q_SLOTS:
        void updateZoom(bool inZoomIn, plPoint inPoint);
        void updateMove(double inDx, double inDy);
        void updateCursor(plPoint inPoint);

        void updateColor(string inIdentifier, Color inColor);
        void updateTransparency(string inIdentifier, uint8_t inTransp);

    private:
        void updateLayersList(PcbLab &inPcb);
        void clearLayersList();

    protected:
        virtual void resizeEvent(QResizeEvent *event);

    private:
        Ui::QPcbLabDisplayerWidget *ui;

        DisplayViewProcessor *mProcessor;


        QList<QLayerConfigWidget *> mLayersList;
};

#endif // QPCBLABDISPLAYERWIDGET_H
