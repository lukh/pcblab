#ifndef NETLISTDISPLAYERWIDGET_H
#define NETLISTDISPLAYERWIDGET_H

#include <QWidget>

#include "netlistmodelwrapper.h"

namespace Ui {
class NetlistDisplayerWidget;
}

class NetlistDisplayerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NetlistDisplayerWidget(QWidget *parent = 0);
    ~NetlistDisplayerWidget();

    void setModel(NetlistModelWrapper* inModel);

private:
    Ui::NetlistDisplayerWidget *ui;
};

#endif // NETLISTDISPLAYERWIDGET_H
