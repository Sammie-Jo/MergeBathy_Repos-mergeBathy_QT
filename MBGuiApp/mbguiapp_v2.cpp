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
#include "qtpropertymanager.h"
#include "qteditorfactory.h"
#include "qttreepropertybrowser.h"
#include "qtbuttonpropertybrowser.h"
#include "qtgroupboxpropertybrowser.h"
#include "qtmbparams.h"

#include "mainwindow.h"
#include <commands.h>
#include <mysetdatapathdialog.h>



// QGIS Includes
#include <qgsapplication.h>
#include <qgsproviderregistry.h>
#include <qgssinglesymbolrendererv2.h>
#include <qgsmaplayerregistry.h>
#include <qgsvectorlayer.h>
#include <qgsmapcanvas.h>
#include <qgsproject.h>
#include <qgslayertree.h>
#include <qgslayertreemodel.h>
#include <qgslayertreemapcanvasbridge.h>
#include <qgsrasterlayer.h>
#include <qgsvectordataprovider.h>
#include <qgslayertreeregistrybridge.h>

#include <QDockWidget>

// QGIS Map tools
#include "qgsmaptoolpan.h"
#include "qgsmaptoolzoom.h"



// These are the other headers for available map tools (not used in this example)
//#include "qgsmaptoolcapture.h"
//#include "qgsmaptoolidentify.h"
//#include "qgsmaptoolselect.h"
//#include "qgsmaptoolvertexedit.h"
//#include "qgsmeasure.h"

#include <qgisinterface.h> //delete when done trying
#include <qgslayertreeviewdefaultactions.h>



void CanvasView::contentsMousePressEvent(QMouseEvent* event)
{
    handleMouseClickEvent(event);
}

void CanvasView::contentsMouseDoubleClickEvent(QMouseEvent* event)
{
    handleMouseClickEvent(event);
}

void CanvasView::handleMouseClickEvent(QMouseEvent* event)
{
    QPoint p = inverseWorldMatrix().map(event->pos());
    QtCanvasItemList l = canvas()->collisions(p);
    moving = 0;
    if (!l.isEmpty())
        moving = l.first();
    moving_start = p;
    emit itemClicked(moving);
}

void CanvasView::contentsMouseMoveEvent(QMouseEvent* event)
{
    if (moving) {
    QPoint p = inverseWorldMatrix().map(event->pos());
    moving->moveBy(p.x() - moving_start.x(), p.y() - moving_start.y());
    moving_start = p;
    canvas()->update();
        emit itemMoved(moving);
    }
}


MyMenuProvider::MyMenuProvider(QgsLayerTreeView *view,QUndoStack *undoStack)
{
    this->view =  view;
    this->undoStack = undoStack;
    //m = createContextMenu();
}
MyMenuProvider::~MyMenuProvider()
{
    //delete view;
    //if(m!=nullptr)
    //    delete m;
    qDebug() << "~MyMenuProvider()" << endl;
}
QMenu * MyMenuProvider::createContextMenu() {
    if(!view->currentLayer())
        return nullptr;
    m = new QMenu();
    m->addAction("Show Extent",this,SLOT(showExtent()));
    //m->addAction("Rename",this,SLOT(rename()),tr("Ctrl+Alt+r"));
    m->addAction("Rename2",this,SLOT(rename2()),tr("Ctrl+Alt+r"));
    m->addAction("RenameLayer",this,SLOT(renameLayer()),tr("Ctrl+Alt+r"));
    m->addAction("RenameGroup",this,SLOT(renameGroup()),tr("Ctrl+Alt+r"));
    m->addAction("Undo",this,SLOT(undo()),QKeySequence::Undo)->setStatusTip(tr("Undo last action only"));
    m->addAction("Redo",this,SLOT(redo()),QKeySequence::Redo)->setToolTip(tr("Undo last action only"));
    m->addAction("Add Layer",this,SLOT(addLayer()),QKeySequence::New);
    m->addAction("Add Group",this,SLOT(addGroup()),QKeySequence::New);
    m->addAction("Remove Layer",this,SLOT(removeLayer()),QKeySequence::New);
    m->addAction("Remove Group",this,SLOT(removeGroup()),QKeySequence::New);
    m->addAction("Save as New Layer",this,SLOT(saveLayerAs()),QKeySequence::New);
    m->addAction("Reset Layer",this,SLOT(resetLayer()),QKeySequence::New)->setToolTip(tr("Discards All Changes"));
    m->addAction("Zoom to Layer0",this,SLOT(actionZoomToLayer()));


    // Exit Action
    zoomtolayerAction = new QAction(tr("Zoom to Layer"), this);
    connect(zoomtolayerAction, SIGNAL(triggered()), this, SLOT(QgsLayerTreeViewDefaultActions::actionZoomToLayer(mpMapCanvas)));
    m->addAction(zoomtolayerAction);

//    QgsLayerTreeViewDefaultActions

    return m;
}
void MyMenuProvider::showExtent(){
    QgsRectangle r = view->currentLayer()->extent();
    QMessageBox::information(nullptr, "Extent", r.toString());
}
//void MyMenuProvider::rename() {
//    bool ok;
//    QString str = tr("Rename %1 as:").arg(view->currentLayer()->name());
//    QString text = QInputDialog::getText(nullptr, tr("Rename Layer"), str, QLineEdit::Normal, "", &ok);
//    if (ok && !text.isEmpty())
//        view->currentLayer()->setLayerName(text);
//}
void MyMenuProvider::renameLayer() {
 //   QUndoCommand *RenameCommand = new RenameCommand(QgsLayerTreeNode::NodeType::NodeLayer, view,);
//        undoStack->push(RenameCommand);
}
void MyMenuProvider::renameGroup() {
 //   QUndoCommand *RenameCommand = new RenameCommand(QgsLayerTreeNode::NodeType::NodeGroup, view);
//        undoStack->push(RenameCommand);
}
void MyMenuProvider::rename2() {
    QUndoCommand *renameCommand = new RenameCommand(view->currentNode()->nodeType(), view);
        undoStack->push(renameCommand);
}

