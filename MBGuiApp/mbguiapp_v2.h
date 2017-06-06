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
#include <qgslayertreenode.h>
#include <mysetdatapathdialog.h>
#include <qgsproject.h>

#include "qtmbparams.h"
#include "qtcanvas.h"

//QT Includes
#include <QtGui>

//Local Includes
#include <ui_mainwindowbase.h>

/**
@author Tim Sutton
*/

class DockWidget : public QDockWidget
{
    Q_OBJECT

public:
    DockWidget(QWidget *parent = 0) : QDockWidget(parent), dock(0) {}

protected:
    void contentsMousePressEvent(QMouseEvent *event);

private:
    void handleMouseClickEvent(QMouseEvent *event);
    QDockWidget *dock;
    QAction *act;
};

class QtProperty;

class CanvasView : public QtCanvasView
{
    Q_OBJECT
public:
    CanvasView(QWidget *parent = 0)
        : QtCanvasView(parent), moving(0) { }
    CanvasView(QtCanvas *canvas, QWidget *parent = 0)
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

//********************************
// MainWindow
//********************************
class MainWindow : public QMainWindow, private Ui::MainWindowBase
{
    Q_OBJECT;
    public:
        explicit MainWindow(QWidget* parent = 0, Qt::WFlags fl = 0 );
        ~MainWindow();
    public slots:
        void zoomInMode();
        void zoomOutMode();
        void panMode();
        //void addLayer();
        void onWillAddChildren(QgsLayerTreeNode *node, int indexFrom, int indexTo);
        void onAddedChildren(QgsLayerTreeNode *node, int indexFrom, int indexTo);
        void onWillRemoveChildren(QgsLayerTreeNode *node, int indexFrom, int indexTo);
        void onRemovedChildren(QgsLayerTreeNode *node, int indexFrom, int indexTo);
        void onVisibilityChanged(QgsLayerTreeNode *node,Qt::CheckState state);
        void onCustomPropertyChanged(QgsLayerTreeNode *node, const QString &key);
        void onExpandedChanged(QgsLayerTreeNode *node, bool expanded);
        void onNameChanged(QgsLayerTreeNode *node, QString name);
        void onChange(QgsMapLayer *layer);
        void onLayerSavedAs(QgsMapLayer *l, const QString &path);

    private slots:
        void deleteItem();
        void addLayer();
        void addGroup();
        void setDataPath(bool isGlobal);
        void setLocalDataPath();
        void setGlobalDataPath();
        void open();
        void closeProject();
        bool save();
        void saveAs();
//        void quit();
        void about();
        void itemMenuAboutToShow();
        void itemMenuAboutToHide();
        void fileMenuAboutToShow();
        void fileMenuAboutToHide();



        void itemClicked(QtCanvasItem *item);
        void itemMoved(QtCanvasItem *item);
        void valueChanged(QtProperty *property, double value);
        void valueChanged(QtProperty *property, const QString &value);
        void valueChanged(QtProperty *property, const QColor &value);
        void valueChanged(QtProperty *property, const QPoint &value);
        void valueChanged(QtProperty *property, const QSize &value);





    private:
        void createActions();
        void createMenus();
        void createUndoView();
        void readSettings();
        void writeSettings();
        void createStatusBar();
        bool userReallyWantsToQuit();
        void closeEvent(QCloseEvent *event);

        QAction *deleteAction;
        QAction *addLayerAction;
        QAction *addGroupAction;
        QAction *setLocalDataPathAction;
        QAction *setGlobalDataPathAction;
        QAction *undoAction;
        QAction *redoAction;
        QAction *exitAction;
        QAction *aboutAction;

        QAction *closeProjectAction;
        QAction *openAction;
        QAction *saveAction;
        QAction *saveAsAction;

        QUndoStack *undoStack;
        QUndoView *undoView;

        QgsMapCanvas * canvas_xy;
        QgsMapCanvas * canvas_yz;
        QgsMapCanvas * canvas_error;
        QgsMapCanvas * canvas_rms;
        QgsMapCanvas * canvas_mean;
//        QVBoxLayout  * mpLayout;
        QVBoxLayout  * mpLayout_canvas_xy;
        QVBoxLayout  * mpLayout_canvas_yz;
        QVBoxLayout  * mpLayout_canvas_error;
        QVBoxLayout  * mpLayout_canvas_rms;
        QVBoxLayout  * mpLayout_canvas_mean;
        QHBoxLayout  * mpLayout_CanvasEdits;
        QHBoxLayout  * mpLayout_CanvasResults;
        QGroupBox * hGroupBox_CanvasEdits;
        QGroupBox * hGroupBox_CanvasResults;
//        QGroupBox * vGroupBox
        QGroupBox * vGroupBox_canvas_xy;
        QGroupBox * vGroupBox_canvas_yz;
        QGroupBox * vGroupBox_canvas_error;
        QGroupBox * vGroupBox_canvas_rms;
        QGroupBox * vGroupBox_canvas_mean;
        QCheckBox * checkBox_lock_canvas_xy;
        QCheckBox * checkBox_lock_canvas_yz;
        QCheckBox * checkBox_lock_canvas_error;
        QCheckBox * checkBox_lock_canvas_rms;
        QCheckBox * checkBox_lock_canvas_mean;

        QToolBar * mpMapToolBar;
        QgsMapTool * mpPanTool;
        QgsMapTool * mpZoomInTool;
        QgsMapTool * mpZoomOutTool;

        QgsMapTool * mpPanTool2;
        QgsMapTool * mpZoomInTool2;
        QgsMapTool * mpZoomOutTool2;

        QgsProviderRegistry *r;
        QgsLayerTreeView * view;
        MyMenuProvider * provider;
        QgsLayerTreeModel * model;

        // Do not deconstructor
        QMenu *fileMenu;
        QMenu *editMenu;
        QMenu *itemMenu;
        QMenu *helpMenu;
        QMenu *viewMenu;

        QList<QgsMapCanvasLayer> myLayerSet;
        QStringList myFileNameList;

        QgsLayerTreeGroup * root;

         MySetDataPathDialog * setDataPathDialog;

    //     QgsProject *project;
         QString globalDataPath;
         QString localDataPath;

        int openDataFile(QString myLayerPath, QString myLayerBaseName, QString myProviderName, QgsMapLayer &mypLayer, QgsFeatureIterator &features);
        int validateProvider(QString myProviderName);
        void readProjectSettings();

        void createMapTools();
        void createEditCanvasGroupBox();
        void createResultsCanvasGroupBox();
        void createMBParamsGroupBox();








        void addProperty(QtProperty *property, const QString &id);
        void updateExpandState();

        class QtDoublePropertyManager *doubleManager;
        class QtStringPropertyManager *stringManager;
        class QtColorPropertyManager *colorManager;
        class QtPointPropertyManager *pointManager;
        class QtSizePropertyManager *sizeManager;
        class QtBoolPropertyManager *boolManager;
        class QtIntPropertyManager *intManager;
        class QtRectPropertyManager *rectManager;
        class QtSizePolicyPropertyManager *sizePolicyManager;
        class QtEnumPropertyManager *enumManager;
        class QtGroupPropertyManager *groupManager;

        class QtTreePropertyBrowser *propertyEditor;

        QtMBParams *params;
        QtCanvasItem *currentItem;
        QMap<QtProperty *, QString> propertyToId;
        QMap<QString, QtProperty *> idToProperty;
        QMap<QString, bool> idToExpanded;

        QDockWidget *dock1;
        QDockWidget *dock2;
        bool dock1Vis;
        bool dock2Vis;





CanvasView *canvasView;











};

#endif
