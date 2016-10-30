#ifndef QPCBLABDISPLAYERWIDGET_H
#define QPCBLABDISPLAYERWIDGET_H

#include <QWidget>

namespace Ui {
class QPcbLabDisplayerWidget;
}

class QPcbLabDisplayerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QPcbLabDisplayerWidget(QWidget *parent = 0);
    ~QPcbLabDisplayerWidget();

private:
    Ui::QPcbLabDisplayerWidget *ui;
};

#endif // QPCBLABDISPLAYERWIDGET_H
