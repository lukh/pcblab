#ifndef QOPENCVWIDGET_H
#define QOPENCVWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <iostream>

using namespace std;

#include <opencv2/core/core.hpp>

#include "iopencvwidget.h"

#include "pcblab/pcblab.h"



class QOpenCVWidget : public QWidget, public IOpenCVWidget
{
    Q_OBJECT
    public:
        explicit QOpenCVWidget(QWidget *parent = 0);
        virtual ~QOpenCVWidget() {

        }


    public:
        bool showImage(cv::Mat &inImage); /// Used to set the image to be viewed

        virtual uint32_t getWidth() const;
        virtual uint32_t getHeight() const;


        virtual void paintEvent(QPaintEvent *);
        virtual void mousePressEvent(QMouseEvent *event);

    Q_SIGNALS:
        void clicked(plPoint inPoint);

    private:
        cv::Mat mOrigImage;             /// original OpenCV image to be shown

        QImage mRenderQtImg;
        QPoint mRenderImgPos;

        QColor mBgColor;		/// Background color


        void recalculatePosition();
};

#endif // CQTOPENCVVIEWERGL_H