void MyMenuProvider::undoLast() {

}

void MyMenuProvider::redoLast() {

}

void MyMenuProvider::addLayer() {

}

void MyMenuProvider::addGroup() {

}

void MyMenuProvider::removeLayer() {

}

void MyMenuProvider::removeGroup() {

}

void MyMenuProvider::saveLayerAs() {

}

void MyMenuProvider::resetLayer() {

}










//*********************************************************************************

MainWindow::MainWindow(QWidget* parent, Qt::WFlags fl)
    : QMainWindow(parent,fl)
{
    //required by Qt4 to initialise the ui
    setupUi(this);
    undoStack = new QUndoStack(this);

    createActions();
    createMenus();
    createUndoView();

    setWindowTitle(tr("MergeBathy GUI: Untitled Project"));

    readSettings();

    // Instantiate Provider Registry
    #if defined(Q_WS_MAC)
        // Be sure to set this to an appropriate place if you are on OSX
        //QString myPluginsDir        = "/Users/timsutton/apps/qgis.app/Contents/MacOS/lib/qgis";
        QString myPluginsDir = "C:/OSGeo4W/apps/qgis-ltr/plugins";
    #else
        // Be sure to set this to an appropriate place if you are on Linux or windows
        //QString myPluginsDir        = "/home/timlinux/apps/qgis_trunk/lib/qgis";
        QString myPluginsDir = "C:/OSGeo4W/apps/qgis-ltr/plugins";
    #endif
    r = QgsProviderRegistry::instance(myPluginsDir);

    //******************************************************
    // II. Layer Legend Widget
    //******************************************************
    // A. Start Layer Tree for Legend Widget
    // Access project's layer tree
    QgsLayerTreeGroup * root = QgsProject::instance()->layerTreeRoot();
    QgsLayerTreeGroup * root2 = QgsProject::instance()->layerTreeRoot();

    //Model-View Paradigm
    model = new QgsLayerTreeModel(root);

    // Modify model
    model->setFlag(QgsLayerTreeModel::AllowNodeReorder);
    model->setFlag(QgsLayerTreeModel::AllowNodeChangeVisibility);
    model->setFlag(QgsLayerTreeModel::AllowNodeRename);
    model->setFlag(QgsLayerTreeModel::AllowLegendChangeState);
    model->setFlag(QgsLayerTreeModel::ShowLegend);
    view = new QgsLayerTreeView();
    view->setModel(model);
    //view->defaultActions();
//    view->show();

    // Connect to signals emitted from Layer Tree
    connect(root, SIGNAL(willAddChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onWillAddChildren(QgsLayerTreeNode*,int,int)));
    connect(root, SIGNAL(addedChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onWillAddChildren(QgsLayerTreeNode*,int,int)));
    connect(root, SIGNAL(willRemoveChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onWillRemoveChildren(QgsLayerTreeNode*,int,int)));
    connect(root, SIGNAL(removedChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onRemovedChildren(QgsLayerTreeNode*,int,int)));
    connect(root, SIGNAL(visibilityChanged(QgsLayerTreeNode*,Qt::CheckState)), this, SLOT(onVisibilityChanged(QgsLayerTreeNode*,Qt::CheckState)));
    connect(root, SIGNAL(customPropertyChanged(QgsLayerTreeNode*,QString)), this, SLOT(onCustomPropertyChanged(QgsLayerTreeNode*,QString)));
    connect(root, SIGNAL(expandedChanged(QgsLayerTreeNode*,bool)), this, SLOT(onExpandedChanged(QgsLayerTreeNode*,bool)));
    connect(view, SIGNAL(currentLayerChanged(QgsMapLayer*)), this, SLOT(onChange(QgsMapLayer*)));
    connect(view, SIGNAL(activated(QModelIndex)), this, SLOT(onChange(QgsMapLayer*)));
    connect(view, SIGNAL(), this, SLOT(onLayerSavedAs(QgsMapLayer*,QString)));

    // Create provider to context menu
    provider = new MyMenuProvider(view,undoStack);
    view->setMenuProvider(provider);

    // Create the Map Canvas
    canvas_xy = new QgsMapCanvas(0,"Canvas XY");
    QgsLayerTreeMapCanvasBridge * bridge_canvas_xy = new QgsLayerTreeMapCanvasBridge(root, canvas_xy);
    canvas_xy->enableAntiAliasing(true);
    canvas_xy->setCanvasColor(QColor(255, 255, 255));
    canvas_xy->freeze(false);
    canvas_xy->setVisible(true);
    canvas_xy->zoomToFullExtent();
//    canvas_xy->refresh();
    canvas_xy->show();

    canvas_yz = new QgsMapCanvas(0,"Canvas YZ");
    QgsLayerTreeMapCanvasBridge * bridge_canvas_yz = new QgsLayerTreeMapCanvasBridge(root, canvas_yz);
    canvas_yz->zoomToFullExtent();
    canvas_yz->show();

    canvas_error = new QgsMapCanvas();
    QgsLayerTreeMapCanvasBridge * bridge_canvas_error = new QgsLayerTreeMapCanvasBridge(root2, canvas_error);
    canvas_error->zoomToFullExtent();
    canvas_error->show();

    canvas_rms = new QgsMapCanvas();
    QgsLayerTreeMapCanvasBridge * bridge_canvas_rms = new QgsLayerTreeMapCanvasBridge(root2, canvas_rms);
    canvas_rms->zoomToFullExtent();
    canvas_rms->show();

    canvas_mean = new QgsMapCanvas();
    QgsLayerTreeMapCanvasBridge * bridge_canvas_mean = new QgsLayerTreeMapCanvasBridge(root2, canvas_mean);
    canvas_mean->zoomToFullExtent();
    canvas_mean->show();

    QDockWidget * legendDock = new QDockWidget("Layers",0);
    legendDock->setObjectName("Layers");
    legendDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    legendDock->setWidget(view);
//    legendDock->setContentsMargins(9,9,9,9);
    addDockWidget(Qt::LeftDockWidgetArea, legendDock);

    createResultsCanvasGroupBox();
    createEditCanvasGroupBox();
    createMBParamsGroupBox();
    createMapTools();

    QCheckBox * checkBox_lock_canvas_xy = new QCheckBox("Lock", this);
    QCheckBox * checkBox_lock_canvas_yz = new QCheckBox("Lock", this);
    QCheckBox * checkBox_lock_canvas_error = new QCheckBox("Lock", this);
    QCheckBox * checkBox_lock_canvas_rms = new QCheckBox("Lock", this);
    QCheckBox * checkBox_lock_canvas_mean = new QCheckBox("Lock", this);
    checkBox_lock_canvas_xy->setChecked(true);
    checkBox_lock_canvas_yz->setChecked(true);
    checkBox_lock_canvas_error->setChecked(true);
    checkBox_lock_canvas_rms->setChecked(true);
    checkBox_lock_canvas_mean->setChecked(true);

    QLabel * label_title_canvas_xy = new QLabel("XY-Plot",this);
    QLabel * label_title_canvas_yz = new QLabel("ZY-Plot",this);
    QLabel * label_title_canvas_error = new QLabel("Error",this);
    QLabel * label_title_canvas_rms = new QLabel("RMS",this);
    QLabel * label_title_canvas_mean = new QLabel("Mean",this);

    // Lay our widgets out in the main window
//    vGroupBox = new QGroupBox(tr("Vertical layout"),this);
    hGroupBox_CanvasEdits = new QGroupBox(tr("Edit Layers"),this);
    hGroupBox_CanvasResults = new QGroupBox(tr("Results"),this);

//    mpLayout = new QVBoxLayout(frameMap);
//    mpLayout->addWidget(checkBox_lock1);
//    vGroupBox->setLayout(mpLayout);

    vGroupBox_canvas_xy = new QGroupBox(tr("Canvas XY Vertical layout"),this);
    vGroupBox_canvas_yz = new QGroupBox(tr("Canvas YZ Vertical layout"),this);
    vGroupBox_canvas_error = new QGroupBox(tr("Canvas Error Vertical layout"),this);
    vGroupBox_canvas_rms = new QGroupBox(tr("Canvas RMS Vertical layout"),this);
    vGroupBox_canvas_mean = new QGroupBox(tr("Canvas Mean Vertical layout"),this);



    mpLayout_canvas_xy = new QVBoxLayout(frameMap);
    mpLayout_canvas_xy->addWidget(label_title_canvas_xy);
    mpLayout_canvas_xy->addWidget(canvas_xy);
    mpLayout_canvas_xy->addWidget(checkBox_lock_canvas_xy);
    vGroupBox_canvas_xy->setLayout(mpLayout_canvas_xy);

    mpLayout_canvas_yz = new QVBoxLayout(frameMap);
    mpLayout_canvas_yz->addWidget(label_title_canvas_yz);
    mpLayout_canvas_yz->addWidget(canvas_yz);
    mpLayout_canvas_yz->addWidget(checkBox_lock_canvas_yz);

    mpLayout_CanvasEdits = new QHBoxLayout(frameMap);
    mpLayout_CanvasEdits->addWidget(vGroupBox_canvas_xy);
    mpLayout_CanvasEdits->addWidget(vGroupBox_canvas_yz);
    mpLayout_CanvasEdits->addWidget(dock1);
    mpLayout_CanvasEdits->addWidget(dock2);
    hGroupBox_CanvasEdits->setLayout(mpLayout_CanvasEdits);



    mpLayout_canvas_error = new QVBoxLayout(frameMap);
    mpLayout_canvas_error->addWidget(label_title_canvas_error);
    mpLayout_canvas_error->addWidget(canvas_error);
    mpLayout_canvas_error->addWidget(checkBox_lock_canvas_error);

    mpLayout_canvas_rms = new QVBoxLayout(frameMap);
    mpLayout_canvas_rms->addWidget(label_title_canvas_rms);
    mpLayout_canvas_rms->addWidget(canvas_rms);
    mpLayout_canvas_rms->addWidget(checkBox_lock_canvas_rms);

    mpLayout_canvas_mean = new QVBoxLayout(frameMap);
    mpLayout_canvas_mean->addWidget(label_title_canvas_mean);
    mpLayout_canvas_mean->addWidget(canvas_mean);
    mpLayout_canvas_mean->addWidget(checkBox_lock_canvas_mean);

    mpLayout_CanvasResults = new QHBoxLayout(frameMap);
    mpLayout_CanvasResults->addLayout(mpLayout_canvas_error);
    mpLayout_CanvasResults->addLayout(mpLayout_canvas_rms);
    mpLayout_CanvasResults->addLayout(mpLayout_canvas_mean);
    hGroupBox_CanvasResults->setLayout(mpLayout_CanvasResults);




//    mpLayoutCanvas_Edits = new QHBoxLayout(frameMap);
//    mpLayoutCanvas_Edits->addWidget(canvas_xy);
//    mpLayoutCanvas_Edits->addWidget(canvas_yz);
//    mpLayoutCanvas_Edits->addWidget(canvas_yz);
//    hGroupBox_CanvasEdits->setLayout(mpLayoutCanvas_Edits);


//    mpLayoutCanvas_Results = new QHBoxLayout(frameMap);
//    mpLayoutCanvas_Results->addWidget(canvas3);
//    mpLayoutCanvas_Results->addWidget(canvas4);
//    mpLayoutCanvas_Results->addWidget(canvas5);
//    hGroupBox_CanvasResults->setLayout(mpLayoutCanvas_Results);


//    mpLayout->addLayout(mpLayout_CanvasEdits);
//    mpLayout->addLayout(mpLayout_CanvasResults);

    QVBoxLayout * mainLayout = new QVBoxLayout(frameMap);
//    mainLayout->addWidget(vGroupBox);
    mainLayout->addWidget(hGroupBox_CanvasEdits);
    mainLayout->addWidget(hGroupBox_CanvasResults);
    setLayout(mainLayout);

}
void MainWindow::createMapTools()
{
    //create the action behaviours
    connect(mpActionPan, SIGNAL(triggered()), this, SLOT(panMode()));
    connect(mpActionZoomIn, SIGNAL(triggered()), this, SLOT(zoomInMode()));
    connect(mpActionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOutMode()));
    connect(mpActionAddLayer, SIGNAL(triggered()), this, SLOT(addLayer()));

    //create a little toolbar
    mpMapToolBar = addToolBar(tr("File"));
    mpMapToolBar->addAction(mpActionAddLayer);
    mpMapToolBar->addAction(mpActionZoomIn);
    mpMapToolBar->addAction(mpActionZoomOut);
    mpMapToolBar->addAction(mpActionPan);

    //create the maptools
    mpPanTool = new QgsMapToolPan(canvas_xy);
    mpPanTool->setAction(mpActionPan);
    mpZoomInTool = new QgsMapToolZoom(canvas_xy, FALSE); // false = in
    mpZoomInTool->setAction(mpActionZoomIn);
    mpZoomOutTool = new QgsMapToolZoom(canvas_xy, TRUE ); //true = out
    mpZoomOutTool->setAction(mpActionZoomOut);

    mpPanTool2 = new QgsMapToolPan(canvas_yz);
    mpPanTool2->setAction(mpActionPan);
    mpZoomInTool2 = new QgsMapToolZoom(canvas_yz, FALSE); // false = in
    mpZoomInTool2->setAction(mpActionZoomIn);
    mpZoomOutTool2 = new QgsMapToolZoom(canvas_yz, TRUE ); //true = out
    mpZoomOutTool2->setAction(mpActionZoomOut);
}
void MainWindow::createResultsCanvasGroupBox()
{

}

