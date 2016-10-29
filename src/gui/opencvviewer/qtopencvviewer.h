#ifndef CQTOPENCVVIEWERGL_H
#define CQTOPENCVVIEWERGL_H

#include <QWidget>
#include <QPainter>

#include <iostream>

using namespace std;

#include <opencv2/core/core.hpp>

#include "iopencvviewer.h"



class QtOpenCVViewer : public QWidget, public IOpenCVViewer
{
    Q_OBJECT
    public:
        explicit QtOpenCVViewer(QWidget *parent = 0);
        virtual ~QtOpenCVViewer() {

        }


    public:
        bool showImage(cv::Mat &inImage); /// Used to set the image to be viewed

        virtual uint32_t getWidth() const;
        virtual uint32_t getHeight() const;


        virtual void paintEvent(QPaintEvent *);

    private:
        cv::Mat mOrigImage;             /// original OpenCV image to be shown
        QImage mRenderQtImg;

        QColor mBgColor;		/// Background color


        void recalculatePosition();
};

#endif // CQTOPENCVVIEWERGL_H
