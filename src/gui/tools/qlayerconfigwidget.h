#ifndef QLAYERCONFIGWIDGET_H
#define QLAYERCONFIGWIDGET_H

#include <QWidget>
#include <QLabel>

#include <QHBoxLayout>
#include <QSlider>
#include <QColorDialog>
#include <QPushButton>

#include <string>

#include "pcblab/common.h"

using namespace std;

class QLayerConfigWidget : public QWidget
{
    Q_OBJECT
    public:
        QLayerConfigWidget(string inIdentifier, QWidget *parent = 0);

        void updateColor(Color inColor);
        void updateTransparency(uint8_t inTransp);

    Q_SIGNALS:
        void colorUpdated(string inIdentifier, Color inColor);
        void transparencyUpdated(string inIdentifier, uint8_t inTransp);

    private Q_SLOTS:
        void colorLabelPressed();
        void transparencySliderUpdated();

    private:
        string mIdentifier;

        QPushButton *mColorButton;
        QLabel *mLabel;
        QSlider *mTransparencySlider;
};

#endif // QLAYERCONFIGWIDGET_H
