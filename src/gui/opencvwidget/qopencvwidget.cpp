#include "qopencvwidget.h"

#include <opencv2/opencv.hpp>









QOpenCVWidget::QOpenCVWidget(QWidget *parent)
{
    mBgColor = QColor::fromRgb(150, 150, 150);

}




bool QOpenCVWidget::showImage(cv::Mat &inImage)
{
    //convert the image from BGR to RGBA
    if (inImage.channels() == 3)
        cvtColor(inImage, mOrigImage, CV_BGR2BGRA);
    else if (inImage.channels() == 1)
        cvtColor(inImage, mOrigImage, CV_GRAY2RGBA);
    else return false;

    //render to qimage
    mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),
                          mOrigImage.cols, mOrigImage.rows,
                          mOrigImage.step1(), QImage::Format_RGB32);

    repaint();

    return true;
}

uint32_t QOpenCVWidget::getWidth() const
{
    //return mRenderWidth;
    return width();
}

uint32_t QOpenCVWidget::getHeight() const
{
    //return mRenderHeight;
    return height();
}



void QOpenCVWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // paint background
    painter.fillRect(rect(), mBgColor);

    //draw image

    //crop...
    int cropped_w=mRenderQtImg.width(), cropped_h=mRenderQtImg.height();
    int x=0, y=0;

    if(mRenderQtImg.width() > width()){
        cropped_w=width();
        x = (mRenderQtImg.width() - width())/2;
    }
    if(mRenderQtImg.height() > height()){
        cropped_h = height();
        y = (mRenderQtImg.height() - height())/2;
    }


    QImage croppedImg = mRenderQtImg.copy(x, y, cropped_w, cropped_h);


    //paint image
    x = (width()-croppedImg.width())/2;
    y = (height() - croppedImg.height())/2;
    painter.drawImage(x, y, croppedImg);

    mRenderImgPos = QPoint(x ,y);


    // messages
    if(mRenderQtImg.width() > width() || mRenderQtImg.height() > height()){
        painter.setPen(QColor("red"));
        painter.drawText(QPointF(width() - 55, height() - 5), "Cropped !");
    }
}

void QOpenCVWidget::mousePressEvent(QMouseEvent *event)
{
    int x = (event->pos().x() - mRenderImgPos.x());
    int y = (event->pos().y() - mRenderImgPos.y());

    if(x >= 0 && x <= mRenderQtImg.width() && y >= 0 && y <= mRenderQtImg.height()){
        Q_EMIT(clicked(plPoint(x,y)));
    }
}











