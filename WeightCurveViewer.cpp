#include "WeightCurveViewer.h"

#include <QQuickWindow>
#include <QDesktopWidget>

void WeightCurveViewer::setMaximized(bool maximized)
{
    if (maximized)
        window().showMaximized();
    else
        window().show();
}

bool WeightCurveViewer::isMaximized() const
{
    return window().windowState() & Qt::WindowMaximized;
}

void WeightCurveViewer::setPosition(const QPoint& position)
{
/*    if (position == QPoint(-1, -1))
    {
        if (isMaximized())
            return;

        QDesktopWidget* d = QApplication::desktop();
        int ws = d->width();    // returns screen width
        int h = d->height();    // returns screen height
        int mw = size().width();
        int mh = size().height();
        int cw = (ws / 2) - (mw / 2);
        int ch = (h / 2) - (mh / 2);

        window().setPosition(cw, ch);
    }
    else*/
    if (isMaximized() == false && position != QPoint(-1, -1))
        window().setPosition(position);
}

QPoint WeightCurveViewer::position() const
{
    return window().position();
}

void WeightCurveViewer::setSize(const QSize& size)
{
    if (isMaximized() == false && size != QSize(-1, -1))
        window().resize(size);
}

QSize WeightCurveViewer::size() const
{
    return window().size();
}
