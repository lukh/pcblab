#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "q_debugstream.h"

#include "pcblab/pcblab.h"
#include "cairogerberrenderer.h"

#include "displayviewprocessor.h"

#include "componentsmodelwrapper.h"
#include "netlistmodelwrapper.h"

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

        void on_actionActiveComponentSelection_toggled(bool inEn);

        void on_actionShowComponentsList_triggered();

        void on_actionShowNetlist_triggered();

        void on_actionShowLayersList_triggered();


    private:
        Ui::MainWindow *ui;

        PcbLab mPcb;

        ComponentModelWrapper mCompoModel;
        NetlistModelWrapper mNetlistModel;


        bool mComponentSelEnable;


//<<< PCB View
        void init();

    private Q_SLOTS:
        void updateZoom(bool inZoomIn, plPoint inPoint);
        void updateMove(double inDx, double inDy);
        void click(plPoint inPoint);
        void updateCursor(plPoint inPoint);

        void updateColor(string inIdentifier, Color inColor);
        void updateTransparency(string inIdentifier, uint8_t inTransp);


        void updateCurrentComponent(string inDes);

private:
        void updateLayersList();
        void clearLayersList();

    protected:
        virtual void resizeEvent(QResizeEvent *event);

    private:
        DisplayViewProcessor *mProcessor;
//>>>
};

#endif // MAINWINDOW_H
