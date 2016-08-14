#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "q_debugstream.h"

#include "pcblab/gerber/gerberlayer.h"
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

private:
    Ui::MainWindow *ui;

    //Q_DebugStream *mQdbcout;
    //Q_DebugStream *mQdbcerr;

    GerberLayer *mGerberLayer;

    CairoGerberViewer *mGerberViewer;


    ViewProcessor *mProcessor;
};

#endif // MAINWINDOW_H
