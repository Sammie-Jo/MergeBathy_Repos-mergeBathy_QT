#ifndef MYCANVASVIEW_H
#define MYCANVASVIEW_H

#include "qtcanvas.h"

//class QtProperty;

class MyCanvasView : public QtCanvasView
{
    Q_OBJECT
public:
    MyCanvasView(QWidget *parent = 0)
        : QtCanvasView(parent), moving(0) { }
    MyCanvasView(QtCanvas *canvas, QWidget *parent = 0)
        : QtCanvasView(canvas, parent), moving(0) { }
signals:
    void itemClicked(QtCanvasItem *item);
    void itemMoved(QtCanvasItem *item);
//    void toggleDock1();
//    void toggleDock2();
protected:
    void contentsMousePressEvent(QMouseEvent *event);
    void contentsMouseDoubleClickEvent(QMouseEvent *event);
    void contentsMouseMoveEvent(QMouseEvent* event);
private:
    void handleMouseClickEvent(QMouseEvent *event);
    QPoint moving_start;
    QtCanvasItem *moving;
};

#endif // MYCANVASVIEW_H