void MainWindow::createEditCanvasGroupBox()
{

}

void MainWindow::createMBParamsGroupBox()
{

    //EXAMPLE 1
    doubleManager = new QtDoublePropertyManager(this);
    stringManager = new QtStringPropertyManager(this);
    colorManager = new QtColorPropertyManager(this);
    pointManager = new QtPointPropertyManager(this);
    sizeManager = new QtSizePropertyManager(this);
    //EXAMPLE 2
    boolManager = new QtBoolPropertyManager(this);
    intManager = new QtIntPropertyManager(this);
    rectManager = new QtRectPropertyManager(this);
    sizePolicyManager = new QtSizePolicyPropertyManager(this);
    enumManager = new QtEnumPropertyManager(this);
    groupManager = new QtGroupPropertyManager(this);

    //List: Dock
      //EXAMPLE 1
      //CONNECT ACTIONS TO PROPERTIES
      connect(doubleManager, SIGNAL(valueChanged(QtProperty *, double)),
                  this, SLOT(valueChanged(QtProperty *, double)));
      connect(stringManager, SIGNAL(valueChanged(QtProperty *, const QString &)),
                  this, SLOT(valueChanged(QtProperty *, const QString &)));
      connect(colorManager, SIGNAL(valueChanged(QtProperty *, const QColor &)),
                  this, SLOT(valueChanged(QtProperty *, const QColor &)));
      connect(pointManager, SIGNAL(valueChanged(QtProperty *, const QPoint &)),
                  this, SLOT(valueChanged(QtProperty *, const QPoint &)));
      connect(sizeManager, SIGNAL(valueChanged(QtProperty *, const QSize &)),
                  this, SLOT(valueChanged(QtProperty *, const QSize &)));

      //FACTORIES: property types
      QtDoubleSpinBoxFactory *doubleSpinBoxFactory = new QtDoubleSpinBoxFactory(this);
      QtCheckBoxFactory *checkBoxFactory = new QtCheckBoxFactory(this);
      QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory(this);
      QtLineEditFactory *lineEditFactory = new QtLineEditFactory(this);
      QtEnumEditorFactory *comboBoxFactory = new QtEnumEditorFactory(this);
      QtSliderFactory *sliderFactory = new QtSliderFactory(this);
      QtScrollBarFactory *scrollBarFactory = new QtScrollBarFactory(this);

      //DOCKED WIDGET: Property box container
      QDockWidget *dock1 = new QDockWidget(tr("Run Params (list)"),this);
      QDockWidget *dock2 = new QDockWidget(tr("Run Params"),this);
      addDockWidget(Qt::LeftDockWidgetArea, dock1);
      addDockWidget(Qt::LeftDockWidgetArea, dock2);
      tabifyDockWidget(dock1, dock2);
      setDockNestingEnabled(false);
      viewMenu->addAction(dock1->toggleViewAction());
      viewMenu->addAction(dock2->toggleViewAction());
  //    dock1->raise();
      dock2->setHidden(true);
      dock1Vis = dock1->isVisible();
      dock2Vis = dock2->isVisible();


      //EXAMPLE 1: Instantiate property fields
       //List display: Dock1
       propertyEditor = new QtTreePropertyBrowser(dock1);
       propertyEditor->setFactoryForManager(doubleManager, doubleSpinBoxFactory);
       propertyEditor->setFactoryForManager(stringManager, lineEditFactory);
       propertyEditor->setFactoryForManager(colorManager->subIntPropertyManager(), spinBoxFactory);
       propertyEditor->setFactoryForManager(pointManager->subIntPropertyManager(), spinBoxFactory);
       propertyEditor->setFactoryForManager(sizeManager->subIntPropertyManager(), spinBoxFactory);
       propertyEditor->setFactoryForManager(enumManager, comboBoxFactory);
       propertyEditor->setFactoryForManager(boolManager, checkBoxFactory);
       dock1->setWidget(propertyEditor);

       //List display: Dock1
       updateExpandState();

       QMap<QtProperty *, QString>::ConstIterator itProp = propertyToId.constBegin();
       while (itProp != propertyToId.constEnd()) {
           delete itProp.key();
           itProp++;
       }
       propertyToId.clear();
       idToProperty.clear();

       params = new QtMBParams();

       //UI display: Dock2
       QtProperty *propertyRunParams0 = groupManager->addProperty("Run Params");

       QtProperty *propertyRunParams1 = enumManager->addProperty(tr("Configuration"));
       enumManager->setEnumNames(propertyRunParams1, params->enumBits);
       enumManager->setValue(propertyRunParams1, params->getBitStr());
       addProperty(propertyRunParams1, QLatin1String("bitstr"));

       QtProperty *propertyRunParams2 = stringManager->addProperty(tr("Output Filename"));
       stringManager->setValue(propertyRunParams2, params->getOutputFilename());
       addProperty(propertyRunParams2, QLatin1String("outputfilename"));

       QtProperty *propertyRunParams3 = stringManager->addProperty(tr("Input Filelist"));
       stringManager->setValue(propertyRunParams3, params->getInputFilelist());
       addProperty(propertyRunParams3, QLatin1String("inputfilelist"));

       QtProperty *propertyRunParams4 = sizeManager->addProperty(tr("Grid Spacing (X,Y)"));
       sizeManager->setValue(propertyRunParams4, params->getGridSpacing());
       addProperty(propertyRunParams4, QLatin1String("gridspacing"));

       QtProperty *propertyRunParams5 = enumManager->addProperty(tr("Smoothing Filter"));
       enumManager->setEnumNames(propertyRunParams5, params->enumFilters);
       enumManager->setValue(propertyRunParams5, params->getSmoothingFilter());
       addProperty(propertyRunParams5, QLatin1String("filter"));

       QtProperty *propertyRunParams6 = sizeManager->addProperty(tr("Smoothing Scale (X,Y)"));
       sizeManager->setValue(propertyRunParams6, params->getSmoothingScales());
       sizeManager->setMinimum(propertyRunParams6, params->getGridSpacing());
       addProperty(propertyRunParams6, QLatin1String("smoothingscale"));

       QtProperty *propertyRunParams7 = pointManager->addProperty(tr("Reference Lon,Lat (X,Y)"));
       pointManager->setValue(propertyRunParams7, params->getRefLL());
       addProperty(propertyRunParams7, QLatin1String("reflonlat"));

       QtProperty *propertyRunParams8 = doubleManager->addProperty(tr("Rotation Angle (deg)"));
       doubleManager->setValue(propertyRunParams8, params->getRotAngle());
       addProperty(propertyRunParams8, QLatin1String("rotang"));

       propertyRunParams0->addSubProperty(propertyRunParams1);
       propertyRunParams0->addSubProperty(propertyRunParams2);
       propertyRunParams0->addSubProperty(propertyRunParams3);
       propertyRunParams0->addSubProperty(propertyRunParams4);
       propertyRunParams0->addSubProperty(propertyRunParams5);
       propertyRunParams0->addSubProperty(propertyRunParams7);
       propertyRunParams0->addSubProperty(propertyRunParams8);

       //Dock2: ButtonPropertyBrowser
       QtAbstractPropertyBrowser *editor2 = new QtGroupBoxPropertyBrowser(dock2);//QtButtonPropertyBrowser(dock2);
       editor2->setFactoryForManager(boolManager, checkBoxFactory);
       editor2->setFactoryForManager(intManager, scrollBarFactory);
       editor2->setFactoryForManager(enumManager, comboBoxFactory);
       editor2->setFactoryForManager(doubleManager, doubleSpinBoxFactory);
       editor2->setFactoryForManager(stringManager, lineEditFactory);
       editor2->setFactoryForManager(colorManager->subIntPropertyManager(), spinBoxFactory);
       editor2->setFactoryForManager(pointManager->subIntPropertyManager(), spinBoxFactory);
       editor2->setFactoryForManager(sizeManager->subIntPropertyManager(), spinBoxFactory);
       editor2->addProperty(propertyRunParams0);
       QScrollArea *scroll2 = new QScrollArea(dock2);
       scroll2->setWidgetResizable(true);
       scroll2->setWidget(editor2);
       dock2->setWidget(scroll2);

}
void MainWindow::updateExpandState()
{
    QList<QtBrowserItem *> list = propertyEditor->topLevelItems();
    QListIterator<QtBrowserItem *> it(list);
    while (it.hasNext()) {
        QtBrowserItem *item = it.next();
        QtProperty *prop = item->property();
        idToExpanded[propertyToId[prop]] = propertyEditor->isExpanded(item);
    }
}
void MainWindow::addProperty(QtProperty *property, const QString &id)
{
    propertyToId[property] = id;
    idToProperty[id] = property;
    QtBrowserItem *item = propertyEditor->addProperty(property);
    if (idToExpanded.contains(id))
        propertyEditor->setExpanded(item, idToExpanded[id]);
}
void MainWindow::valueChanged(QtProperty *property, double value)
{
    if (!propertyToId.contains(property))
        return;

    if (!currentItem)
        return;

    QString id = propertyToId[property];
    if (id == QLatin1String("xpos")) {
        if (currentItem->rtti() == QtCanvasItem::Rtti_Rectangle ||
                currentItem->rtti() == QtCanvasItem::Rtti_Ellipse) {
            QtCanvasPolygonalItem *i = (QtCanvasPolygonalItem *)currentItem;
            i->setX(value);
        }
    } else if (id == QLatin1String("ypos")) {
        if (currentItem->rtti() == QtCanvasItem::Rtti_Rectangle ||
                currentItem->rtti() == QtCanvasItem::Rtti_Ellipse) {
            QtCanvasPolygonalItem *i = (QtCanvasPolygonalItem *)currentItem;
            i->setY(value);
        }
    } else if (id == QLatin1String("zpos")) {
        if (currentItem->rtti() == QtCanvasItem::Rtti_Rectangle ||
                currentItem->rtti() == QtCanvasItem::Rtti_Ellipse) {
            QtCanvasPolygonalItem *i = (QtCanvasPolygonalItem *)currentItem;
            i->setZ(value);
        }
    } else if (id == QLatin1String("rotangle")) {
        params->setRotAngle(value);
    }
    canvas_xy->update();
}

