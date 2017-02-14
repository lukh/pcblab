#ifndef COLOREXTRACTIONSETTINGSDIALOG_H
#define COLOREXTRACTIONSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class ColorExtractionSettingsDialog;
}

class ColorExtractionSettingsDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit ColorExtractionSettingsDialog(QWidget *parent = 0);
        ~ColorExtractionSettingsDialog();

    private:
        Ui::ColorExtractionSettingsDialog *ui;
};

#endif // COLOREXTRACTIONSETTINGSDIALOG_H
