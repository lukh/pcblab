#ifndef CAIROWIDGET_H
#define CAIROWIDGET_H

#include <QWidget>
#include <QImage>
#include <QVBoxLayout>
#include <QPainter>

#include <cairo/cairo.h>

class CairoWidget : public QWidget
{
    Q_OBJECT
    public:
        explicit CairoWidget(QWidget *parent = 0);
        ~CairoWidget();

        void showImage(cairo_surface_t *inSurface);

    public:
        void paintEvent(QPaintEvent *event);

    private:
        cairo_surface_t *mSurface;
};

#endif // CAIROWIDGET_H
