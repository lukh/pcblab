#include "qlayerconfigwidget.h"

QLayerConfigWidget::QLayerConfigWidget(string inIdentifier, QWidget *parent) : QWidget(parent), mIdentifier(inIdentifier)
{
    mCheckBox = new QCheckBox();
    mCheckBox->setCheckState(Qt::Checked);

    mColorButton= new QPushButton();
    mColorButton->setMinimumWidth(10);

    QString str = QString::fromStdString(inIdentifier);
    mLabel = new QLabel(str);

    mTransparencySlider = new QSlider(Qt::Horizontal);
    mTransparencySlider->setMinimum(0);
    mTransparencySlider->setMaximum(255);
    mTransparencySlider->setMinimumWidth(30);

    //layout
    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->addWidget(mCheckBox);
    layout->addWidget(mColorButton);
    layout->addWidget(mLabel);
    layout->addWidget(mTransparencySlider);


    // connections
    QObject::connect(mCheckBox, SIGNAL(stateChanged(int)), this, SLOT(enableUpdated(int)));
    QObject::connect(mColorButton, SIGNAL(clicked()), this, SLOT(colorLabelPressed()));
    QObject::connect(mTransparencySlider, SIGNAL(sliderReleased()), this, SLOT(transparencySliderUpdated()));
}

void QLayerConfigWidget::updateColor(Color inColor)
{
    QString style = QString::fromStdString("background-color: rgb("+to_string(inColor.mR)+", "+to_string(inColor.mG)+", "+to_string(inColor.mB)+")");
    mColorButton->setStyleSheet(style);
}

void QLayerConfigWidget::updateTransparency(uint8_t inTransp)
{
    mTransparencySlider->setValue(inTransp);
}

void QLayerConfigWidget::colorLabelPressed()
{
    QColor qc = QColorDialog::getColor();
    Color c(qc.red(), qc.green(), qc.blue());

    updateColor(c);

    Q_EMIT(colorUpdated(mIdentifier, c));
}

void QLayerConfigWidget::transparencySliderUpdated()
{
    uint8_t transp = mTransparencySlider->value();
    Q_EMIT(transparencyUpdated(mIdentifier, (uint8_t)transp));
    mCheckBox->setCheckState(Qt::Checked);
}

void QLayerConfigWidget::enableUpdated(int inState)
{
    uint8_t transp = mTransparencySlider->value();
    if(inState == Qt::Checked){
        Q_EMIT(transparencyUpdated(mIdentifier, (uint8_t)transp));
    }
    else{
        Q_EMIT(transparencyUpdated(mIdentifier, 0));
    }
}