void MainWindow::valueChanged(QtProperty *property, const QString &value)
{
    if (!propertyToId.contains(property))
        return;

    if (!currentItem)
        return;

    QString id = propertyToId[property];
    if (id == QLatin1String("outputfilename")) {
        params->setOutputFilename(value);
    } else if (id == QLatin1String("inputfilelist")) {
        params->setInputFilelist(value);
    }
    canvas_xy->update();
}

void MainWindow::valueChanged(QtProperty *property, const QColor &value)
{
    if (!propertyToId.contains(property))
        return;

    if (!currentItem)
        return;

    QString id = propertyToId[property];
    if (id == QLatin1String("excludecolor")) {
        if (currentItem->rtti() == QtCanvasItem::Rtti_Rectangle ||
                currentItem->rtti() == QtCanvasItem::Rtti_Ellipse) {
            QtCanvasPolygonalItem *i = (QtCanvasPolygonalItem *)currentItem;
            QPen p = i->pen();
            p.setColor(value);
            i->setPen(p);
        }
    }
    canvas_xy->update();
}

void MainWindow::valueChanged(QtProperty *property, const QPoint &value)
{
    if (!propertyToId.contains(property))
        return;

    if (!currentItem)
        return;

    QString id = propertyToId[property];
    if (currentItem->rtti() == QtCanvasItem::Rtti_Line) {
        QtCanvasLine *i = (QtCanvasLine *)currentItem;
        if (id == QLatin1String("endpoint")) {
            i->setPoints(i->startPoint().x(), i->startPoint().y(), value.x(), value.y());
        }
    }
    canvas_xy->update();
}

