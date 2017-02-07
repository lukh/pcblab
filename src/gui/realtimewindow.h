#ifndef REALTIMEWINDOW_H
#define REALTIMEWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QThread>

#include "projectorviewprocessor.h"

#include "pcblab/pcblab.h"

using namespace cv;

Q_DECLARE_METATYPE(Mat)

class ProjProcessorWrapper: public QObject{
    Q_OBJECT

    public:
        ProjProcessorWrapper(ProjectorViewProcessor *inPVP);
        virtual ~ProjProcessorWrapper();

    public Q_SLOTS:
        void refresh();
        void process();

    Q_SIGNALS:
        void imageReady(Mat);

    private:
        ProjectorViewProcessor *mProcessor;
};


namespace Ui {
    class RealTimeWindow;
}

class RealTimeWindow : public QWidget
{
    Q_OBJECT
    QThread workerThread;

    public:
        explicit RealTimeWindow(QWidget *parent = 0);
        ~RealTimeWindow();

        void init(PcbLab *inPcb);

    public Q_SLOTS:
        void refreshWidget(Mat img);

    private:
        Ui::RealTimeWindow *ui;
        ProjProcessorWrapper *mWrapper;
        QTimer mTimer;

        PcbLab *mPcb;

        ProjectorViewProcessor *mProcessor;
};

#endif // REALTIMEWINDOW_H
