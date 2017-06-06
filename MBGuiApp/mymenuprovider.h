#ifndef MYMENUPROVIDER_H
#define MYMENUPROVIDER_H
#include <QtGui>
#include <qgslayertreeview.h>
#include <QUndoStack>

//********************************
// MyMenuProvider
//********************************
class MyMenuProvider: public QObject, public QgsLayerTreeViewMenuProvider
{
    Q_OBJECT;
public:
    MyMenuProvider(QgsLayerTreeView *view, QUndoStack *undoStack);
    ~MyMenuProvider();

public slots:
    void showExtent();
//    void rename();
    void renameLayer();
    void renameGroup();
    void rename2();
    void undoLast();
    void redoLast();
    void addLayer();
    void addGroup();
    void removeLayer();
    void removeGroup();
    void saveLayerAs();
    void resetLayer();


private:
    QAction *zoomtolayerAction;
    QgsLayerTreeView * view;
    QMenu * m;
    QMenu * createContextMenu();
    QUndoStack *undoStack;
};


#endif // MYMENUPROVIDER_H