void MainWindow::valueChanged(QtProperty *property, const QSize &value)
{
    if (!propertyToId.contains(property))
        return;

    if (!currentItem)
        return;

    QString id = propertyToId[property];
    if (id == QLatin1String("size")) {
        if (currentItem->rtti() == QtCanvasItem::Rtti_Rectangle) {
            QtCanvasRectangle *i = (QtCanvasRectangle *)currentItem;
            i->setSize(value.width(), value.height());
        } else if (currentItem->rtti() == QtCanvasItem::Rtti_Ellipse) {
            QtCanvasEllipse *i = (QtCanvasEllipse *)currentItem;
            i->setSize(value.width(), value.height());
        } else {
            QtCanvasEllipse *i = (QtCanvasEllipse *)currentItem;
            i->setSize(value.width(), value.height());
        }
    }
    canvas_xy->update();
}


void MainWindow::createUndoView()
{
    undoView = new QUndoView(undoStack);
    undoView->setWindowTitle(tr("Command List"));
    undoView->show();
    undoView->setAttribute(Qt::WA_QuitOnClose, false);
}
MainWindow::~MainWindow()
{
    qDebug() << "~MainWindow()" << endl;
//    delete checkBox_MirrorCanvas;
    delete mpZoomInTool;
    delete mpZoomOutTool;
    delete mpPanTool;
    delete mpMapToolBar;
//    delete canvas1;
//    delete canvas2;
//    delete mpLayout;
    delete model;
    delete view;
    delete undoStack;
    delete undoView;
    delete deleteAction;
    delete addLayerAction;
    delete addGroupAction;
    delete undoAction;
    delete redoAction;
    delete exitAction;
    delete aboutAction;
    delete provider;
}
void MainWindow::createActions()
{
    // Delete Action
    deleteAction = new QAction(tr("&Delete Item"), this);
    deleteAction->setShortcut(tr("Del"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    //Layer Action
    addLayerAction = new QAction(tr("Add &Layer"), this);
    addLayerAction->setShortcut(tr("Ctrl+L"));
    connect(addLayerAction, SIGNAL(triggered()), this, SLOT(addLayer()));

    // Group Action
    addGroupAction = new QAction(tr("Add &Group"), this);
    addGroupAction->setShortcut(tr("Ctrl+G"));
    connect(addGroupAction, SIGNAL(triggered()), this, SLOT(addGroup()));

    // Set Local Data Directory Action
    setLocalDataPathAction = new QAction(tr("&Set Local Data Path..."), this);
    setLocalDataPathAction->setShortcut(tr("Ctrl+L"));
    connect(setLocalDataPathAction, SIGNAL(triggered()), this, SLOT(setLocalDataPath()));

    // Set Global Data Directory Action
    setGlobalDataPathAction = new QAction(tr("&Set Global Data Path..."), this);
    setGlobalDataPathAction->setShortcut(tr("Ctrl+G"));
    connect(setGlobalDataPathAction, SIGNAL(triggered()), this, SLOT(setGlobalDataPath()));

    // Undo Action
    undoAction = undoStack->createUndoAction(this, tr("&Undo"));
    undoAction->setShortcuts(QKeySequence::Undo);

    // Redo Action
    redoAction = undoStack->createRedoAction(this, tr("&Redo"));
    redoAction->setShortcuts(QKeySequence::Redo);

    // Exit Action
    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));//quit())); qApp

    // Open Action
    openAction = new QAction(tr("Open Project..."), this);
    openAction->setShortcuts(QKeySequence::Open);
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(open()));

    // Close Project Action
    closeProjectAction = new QAction(tr("Close Project..."), this);
    closeProjectAction->setShortcuts(QKeySequence::Close);
    connect(closeProjectAction, SIGNAL(triggered(bool)), this, SLOT(closeProject()));

    // Save Action
    saveAction = new QAction(tr("Save"), this);
    saveAction->setShortcuts(QKeySequence::Save);
    connect(saveAction, SIGNAL(triggered(bool)), this, SLOT(save()));

    // Save As Action
    saveAsAction = new QAction(tr("Save As..."), this);
    saveAsAction->setShortcuts(QKeySequence::SaveAs);
    connect(saveAsAction, SIGNAL(triggered(bool)), this, SLOT(saveAs()));

    // About Action
    aboutAction = new QAction(tr("&About"), this);
    QList<QKeySequence> aboutShortcuts;
    aboutShortcuts << tr("Ctrl+A") << tr("Ctrl+B");
    aboutAction->setShortcuts(aboutShortcuts);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}
