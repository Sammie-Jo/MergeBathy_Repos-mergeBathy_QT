/***************************************************************************
 *   Copyright (C) 2006 by Tim Sutton   *
 *   tim@linfiniti.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef OMGMAINWINDOW_H
#define OMGMAINWINDOW_H

//QGis Includes
#include <qgsmapcanvas.h>
#include <qgsmaptool.h>
#include <qgsproviderregistry.h>
#include <qgslayertreeview.h>
#include <qgsfeatureiterator.h>
//QT Includes
#include <QtGui>

//Local Includes
#include <ui_mainwindowbase.h>

/**
@author Tim Sutton
*/

//********************************
// MyMenuProvider
//********************************
class MyMenuProvider: public QObject, public QgsLayerTreeViewMenuProvider//, QMainWindow
{
    Q_OBJECT;
public:
    MyMenuProvider(QgsLayerTreeView *view);
    ~MyMenuProvider();

public slots:
    void showExtent();
    void rename();
    void undoLast();
    void redoLast();
    void addLayer();
    void addGroup();
    void removeLayer();
    void removeGroup();
    void saveLayerAs();
    void resetLayer();


private:
    QgsLayerTreeView * view;
    QMenu * m;
    QMenu * createContextMenu();
};

//********************************
// AddCommand
//********************************
class AddCommand : public QObject, public QUndoCommand
{
public:
    AddCommand(QgsMapCanvas *mpMapCanvas,QList<QgsMapCanvasLayer> myLayerSet,QgsLayerTreeView * view,QUndoCommand *parent = 0);
    //           DiagramItem::DiagramType addType, QGraphicsScene *graphicsScene,
      //         QUndoCommand *parent = 0);
    ~AddCommand();

    void undo() override;
    void redo() override;

private:
    QStringList myFileNameList;
    QList<QgsMapCanvasLayer> myLayerSet;
    QgsMapCanvas *mpMapCanvas;
    QgsLayerTreeView * view;
    int initialPosition;
    QgsLayerTreeNode * node;
};

//********************************
// DeleteCommand
//********************************
class DeleteCommand : public QUndoCommand
{
public:
    explicit DeleteCommand(QGraphicsScene *graphicsScene, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    QgsLayerTreeNode *myDiagramItem;
    QgsLayerTreeView * view;
    QStringList myFileNameList;
    QList<QgsMapCanvasLayer> myLayerSet;
};
QString createCommandString(QStringList myFileNameList, const int &initialPosition);
//********************************
// MainWindow
//********************************
class MainWindow : public QMainWindow, private Ui::MainWindowBase
{
  Q_OBJECT;
    public:
      MainWindow(QWidget* parent = 0, Qt::WFlags fl = 0 );
      ~MainWindow();
    public slots:
      void zoomInMode();
      void zoomOutMode();
      void panMode();
      void addLayer();
      void onWillAddChildren(QgsLayerTreeNode *node, int indexFrom, int indexTo);
      void onAddedChildren(QgsLayerTreeNode *node, int indexFrom, int indexTo);
      void onWillRemoveChildren(QgsLayerTreeNode *node, int indexFrom, int indexTo);
      void onRemovedChildren(QgsLayerTreeNode *node, int indexFrom, int indexTo);
      void onVisibilityChanged(QgsLayerTreeNode *node);
      void onCustomPropertyChanged(QgsLayerTreeNode *node, const QString &key);
      void onExpandedChanged(QgsLayerTreeNode *node, bool expanded);
      void onNameChanged(QgsLayerTreeNode *node, QString name);
      void onChange(QgsMapLayer *layer);
      void onLayerSavedAs(QgsMapLayer *l, const QString &path);


    private:
      void deleteItem();
      void itemMenuAboutToShow();
      void itemMenuAboutToHide();
      void createActions();
      void createMenus();
      void createUndoView();

      QAction *undoAction;
      QAction *redoAction;
      QAction *deleteAction;

     QUndoStack *undoStack;
     QUndoView *undoView;
     QMenu *editMenu;
QList<QgsMapCanvasLayer> myLayerSet;
QStringList myFileNameList;


     QgsMapCanvas * mpMapCanvas;
     QVBoxLayout  * mpLayout;
     QToolBar * mpMapToolBar;
     QgsMapTool * mpPanTool;
     QgsMapTool * mpZoomInTool;
     QgsMapTool * mpZoomOutTool;

     QgsProviderRegistry *r;
     QgsLayerTreeView * view;

     int openDataFile(QString myLayerPath, QString myLayerBaseName, QString myProviderName, QgsMapLayer &mypLayer, QgsFeatureIterator &features);
     int validateProvider(QString myProviderName);

};

#endif
