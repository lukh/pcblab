#include "cairowidget.h"

CairoWidget::CairoWidget(QWidget *parent) : QWidget(parent), mSurface(NULL)
{

}

CairoWidget::~CairoWidget()
{
}

void CairoWidget::showImage(cairo_surface_t *inSurface)
{
    mSurface = inSurface;
    int w = cairo_image_surface_get_width(inSurface);
    int h = cairo_image_surface_get_height(inSurface);
    resize(w,h);

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


    QImage img(data,w,h,stride,QImage::Format_ARGB32);

    if(w > width() || h > height()){
        img = img.scaled(QSize(width(), height()), Qt::KeepAspectRatio);
    }

    int x = (width()-img.width())/2;
    int y = (height() - img.height())/2;
    painter.drawImage(QPoint(x,y), img);
}
