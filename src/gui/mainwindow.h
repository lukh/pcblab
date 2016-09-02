#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "q_debugstream.h"

#include "pcblab/pcblab.h"
#include "cairogerberviewer.h"

#include "viewprocessor.h"

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
    void on_actionStart_triggered();


    void updateZoom(bool inZoomIn, plPoint inPoint);
    void updateMove(double inDx, double inDy);

private:
    Ui::MainWindow *ui;

    //Q_DebugStream *mQdbcout;
    //Q_DebugStream *mQdbcerr;

    PcbLab mPcb;

    CairoGerberViewer *mGerberViewer;


    ViewProcessor *mProcessor;
};

#endif // MAINWINDOW_H
