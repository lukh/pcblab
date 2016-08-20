#ifndef CQTOPENCVVIEWERGL_H
#define CQTOPENCVVIEWERGL_H

#include <iostream>

using namespace std;

#include <QOpenGLWidget>
#include <QOpenGLFunctions_2_0>
#include <opencv2/core/core.hpp>

#include "iopencvviewer.h"



class QtOpenCVViewer : public QOpenGLWidget, protected QOpenGLFunctions_2_0, public IOpenCVViewer
{
    Q_OBJECT
    public:
        explicit QtOpenCVViewer(QWidget *parent = 0);
        virtual ~QtOpenCVViewer() {

        }

    Q_SIGNALS:
        void imageSizeChanged( int outW, int outH ); /// Used to resize the image outside the widget

    public:
        virtual bool showImage(cv::Mat &inImage); /// Used to set the image to be viewed

        virtual uint32_t getWidth() const;
        virtual uint32_t getHeight() const;

    protected:
        void initializeGL(); /// OpenGL initialization
        void paintGL(); /// OpenGL Rendering
        void resizeGL(int width, int height);        /// Widget Resize Event


        void updateScene();
        void renderImage();

    private:
        QImage mRenderQtImg;           /// Qt image to be rendered
        //QImage mResizedImg;

        cv::Mat mOrigImage;             /// original OpenCV image to be shown


        QColor mBgColor;		/// Background color

        //float  mImgRatio;             /// height/width ratio

        int mRenderWidth;
        int mRenderHeight;
        int mRenderPosX;
        int mRenderPosY;

        void recalculatePosition();
};

#endif // CQTOPENCVVIEWERGL_H