void MainWindow::createMenus()
{
//    menuBar = new QMenuBar();
    // File Tab
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(closeProjectAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    connect(fileMenu, SIGNAL(aboutToShow()),this, SLOT(fileMenuAboutToShow()));
    connect(fileMenu, SIGNAL(aboutToHide()),this, SLOT(fileMenuAboutToHide()));

    // Edit Tab
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    editMenu->addAction(deleteAction);
    connect(editMenu, SIGNAL(aboutToShow()),this, SLOT(itemMenuAboutToShow()));
    connect(editMenu, SIGNAL(aboutToHide()),this, SLOT(itemMenuAboutToHide()));

    // Item Tab
    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(addLayerAction);
    itemMenu->addAction(addGroupAction);
    itemMenu->addSeparator();
    itemMenu->addAction(setLocalDataPathAction);
    itemMenu->addAction(setGlobalDataPathAction);

    // Help Tab
    helpMenu = menuBar()->addMenu(tr("&About"));
    helpMenu->addAction(aboutAction);

    viewMenu = menuBar()->addMenu(tr("View"));
    viewMenu->addAction(dock1->toggleViewAction());
    viewMenu->addAction(dock2->toggleViewAction());

}

void MainWindow::panMode()
{
    if(checkBox_lock_canvas_yz->isChecked())
        canvas_xy->setMapTool(mpPanTool);
    if(checkBox_lock_canvas_yz->isChecked())
        canvas_yz->setMapTool(mpPanTool);
}
void MainWindow::zoomInMode()
{
    if(checkBox_lock_canvas_yz->isChecked())
        canvas_xy->setMapTool(mpZoomInTool);
    if(checkBox_lock_canvas_yz->isChecked())
        canvas_yz->setMapTool(mpZoomInTool);
}
void MainWindow::zoomOutMode()
{
  if(checkBox_lock_canvas_xy->isChecked())
    canvas_xy->setMapTool(mpZoomOutTool);
  if(checkBox_lock_canvas_yz->isChecked())
    canvas_yz->setMapTool(mpZoomOutTool);
}
void MainWindow::onWillAddChildren(QgsLayerTreeNode *node, int indexFrom, int indexTo) {
    qDebug() << "WILL ADD" << node << indexFrom << indexTo << endl;
}
// Access newly added nodes
void MainWindow::onAddedChildren(QgsLayerTreeNode *node, int indexFrom, int indexTo) {
    qDebug() << "ADDED" << node << indexFrom << indexTo << endl;
}
// Access nodes being removed
void MainWindow::onWillRemoveChildren(QgsLayerTreeNode *node, int indexFrom, int indexTo) {
  qDebug() << "WILL REMOVE" << node << indexFrom << indexTo << endl;
}
void MainWindow::onRemovedChildren(QgsLayerTreeNode *node, int indexFrom, int indexTo) {
  qDebug() << "REMOVED" << node << indexFrom << indexTo << endl;
}
void MainWindow::onVisibilityChanged(QgsLayerTreeNode *node,Qt::CheckState state) {
    qDebug() << "onVisibilityChanged TOGGLED" << node << " " << state << endl;

//    QgsMapLayerRegistry::instance()->mapLayer(view->currentLayer()->id());
//    qDebug() << "isVisible" << myLayerSet.at(myLayerSet.indexOf(view->currentLayer())).isVisible() << endl;
//    mpMapCanvas->setLayerSet(myLayerSet);
}
void MainWindow::onCustomPropertyChanged(QgsLayerTreeNode *node, const QString &key) {
  qDebug() << "CHANGED" << node << " " << key << endl;
}
void MainWindow::onExpandedChanged(QgsLayerTreeNode *node, bool expanded) {
  qDebug() << "FOLDED" << node << " " << expanded << endl;
}
void MainWindow::onNameChanged(QgsLayerTreeNode *node, QString name) {
  qDebug() << "RENAMED" << node << " to " << name << endl;
}
void MainWindow::onChange(QgsMapLayer *layer) {
    qDebug() << "OnChange THIS HAS CHANGED" << endl;
    //QMessageBox::information(nullptr, "Change", "Current Layer: "+layer->name());
}
void MainWindow::onLayerSavedAs(QgsMapLayer *l, const QString &path) {
    qDebug() << "SAVEDAS" << l << " to " << path << endl;
}
void MainWindow::deleteItem()
{
    qDebug()<<"SIZE"<<endl;
    qDebug()<<view->size()<<endl;
    if (view->selectedLayerNodes().isEmpty())
        return;

    QUndoCommand *deleteCommand = new DeleteCommand(view);
    undoStack->push(deleteCommand);
}
void MainWindow::addLayer()
{
    QUndoCommand *addCommand = new AddCommand(root,canvas_xy,canvas_yz,myLayerSet,view);
    undoStack->push(addCommand);
}
void MainWindow::addGroup()
{
    QUndoCommand *addGroupCommand = new AddGroupCommand(view);
    undoStack->push(addGroupCommand);
}
void MainWindow::itemMenuAboutToHide()
{
    deleteAction->setEnabled(true);
}
// Check if we have files selected in order to enable delete action
void MainWindow::itemMenuAboutToShow()
{
    if(view==nullptr)
        deleteAction->setEnabled(false);
    else
        deleteAction->setEnabled(!view->selectedLayers().isEmpty());
}
void MainWindow::fileMenuAboutToHide()
{
    closeProjectAction->setEnabled(true);
}
// Check if we have files selected in order to enable delete action
void MainWindow::fileMenuAboutToShow()
{
    closeProjectAction->setEnabled(!QgsProject::instance()->fileName().isEmpty());
}
void MainWindow::about()
{
    QMessageBox::about(this, tr("MergeBathy GUI"),
                       tr("The <b>MergeBathy GUI </b> was developed by NRL in support of NAVO."
                          "(2017) Version 0.1."));
}
void MainWindow::setLocalDataPath()
{
    setDataPath(false);
}
void MainWindow::setGlobalDataPath()
{
    setDataPath(true);
}
void MainWindow::setDataPath(bool isGlobal)
{
    QgsProject * project = QgsProject::instance();
    QString dataPath;
    QString oldPath;
    if(isGlobal)
        dataPath = globalDataPath;
    else
        dataPath = localDataPath;
    oldPath = dataPath;

    QMessageBox msgBox;
    if(isGlobal)
        msgBox.setWindowTitle(tr("Set Local Data Path"));
    else
        msgBox.setWindowTitle(tr("Set Global Data Path"));
    msgBox.setModal(false);
    msgBox.setText(tr("Current Data Path set to: "));
    msgBox.setInformativeText(dataPath);
    QPushButton *directoryButton = msgBox.addButton(tr("Browse..."), QMessageBox::ActionRole);
    QPushButton *okButton = msgBox.addButton(QMessageBox::Ok);
    QPushButton *cancelButton = msgBox.addButton(QMessageBox::Cancel);

    msgBox.exec();

    while(true) {
        if(msgBox.clickedButton() == directoryButton) {
                QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
                dataPath = QFileDialog::getExistingDirectory(this, tr("Set Data Path"), dataPath, options);
                if(dataPath.isEmpty())
                        dataPath = oldPath;
                msgBox.setInformativeText(dataPath);
                msgBox.exec();
        }
        else if(msgBox.clickedButton() == okButton && oldPath != dataPath) {
            if(isGlobal) {
                globalDataPath = dataPath;
                QSettings settings("NRL", "MergeBathy GUI");
                settings.setValue("MainWindow/GlobalDataPath", dataPath);
            }
            else {
                localDataPath = dataPath;
                project->writeEntry("myproject", "LocalDataPath", dataPath);
                project->writeEntry("myproject", "isLocalDataPathSet", true);
            }
            break;
        } else break;
    }
}

void MainWindow::closeProject()
{
    QgsProject * project = QgsProject::instance();
    if (userReallyWantsToQuit()) {
        project->clear();
        if(project->fileName().isEmpty())
            localDataPath = globalDataPath;
            setWindowTitle("MergeBathy GUI: Untitled Project");
    }
}
void MainWindow::open()
{
    if (userReallyWantsToQuit()) {
        QString myProjectName = QFileDialog::getOpenFileName(this, tr("Open File"),QCoreApplication::applicationDirPath () + "./","");

        // Get the project instance
        QgsProject * project = QgsProject::instance();

        // Load another project
        bool isOpen = project->read(QFileInfo(myProjectName));
        if(isOpen) {
            readProjectSettings();
            setWindowTitle("MergeBathy GUI: "+ QFileInfo(myProjectName).fileName());
        }
    }
}
bool MainWindow::save()
{
    // Get the project instance
    QgsProject * project = QgsProject::instance();

    if(project->fileName().isEmpty())
        saveAs();
    else {
        project->write(project->fileName());
        statusBar()->showMessage(tr("Project saved"), 2000);
    }
    return true;
}
void MainWindow::saveAs()
{
    QString myProjectName = QFileDialog::getSaveFileName(this, tr("Save File As"),QCoreApplication::applicationDirPath () + "./",".qgs");

    // Get the project instance
    QgsProject * project = QgsProject::instance();

    // Save as another project
    project->write(myProjectName);
    setWindowTitle("MergeBathy GUI: "+ QFileInfo(myProjectName).fileName());
    statusBar()->showMessage(tr("Project saved"), 2000);
}
//We use close() instead
//void MainWindow::quit()
//{
//    QMessageBox messageBox;
//    messageBox.setWindowTitle(tr("MergeBathy GUI"));
//    messageBox.setText(tr("Do you really want to quit2?"));
//    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
//    messageBox.setDefaultButton(QMessageBox::No);
//    if (messageBox.exec() == QMessageBox::Yes)
//    {
//        qApp->quit();// close().trigger
//    }
//}

//Exit program
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (userReallyWantsToQuit()) {
        writeSettings();
        event->accept();
    } else
        event->ignore();
}

