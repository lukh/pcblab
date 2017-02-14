#include "colorextractionsettingsdialog.h"
#include "ui_colorextractionsettingsdialog.h"

ColorExtractionSettingsDialog::ColorExtractionSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorExtractionSettingsDialog)
{
    ui->setupUi(this);
}

ColorExtractionSettingsDialog::~ColorExtractionSettingsDialog()
{
    delete ui;
}
