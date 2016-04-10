#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "q_debugstream.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Q_DebugStream *mQdbcout;
    Q_DebugStream *mQdbcerr;
};

#endif // MAINWINDOW_H