//Global settings
void MainWindow::writeSettings()
{
    QSettings settings("NRL", "MergeBathy GUI");

    settings.beginGroup("MainWindow");
        settings.setValue("size", size());
        settings.setValue("pos", pos());
        settings.setValue("GlobalDataPath", globalDataPath);
    settings.endGroup();
}

void MainWindow::readSettings()
{
    // Global Settings
    QSettings settings("NRL", "MergeBathy GUI");

    settings.beginGroup("MainWindow");
        resize(settings.value("size", QSize(400, 400)).toSize());
        move(settings.value("pos", QPoint(200, 200)).toPoint());
        globalDataPath = settings.value("GlobalDataPath").toString();
        readProjectSettings();
    settings.endGroup();
}
void MainWindow::readProjectSettings()
{
    QgsProject * project = QgsProject::instance();
    // Check project settings
    bool isLocalDataPathSet =project->readBoolEntry("myproject", "isLocalDataPathSet");
    if(isLocalDataPathSet)
        localDataPath = project->readEntry("myproject", "LocalDataPath");
    else {
        localDataPath = globalDataPath;
        project->writeEntry("myproject", "LocalDataPath", localDataPath);
        project->writeEntry("myproject", "isLocalDataPathSet", true);
    }
}
bool MainWindow::userReallyWantsToQuit()
{
    if (QgsProject::instance()->isDirty()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                     tr("The project has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}







