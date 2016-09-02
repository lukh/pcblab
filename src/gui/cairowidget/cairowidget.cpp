#include "cairowidget.h"

#include <iostream>

using namespace std;

CairoWidget::CairoWidget(QWidget *parent) : QWidget(parent), mSurface(NULL)
{
    setMouseTracking(true);
}

CairoWidget::~CairoWidget()
{
}

void CairoWidget::showImage(cairo_surface_t *inSurface)
{
    mSurface = inSurface;
    int w = cairo_image_surface_get_width(inSurface);
    int h = cairo_image_surface_get_height(inSurface);

    repaint();
}

void CairoWidget::paintEvent(QPaintEvent *event)
{
    if(mSurface == NULL) { return; }


    QPainter painter(this);

    //cairo_surface_flush (mSurface);

    int w = cairo_image_surface_get_width(mSurface);
    int h = cairo_image_surface_get_height(mSurface);
    int stride = cairo_image_surface_get_stride(mSurface);
    cairo_format_t format = cairo_image_surface_get_format(mSurface);

    unsigned char *data = cairo_image_surface_get_data(mSurface);

    mRatio = 1.0;
    mSurfaceW = (double)w;
    mSurfaceH = (double)h;

    QImage img(data,w,h,stride,QImage::Format_ARGB32);

    if(w > width() || h > height()){
        img = img.scaled(QSize(width(), height()), Qt::KeepAspectRatio);
        double rx = (double)w/(double)width();
        double ry = (double)h/(double)height();
        mRatio = rx > ry ? rx : ry;
    }

    int x = (width()-img.width())/2;
    int y = (height() - img.height())/2;
    painter.drawImage(QPoint(x,y), img);

    //update the rectangle area
    mRenderArea.setX(x);
    mRenderArea.setY(y);
    mRenderArea.setWidth(w);
    mRenderArea.setHeight(h);
}

void CairoWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() & Qt::LeftButton){
        mLastPosition.setX(event->x());
        mLastPosition.setY(event->y());
    }
}



void CairoWidget::mouseMoveEvent(QMouseEvent *event)
{
    double dx, dy;

    if(mLastPosition.x() == 0) { mLastPosition.setX(event->x()); }
    if(mLastPosition.y() == 0) { mLastPosition.setY(event->y()); }

    if(event->buttons() & Qt::LeftButton){
        dx = (event->x()-mLastPosition.x())*mRatio;
        dy = -(event->y()-mLastPosition.y())*mRatio;

        mLastPosition = event->pos();

        Q_EMIT(moved(dx, dy));
    }
}

void CairoWidget::wheelEvent(QWheelEvent *event)
{
    plPoint p = getCoordWidget2Img(plPoint(event->x(), event->y()));

    bool zoom_in = event->delta() > 0;

    Q_EMIT(zoomed(zoom_in, p));
}

plPoint CairoWidget::getCoordWidget2Img(plPoint inPoint)
{
    double x, y; //surface points

    x = ((double)inPoint.mX - (double)mRenderArea.x())*mRatio;
    y = ((double)inPoint.mY - (double)mRenderArea.y())*mRatio;

    x = x < 0.0 ? 0.0 : x;
    y = y < 0.0 ? 0.0 : y;

    x = x > mSurfaceW ? mSurfaceW : x;
    y = y > mSurfaceH ? mSurfaceH : y;

    return plPoint(x, y);
}
