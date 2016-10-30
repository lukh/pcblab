#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "q_debugstream.h"

#include "pcblab/pcblab.h"
#include "cairogerberrenderer.h"

#include "displayviewprocessor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private Q_SLOTS:
    void on_actionOpenFolder_triggered();


private:
    Ui::MainWindow *ui;


    PcbLab mPcb;
};

#endif // MAINWINDOW_H
