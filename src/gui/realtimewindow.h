#ifndef REALTIMEWINDOW_H
#define REALTIMEWINDOW_H

#include <QWidget>

namespace Ui {
class RealTimeWindow;
}

class RealTimeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RealTimeWindow(QWidget *parent = 0);
    ~RealTimeWindow();

private:
    Ui::RealTimeWindow *ui;
};

#endif // REALTIMEWINDOW_H
