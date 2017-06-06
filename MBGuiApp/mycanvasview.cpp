#include "mycanvasview.h"
#include "qtcanvas.h"
#include <QtGui>

/*******************************************************
 * Class: MyCanvasView
 ******************************************************/
void MyCanvasView::contentsMousePressEvent(QMouseEvent* event)
{
    handleMouseClickEvent(event);
}

void MyCanvasView::contentsMouseDoubleClickEvent(QMouseEvent* event)
{
    handleMouseClickEvent(event);
}

void MyCanvasView::handleMouseClickEvent(QMouseEvent* event)
{
    QPoint p = inverseWorldMatrix().map(event->pos());
    QtCanvasItemList l = canvas()->collisions(p);
    moving = 0;
    if (!l.isEmpty())
        moving = l.first();
    moving_start = p;
    emit itemClicked(moving);
}

void MyCanvasView::contentsMouseMoveEvent(QMouseEvent* event)
{
    if (moving) {
    QPoint p = inverseWorldMatrix().map(event->pos());
    moving->moveBy(p.x() - moving_start.x(), p.y() - moving_start.y());
    moving_start = p;
    canvas()->update();
        emit itemMoved(moving);
    }
}



