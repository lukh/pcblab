#ifndef CAIROWIDGET_H
#define CAIROWIDGET_H

#include <QWidget>
#include <QImage>
#include <QVBoxLayout>
#include <QPainter>
#include <QRect>
#include <QMouseEvent>
#include <QWheelEvent>

#include <cairo/cairo.h>

#include "pcblab/common.h"


/// draws a cairo_surface_t to a widget, and provide signals for mouse events
class CairoWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit CairoWidget(QWidget *parent = 0);
        ~CairoWidget();

        void showImage(cairo_surface_t *inSurface);


    Q_SIGNALS:
        void zoomed(bool inZoomIn, plPoint p);
        void moved(double dx, double dy);
        void cursor(plPoint p);

    protected:
        void paintEvent(QPaintEvent *event);

        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent *event);


        plPoint getCoordWidget2Img(plPoint inPoint);

    private:
        cairo_surface_t *mSurface;

        QRect mRenderArea; //used to convert point in widget to real coords
        double mRatio; //used to convert point in widget to real coords

        double mSurfaceW, mSurfaceH; //used to convert point in widget to real coords


        QPoint mLastPosition;
};

#endif // CAIROWIDGET_H
