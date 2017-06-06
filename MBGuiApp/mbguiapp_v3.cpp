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
#include "mainwindow.h"
#include <commands.h>
#include <mysetdatapathdialog.h>

#include "qtpropertymanager.h"
#include "qteditorfactory.h"
#include "qttreepropertybrowser.h"
#include "qtbuttonpropertybrowser.h"
#include "qtgroupboxpropertybrowser.h"
#include "qtmbparams.h"

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

//#include <qgslayertreeviewdefaultactions.h>
#include "qgsattributeaction.h"
#include "qgsattributetabledialog.h"

#include "qgsmessageviewer.h"
#include "qgsmessagebar.h"
#include "qgsmessagebaritem.h"

#include "qgstransaction.h"
#include "qgstransactiongroup.h"
#include <QClipboard>
#include "qgsclipboard.h"

#include "qgssublayersdialog.h"
#include "ogr/qgsopenvectorlayerdialog.h"
#include "ogr/qgsvectorlayersaveasdialog.h"

#include "qgsdatumtransformdialog.h"
//#include "qgsrasterlayerproperties.h"
#include "qgslayertreeutils.h"
#include "qgsmaplayeractionregistry.h"
#include "qgsfieldcalculator.h"

#include "qgsmaptoolidentifyaction.h"




void MainWindow::layerTreeViewDoubleClicked( const QModelIndex& index )
{
//  Q_UNUSED( index )
//  QSettings settings;
//  switch ( settings.value( "/qgis/legendDoubleClickAction", 0 ).toInt() )
//  {
//    case 0:
//    {
//      //show properties
//      if ( mLayerTreeView )
//      {
//        // if it's a legend node, open symbol editor directly
//        if ( QgsSymbolV2LegendNode* node = dynamic_cast<QgsSymbolV2LegendNode*>( mLayerTreeView->currentLegendNode() ) )
//        {
//          const QgsSymbolV2* originalSymbol = node->symbol();
//          if ( !originalSymbol )
//            return;

//          QScopedPointer< QgsSymbolV2 > symbol( originalSymbol->clone() );
//          QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer*>( node->layerNode()->layer() );
//          QgsSymbolV2SelectorDialog dlg( symbol.data(), QgsStyleV2::defaultStyle(), vlayer, this );
//          dlg.setMapCanvas( mMapCanvas );
//          if ( dlg.exec() )
//          {
//            node->setSymbol( symbol.take() );
//          }

//          return;
//        }
//      }
//      MainWindow::instance()->layerProperties();
//      break;
//    }
//    case 1:
//      MainWindow::instance()->attributeTable();
//      break;
//    default:
//      break;
//  }
}

static bool cmpByText_( QAction* a, QAction* b )
{
  return QString::localeAwareCompare( a->text(), b->text() ) < 0;
}

MainWindow *MainWindow::smInstance = nullptr;

MainWindow::~MainWindow()
{
    qDebug() << "~MainWindow()" << endl;
    delete checkBox_lock_canvas_xy;
    delete checkBox_lock_canvas_yz;
    delete checkBox_lock_canvas_error;
    delete checkBox_lock_canvas_rms;
    delete checkBox_lock_canvas_mean;

    delete mpZoomInTool;
    delete mpZoomOutTool;
    delete mpPanTool;
    delete mpMapToolBar;

    delete canvas_xy;
    delete canvas_yz;
    delete canvas_error;
    delete canvas_rms;
    delete canvas_mean;

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
MainWindow::MainWindow(QWidget* parent, Qt::WFlags fl)
    : QMainWindow(parent,fl)
{
    smInstance = this;
    mInternalClipboard = new QgsClipboard; // create clipboard
//    connect( mInternalClipboard, SIGNAL( changed() ), this, SLOT( clipboardChanged() ) );

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

    createMBParamsGroupBox();
    createMapTools();

    QCheckBox * checkBox_lock_canvas_xy = new QCheckBox("Lock");
    QCheckBox * checkBox_lock_canvas_yz = new QCheckBox("Lock");
    QCheckBox * checkBox_lock_canvas_error = new QCheckBox("Lock");
    QCheckBox * checkBox_lock_canvas_rms = new QCheckBox("Lock");
    QCheckBox * checkBox_lock_canvas_mean = new QCheckBox("Lock");
    checkBox_lock_canvas_xy->setChecked(true);
    checkBox_lock_canvas_yz->setChecked(true);
    checkBox_lock_canvas_error->setChecked(true);
    checkBox_lock_canvas_rms->setChecked(true);
    checkBox_lock_canvas_mean->setChecked(true);

    QLabel * label_title_canvas_xy = new QLabel("XY-Plot");
    QLabel * label_title_canvas_yz = new QLabel("YZ-Plot");
    QLabel * label_title_canvas_error = new QLabel("Error");
    QLabel * label_title_canvas_rms = new QLabel("RMS");
    QLabel * label_title_canvas_mean = new QLabel("Mean");

    // Lay our widgets out in the main window
    hGroupBox_CanvasEdits = new QGroupBox(tr("Edit Layers"),frameMap);
    hGroupBox_CanvasResults = new QGroupBox(tr("Results"),frameMap);

    vGroupBox_canvas_xy = new QGroupBox(tr("Canvas XY Vertical layout"),frameMap);
    vGroupBox_canvas_yz = new QGroupBox(tr("Canvas YZ Vertical layout"),frameMap);
    vGroupBox_canvas_error = new QGroupBox(tr("Canvas Error Vertical layout"),frameMap);
    vGroupBox_canvas_rms = new QGroupBox(tr("Canvas RMS Vertical layout"),frameMap);
    vGroupBox_canvas_mean = new QGroupBox(tr("Canvas Mean Vertical layout"),frameMap);

    // Create Edit Layer Panel
    mpLayout_canvas_xy = new QVBoxLayout(frameMap);
    mpLayout_canvas_xy->addWidget(label_title_canvas_xy);
    mpLayout_canvas_xy->addWidget(canvas_xy);
    mpLayout_canvas_xy->addWidget(checkBox_lock_canvas_xy);
    vGroupBox_canvas_xy->setLayout(mpLayout_canvas_xy);

    mpLayout_canvas_yz = new QVBoxLayout(frameMap);
    mpLayout_canvas_yz->addWidget(label_title_canvas_yz);
    mpLayout_canvas_yz->addWidget(canvas_yz);
    mpLayout_canvas_yz->addWidget(checkBox_lock_canvas_yz);
    vGroupBox_canvas_yz->setLayout(mpLayout_canvas_yz);

    mpLayout_CanvasEdits = new QHBoxLayout(frameMap);
    mpLayout_CanvasEdits->addWidget(vGroupBox_canvas_xy);
    mpLayout_CanvasEdits->addWidget(vGroupBox_canvas_yz);
//    mpLayout_CanvasEdits->addWidget(dock1);
//    mpLayout_CanvasEdits->addWidget(dock2);
    hGroupBox_CanvasEdits->setLayout(mpLayout_CanvasEdits);

    // Create Results panel
    mpLayout_canvas_error = new QVBoxLayout(frameMap);
    mpLayout_canvas_error->addWidget(label_title_canvas_error);
    mpLayout_canvas_error->addWidget(canvas_error);
    mpLayout_canvas_error->addWidget(checkBox_lock_canvas_error);
    vGroupBox_canvas_error->setLayout(mpLayout_canvas_error);

    mpLayout_canvas_rms = new QVBoxLayout(frameMap);
    mpLayout_canvas_rms->addWidget(label_title_canvas_rms);
    mpLayout_canvas_rms->addWidget(canvas_rms);
    mpLayout_canvas_rms->addWidget(checkBox_lock_canvas_rms);
    vGroupBox_canvas_rms->setLayout(mpLayout_canvas_rms);

    mpLayout_canvas_mean = new QVBoxLayout(frameMap);
    mpLayout_canvas_mean->addWidget(label_title_canvas_mean);
    mpLayout_canvas_mean->addWidget(canvas_mean);
    mpLayout_canvas_mean->addWidget(checkBox_lock_canvas_mean);
    vGroupBox_canvas_mean->setLayout(mpLayout_canvas_mean);

    mpLayout_CanvasResults = new QHBoxLayout(frameMap);
    mpLayout_CanvasResults->addWidget(vGroupBox_canvas_error);
    mpLayout_CanvasResults->addWidget(vGroupBox_canvas_rms);
    mpLayout_CanvasResults->addWidget(vGroupBox_canvas_mean);
    hGroupBox_CanvasResults->setLayout(mpLayout_CanvasResults);

    // Add Panels to MainLayout
    QVBoxLayout * mainLayout = new QVBoxLayout(frameMap);
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
//    connect(mpActionSelect, SIGNAL(triggered()), this, SLOT(selectMode()));

    //create a little toolbar
    mpMapToolBar = addToolBar(tr("File"));
    mpMapToolBar->addAction(mpActionAddLayer);
    mpMapToolBar->addAction(mpActionZoomIn);
    mpMapToolBar->addAction(mpActionZoomOut);
    mpMapToolBar->addAction(mpActionPan);
//    mpMapToolBar->addAction(mpActionSelect);

    //create the maptools
    mpPanTool = new QgsMapToolPan(canvas_xy);
    mpPanTool->setAction(mpActionPan);
    mpZoomInTool = new QgsMapToolZoom(canvas_xy, FALSE); // false = in
    mpZoomInTool->setAction(mpActionZoomIn);
    mpZoomOutTool = new QgsMapToolZoom(canvas_xy, TRUE ); //true = out
    mpZoomOutTool->setAction(mpActionZoomOut);
//    mpSelectTool = new QgsMapToolSelect(canvas_xy);
//    mpSelectTool->setAction(mpActionSelect);

//    mpPanTool2 = new QgsMapToolPan(canvas_yz);
//    mpPanTool2->setAction(mpActionPan);
//    mpZoomInTool2 = new QgsMapToolZoom(canvas_yz, FALSE); // false = in
//    mpZoomInTool2->setAction(mpActionZoomIn);
//    mpZoomOutTool2 = new QgsMapToolZoom(canvas_yz, TRUE ); //true = out
//    mpZoomOutTool2->setAction(mpActionZoomOut);




//    connect( mActionOpenTable, SIGNAL( triggered() ), this, SLOT( attributeTable() ) );
////    connect( mActionOpenFieldCalc, SIGNAL( triggered() ), this, SLOT( fieldCalculator() ) );
//    connect( mActionToggleEditing, SIGNAL( triggered() ), this, SLOT( toggleEditing() ) );


//    mActionOpenTable->setIcon( QgsApplication::getThemeIcon( "/mActionOpenTable.svg" ) );

//    mActionOpenTable->setEnabled( false );
//    mActionIdentify->setEnabled( true );


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

    tab1ToggleAction = new QAction("ToggleTab1",this);
    connect(tab1ToggleAction, SIGNAL(toggled()),this,SLOT(tab1Changed()));

    tab2ToggleAction = new QAction("ToggleTab2",this);
    connect(tab2ToggleAction, SIGNAL(toggled()),this,SLOT(tab2Changed()));

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
      dock = new QDockWidget(tr("Run Params (list)"),this);
      addDockWidget(Qt::LeftDockWidgetArea, dock);

      dock1 = new QDockWidget(tr("Run Params (list)"),this);
      dock2 = new QDockWidget(tr("Run Params"),this);
      addDockWidget(Qt::LeftDockWidgetArea, dock1);
      addDockWidget(Qt::LeftDockWidgetArea, dock2);
      tabifyDockWidget(dock1, dock2);
      setDockNestingEnabled(false);
//      viewMenu->addAction(tab1ToggleAction); //setTabEnables()
//      viewMenu->addAction(tab2ToggleAction);

      viewMenu->addAction(dock1->toggleViewAction());
      viewMenu->addAction(dock2->toggleViewAction());
      dock1->setHidden(true);
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
//       dock1->setWidget(propertyEditor);

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
//       dock2->setWidget(scroll2);


//This will create
//       QPushButton *runButton1 = new QPushButton("&Run");
//       vGroupBox_mbparams_dock1 = new QGroupBox(tr("MBParams1 layout"),dock1);
//       mpLayout_mbparams_dock1 = new QVBoxLayout(dock1);
//       mpLayout_mbparams_dock1->addWidget(propertyEditor);
//       mpLayout_mbparams_dock1->addWidget(runButton1);
//       vGroupBox_mbparams_dock1->setLayout(mpLayout_mbparams_dock1);
//       dock1->setWidget(vGroupBox_mbparams_dock1);

//       QPushButton *runButton2 = new QPushButton("&Run");
//       vGroupBox_mbparams_dock2 = new QGroupBox(tr("MBParams2 layout"),dock2);
//       mpLayout_mbparams_dock2 = new QVBoxLayout(dock2);
//       mpLayout_mbparams_dock2->addWidget(scroll2);
//       mpLayout_mbparams_dock2->addWidget(runButton2);
//       vGroupBox_mbparams_dock2->setLayout(mpLayout_mbparams_dock2);
//       dock2->setWidget(vGroupBox_mbparams_dock2);



        QTabWidget * tab = new QTabWidget();
        tab->addTab(scroll2,"Form View");
        tab->addTab(propertyEditor,"List View");
        QPushButton *runButton = new QPushButton("&Run");
        vGroupBox_mbparams_dock = new QGroupBox(tr("MBParams layout"),dock);
        mpLayout_mbparams_dock = new QVBoxLayout(dock);
        mpLayout_mbparams_dock->addWidget(tab);
        mpLayout_mbparams_dock->addWidget(runButton);
        vGroupBox_mbparams_dock->setLayout(mpLayout_mbparams_dock);
        dock->setWidget(vGroupBox_mbparams_dock);

}
void MainWindow::itemMoved(QtCanvasItem *item)
{
    if (item != currentItem)
        return;

    doubleManager->setValue(idToProperty[QLatin1String("xpos")], item->x());
    doubleManager->setValue(idToProperty[QLatin1String("ypos")], item->y());
    doubleManager->setValue(idToProperty[QLatin1String("zpos")], item->z());
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
void MainWindow::itemClicked(QtCanvasItem *item)
{
    updateExpandState();

    QMap<QtProperty *, QString>::ConstIterator itProp = propertyToId.constBegin();
    while (itProp != propertyToId.constEnd()) {
        delete itProp.key();
        itProp++;
    }
    propertyToId.clear();
    idToProperty.clear();

    currentItem = item;
    if (!currentItem) {
        //deleteAction->setEnabled(false);
        return;
    }

  //  deleteAction->setEnabled(true);
    QtProperty *propertyEditParams;

    propertyEditParams = doubleManager->addProperty(tr("Position X"));
    doubleManager->setRange(propertyEditParams, 0, canvas_xy->width());
    doubleManager->setValue(propertyEditParams, item->x());
    addProperty(propertyEditParams, QLatin1String("xpos"));

    propertyEditParams = doubleManager->addProperty(tr("Position Y"));
    doubleManager->setRange(propertyEditParams, 0, canvas_xy->height());
    doubleManager->setValue(propertyEditParams, item->y());
    addProperty(propertyEditParams, QLatin1String("ypos"));

    propertyEditParams = doubleManager->addProperty(tr("Position Z"));
    doubleManager->setRange(propertyEditParams, 0, 256);
    doubleManager->setValue(propertyEditParams, item->z());
    addProperty(propertyEditParams, QLatin1String("zpos"));

    if (item->rtti() == QtCanvasItem::Rtti_Rectangle) {
        QtCanvasRectangle *i = (QtCanvasRectangle *)item;

        propertyEditParams = sizeManager->addProperty(tr("Size"));
        sizeManager->setValue(propertyEditParams, i->size());
        addProperty(propertyEditParams, QLatin1String("size"));
    }
    if (item->rtti() == QtCanvasItem::Rtti_Rectangle ||
            item->rtti() == QtCanvasItem::Rtti_Ellipse) {
        QtCanvasPolygonalItem *i = (QtCanvasPolygonalItem *)item;

        propertyEditParams = colorManager->addProperty(tr("Point Exclusion Color"));
        colorManager->setValue(propertyEditParams, i->pen().color());
        addProperty(propertyEditParams, QLatin1String("excludecolor"));

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

void MainWindow::tab1Changed()
{
    qDebug() <<"state1=" << tab1ToggleAction->isChecked() << endl;
//    tab1ToggleAction->setChecked(!tab1ToggleAction->isChecked());

}
void MainWindow::tab2Changed()
{
    qDebug() <<"state2=" << tab2ToggleAction->isChecked() << endl;
//    tab2ToggleAction->setChecked(!tab2ToggleAction->isChecked());

}

void MainWindow::createMenus()
{
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
    viewMenu->addAction(tab1ToggleAction);
    viewMenu->addAction(tab2ToggleAction);
    connect(viewMenu, SIGNAL(aboutToShow()),this, SLOT(viewMenuAboutToShow1()));
    connect(viewMenu, SIGNAL(aboutToHide()),this, SLOT(viewMenuAboutToHide1()));
    connect(viewMenu, SIGNAL(aboutToShow()),this, SLOT(viewMenuAboutToShow2()));
    connect(viewMenu, SIGNAL(aboutToHide()),this, SLOT(viewMenuAboutToHide2()));
}
void MainWindow::selectMode()
{
    canvas_xy->setMapTool(mpSelectTool);
    canvas_yz->setMapTool(mpSelectTool);
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
void MainWindow::fileMenuAboutToShow()
{
    closeProjectAction->setEnabled(!QgsProject::instance()->fileName().isEmpty());
}
void MainWindow::viewMenuAboutToHide1()
{
    tab1ToggleAction->setEnabled(true);
}
void MainWindow::viewMenuAboutToShow1()
{
//    tab1ToggleAction->setEnabled(!tab->isTabEnabled(tab->indexOf(propertyEditor)));
}
void MainWindow::viewMenuAboutToHide2()
{
    tab2ToggleAction->setEnabled(true);
}
void MainWindow::viewMenuAboutToShow2()
{
//    tab2ToggleAction->setEnabled(!tab->isTabEnabled(1));
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









void MainWindow::fieldCalculator()
{
  QgsVectorLayer *myLayer = qobject_cast<QgsVectorLayer *>( activeLayer() );
  if ( !myLayer )
  {
    return;
  }

  QgsFieldCalculator calc( myLayer, this );
  if ( calc.exec() )
  {
    canvas_xy->refresh();
  }
}



void MainWindow::attributeTable()
{
  QgsVectorLayer *myLayer = qobject_cast<QgsVectorLayer *>( activeLayer() );
  if ( !myLayer )
  {
    return;
  }

  QgsAttributeTableDialog *mDialog = new QgsAttributeTableDialog( myLayer );
  mDialog->show();
  // the dialog will be deleted by itself on close
}

//////////////////////////////////////////////////////////////////////
//            Set Up the gui toolbars, menus, statusbar etc
//////////////////////////////////////////////////////////////////////

void MainWindow::createActions2()
{
    //  mActionPluginSeparator1 = nullptr;  // plugin list separator will be created when the first plugin is loaded
    //  mActionPluginSeparator2 = nullptr;  // python separator will be created only if python is found
    //  mActionRasterSeparator = nullptr;   // raster plugins list separator will be created when the first plugin is loaded

    // Project Menu Items

//    connect( mActionNewProject, SIGNAL( triggered() ), this, SLOT( fileNew() ) );
//    connect( mActionNewBlankProject, SIGNAL( triggered() ), this, SLOT( fileNewBlank() ) );
//    connect( mActionOpenProject, SIGNAL( triggered() ), this, SLOT( fileOpen() ) );
//    connect( mActionSaveProject, SIGNAL( triggered() ), this, SLOT( fileSave() ) );
//    connect( mActionSaveProjectAs, SIGNAL( triggered() ), this, SLOT( fileSaveAs() ) );
//    connect( mActionSaveMapAsImage, SIGNAL( triggered() ), this, SLOT( saveMapAsImage() ) );
//    connect( mActionNewPrintComposer, SIGNAL( triggered() ), this, SLOT( newPrintComposer() ) );
//    connect( mActionShowComposerManager, SIGNAL( triggered() ), this, SLOT( showComposerManager() ) );
//    connect( mActionExit, SIGNAL( triggered() ), this, SLOT( fileExit() ) );
//    connect( mActionDxfExport, SIGNAL( triggered() ), this, SLOT( dxfExport() ) );

    // Edit Menu Items

//    connect( mActionUndo, SIGNAL( triggered() ), mUndoWidget, SLOT( undo() ) );
//    connect( mActionRedo, SIGNAL( triggered() ), mUndoWidget, SLOT( redo() ) );
//    connect( mActionCutFeatures, SIGNAL( triggered() ), this, SLOT( editCut() ) );
//    connect( mActionCopyFeatures, SIGNAL( triggered() ), this, SLOT( editCopy() ) );
//    connect( mActionPasteFeatures, SIGNAL( triggered() ), this, SLOT( editPaste() ) );
//    connect( mActionPasteAsNewVector, SIGNAL( triggered() ), this, SLOT( pasteAsNewVector() ) );
//    connect( mActionPasteAsNewMemoryVector, SIGNAL( triggered() ), this, SLOT( pasteAsNewMemoryVector() ) );
//    connect( mActionCopyStyle, SIGNAL( triggered() ), this, SLOT( copyStyle() ) );
//    connect( mActionPasteStyle, SIGNAL( triggered() ), this, SLOT( pasteStyle() ) );
//    connect( mActionAddFeature, SIGNAL( triggered() ), this, SLOT( addFeature() ) );
//    connect( mActionCircularStringCurvePoint, SIGNAL( triggered() ), this, SLOT( circularStringCurvePoint() ) );
//    connect( mActionCircularStringRadius, SIGNAL( triggered() ), this, SLOT( circularStringRadius() ) );
//    connect( mActionMoveFeature, SIGNAL( triggered() ), this, SLOT( moveFeature() ) );
//    connect( mActionRotateFeature, SIGNAL( triggered() ), this, SLOT( rotateFeature() ) );

//    connect( mActionReshapeFeatures, SIGNAL( triggered() ), this, SLOT( reshapeFeatures() ) );
//    connect( mActionSplitFeatures, SIGNAL( triggered() ), this, SLOT( splitFeatures() ) );
//    connect( mActionSplitParts, SIGNAL( triggered() ), this, SLOT( splitParts() ) );
    connect( mActionDeleteSelected, SIGNAL( triggered() ), this, SLOT( deleteSelected() ) );
//    connect( mActionAddRing, SIGNAL( triggered() ), this, SLOT( addRing() ) );
//    connect( mActionFillRing, SIGNAL( triggered() ), this, SLOT( fillRing() ) );
//    connect( mActionAddPart, SIGNAL( triggered() ), this, SLOT( addPart() ) );
//    connect( mActionSimplifyFeature, SIGNAL( triggered() ), this, SLOT( simplifyFeature() ) );
//    connect( mActionDeleteRing, SIGNAL( triggered() ), this, SLOT( deleteRing() ) );
//    connect( mActionDeletePart, SIGNAL( triggered() ), this, SLOT( deletePart() ) );
//    connect( mActionMergeFeatures, SIGNAL( triggered() ), this, SLOT( mergeSelectedFeatures() ) );
//    connect( mActionMergeFeatureAttributes, SIGNAL( triggered() ), this, SLOT( mergeAttributesOfSelectedFeatures() ) );
//    connect( mActionNodeTool, SIGNAL( triggered() ), this, SLOT( nodeTool() ) );
//    connect( mActionRotatePointSymbols, SIGNAL( triggered() ), this, SLOT( rotatePointSymbols() ) );
//    connect( mActionSnappingOptions, SIGNAL( triggered() ), this, SLOT( snappingOptions() ) );
//    connect( mActionOffsetCurve, SIGNAL( triggered() ), this, SLOT( offsetCurve() ) );

    // View Menu Items

//    #ifdef HAVE_TOUCH
//    connect( mActionTouch, SIGNAL( triggered() ), this, SLOT( touch() ) );
//    #endif
    connect( mActionPan, SIGNAL( triggered() ), this, SLOT( pan() ) );
    connect( mActionPanToSelected, SIGNAL( triggered() ), this, SLOT( panToSelected() ) );
    connect( mActionZoomIn, SIGNAL( triggered() ), this, SLOT( zoomIn() ) );
    connect( mActionZoomOut, SIGNAL( triggered() ), this, SLOT( zoomOut() ) );
    connect( mActionSelectFeatures, SIGNAL( triggered() ), this, SLOT( selectFeatures() ) );
    connect( mActionSelectPolygon, SIGNAL( triggered() ), this, SLOT( selectByPolygon() ) );
    connect( mActionSelectFreehand, SIGNAL( triggered() ), this, SLOT( selectByFreehand() ) );
    connect( mActionSelectRadius, SIGNAL( triggered() ), this, SLOT( selectByRadius() ) );
    connect( mActionDeselectAll, SIGNAL( triggered() ), this, SLOT( deselectAll() ) );
    connect( mActionSelectAll, SIGNAL( triggered() ), this, SLOT( selectAll() ) );
    connect( mActionInvertSelection, SIGNAL( triggered() ), this, SLOT( invertSelection() ) );
    connect( mActionSelectByExpression, SIGNAL( triggered() ), this, SLOT( selectByExpression() ) );
    connect( mActionIdentify, SIGNAL( triggered() ), this, SLOT( identify() ) );
    connect( mActionFeatureAction, SIGNAL( triggered() ), this, SLOT( doFeatureAction() ) );
    connect( mActionMeasure, SIGNAL( triggered() ), this, SLOT( measure() ) );
    connect( mActionMeasureArea, SIGNAL( triggered() ), this, SLOT( measureArea() ) );
    connect( mActionMeasureAngle, SIGNAL( triggered() ), this, SLOT( measureAngle() ) );
    connect( mActionZoomFullExtent, SIGNAL( triggered() ), this, SLOT( zoomFull() ) );
    connect( mActionZoomToLayer, SIGNAL( triggered() ), this, SLOT( zoomToLayerExtent() ) );
    connect( mActionZoomToSelected, SIGNAL( triggered() ), this, SLOT( zoomToSelected() ) );
    connect( mActionZoomLast, SIGNAL( triggered() ), this, SLOT( zoomToPrevious() ) );
    connect( mActionZoomNext, SIGNAL( triggered() ), this, SLOT( zoomToNext() ) );
    connect( mActionZoomActualSize, SIGNAL( triggered() ), this, SLOT( zoomActualSize() ) );
    connect( mActionMapTips, SIGNAL( triggered() ), this, SLOT( toggleMapTips() ) );
    connect( mActionNewBookmark, SIGNAL( triggered() ), this, SLOT( newBookmark() ) );
    connect( mActionShowBookmarks, SIGNAL( triggered() ), this, SLOT( showBookmarks() ) );
    connect( mActionDraw, SIGNAL( triggered() ), this, SLOT( refreshMapCanvas() ) );
//    connect( mActionTextAnnotation, SIGNAL( triggered() ), this, SLOT( addTextAnnotation() ) );
//    connect( mActionFormAnnotation, SIGNAL( triggered() ), this, SLOT( addFormAnnotation() ) );
//    connect( mActionHtmlAnnotation, SIGNAL( triggered() ), this, SLOT( addHtmlAnnotation() ) );
//    connect( mActionSvgAnnotation, SIGNAL( triggered() ), this, SLOT( addSvgAnnotation() ) );
//    connect( mActionAnnotation, SIGNAL( triggered() ), this, SLOT( modifyAnnotation() ) );
//    connect( mActionLabeling, SIGNAL( triggered() ), this, SLOT( labeling() ) );
//    connect( mActionStatisticalSummary, SIGNAL( triggered() ), this, SLOT( showStatisticsDockWidget() ) );

    // Layer Menu Items

//    connect( mActionNewVectorLayer, SIGNAL( triggered() ), this, SLOT( newVectorLayer() ) );
//    connect( mActionNewSpatiaLiteLayer, SIGNAL( triggered() ), this, SLOT( newSpatialiteLayer() ) );
//    connect( mActionNewMemoryLayer, SIGNAL( triggered() ), this, SLOT( newMemoryLayer() ) );
//    connect( mActionShowRasterCalculator, SIGNAL( triggered() ), this, SLOT( showRasterCalculator() ) );
//    connect( mActionShowAlignRasterTool, SIGNAL( triggered() ), this, SLOT( showAlignRasterTool() ) );
//    connect( mActionEmbedLayers, SIGNAL( triggered() ), this, SLOT( embedLayers() ) );
//    connect( mActionAddLayerDefinition, SIGNAL( triggered() ), this, SLOT( addLayerDefinition() ) );
//    connect( mActionAddOgrLayer, SIGNAL( triggered() ), this, SLOT( addVectorLayer() ) );
//    connect( mActionAddRasterLayer, SIGNAL( triggered() ), this, SLOT( addRasterLayer() ) );
//    connect( mActionAddPgLayer, SIGNAL( triggered() ), this, SLOT( addDatabaseLayer() ) );
//    connect( mActionAddSpatiaLiteLayer, SIGNAL( triggered() ), this, SLOT( addSpatiaLiteLayer() ) );
//    connect( mActionAddMssqlLayer, SIGNAL( triggered() ), this, SLOT( addMssqlLayer() ) );
//    connect( mActionAddOracleLayer, SIGNAL( triggered() ), this, SLOT( addOracleLayer() ) );
//    connect( mActionAddWmsLayer, SIGNAL( triggered() ), this, SLOT( addWmsLayer() ) );
//    connect( mActionAddWcsLayer, SIGNAL( triggered() ), this, SLOT( addWcsLayer() ) );
//    connect( mActionAddWfsLayer, SIGNAL( triggered() ), this, SLOT( addWfsLayer() ) );
//    connect( mActionAddDelimitedText, SIGNAL( triggered() ), this, SLOT( addDelimitedTextLayer() ) );
//    connect( mActionAddVirtualLayer, SIGNAL( triggered() ), this, SLOT( addVirtualLayer() ) );
    connect( mActionOpenTable, SIGNAL( triggered() ), this, SLOT( attributeTable() ) );
    connect( mActionOpenFieldCalc, SIGNAL( triggered() ), this, SLOT( fieldCalculator() ) );
    connect( mActionToggleEditing, SIGNAL( triggered() ), this, SLOT( toggleEditing() ) );
    connect( mActionSaveLayerEdits, SIGNAL( triggered() ), this, SLOT( saveActiveLayerEdits() ) );
    connect( mActionSaveEdits, SIGNAL( triggered() ), this, SLOT( saveEdits() ) );
    connect( mActionSaveAllEdits, SIGNAL( triggered() ), this, SLOT( saveAllEdits() ) );
    connect( mActionRollbackEdits, SIGNAL( triggered() ), this, SLOT( rollbackEdits() ) );
    connect( mActionRollbackAllEdits, SIGNAL( triggered() ), this, SLOT( rollbackAllEdits() ) );
    connect( mActionCancelEdits, SIGNAL( triggered() ), this, SLOT( cancelEdits() ) );
    connect( mActionCancelAllEdits, SIGNAL( triggered() ), this, SLOT( cancelAllEdits() ) );
    connect( mActionLayerSaveAs, SIGNAL( triggered() ), this, SLOT( saveAsFile() ) );
//    connect( mActionSaveLayerDefinition, SIGNAL( triggered() ), this, SLOT( saveAsLayerDefinition() ) );
//    connect( mActionRemoveLayer, SIGNAL( triggered() ), this, SLOT( removeLayer() ) );
//    connect( mActionDuplicateLayer, SIGNAL( triggered() ), this, SLOT( duplicateLayers() ) );
//    connect( mActionSetLayerScaleVisibility, SIGNAL( triggered() ), this, SLOT( setLayerScaleVisibility() ) );
//    connect( mActionSetLayerCRS, SIGNAL( triggered() ), this, SLOT( setLayerCRS() ) );
//    connect( mActionSetProjectCRSFromLayer, SIGNAL( triggered() ), this, SLOT( setProjectCRSFromLayer() ) );
//    connect( mActionLayerProperties, SIGNAL( triggered() ), this, SLOT( layerProperties() ) );
//    connect( mActionLayerSubsetString, SIGNAL( triggered() ), this, SLOT( layerSubsetString() ) );
//    connect( mActionAddToOverview, SIGNAL( triggered() ), this, SLOT( isInOverview() ) );
//    connect( mActionAddAllToOverview, SIGNAL( triggered() ), this, SLOT( addAllToOverview() ) );
//    connect( mActionRemoveAllFromOverview, SIGNAL( triggered() ), this, SLOT( removeAllFromOverview() ) );
    connect( mActionShowAllLayers, SIGNAL( triggered() ), this, SLOT( showAllLayers() ) );
    connect( mActionHideAllLayers, SIGNAL( triggered() ), this, SLOT( hideAllLayers() ) );
    connect( mActionShowSelectedLayers, SIGNAL( triggered() ), this, SLOT( showSelectedLayers() ) );
    connect( mActionHideSelectedLayers, SIGNAL( triggered() ), this, SLOT( hideSelectedLayers() ) );



//    // Plugin Menu Items

//    connect( mActionManagePlugins, SIGNAL( triggered() ), this, SLOT( showPluginManager() ) );
//    connect( mActionShowPythonDialog, SIGNAL( triggered() ), this, SLOT( showPythonDialog() ) );

//    // Settings Menu Items

//    connect( mActionToggleFullScreen, SIGNAL( triggered() ), this, SLOT( toggleFullScreen() ) );
//    connect( mActionProjectProperties, SIGNAL( triggered() ), this, SLOT( projectProperties() ) );
//    connect( mActionOptions, SIGNAL( triggered() ), this, SLOT( options() ) );
//    connect( mActionCustomProjection, SIGNAL( triggered() ), this, SLOT( customProjection() ) );
//    connect( mActionConfigureShortcuts, SIGNAL( triggered() ), this, SLOT( configureShortcuts() ) );
//    connect( mActionStyleManagerV2, SIGNAL( triggered() ), this, SLOT( showStyleManagerV2() ) );
//    connect( mActionCustomization, SIGNAL( triggered() ), this, SLOT( customize() ) );

//    #ifdef Q_OS_MAC
//    // Window Menu Items

//    mActionWindowMinimize = new QAction( tr( "Minimize" ), this );
//    mActionWindowMinimize->setShortcut( tr( "Ctrl+M", "Minimize Window" ) );
//    mActionWindowMinimize->setStatusTip( tr( "Minimizes the active window to the dock" ) );
//    connect( mActionWindowMinimize, SIGNAL( triggered() ), this, SLOT( showActiveWindowMinimized() ) );

//    mActionWindowZoom = new QAction( tr( "Zoom" ), this );
//    mActionWindowZoom->setStatusTip( tr( "Toggles between a predefined size and the window size set by the user" ) );
//    connect( mActionWindowZoom, SIGNAL( triggered() ), this, SLOT( toggleActiveWindowMaximized() ) );

//    mActionWindowAllToFront = new QAction( tr( "Bring All to Front" ), this );
//    mActionWindowAllToFront->setStatusTip( tr( "Bring forward all open windows" ) );
//    connect( mActionWindowAllToFront, SIGNAL( triggered() ), this, SLOT( bringAllToFront() ) );

//    // list of open windows
//    mWindowActions = new QActionGroup( this );
//    #endif

    // Vector edits menu
    QMenu* menuAllEdits = new QMenu( tr( "Current Edits" ), this );
    menuAllEdits->addAction( mActionSaveEdits );
    menuAllEdits->addAction( mActionRollbackEdits );
    menuAllEdits->addAction( mActionCancelEdits );
    menuAllEdits->addSeparator();
    menuAllEdits->addAction( mActionSaveAllEdits );
    menuAllEdits->addAction( mActionRollbackAllEdits );
    menuAllEdits->addAction( mActionCancelAllEdits );
    mActionAllEdits->setMenu( menuAllEdits );

//    // Raster toolbar items
//    connect( mActionLocalHistogramStretch, SIGNAL( triggered() ), this, SLOT( localHistogramStretch() ) );
//    connect( mActionFullHistogramStretch, SIGNAL( triggered() ), this, SLOT( fullHistogramStretch() ) );
//    connect( mActionLocalCumulativeCutStretch, SIGNAL( triggered() ), this, SLOT( localCumulativeCutStretch() ) );
//    connect( mActionFullCumulativeCutStretch, SIGNAL( triggered() ), this, SLOT( fullCumulativeCutStretch() ) );
//    connect( mActionIncreaseBrightness, SIGNAL( triggered() ), this, SLOT( increaseBrightness() ) );
//    connect( mActionDecreaseBrightness, SIGNAL( triggered() ), this, SLOT( decreaseBrightness() ) );
//    connect( mActionIncreaseContrast, SIGNAL( triggered() ), this, SLOT( increaseContrast() ) );
//    connect( mActionDecreaseContrast, SIGNAL( triggered() ), this, SLOT( decreaseContrast() ) );

//    // Vector Menu Items
//    connect( mActionOSMDownload, SIGNAL( triggered() ), this, SLOT( osmDownloadDialog() ) );
//    connect( mActionOSMImport, SIGNAL( triggered() ), this, SLOT( osmImportDialog() ) );
//    connect( mActionOSMExport, SIGNAL( triggered() ), this, SLOT( osmExportDialog() ) );

//    // Help Menu Items

//    #ifdef Q_OS_MAC
//    mActionHelpContents->setShortcut( QString( "Ctrl+?" ) );
//    mActionQgisHomePage->setShortcut( QString() );
//    mActionReportaBug->setShortcut( QString() );
//    #endif

//    mActionHelpContents->setEnabled( QFileInfo( QgsApplication::pkgDataPath() + "/doc/index.html" ).exists() );

//    connect( mActionHelpContents, SIGNAL( triggered() ), this, SLOT( helpContents() ) );
//    connect( mActionHelpAPI, SIGNAL( triggered() ), this, SLOT( apiDocumentation() ) );
//    connect( mActionReportaBug, SIGNAL( triggered() ), this, SLOT( reportaBug() ) );
//    connect( mActionNeedSupport, SIGNAL( triggered() ), this, SLOT( supportProviders() ) );
//    connect( mActionQgisHomePage, SIGNAL( triggered() ), this, SLOT( helpQgisHomePage() ) );
//    connect( mActionCheckQgisVersion, SIGNAL( triggered() ), this, SLOT( checkQgisVersion() ) );
//    connect( mActionAbout, SIGNAL( triggered() ), this, SLOT( about() ) );
//    connect( mActionSponsors, SIGNAL( triggered() ), this, SLOT( sponsors() ) );

//    connect( mActionShowPinnedLabels, SIGNAL( toggled( bool ) ), this, SLOT( showPinnedLabels( bool ) ) );
//    connect( mActionPinLabels, SIGNAL( triggered() ), this, SLOT( pinLabels() ) );
//    connect( mActionShowHideLabels, SIGNAL( triggered() ), this, SLOT( showHideLabels() ) );
//    connect( mActionMoveLabel, SIGNAL( triggered() ), this, SLOT( moveLabel() ) );
//    connect( mActionRotateLabel, SIGNAL( triggered() ), this, SLOT( rotateLabel() ) );
//    connect( mActionChangeLabelProperties, SIGNAL( triggered() ), this, SLOT( changeLabelProperties() ) );

//    #ifndef HAVE_POSTGRESQL
//    delete mActionAddPgLayer;
//    mActionAddPgLayer = 0;
//    #endif

//    #ifndef HAVE_MSSQL
//    delete mActionAddMssqlLayer;
//    mActionAddMssqlLayer = 0;
//    #endif

//    #ifndef HAVE_ORACLE
//    delete mActionAddOracleLayer;
//    mActionAddOracleLayer = nullptr;
//    #endif

}
void MainWindow::createActionGroups()
{
  //
  // Map Tool Group
  mMapToolGroup = new QActionGroup( this );
//#ifdef HAVE_TOUCH
//  mMapToolGroup->addAction( mActionTouch );
//#endif
  mMapToolGroup->addAction( mActionPan );
  mMapToolGroup->addAction( mActionZoomIn );
  mMapToolGroup->addAction( mActionZoomOut );
  mMapToolGroup->addAction( mActionIdentify );
  mMapToolGroup->addAction( mActionFeatureAction );
  mMapToolGroup->addAction( mActionSelectFeatures );
  mMapToolGroup->addAction( mActionSelectPolygon );
  mMapToolGroup->addAction( mActionSelectFreehand );
  mMapToolGroup->addAction( mActionSelectRadius );
  mMapToolGroup->addAction( mActionDeselectAll );
  mMapToolGroup->addAction( mActionSelectAll );
  mMapToolGroup->addAction( mActionInvertSelection );
  mMapToolGroup->addAction( mActionMeasure );
  mMapToolGroup->addAction( mActionMeasureArea );
  mMapToolGroup->addAction( mActionMeasureAngle );
//  mMapToolGroup->addAction( mActionAddFeature );
//  mMapToolGroup->addAction( mActionCircularStringCurvePoint );
//  mMapToolGroup->addAction( mActionCircularStringRadius );
//  mMapToolGroup->addAction( mActionMoveFeature );
//  mMapToolGroup->addAction( mActionRotateFeature );
//#if defined(GEOS_VERSION_MAJOR) && defined(GEOS_VERSION_MINOR) && \
//    ((GEOS_VERSION_MAJOR>3) || ((GEOS_VERSION_MAJOR==3) && (GEOS_VERSION_MINOR>=3)))
//  mMapToolGroup->addAction( mActionOffsetCurve );
//#endif
//  mMapToolGroup->addAction( mActionReshapeFeatures );
//  mMapToolGroup->addAction( mActionSplitFeatures );
//  mMapToolGroup->addAction( mActionSplitParts );
  mMapToolGroup->addAction( mActionDeleteSelected );
//  mMapToolGroup->addAction( mActionAddRing );
//  mMapToolGroup->addAction( mActionFillRing );
//  mMapToolGroup->addAction( mActionAddPart );
//  mMapToolGroup->addAction( mActionSimplifyFeature );
//  mMapToolGroup->addAction( mActionDeleteRing );
//  mMapToolGroup->addAction( mActionDeletePart );
//  mMapToolGroup->addAction( mActionMergeFeatures );
//  mMapToolGroup->addAction( mActionMergeFeatureAttributes );
//  mMapToolGroup->addAction( mActionNodeTool );
//  mMapToolGroup->addAction( mActionRotatePointSymbols );
//  mMapToolGroup->addAction( mActionPinLabels );
//  mMapToolGroup->addAction( mActionShowHideLabels );
//  mMapToolGroup->addAction( mActionMoveLabel );
//  mMapToolGroup->addAction( mActionRotateLabel );
//  mMapToolGroup->addAction( mActionChangeLabelProperties );

  //
//  // Preview Modes Group
//  QActionGroup* mPreviewGroup = new QActionGroup( this );
//  mPreviewGroup->setExclusive( true );
//  mActionPreviewModeOff->setActionGroup( mPreviewGroup );
//  mActionPreviewModeGrayscale->setActionGroup( mPreviewGroup );
//  mActionPreviewModeMono->setActionGroup( mPreviewGroup );
//  mActionPreviewProtanope->setActionGroup( mPreviewGroup );
//  mActionPreviewDeuteranope->setActionGroup( mPreviewGroup );
}

void MainWindow::createMenus2()
{
  /*
   * The User Interface Guidelines for each platform specify different locations
   * for the following items.
   *
   * Project Properties:
   * Gnome, Mac, Win - File/Project menu above print commands (Win doesn't specify)
   * Kde - Settings menu
   *
   * Custom CRS, Options:
   * Gnome - bottom of Edit menu
   * Mac - Application menu (moved automatically by Qt)
   * Kde, Win - Settings menu (Win should use Tools menu but we don't have one)
   *
   * Panel and Toolbar submenus, Toggle Full Screen:
   * Gnome, Mac, Win - View menu
   * Kde - Settings menu
   *
   * For Mac, About and Exit are also automatically moved by Qt to the Application menu.
   */

  // Panel and Toolbar Submenus
  mPanelMenu = new QMenu( tr( "Panels" ), this );
  mPanelMenu->setObjectName( "mPanelMenu" );
  mToolbarMenu = new QMenu( tr( "Toolbars" ), this );
  mToolbarMenu->setObjectName( "mToolbarMenu" );

  // Get platform for menu layout customization (Gnome, Kde, Mac, Win)
  QDialogButtonBox::ButtonLayout layout =
    QDialogButtonBox::ButtonLayout( style()->styleHint( QStyle::SH_DialogButtonLayout, nullptr, this ) );

  // Project Menu

  // Connect once for the entire submenu.
//  connect( mRecentProjectsMenu, SIGNAL( triggered( QAction * ) ),
//           this, SLOT( openProject( QAction * ) ) );
//  connect( mProjectFromTemplateMenu, SIGNAL( triggered( QAction * ) ),
//           this, SLOT( fileNewFromTemplateAction( QAction * ) ) );

//  if ( layout == QDialogButtonBox::GnomeLayout || layout == QDialogButtonBox::MacLayout || layout == QDialogButtonBox::WinLayout )
//  {
//    QAction* before = mActionNewPrintComposer;
//    mSettingsMenu->removeAction( mActionProjectProperties );
//    mProjectMenu->insertAction( before, mActionProjectProperties );
//    mProjectMenu->insertSeparator( before );
//  }

  // View Menu

  if ( layout != QDialogButtonBox::KdeLayout )
  {
    mViewMenu->addSeparator();
    mViewMenu->addMenu( mPanelMenu );
    mViewMenu->addMenu( mToolbarMenu );
    mViewMenu->addAction( mActionToggleFullScreen );
  }
  else
  {
//    // on the top of the settings menu
//    QAction* before = mActionProjectProperties;
//    mSettingsMenu->insertMenu( before, mPanelMenu );
//    mSettingsMenu->insertMenu( before, mToolbarMenu );
//    mSettingsMenu->insertAction( before, mActionToggleFullScreen );
//    mSettingsMenu->insertSeparator( before );
  }


#ifdef Q_OS_MAC
  //disabled for OSX - see #10761
  //also see http://qt-project.org/forums/viewthread/3630 QGraphicsEffects are not well supported on OSX
  mMenuPreviewMode->menuAction()->setVisible( false );
#endif


#ifdef Q_OS_MAC

  // keep plugins from hijacking About and Preferences application menus
  // these duplicate actions will be moved to application menus by Qt
//  mProjectMenu->addAction( mActionAbout );
//  mProjectMenu->addAction( mActionOptions );

  // Window Menu

  mWindowMenu = new QMenu( tr( "Window" ), this );

  mWindowMenu->addAction( mActionWindowMinimize );
  mWindowMenu->addAction( mActionWindowZoom );
  mWindowMenu->addSeparator();

  mWindowMenu->addAction( mActionWindowAllToFront );
  mWindowMenu->addSeparator();

  // insert before Help menu, as per Mac OS convention
  menuBar()->insertMenu( mHelpMenu->menuAction(), mWindowMenu );
#endif

//  // Database Menu
//  // don't add it yet, wait for a plugin
//  mDatabaseMenu = new QMenu( tr( "&Database" ), menuBar() );
//  mDatabaseMenu->setObjectName( "mDatabaseMenu" );
//  // Web Menu
//  // don't add it yet, wait for a plugin
//  mWebMenu = new QMenu( tr( "&Web" ), menuBar() );
//  mWebMenu->setObjectName( "mWebMenu" );

  // Help menu
  // add What's this button to it
//  QAction* before = mActionHelpAPI;
//  QAction* actionWhatsThis = QWhatsThis::createAction2( this );
//  actionWhatsThis->setIcon( QgsApplication::getThemeIcon( "/mActionWhatsThis.svg" ) );
//  mHelpMenu->insertAction( before, actionWhatsThis );
}

void MainWindow::createToolBars()
{
  QSettings settings;
  // QSize myIconSize ( 32,32 ); //large icons
  // Note: we need to set each object name to ensure that
  // qmainwindow::saveState and qmainwindow::restoreState
  // work properly

  QList<QToolBar*> toolbarMenuToolBars;
  toolbarMenuToolBars << mFileToolBar
  << mLayerToolBar
//  << mDigitizeToolBar
//  << mAdvancedDigitizeToolBar
  << mMapNavToolBar
  << mAttributesToolBar
//  << mPluginToolBar
//  << mHelpToolBar
//  << mRasterToolBar
  << mVectorToolBar;
//  << mDatabaseToolBar
//  << mWebToolBar
//  << mLabelToolBar;

  QList<QAction*> toolbarMenuActions;
  // Set action names so that they can be used in customization
  Q_FOREACH ( QToolBar *toolBar, toolbarMenuToolBars )
  {
    toolBar->toggleViewAction()->setObjectName( "mActionToggle" + toolBar->objectName().mid( 1 ) );
    toolbarMenuActions << toolBar->toggleViewAction();
  }

  // sort actions in toolbar menu
  qSort( toolbarMenuActions.begin(), toolbarMenuActions.end(), cmpByText_ );

  mToolbarMenu->addActions( toolbarMenuActions );

  // selection tool button

  QToolButton *bt = new QToolButton( mAttributesToolBar );
  bt->setPopupMode( QToolButton::MenuButtonPopup );
  QList<QAction*> selectActions;
  selectActions << mActionSelectByExpression << mActionSelectAll
  << mActionInvertSelection;
  bt->addActions( selectActions );
  bt->setDefaultAction( mActionSelectByExpression );
  QAction* selectionAction = mAttributesToolBar->insertWidget( mActionDeselectAll, bt );
  selectionAction->setObjectName( "ActionSelection" );

  // select tool button

  bt = new QToolButton( mAttributesToolBar );
  bt->setPopupMode( QToolButton::MenuButtonPopup );
  QList<QAction*> selectionActions;
  selectionActions << mActionSelectFeatures << mActionSelectPolygon
  << mActionSelectFreehand << mActionSelectRadius;
  bt->addActions( selectionActions );

  QAction* defSelectAction = mActionSelectFeatures;
  switch ( settings.value( "/UI/selectTool", 0 ).toInt() )
  {
    case 0:
      defSelectAction = mActionSelectFeatures;
      break;
    case 1:
      defSelectAction = mActionSelectFeatures;
      break;
    case 2:
      defSelectAction = mActionSelectRadius;
      break;
    case 3:
      defSelectAction = mActionSelectPolygon;
      break;
    case 4:
      defSelectAction = mActionSelectFreehand;
      break;
  }
  bt->setDefaultAction( defSelectAction );
  QAction* selectAction = mAttributesToolBar->insertWidget( selectionAction, bt );
  selectAction->setObjectName( "ActionSelect" );
  connect( bt, SIGNAL( triggered( QAction * ) ), this, SLOT( toolButtonActionTriggered( QAction * ) ) );

  // feature action tool button

  bt = new QToolButton( mAttributesToolBar );
  bt->setPopupMode( QToolButton::MenuButtonPopup );
  bt->setDefaultAction( mActionFeatureAction );
  mFeatureActionMenu = new QMenu( bt );
  connect( mFeatureActionMenu, SIGNAL( triggered( QAction * ) ), this, SLOT( updateDefaultFeatureAction( QAction * ) ) );
  connect( mFeatureActionMenu, SIGNAL( aboutToShow() ), this, SLOT( refreshFeatureActions() ) );
  bt->setMenu( mFeatureActionMenu );
  QAction* featureActionAction = mAttributesToolBar->insertWidget( selectAction, bt );
  featureActionAction->setObjectName( "ActionFeatureAction" );

  // measure tool button

  bt = new QToolButton( mAttributesToolBar );
  bt->setPopupMode( QToolButton::MenuButtonPopup );
  bt->addAction( mActionMeasure );
  bt->addAction( mActionMeasureArea );
  bt->addAction( mActionMeasureAngle );

  QAction* defMeasureAction = mActionMeasure;
  switch ( settings.value( "/UI/measureTool", 0 ).toInt() )
  {
    case 0:
      defMeasureAction = mActionMeasure;
      break;
    case 1:
      defMeasureAction = mActionMeasureArea;
      break;
    case 2:
      defMeasureAction = mActionMeasureAngle;
      break;
  }
  bt->setDefaultAction( defMeasureAction );
  QAction* measureAction = mAttributesToolBar->insertWidget( mActionMapTips, bt );
  measureAction->setObjectName( "ActionMeasure" );
  connect( bt, SIGNAL( triggered( QAction * ) ), this, SLOT( toolButtonActionTriggered( QAction * ) ) );

  // annotation tool button

//  bt = new QToolButton();
//  bt->setPopupMode( QToolButton::MenuButtonPopup );
//  bt->addAction( mActionTextAnnotation );
//  bt->addAction( mActionFormAnnotation );
//  bt->addAction( mActionHtmlAnnotation );
//  bt->addAction( mActionSvgAnnotation );
//  bt->addAction( mActionAnnotation );

//  QAction* defAnnotationAction = mActionTextAnnotation;
//  switch ( settings.value( "/UI/annotationTool", 0 ).toInt() )
//  {
//    case 0:
//      defAnnotationAction = mActionTextAnnotation;
//      break;
//    case 1:
//      defAnnotationAction = mActionFormAnnotation;
//      break;
//    case 2:
//      defAnnotationAction = mActionHtmlAnnotation;
//      break;
//    case 3:
//      defAnnotationAction = mActionSvgAnnotation;
//      break;
//    case 4:
//      defAnnotationAction =  mActionAnnotation;
//      break;

//  }
//  bt->setDefaultAction( defAnnotationAction );
//  QAction* annotationAction = mAttributesToolBar->addWidget( bt );
//  annotationAction->setObjectName( "ActionAnnotation" );
//  connect( bt, SIGNAL( triggered( QAction * ) ), this, SLOT( toolButtonActionTriggered( QAction * ) ) );

  // vector layer edits tool buttons
//  QToolButton* tbAllEdits = qobject_cast<QToolButton *>( mDigitizeToolBar->widgetForAction( mActionAllEdits ) );
//  tbAllEdits->setPopupMode( QToolButton::InstantPopup );

  // new layer tool button

//  bt = new QToolButton();
//  bt->setPopupMode( QToolButton::MenuButtonPopup );
//  bt->addAction( mActionNewVectorLayer );
//  bt->addAction( mActionNewSpatiaLiteLayer );
//  bt->addAction( mActionNewMemoryLayer );

//  QAction* defNewLayerAction = mActionNewVectorLayer;
//  switch ( settings.value( "/UI/defaultNewLayer", 1 ).toInt() )
//  {
//    case 0:
//      defNewLayerAction = mActionNewSpatiaLiteLayer;
//      break;
//    case 1:
//      defNewLayerAction = mActionNewVectorLayer;
//      break;
//    case 2:
//      defNewLayerAction = mActionNewMemoryLayer;
//      break;
//  }
//  bt->setDefaultAction( defNewLayerAction );
//  QAction* newLayerAction = mLayerToolBar->addWidget( bt );

//  newLayerAction->setObjectName( "ActionNewLayer" );
//  connect( bt, SIGNAL( triggered( QAction * ) ), this, SLOT( toolButtonActionTriggered( QAction * ) ) );

//  //circular string digitize tool button
//  QToolButton* tbAddCircularString = new QToolButton( mDigitizeToolBar );
//  tbAddCircularString->setPopupMode( QToolButton::MenuButtonPopup );
//  tbAddCircularString->addAction( mActionCircularStringCurvePoint );
//  tbAddCircularString->addAction( mActionCircularStringRadius );
//  tbAddCircularString->setDefaultAction( mActionCircularStringCurvePoint );
//  connect( tbAddCircularString, SIGNAL( triggered( QAction * ) ), this, SLOT( toolButtonActionTriggered( QAction * ) ) );
//  mDigitizeToolBar->insertWidget( mActionMoveFeature, tbAddCircularString );

//  // Cad toolbar
//  mAdvancedDigitizeToolBar->insertAction( mActionEnableTracing, mAdvancedDigitizingDockWidget->enableAction() );

//  mTracer = new QgsMapCanvasTracer( canvas_xy, messageBar() );
//  mTracer->setActionEnableTracing( mActionEnableTracing );
}

void MainWindow::createStatusBar2()
{
  //remove borders from children under Windows
  statusBar()->setStyleSheet( "QStatusBar::item {border: none;}" );

  // Add a panel to the status bar for the scale, coords and progress
  // And also rendering suppression checkbox
  mProgressBar = new QProgressBar( statusBar() );
  mProgressBar->setObjectName( "mProgressBar" );
  mProgressBar->setMaximumWidth( 100 );
  mProgressBar->hide();
  mProgressBar->setWhatsThis( tr( "Progress bar that displays the status "
                                  "of rendering layers and other time-intensive operations" ) );
  statusBar()->addPermanentWidget( mProgressBar, 1 );

  connect( canvas_xy, SIGNAL( renderStarting() ), this, SLOT( canvasRefreshStarted() ) );
  connect( canvas_xy, SIGNAL( mapCanvasRefreshed() ), this, SLOT( canvasRefreshFinished() ) );

  // Bumped the font up one point size since 8 was too
  // small on some platforms. A point size of 9 still provides
  // plenty of display space on 1024x768 resolutions
  QFont myFont( "Arial", 9 );
  statusBar()->setFont( myFont );

//  //coords status bar widget
//  mCoordsEdit = new QgsStatusBarCoordinatesWidget( statusBar() );
//  mCoordsEdit->setMapCanvas( canvas_xy );
//  mCoordsEdit->setFont( myFont );
//  statusBar()->addPermanentWidget( mCoordsEdit, 0 );

  // add a label to show current scale
  mScaleLabel = new QLabel( QString(), statusBar() );
  mScaleLabel->setObjectName( "mScaleLable" );
  mScaleLabel->setFont( myFont );
  mScaleLabel->setMinimumWidth( 10 );
  //mScaleLabel->setMaximumHeight( 20 );
  mScaleLabel->setMargin( 3 );
  mScaleLabel->setAlignment( Qt::AlignCenter );
  mScaleLabel->setFrameStyle( QFrame::NoFrame );
  mScaleLabel->setText( tr( "Scale" ) );
  mScaleLabel->setToolTip( tr( "Current map scale" ) );
  statusBar()->addPermanentWidget( mScaleLabel, 0 );

//  mScaleEdit = new QgsScaleComboBox( statusBar() );
//  mScaleEdit->setObjectName( "mScaleEdit" );
//  mScaleEdit->setFont( myFont );
//  // seems setFont() change font only for popup not for line edit,
//  // so we need to set font for it separately
//  mScaleEdit->lineEdit()->setFont( myFont );
//  mScaleEdit->setMinimumWidth( 10 );
//  mScaleEdit->setContentsMargins( 0, 0, 0, 0 );
//  mScaleEdit->setWhatsThis( tr( "Displays the current map scale" ) );
//  mScaleEdit->setToolTip( tr( "Current map scale (formatted as x:y)" ) );

//  statusBar()->addPermanentWidget( mScaleEdit, 0 );
//  connect( mScaleEdit, SIGNAL( scaleChanged() ), this, SLOT( userScale() ) );

  if ( QgsMapCanvas::rotationEnabled() )
  {
    // add a widget to show/set current rotation
    mRotationLabel = new QLabel( QString(), statusBar() );
    mRotationLabel->setObjectName( "mRotationLabel" );
    mRotationLabel->setFont( myFont );
    mRotationLabel->setMinimumWidth( 10 );
    //mRotationLabel->setMaximumHeight( 20 );
    mRotationLabel->setMargin( 3 );
    mRotationLabel->setAlignment( Qt::AlignCenter );
    mRotationLabel->setFrameStyle( QFrame::NoFrame );
    mRotationLabel->setText( tr( "Rotation" ) );
    mRotationLabel->setToolTip( tr( "Current clockwise map rotation in degrees" ) );
    statusBar()->addPermanentWidget( mRotationLabel, 0 );

//    mRotationEdit = new QgsDoubleSpinBox( statusBar() );
//    mRotationEdit->setObjectName( "mRotationEdit" );
//    mRotationEdit->setClearValue( 0.0 );
//    mRotationEdit->setKeyboardTracking( false );
//    mRotationEdit->setMaximumWidth( 120 );
//    mRotationEdit->setDecimals( 1 );
//    mRotationEdit->setRange( -180.0, 180.0 );
//    mRotationEdit->setWrapping( true );
//    mRotationEdit->setSingleStep( 5.0 );
//    mRotationEdit->setFont( myFont );
//    mRotationEdit->setWhatsThis( tr( "Shows the current map clockwise rotation "
//                                     "in degrees. It also allows editing to set "
//                                     "the rotation" ) );
//    mRotationEdit->setToolTip( tr( "Current clockwise map rotation in degrees" ) );
//    statusBar()->addPermanentWidget( mRotationEdit, 0 );
//    connect( mRotationEdit, SIGNAL( valueChanged( double ) ), this, SLOT( userRotation() ) );

//    showRotation();
  }

  // render suppression status bar widget
  mRenderSuppressionCBox = new QCheckBox( tr( "Render" ), statusBar() );
  mRenderSuppressionCBox->setObjectName( "mRenderSuppressionCBox" );
  mRenderSuppressionCBox->setChecked( true );
  mRenderSuppressionCBox->setFont( myFont );
  mRenderSuppressionCBox->setWhatsThis( tr( "When checked, the map layers "
                                        "are rendered in response to map navigation commands and other "
                                        "events. When not checked, no rendering is done. This allows you "
                                        "to add a large number of layers and symbolize them before rendering." ) );
  mRenderSuppressionCBox->setToolTip( tr( "Toggle map rendering" ) );
  statusBar()->addPermanentWidget( mRenderSuppressionCBox, 0 );
  // On the fly projection status bar icon
  // Changed this to a tool button since a QPushButton is
  // sculpted on OS X and the icon is never displayed [gsherman]
//  mOnTheFlyProjectionStatusButton = new QToolButton( statusBar() );
//  mOnTheFlyProjectionStatusButton->setAutoRaise( true );
//  mOnTheFlyProjectionStatusButton->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
//  mOnTheFlyProjectionStatusButton->setObjectName( "mOntheFlyProjectionStatusButton" );
//  // Maintain uniform widget height in status bar by setting button height same as labels
//  // For Qt/Mac 3.3, the default toolbutton height is 30 and labels were expanding to match
//  mOnTheFlyProjectionStatusButton->setMaximumHeight( mScaleLabel->height() );
//  mOnTheFlyProjectionStatusButton->setIcon( QgsApplication::getThemeIcon( "mIconProjectionEnabled.png" ) );
//  mOnTheFlyProjectionStatusButton->setWhatsThis( tr( "This icon shows whether "
//      "on the fly coordinate reference system transformation is enabled or not. "
//      "Click the icon to bring up "
//      "the project properties dialog to alter this behaviour." ) );
//  mOnTheFlyProjectionStatusButton->setToolTip( tr( "CRS status - Click "
//      "to open coordinate reference system dialog" ) );
//  connect( mOnTheFlyProjectionStatusButton, SIGNAL( clicked() ),
//           this, SLOT( projectPropertiesProjections() ) );//bring up the project props dialog when clicked
//  statusBar()->addPermanentWidget( mOnTheFlyProjectionStatusButton, 0 );
//  statusBar()->showMessage( tr( "Ready" ) );

//  mMessageButton = new QToolButton( statusBar() );
//  mMessageButton->setAutoRaise( true );
//  mMessageButton->setIcon( QgsApplication::getThemeIcon( "/mMessageLogRead.svg" ) );
//  mMessageButton->setToolTip( tr( "Messages" ) );
//  mMessageButton->setWhatsThis( tr( "Messages" ) );
//  mMessageButton->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
//  mMessageButton->setObjectName( "mMessageLogViewerButton" );
//  mMessageButton->setMaximumHeight( mScaleLabel->height() );
//  mMessageButton->setCheckable( true );
//  statusBar()->addPermanentWidget( mMessageButton, 0 );
}

void MainWindow::setTheme( const QString& theThemeName )
{
  /*****************************************************************
  // Init the toolbar icons by setting the icon for each action.
  // All toolbar/menu items must be a QAction in order for this
  // to work.
  //
  // When new toolbar/menu QAction objects are added to the interface,
  // add an entry below to set the icon
  //
  // PNG names must match those defined for the default theme. The
  // default theme is installed in <prefix>/share/qgis/themes/default.
  //
  // New core themes can be added by creating a subdirectory under src/themes
  // and modifying the appropriate CMakeLists.txt files. User contributed themes
  // will be installed directly into <prefix>/share/qgis/themes/<themedir>.
  //
  // Themes can be selected from the preferences dialog. The dialog parses
  // the themes directory and builds a list of themes (ie subdirectories)
  // for the user to choose from.
  //
  */
  QgsApplication::setUITheme( theThemeName );
//  //QgsDebugMsg("Setting theme to \n" + theThemeName);
//  mActionNewProject->setIcon( QgsApplication::getThemeIcon( "/mActionFileNew.svg" ) );
//  mActionOpenProject->setIcon( QgsApplication::getThemeIcon( "/mActionFileOpen.svg" ) );
//  mActionSaveProject->setIcon( QgsApplication::getThemeIcon( "/mActionFileSave.svg" ) );
//  mActionSaveProjectAs->setIcon( QgsApplication::getThemeIcon( "/mActionFileSaveAs.svg" ) );
//  mActionNewPrintComposer->setIcon( QgsApplication::getThemeIcon( "/mActionNewComposer.svg" ) );
//  mActionShowComposerManager->setIcon( QgsApplication::getThemeIcon( "/mActionComposerManager.svg" ) );
//  mActionSaveMapAsImage->setIcon( QgsApplication::getThemeIcon( "/mActionSaveMapAsImage.png" ) );
//  mActionExit->setIcon( QgsApplication::getThemeIcon( "/mActionFileExit.png" ) );
//  mActionAddOgrLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddOgrLayer.svg" ) );
//  mActionAddRasterLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddRasterLayer.svg" ) );
//#ifdef HAVE_POSTGRESQL
//  mActionAddPgLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddPostgisLayer.svg" ) );
//#endif
//  mActionNewSpatiaLiteLayer->setIcon( QgsApplication::getThemeIcon( "/mActionNewSpatiaLiteLayer.svg" ) );
//  mActionAddSpatiaLiteLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddSpatiaLiteLayer.svg" ) );
//#ifdef HAVE_MSSQL
//  mActionAddMssqlLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddMssqlLayer.svg" ) );
//#endif
//#ifdef HAVE_ORACLE
//  mActionAddOracleLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddOracleLayer.svg" ) );
//#endif
//  mActionRemoveLayer->setIcon( QgsApplication::getThemeIcon( "/mActionRemoveLayer.svg" ) );
//  mActionDuplicateLayer->setIcon( QgsApplication::getThemeIcon( "/mActionDuplicateLayer.svg" ) );
//  mActionSetLayerCRS->setIcon( QgsApplication::getThemeIcon( "/mActionSetLayerCRS.png" ) );
//  mActionSetProjectCRSFromLayer->setIcon( QgsApplication::getThemeIcon( "/mActionSetProjectCRSFromLayer.png" ) );
//  mActionNewVectorLayer->setIcon( QgsApplication::getThemeIcon( "/mActionNewVectorLayer.svg" ) );
//  mActionNewMemoryLayer->setIcon( QgsApplication::getThemeIcon( "/mActionCreateMemory.svg" ) );
//  mActionAddAllToOverview->setIcon( QgsApplication::getThemeIcon( "/mActionAddAllToOverview.svg" ) );
  mActionHideAllLayers->setIcon( QgsApplication::getThemeIcon( "/mActionHideAllLayers.png" ) );
  mActionShowAllLayers->setIcon( QgsApplication::getThemeIcon( "/mActionShowAllLayers.png" ) );
  mActionHideSelectedLayers->setIcon( QgsApplication::getThemeIcon( "/mActionHideSelectedLayers.png" ) );
  mActionShowSelectedLayers->setIcon( QgsApplication::getThemeIcon( "/mActionShowSelectedLayers.png" ) );
//  mActionRemoveAllFromOverview->setIcon( QgsApplication::getThemeIcon( "/mActionRemoveAllFromOverview.svg" ) );
  mActionToggleFullScreen->setIcon( QgsApplication::getThemeIcon( "/mActionToggleFullScreen.png" ) );
//  mActionProjectProperties->setIcon( QgsApplication::getThemeIcon( "/mActionProjectProperties.png" ) );
//  mActionManagePlugins->setIcon( QgsApplication::getThemeIcon( "/mActionShowPluginManager.svg" ) );
//  mActionShowPythonDialog->setIcon( QgsApplication::getThemeIcon( "console/iconRunConsole.png" ) );
//  mActionCheckQgisVersion->setIcon( QgsApplication::getThemeIcon( "/mActionCheckQgisVersion.png" ) );
//  mActionOptions->setIcon( QgsApplication::getThemeIcon( "/mActionOptions.svg" ) );
//  mActionConfigureShortcuts->setIcon( QgsApplication::getThemeIcon( "/mActionOptions.svg" ) );
//  mActionCustomization->setIcon( QgsApplication::getThemeIcon( "/mActionOptions.svg" ) );
//  mActionHelpContents->setIcon( QgsApplication::getThemeIcon( "/mActionHelpContents.svg" ) );
//  mActionLocalHistogramStretch->setIcon( QgsApplication::getThemeIcon( "/mActionLocalHistogramStretch.png" ) );
//  mActionFullHistogramStretch->setIcon( QgsApplication::getThemeIcon( "/mActionFullHistogramStretch.png" ) );
//  mActionIncreaseBrightness->setIcon( QgsApplication::getThemeIcon( "/mActionIncreaseBrightness.svg" ) );
//  mActionDecreaseBrightness->setIcon( QgsApplication::getThemeIcon( "/mActionDecreaseBrightness.svg" ) );
//  mActionIncreaseContrast->setIcon( QgsApplication::getThemeIcon( "/mActionIncreaseContrast.svg" ) );
//  mActionDecreaseContrast->setIcon( QgsApplication::getThemeIcon( "/mActionDecreaseContrast.svg" ) );
  mActionZoomActualSize->setIcon( QgsApplication::getThemeIcon( "/mActionZoomNative.png" ) );
//  mActionQgisHomePage->setIcon( QgsApplication::getThemeIcon( "/mActionQgisHomePage.png" ) );
//  mActionAbout->setIcon( QgsApplication::getThemeIcon( "/mActionHelpAbout.png" ) );
//  mActionSponsors->setIcon( QgsApplication::getThemeIcon( "/mActionHelpSponsors.png" ) );
  mActionDraw->setIcon( QgsApplication::getThemeIcon( "/mActionDraw.svg" ) );
  mActionToggleEditing->setIcon( QgsApplication::getThemeIcon( "/mActionToggleEditing.svg" ) );
  mActionSaveLayerEdits->setIcon( QgsApplication::getThemeIcon( "/mActionSaveAllEdits.svg" ) );
  mActionAllEdits->setIcon( QgsApplication::getThemeIcon( "/mActionAllEdits.svg" ) );
  mActionSaveEdits->setIcon( QgsApplication::getThemeIcon( "/mActionSaveEdits.svg" ) );
  mActionSaveAllEdits->setIcon( QgsApplication::getThemeIcon( "/mActionSaveAllEdits.svg" ) );
  mActionRollbackEdits->setIcon( QgsApplication::getThemeIcon( "/mActionRollbackEdits.svg" ) );
  mActionRollbackAllEdits->setIcon( QgsApplication::getThemeIcon( "/mActionRollbackAllEdits.svg" ) );
  mActionCancelEdits->setIcon( QgsApplication::getThemeIcon( "/mActionCancelEdits.svg" ) );
  mActionCancelAllEdits->setIcon( QgsApplication::getThemeIcon( "/mActionCancelAllEdits.svg" ) );
//  mActionCutFeatures->setIcon( QgsApplication::getThemeIcon( "/mActionEditCut.png" ) );
//  mActionCopyFeatures->setIcon( QgsApplication::getThemeIcon( "/mActionEditCopy.png" ) );
//  mActionPasteFeatures->setIcon( QgsApplication::getThemeIcon( "/mActionEditPaste.png" ) );
//  mActionAddFeature->setIcon( QgsApplication::getThemeIcon( "/mActionCapturePoint.png" ) );
//  mActionMoveFeature->setIcon( QgsApplication::getThemeIcon( "/mActionMoveFeature.png" ) );
//  mActionRotateFeature->setIcon( QgsApplication::getThemeIcon( "/mActionRotateFeature.png" ) );
//  mActionReshapeFeatures->setIcon( QgsApplication::getThemeIcon( "/mActionReshape.png" ) );
//  mActionSplitFeatures->setIcon( QgsApplication::getThemeIcon( "/mActionSplitFeatures.svg" ) );
//  mActionSplitParts->setIcon( QgsApplication::getThemeIcon( "/mActionSplitParts.svg" ) );
  mActionDeleteSelected->setIcon( QgsApplication::getThemeIcon( "/mActionDeleteSelected.svg" ) );
//  mActionNodeTool->setIcon( QgsApplication::getThemeIcon( "/mActionNodeTool.png" ) );
//  mActionSimplifyFeature->setIcon( QgsApplication::getThemeIcon( "/mActionSimplify.png" ) );
//  mActionUndo->setIcon( QgsApplication::getThemeIcon( "/mActionUndo.png" ) );
//  mActionRedo->setIcon( QgsApplication::getThemeIcon( "/mActionRedo.png" ) );
//  mActionAddRing->setIcon( QgsApplication::getThemeIcon( "/mActionAddRing.png" ) );
//  mActionFillRing->setIcon( QgsApplication::getThemeIcon( "/mActionFillRing.svg" ) );
//  mActionAddPart->setIcon( QgsApplication::getThemeIcon( "/mActionAddPart.png" ) );
//  mActionDeleteRing->setIcon( QgsApplication::getThemeIcon( "/mActionDeleteRing.png" ) );
//  mActionDeletePart->setIcon( QgsApplication::getThemeIcon( "/mActionDeletePart.png" ) );
//  mActionMergeFeatures->setIcon( QgsApplication::getThemeIcon( "/mActionMergeFeatures.png" ) );
//  mActionOffsetCurve->setIcon( QgsApplication::getThemeIcon( "/mActionOffsetCurve.png" ) );
//  mActionMergeFeatureAttributes->setIcon( QgsApplication::getThemeIcon( "/mActionMergeFeatureAttributes.png" ) );
//  mActionRotatePointSymbols->setIcon( QgsApplication::getThemeIcon( "mActionRotatePointSymbols.png" ) );
  mActionZoomIn->setIcon( QgsApplication::getThemeIcon( "/mActionZoomIn.svg" ) );
  mActionZoomOut->setIcon( QgsApplication::getThemeIcon( "/mActionZoomOut.svg" ) );
  mActionZoomFullExtent->setIcon( QgsApplication::getThemeIcon( "/mActionZoomFullExtent.svg" ) );
  mActionZoomToSelected->setIcon( QgsApplication::getThemeIcon( "/mActionZoomToSelected.svg" ) );
//  mActionShowRasterCalculator->setIcon( QgsApplication::getThemeIcon( "/mActionShowRasterCalculator.png" ) );
//#ifdef HAVE_TOUCH
//  mActionTouch->setIcon( QgsApplication::getThemeIcon( "/mActionTouch.svg" ) );
//#endif
  mActionPan->setIcon( QgsApplication::getThemeIcon( "/mActionPan.svg" ) );
  mActionPanToSelected->setIcon( QgsApplication::getThemeIcon( "/mActionPanToSelected.svg" ) );
  mActionZoomLast->setIcon( QgsApplication::getThemeIcon( "/mActionZoomLast.svg" ) );
  mActionZoomNext->setIcon( QgsApplication::getThemeIcon( "/mActionZoomNext.svg" ) );
  mActionZoomToLayer->setIcon( QgsApplication::getThemeIcon( "/mActionZoomToLayer.svg" ) );
  mActionZoomActualSize->setIcon( QgsApplication::getThemeIcon( "/mActionZoomActual.svg" ) );
  mActionIdentify->setIcon( QgsApplication::getThemeIcon( "/mActionIdentify.svg" ) );
  mActionFeatureAction->setIcon( QgsApplication::getThemeIcon( "/mAction.svg" ) );
  mActionSelectFeatures->setIcon( QgsApplication::getThemeIcon( "/mActionSelectRectangle.svg" ) );
  mActionSelectPolygon->setIcon( QgsApplication::getThemeIcon( "/mActionSelectPolygon.svg" ) );
  mActionSelectFreehand->setIcon( QgsApplication::getThemeIcon( "/mActionSelectFreehand.svg" ) );
  mActionSelectRadius->setIcon( QgsApplication::getThemeIcon( "/mActionSelectRadius.svg" ) );
  mActionDeselectAll->setIcon( QgsApplication::getThemeIcon( "/mActionDeselectAll.svg" ) );
  mActionSelectAll->setIcon( QgsApplication::getThemeIcon( "/mActionSelectAll.svg" ) );
  mActionInvertSelection->setIcon( QgsApplication::getThemeIcon( "/mActionInvertSelection.svg" ) );
  mActionSelectByExpression->setIcon( QgsApplication::getThemeIcon( "/mIconExpressionSelect.svg" ) );
  mActionOpenTable->setIcon( QgsApplication::getThemeIcon( "/mActionOpenTable.svg" ) );
  mActionOpenFieldCalc->setIcon( QgsApplication::getThemeIcon( "/mActionCalculateField.png" ) );
  mActionMeasure->setIcon( QgsApplication::getThemeIcon( "/mActionMeasure.png" ) );
  mActionMeasureArea->setIcon( QgsApplication::getThemeIcon( "/mActionMeasureArea.png" ) );
  mActionMeasureAngle->setIcon( QgsApplication::getThemeIcon( "/mActionMeasureAngle.png" ) );
  mActionMapTips->setIcon( QgsApplication::getThemeIcon( "/mActionMapTips.png" ) );
  mActionShowBookmarks->setIcon( QgsApplication::getThemeIcon( "/mActionShowBookmarks.png" ) );
  mActionNewBookmark->setIcon( QgsApplication::getThemeIcon( "/mActionNewBookmark.png" ) );
//  mActionCustomProjection->setIcon( QgsApplication::getThemeIcon( "/mActionCustomProjection.svg" ) );
//  mActionAddWmsLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddWmsLayer.svg" ) );
//  mActionAddWcsLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddWcsLayer.svg" ) );
//  mActionAddWfsLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddWfsLayer.svg" ) );
//  mActionAddToOverview->setIcon( QgsApplication::getThemeIcon( "/mActionInOverview.svg" ) );
//  mActionAnnotation->setIcon( QgsApplication::getThemeIcon( "/mActionAnnotation.png" ) );
//  mActionFormAnnotation->setIcon( QgsApplication::getThemeIcon( "/mActionFormAnnotation.png" ) );
//  mActionHtmlAnnotation->setIcon( QgsApplication::getThemeIcon( "/mActionFormAnnotation.png" ) );
//  mActionTextAnnotation->setIcon( QgsApplication::getThemeIcon( "/mActionTextAnnotation.png" ) );
//  mActionLabeling->setIcon( QgsApplication::getThemeIcon( "/mActionLabeling.png" ) );
//  mActionShowPinnedLabels->setIcon( QgsApplication::getThemeIcon( "/mActionShowPinnedLabels.svg" ) );
//  mActionPinLabels->setIcon( QgsApplication::getThemeIcon( "/mActionPinLabels.svg" ) );
//  mActionShowHideLabels->setIcon( QgsApplication::getThemeIcon( "/mActionShowHideLabels.svg" ) );
//  mActionMoveLabel->setIcon( QgsApplication::getThemeIcon( "/mActionMoveLabel.png" ) );
//  mActionRotateLabel->setIcon( QgsApplication::getThemeIcon( "/mActionRotateLabel.svg" ) );
//  mActionChangeLabelProperties->setIcon( QgsApplication::getThemeIcon( "/mActionChangeLabelProperties.png" ) );
//  mActionDecorationCopyright->setIcon( QgsApplication::getThemeIcon( "/copyright_label.png" ) );
//  mActionDecorationNorthArrow->setIcon( QgsApplication::getThemeIcon( "/north_arrow.png" ) );
//  mActionDecorationScaleBar->setIcon( QgsApplication::getThemeIcon( "/scale_bar.png" ) );
//  mActionDecorationGrid->setIcon( QgsApplication::getThemeIcon( "/transformed.png" ) );

//  //change themes of all composers
//  QSet<QgsComposer*>::const_iterator composerIt = mPrintComposers.constBegin();
//  for ( ; composerIt != mPrintComposers.constEnd(); ++composerIt )
//  {
//    ( *composerIt )->setupTheme();
//  }

//  emit currentThemeChanged( theThemeName );
}

void MainWindow::setupConnections()
{
  // connect the "cleanup" slot
  connect( qApp, SIGNAL( aboutToQuit() ), this, SLOT( saveWindowState() ) );

  // signal when mouse moved over window (coords display in status bar)
  connect( canvas_xy, SIGNAL( xyCoordinates( const QgsPoint & ) ),
           this, SLOT( saveLastMousePosition( const QgsPoint & ) ) );
  connect( canvas_xy, SIGNAL( extentsChanged() ),
           this, SLOT( extentChanged() ) );
  connect( canvas_xy, SIGNAL( scaleChanged( double ) ),
           this, SLOT( showScale( double ) ) );
  connect( canvas_xy, SIGNAL( rotationChanged( double ) ),
           this, SLOT( showRotation() ) );
  connect( canvas_xy, SIGNAL( scaleChanged( double ) ),
           this, SLOT( updateMouseCoordinatePrecision() ) );
  connect( canvas_xy, SIGNAL( mapToolSet( QgsMapTool *, QgsMapTool * ) ),
           this, SLOT( mapToolChanged( QgsMapTool *, QgsMapTool * ) ) );
  connect( canvas_xy, SIGNAL( selectionChanged( QgsMapLayer * ) ),
           this, SLOT( selectionChanged( QgsMapLayer * ) ) );
  connect( canvas_xy, SIGNAL( extentsChanged() ),
           this, SLOT( markDirty() ) );
  connect( canvas_xy, SIGNAL( layersChanged() ),
           this, SLOT( markDirty() ) );

  connect( canvas_xy, SIGNAL( zoomLastStatusChanged( bool ) ),
           mActionZoomLast, SLOT( setEnabled( bool ) ) );
  connect( canvas_xy, SIGNAL( zoomNextStatusChanged( bool ) ),
           mActionZoomNext, SLOT( setEnabled( bool ) ) );
  connect( mRenderSuppressionCBox, SIGNAL( toggled( bool ) ),
           canvas_xy, SLOT( setRenderFlag( bool ) ) );

  connect( canvas_xy, SIGNAL( destinationCrsChanged() ),
           this, SLOT( reprojectAnnotations() ) );

  // connect MapCanvas keyPress event so we can check if selected feature collection must be deleted
  connect( canvas_xy, SIGNAL( keyPressed( QKeyEvent * ) ),
           this, SLOT( mapCanvas_keyPressed( QKeyEvent * ) ) );

  // connect renderer
  connect( canvas_xy, SIGNAL( hasCrsTransformEnabledChanged( bool ) ),
           this, SLOT( hasCrsTransformEnabled( bool ) ) );
  connect( canvas_xy, SIGNAL( destinationCrsChanged() ),
           this, SLOT( destinationCrsChanged() ) );

//  // connect legend signals
//  connect( mLayerTreeView, SIGNAL( currentLayerChanged( QgsMapLayer * ) ),
//           this, SLOT( activateDeactivateLayerRelatedActions( QgsMapLayer * ) ) );
//  connect( mLayerTreeView->selectionModel(), SIGNAL( selectionChanged( QItemSelection, QItemSelection ) ),
//           this, SLOT( legendLayerSelectionChanged() ) );
//  connect( mLayerTreeView->layerTreeModel()->rootGroup(), SIGNAL( addedChildren( QgsLayerTreeNode*, int, int ) ),
//           this, SLOT( markDirty() ) );
//  connect( mLayerTreeView->layerTreeModel()->rootGroup(), SIGNAL( addedChildren( QgsLayerTreeNode*, int, int ) ),
//           this, SLOT( updateNewLayerInsertionPoint() ) );
//  connect( mLayerTreeView->layerTreeModel()->rootGroup(), SIGNAL( removedChildren( QgsLayerTreeNode*, int, int ) ),
//           this, SLOT( markDirty() ) );
//  connect( mLayerTreeView->layerTreeModel()->rootGroup(), SIGNAL( removedChildren( QgsLayerTreeNode*, int, int ) ),
//           this, SLOT( updateNewLayerInsertionPoint() ) );
//  connect( mLayerTreeView->layerTreeModel()->rootGroup(), SIGNAL( visibilityChanged( QgsLayerTreeNode*, Qt::CheckState ) ),
//           this, SLOT( markDirty() ) );
//  connect( mLayerTreeView->layerTreeModel()->rootGroup(), SIGNAL( customPropertyChanged( QgsLayerTreeNode*, QString ) ),
//           this, SLOT( markDirty() ) );

  // connect map layer registry
  connect( QgsMapLayerRegistry::instance(), SIGNAL( layersAdded( QList<QgsMapLayer *> ) ),
           this, SLOT( layersWereAdded( QList<QgsMapLayer *> ) ) );
  connect( QgsMapLayerRegistry::instance(),
           SIGNAL( layersWillBeRemoved( QStringList ) ),
           this, SLOT( removingLayers( QStringList ) ) );

  // connect initialization signal
  connect( this, SIGNAL( initializationCompleted() ),
           this, SLOT( fileOpenAfterLaunch() ) );

  // Connect warning dialog from project reading
  connect( QgsProject::instance(), SIGNAL( oldProjectVersionWarning( QString ) ),
           this, SLOT( oldProjectVersionWarning( QString ) ) );
  connect( QgsProject::instance(), SIGNAL( layerLoaded( int, int ) ),
           this, SLOT( showProgress( int, int ) ) );
  connect( QgsProject::instance(), SIGNAL( loadingLayer( QString ) ),
           this, SLOT( showStatusMessage( QString ) ) );
  connect( QgsProject::instance(), SIGNAL( readProject( const QDomDocument & ) ),
           this, SLOT( readProject( const QDomDocument & ) ) );
  connect( QgsProject::instance(), SIGNAL( writeProject( QDomDocument & ) ),
           this, SLOT( writeProject( QDomDocument & ) ) );
  connect( QgsProject::instance(), SIGNAL( writeProject( QDomDocument& ) ),
           this, SLOT( writeAnnotationItemsToProject( QDomDocument& ) ) );

  connect( QgsProject::instance(), SIGNAL( readProject( const QDomDocument & ) ), this, SLOT( loadComposersFromProject( const QDomDocument& ) ) );
  connect( QgsProject::instance(), SIGNAL( readProject( const QDomDocument & ) ), this, SLOT( loadAnnotationItemsFromProject( const QDomDocument& ) ) );

  connect( this, SIGNAL( projectRead() ),
           this, SLOT( fileOpenedOKAfterLaunch() ) );

//  // connect preview modes actions
//  connect( mActionPreviewModeOff, SIGNAL( triggered() ), this, SLOT( disablePreviewMode() ) );
//  connect( mActionPreviewModeGrayscale, SIGNAL( triggered() ), this, SLOT( activateGrayscalePreview() ) );
//  connect( mActionPreviewModeMono, SIGNAL( triggered() ), this, SLOT( activateMonoPreview() ) );
//  connect( mActionPreviewProtanope, SIGNAL( triggered() ), this, SLOT( activateProtanopePreview() ) );
//  connect( mActionPreviewDeuteranope, SIGNAL( triggered() ), this, SLOT( activateDeuteranopePreview() ) );

//  // handle deprecated labels in project for QGIS 2.0
//  connect( this, SIGNAL( newProject() ),
//           this, SLOT( checkForDeprecatedLabelsInProject() ) );
//  connect( this, SIGNAL( projectRead() ),
//           this, SLOT( checkForDeprecatedLabelsInProject() ) );

//  // setup undo/redo actions
//  connect( mUndoWidget, SIGNAL( undoStackChanged() ), this, SLOT( updateUndoActions() ) );
}

void MainWindow::createCanvasTools()
{
  // create tools
  mMapTools.mZoomIn = new QgsMapToolZoom( canvas_xy, false /* zoomIn */ );
  mMapTools.mZoomIn->setAction( mActionZoomIn );
  mMapTools.mZoomOut = new QgsMapToolZoom( canvas_xy, true /* zoomOut */ );
  mMapTools.mZoomOut->setAction( mActionZoomOut );
  mMapTools.mPan = new QgsMapToolPan( canvas_xy );
  mMapTools.mPan->setAction( mActionPan );
//#ifdef HAVE_TOUCH
//  mMapTools.mTouch = new QgsMapToolTouch( canvas_xy );
//  mMapTools.mTouch->setAction( mActionTouch );
//#endif
//  mMapTools.mIdentify = new QgsMapToolIdentifyAction( canvas_xy );
//  mMapTools.mIdentify->setAction( mActionIdentify );
//  connect( mMapTools.mIdentify, SIGNAL( copyToClipboard( QgsFeatureStore & ) ),
//           this, SLOT( copyFeatures( QgsFeatureStore & ) ) );
//  mMapTools.mFeatureAction = new QgsMapToolFeatureAction( canvas_xy );
//  mMapTools.mFeatureAction->setAction( mActionFeatureAction );
//  mMapTools.mMeasureDist = new QgsMeasureTool( canvas_xy, false /* area */ );
//  mMapTools.mMeasureDist->setAction( mActionMeasure );
//  mMapTools.mMeasureArea = new QgsMeasureTool( canvas_xy, true /* area */ );
//  mMapTools.mMeasureArea->setAction( mActionMeasureArea );
//  mMapTools.mMeasureAngle = new QgsMapToolMeasureAngle( canvas_xy );
//  mMapTools.mMeasureAngle->setAction( mActionMeasureAngle );
//  mMapTools.mTextAnnotation = new QgsMapToolTextAnnotation( canvas_xy );
//  mMapTools.mTextAnnotation->setAction( mActionTextAnnotation );
//  mMapTools.mFormAnnotation = new QgsMapToolFormAnnotation( canvas_xy );
//  mMapTools.mFormAnnotation->setAction( mActionFormAnnotation );
//  mMapTools.mHtmlAnnotation = new QgsMapToolHtmlAnnotation( canvas_xy );
//  mMapTools.mHtmlAnnotation->setAction( mActionHtmlAnnotation );
//  mMapTools.mSvgAnnotation = new QgsMapToolSvgAnnotation( canvas_xy );
//  mMapTools.mSvgAnnotation->setAction( mActionSvgAnnotation );
//  mMapTools.mAnnotation = new QgsMapToolAnnotation( canvas_xy );
//  mMapTools.mAnnotation->setAction( mActionAnnotation );
//  mMapTools.mAddFeature = new QgsMapToolAddFeature( canvas_xy );
//  mMapTools.mAddFeature->setAction( mActionAddFeature );
//  mMapTools.mCircularStringCurvePoint = new QgsMapToolCircularStringCurvePoint( dynamic_cast<QgsMapToolAddFeature*>( mMapTools.mAddFeature ), canvas_xy );
//  mMapTools.mCircularStringCurvePoint->setAction( mActionCircularStringCurvePoint );
//  mMapTools.mCircularStringRadius = new QgsMapToolCircularStringRadius( dynamic_cast<QgsMapToolAddFeature*>( mMapTools.mAddFeature ), canvas_xy );
//  mMapTools.mCircularStringRadius->setAction( mActionCircularStringRadius );
//  mMapTools.mMoveFeature = new QgsMapToolMoveFeature( canvas_xy );
//  mMapTools.mMoveFeature->setAction( mActionMoveFeature );
//  mMapTools.mRotateFeature = new QgsMapToolRotateFeature( canvas_xy );
//  mMapTools.mRotateFeature->setAction( mActionRotateFeature );
////need at least geos 3.3 for OffsetCurve tool
//#if defined(GEOS_VERSION_MAJOR) && defined(GEOS_VERSION_MINOR) && \
//((GEOS_VERSION_MAJOR>3) || ((GEOS_VERSION_MAJOR==3) && (GEOS_VERSION_MINOR>=3)))
//  mMapTools.mOffsetCurve = new QgsMapToolOffsetCurve( canvas_xy );
//  mMapTools.mOffsetCurve->setAction( mActionOffsetCurve );
//#else
//  mAdvancedDigitizeToolBar->removeAction( mActionOffsetCurve );
//  mEditMenu->removeAction( mActionOffsetCurve );
//  mMapTools.mOffsetCurve = 0;
//#endif //GEOS_VERSION
//  mMapTools.mReshapeFeatures = new QgsMapToolReshape( canvas_xy );
//  mMapTools.mReshapeFeatures->setAction( mActionReshapeFeatures );
//  mMapTools.mSplitFeatures = new QgsMapToolSplitFeatures( canvas_xy );
//  mMapTools.mSplitFeatures->setAction( mActionSplitFeatures );
//  mMapTools.mSplitParts = new QgsMapToolSplitParts( canvas_xy );
//  mMapTools.mSplitParts->setAction( mActionSplitParts );
//  mMapTools.mSelectFeatures = new QgsMapToolSelectFeatures( canvas_xy );
//  mMapTools.mSelectFeatures->setAction( mActionSelectFeatures );
//  mMapTools.mSelectPolygon = new QgsMapToolSelectPolygon( canvas_xy );
//  mMapTools.mSelectPolygon->setAction( mActionSelectPolygon );
//  mMapTools.mSelectFreehand = new QgsMapToolSelectFreehand( canvas_xy );
//  mMapTools.mSelectFreehand->setAction( mActionSelectFreehand );
//  mMapTools.mSelectRadius = new QgsMapToolSelectRadius( canvas_xy );
//  mMapTools.mSelectRadius->setAction( mActionSelectRadius );
//  mMapTools.mAddRing = new QgsMapToolAddRing( canvas_xy );
//  mMapTools.mAddRing->setAction( mActionAddRing );
//  mMapTools.mFillRing = new QgsMapToolFillRing( canvas_xy );
//  mMapTools.mFillRing->setAction( mActionFillRing );
//  mMapTools.mAddPart = new QgsMapToolAddPart( canvas_xy );
//  mMapTools.mAddPart->setAction( mActionAddPart );
//  mMapTools.mSimplifyFeature = new QgsMapToolSimplify( canvas_xy );
//  mMapTools.mSimplifyFeature->setAction( mActionSimplifyFeature );
//  mMapTools.mDeleteRing = new QgsMapToolDeleteRing( canvas_xy );
//  mMapTools.mDeleteRing->setAction( mActionDeleteRing );
//  mMapTools.mDeletePart = new QgsMapToolDeletePart( canvas_xy );
//  mMapTools.mDeletePart->setAction( mActionDeletePart );
//  mMapTools.mNodeTool = new QgsMapToolNodeTool( canvas_xy );
//  mMapTools.mNodeTool->setAction( mActionNodeTool );
//  mMapTools.mRotatePointSymbolsTool = new QgsMapToolRotatePointSymbols( canvas_xy );
//  mMapTools.mRotatePointSymbolsTool->setAction( mActionRotatePointSymbols );
//  mMapTools.mPinLabels = new QgsMapToolPinLabels( canvas_xy );
//  mMapTools.mPinLabels->setAction( mActionPinLabels );
//  mMapTools.mShowHideLabels = new QgsMapToolShowHideLabels( canvas_xy );
//  mMapTools.mShowHideLabels->setAction( mActionShowHideLabels );
//  mMapTools.mMoveLabel = new QgsMapToolMoveLabel( canvas_xy );
//  mMapTools.mMoveLabel->setAction( mActionMoveLabel );

//  mMapTools.mRotateLabel = new QgsMapToolRotateLabel( canvas_xy );
//  mMapTools.mRotateLabel->setAction( mActionRotateLabel );
//  mMapTools.mChangeLabelProperties = new QgsMapToolChangeLabelProperties( canvas_xy );
//  mMapTools.mChangeLabelProperties->setAction( mActionChangeLabelProperties );
////ensure that non edit tool is initialized or we will get crashes in some situations
//  mNonEditMapTool = mMapTools.mPan;
}

void MainWindow::createOverview()
{
//  // overview canvas
//  mOverviewCanvas = new QgsMapOverviewCanvas( nullptr, canvas_xy );

//  //set canvas color to default
//  QSettings settings;
//  int red = settings.value( "/qgis/default_canvas_color_red", 255 ).toInt();
//  int green = settings.value( "/qgis/default_canvas_color_green", 255 ).toInt();
//  int blue = settings.value( "/qgis/default_canvas_color_blue", 255 ).toInt();
//  mOverviewCanvas->setBackgroundColor( QColor( red, green, blue ) );

//  mOverviewCanvas->setWhatsThis( tr( "Map overview canvas. This canvas can be used to display a locator map that shows the current extent of the map canvas. The current extent is shown as a red rectangle. Any layer on the map can be added to the overview canvas." ) );

//  mOverviewMapCursor = new QCursor( Qt::OpenHandCursor );
//  mOverviewCanvas->setCursor( *mOverviewMapCursor );
////  QVBoxLayout *myOverviewLayout = new QVBoxLayout;
////  myOverviewLayout->addWidget(overviewCanvas);
////  overviewFrame->setLayout(myOverviewLayout);
//  mOverviewDock = new QDockWidget( tr( "Overview Panel" ), this );
//  mOverviewDock->setObjectName( "Overview" );
//  mOverviewDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
//  mOverviewDock->setWidget( mOverviewCanvas );
//  addDockWidget( Qt::LeftDockWidgetArea, mOverviewDock );
//  // add to the Panel submenu
//  mPanelMenu->addAction( mOverviewDock->toggleViewAction() );

//  canvas_xy->enableOverviewMode( mOverviewCanvas );

//  // moved here to set anti aliasing to both map canvas and overview
//  QSettings mySettings;
//  // Anti Aliasing enabled by default as of QGIS 1.7
//  canvas_xy->enableAntiAliasing( mySettings.value( "/qgis/enable_anti_aliasing", true ).toBool() );

//  int action = mySettings.value( "/qgis/wheel_action", 2 ).toInt();
//  double zoomFactor = mySettings.value( "/qgis/zoom_factor", 2 ).toDouble();
//  canvas_xy->setWheelAction( static_cast< QgsMapCanvas::WheelAction >( action ), zoomFactor );

//  canvas_xy->setCachingEnabled( mySettings.value( "/qgis/enable_render_caching", true ).toBool() );

//  canvas_xy->setParallelRenderingEnabled( mySettings.value( "/qgis/parallel_rendering", false ).toBool() );

//  canvas_xy->setMapUpdateInterval( mySettings.value( "/qgis/map_update_interval", 250 ).toInt() );
}

void MainWindow::addDockWidget( Qt::DockWidgetArea theArea, QDockWidget * thepDockWidget )
{
  QMainWindow::addDockWidget( theArea, thepDockWidget );
  // Make the right and left docks consume all vertical space and top
  // and bottom docks nest between them
  setCorner( Qt::TopLeftCorner, Qt::LeftDockWidgetArea );
  setCorner( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea );
  setCorner( Qt::TopRightCorner, Qt::RightDockWidgetArea );
  setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea );
  // add to the Panel submenu
  mPanelMenu->addAction( thepDockWidget->toggleViewAction() );

  thepDockWidget->show();

  // refresh the map canvas
  canvas_xy->refresh();
}

void MainWindow::removeDockWidget( QDockWidget * thepDockWidget )
{
  QMainWindow::removeDockWidget( thepDockWidget );
  mPanelMenu->removeAction( thepDockWidget->toggleViewAction() );
}

QToolBar *MainWindow::addToolBar2( const QString& name )
{
  QToolBar *toolBar = QMainWindow::addToolBar( name );
  // add to the Toolbar submenu
  mToolbarMenu->addAction( toolBar->toggleViewAction() );
  return toolBar;
}

void MainWindow::addToolBar2( QToolBar* toolBar, Qt::ToolBarArea area )
{
  QMainWindow::addToolBar( area, toolBar );
  // add to the Toolbar submenu
  mToolbarMenu->addAction( toolBar->toggleViewAction() );
}

QgsLayerTreeView* MainWindow::layerTreeView()
{
  Q_ASSERT( mLayerTreeView );
  return mLayerTreeView;
}

QgsMapCanvas *MainWindow::mapCanvas()
{
  Q_ASSERT( canvas_xy );
  return canvas_xy;
}
QgsMapCanvas *MainWindow::mapCanvas_xy()
{
  Q_ASSERT( canvas_xy );
  return canvas_xy;
}

QgsMessageBar* MainWindow::messageBar()
{
  Q_ASSERT( mInfoBar );
  return mInfoBar;
}


void MainWindow::initLayerTreeView()
{
  mLayerTreeView->setWhatsThis( tr( "Map legend that displays all the layers currently on the map canvas. Click on the check box to turn a layer on or off. Double click on a layer in the legend to customize its appearance and set other properties." ) );

  mLayerTreeDock = new QDockWidget( tr( "Layers Panel" ), this );
  mLayerTreeDock->setObjectName( "Layers" );
  mLayerTreeDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );

  QgsLayerTreeModel* model = new QgsLayerTreeModel( QgsProject::instance()->layerTreeRoot(), this );
//#ifdef ENABLE_MODELTEST
//  new ModelTest( model, this );
//#endif
  model->setFlag( QgsLayerTreeModel::AllowNodeReorder );
  model->setFlag( QgsLayerTreeModel::AllowNodeRename );
  model->setFlag( QgsLayerTreeModel::AllowNodeChangeVisibility );
  model->setFlag( QgsLayerTreeModel::ShowLegendAsTree );
  model->setAutoCollapseLegendNodes( 10 );

  mLayerTreeView->setModel( model );
//  mLayerTreeView->setMenuProvider( new QgsAppLayerTreeViewMenuProvider( mLayerTreeView, canvas_xy ) );

//  setupLayerTreeViewFromSettings();

//  connect( mLayerTreeView, SIGNAL( doubleClicked( QModelIndex ) ), this, SLOT( layerTreeViewDoubleClicked( QModelIndex ) ) );
  connect( mLayerTreeView, SIGNAL( currentLayerChanged( QgsMapLayer* ) ), this, SLOT( activeLayerChanged( QgsMapLayer* ) ) );
  connect( mLayerTreeView->selectionModel(), SIGNAL( currentChanged( QModelIndex, QModelIndex ) ), this, SLOT( updateNewLayerInsertionPoint() ) );
  connect( QgsProject::instance()->layerTreeRegistryBridge(), SIGNAL( addedLayersToLayerTree( QList<QgsMapLayer*> ) ),
           this, SLOT( autoSelectAddedLayer( QList<QgsMapLayer*> ) ) );

  // add group action
  QAction* actionAddGroup = new QAction( tr( "Add Group" ), this );
  actionAddGroup->setIcon( QgsApplication::getThemeIcon( "/mActionAddGroup.svg" ) );
  actionAddGroup->setToolTip( tr( "Add Group" ) );
//  connect( actionAddGroup, SIGNAL( triggered( bool ) ), mLayerTreeView->defaultActions(), SLOT( addGroup() ) );

  // visibility groups tool button
  QToolButton* btnVisibilityPresets = new QToolButton;
  btnVisibilityPresets->setAutoRaise( true );
  btnVisibilityPresets->setToolTip( tr( "Manage Layer Visibility" ) );
  btnVisibilityPresets->setIcon( QgsApplication::getThemeIcon( "/mActionShowAllLayers.svg" ) );
  btnVisibilityPresets->setPopupMode( QToolButton::InstantPopup );
//  btnVisibilityPresets->setMenu( QgsVisibilityPresets::instance()->menu() );

//  // filter legend action
//  mActionFilterLegend = new QAction( tr( "Filter Legend By Map Content" ), this );
//  mActionFilterLegend->setCheckable( true );
//  mActionFilterLegend->setToolTip( tr( "Filter Legend By Map Content" ) );
//  mActionFilterLegend->setIcon( QgsApplication::getThemeIcon( "/mActionFilter2.svg" ) );
//  connect( mActionFilterLegend, SIGNAL( toggled( bool ) ), this, SLOT( updateFilterLegend() ) );

//  mLegendExpressionFilterButton = new QgsLegendFilterButton( this );
//  mLegendExpressionFilterButton->setToolTip( tr( "Filter legend by expression" ) );
//  connect( mLegendExpressionFilterButton, SIGNAL( toggled( bool ) ), this, SLOT( toggleFilterLegendByExpression( bool ) ) );

  // expand / collapse tool buttons
  QAction* actionExpandAll = new QAction( tr( "Expand All" ), this );
  actionExpandAll->setIcon( QgsApplication::getThemeIcon( "/mActionExpandTree.svg" ) );
  actionExpandAll->setToolTip( tr( "Expand All" ) );
  connect( actionExpandAll, SIGNAL( triggered( bool ) ), mLayerTreeView, SLOT( expandAllNodes() ) );
  QAction* actionCollapseAll = new QAction( tr( "Collapse All" ), this );
  actionCollapseAll->setIcon( QgsApplication::getThemeIcon( "/mActionCollapseTree.svg" ) );
  actionCollapseAll->setToolTip( tr( "Collapse All" ) );
  connect( actionCollapseAll, SIGNAL( triggered( bool ) ), mLayerTreeView, SLOT( collapseAllNodes() ) );

  QToolBar* toolbar = new QToolBar();
  toolbar->setIconSize( QSize( 16, 16 ) );
  toolbar->addAction( actionAddGroup );
  toolbar->addWidget( btnVisibilityPresets );
//  toolbar->addAction( mActionFilterLegend );
//  toolbar->addWidget( mLegendExpressionFilterButton );
  toolbar->addAction( actionExpandAll );
  toolbar->addAction( actionCollapseAll );
//  toolbar->addAction( mActionRemoveLayer );

  QVBoxLayout* vboxLayout = new QVBoxLayout;
  vboxLayout->setMargin( 0 );
  vboxLayout->addWidget( toolbar );
  vboxLayout->addWidget( mLayerTreeView );

  QWidget* w = new QWidget;
  w->setLayout( vboxLayout );
  mLayerTreeDock->setWidget( w );
  addDockWidget( Qt::LeftDockWidgetArea, mLayerTreeDock );

//  mLayerTreeCanvasBridge = new QgsLayerTreeMapCanvasBridge( QgsProject::instance()->layerTreeRoot(), canvas_xy, this );
//  connect( QgsProject::instance(), SIGNAL( writeProject( QDomDocument& ) ), mLayerTreeCanvasBridge, SLOT( writeProject( QDomDocument& ) ) );
//  connect( QgsProject::instance(), SIGNAL( readProject( QDomDocument ) ), mLayerTreeCanvasBridge, SLOT( readProject( QDomDocument ) ) );

//  bool otfTransformAutoEnable = QSettings().value( "/Projections/otfTransformAutoEnable", true ).toBool();
//  mLayerTreeCanvasBridge->setAutoEnableCrsTransform( otfTransformAutoEnable );

//  mMapLayerOrder = new QgsCustomLayerOrderWidget( mLayerTreeCanvasBridge, this );
//  mMapLayerOrder->setObjectName( "theMapLayerOrder" );

//  mMapLayerOrder->setWhatsThis( tr( "Map layer list that displays all layers in drawing order." ) );
//  mLayerOrderDock = new QDockWidget( tr( "Layer Order Panel" ), this );
//  mLayerOrderDock->setObjectName( "LayerOrder" );
//  mLayerOrderDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );

//  mLayerOrderDock->setWidget( mMapLayerOrder );
//  addDockWidget( Qt::LeftDockWidgetArea, mLayerOrderDock );
//  mLayerOrderDock->hide();

  connect( canvas_xy, SIGNAL( mapCanvasRefreshed() ), this, SLOT( updateFilterLegend() ) );
}


void MainWindow::updateNewLayerInsertionPoint()
{
  // defaults
  QgsLayerTreeGroup* parentGroup = mLayerTreeView->layerTreeModel()->rootGroup();
  int index = 0;
  QModelIndex current = mLayerTreeView->currentIndex();

  if ( current.isValid() )
  {
    if ( QgsLayerTreeNode* currentNode = mLayerTreeView->currentNode() )
    {
      // if the insertion point is actually a group, insert new layers into the group
      if ( QgsLayerTree::isGroup( currentNode ) )
      {
        QgsProject::instance()->layerTreeRegistryBridge()->setLayerInsertionPoint( QgsLayerTree::toGroup( currentNode ), 0 );
        return;
      }

      // otherwise just set the insertion point in front of the current node
      QgsLayerTreeNode* parentNode = currentNode->parent();
      if ( QgsLayerTree::isGroup( parentNode ) )
        parentGroup = QgsLayerTree::toGroup( parentNode );
    }

    index = current.row();
  }

  QgsProject::instance()->layerTreeRegistryBridge()->setLayerInsertionPoint( parentGroup, index );
}

void MainWindow::autoSelectAddedLayer( QList<QgsMapLayer*> layers )
{
  if ( !layers.isEmpty() )
  {
    QgsLayerTreeLayer* nodeLayer = QgsProject::instance()->layerTreeRoot()->findLayer( layers[0]->id() );

    if ( !nodeLayer )
      return;

    QModelIndex index = mLayerTreeView->layerTreeModel()->node2index( nodeLayer );
    mLayerTreeView->setCurrentIndex( index );
  }
}
void MainWindow::createMapTips()
{
//  // Set up the timer for maptips. The timer is reset everytime the mouse is moved
//  mpMapTipsTimer = new QTimer( canvas_xy );
//  // connect the timer to the maptips slot
//  connect( mpMapTipsTimer, SIGNAL( timeout() ), this, SLOT( showMapTip() ) );
//  // set the interval to 0.850 seconds - timer will be started next time the mouse moves
//  mpMapTipsTimer->setInterval( 850 );
//  // Create the maptips object
//  mpMaptip = new QgsMapTip();
}

void MainWindow::activeLayerChanged( QgsMapLayer* layer )
{
  if ( canvas_xy )
    canvas_xy->setCurrentLayer( layer );
}

/** Get a pointer to the currently selected map layer */
QgsMapLayer *MainWindow::activeLayer()
{
  return mLayerTreeView ? mLayerTreeView->currentLayer() : nullptr;
}

/** Set the current layer */
bool MainWindow::setActiveLayer( QgsMapLayer *layer )
{
  if ( !layer )
    return false;

  if ( !mLayerTreeView->layerTreeModel()->rootGroup()->findLayer( layer->id() ) )
    return false;

  mLayerTreeView->setCurrentLayer( layer );
  return true;
}


void MainWindow::layerProperties()
{
  showLayerProperties( activeLayer() );
}

void MainWindow::deleteSelected( QgsMapLayer *layer, QWidget* parent, bool promptConfirmation )
{
  if ( !layer )
  {
    layer = mLayerTreeView->currentLayer();
  }

  if ( !parent )
  {
    parent = this;
  }

  if ( !layer )
  {
    messageBar()->pushMessage( tr( "No Layer Selected" ),
                               tr( "To delete features, you must select a vector layer in the legend" ),
                               QgsMessageBar::INFO, messageTimeout() );
    return;
  }

  QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer *>( layer );
  if ( !vlayer )
  {
    messageBar()->pushMessage( tr( "No Vector Layer Selected" ),
                               tr( "Deleting features only works on vector layers" ),
                               QgsMessageBar::INFO, messageTimeout() );
    return;
  }

  if ( !( vlayer->dataProvider()->capabilities() & QgsVectorDataProvider::DeleteFeatures ) )
  {
    messageBar()->pushMessage( tr( "Provider does not support deletion" ),
                               tr( "Data provider does not support deleting features" ),
                               QgsMessageBar::INFO, messageTimeout() );
    return;
  }

  if ( !vlayer->isEditable() )
  {
    messageBar()->pushMessage( tr( "Layer not editable" ),
                               tr( "The current layer is not editable. Choose 'Start editing' in the digitizing toolbar." ),
                               QgsMessageBar::INFO, messageTimeout() );
    return;
  }

  //validate selection
  int numberOfSelectedFeatures = vlayer->selectedFeaturesIds().size();
  if ( numberOfSelectedFeatures == 0 )
  {
    messageBar()->pushMessage( tr( "No Features Selected" ),
                               tr( "The current layer has no selected features" ),
                               QgsMessageBar::INFO, messageTimeout() );
    return;
  }
  //display a warning
  if ( promptConfirmation && QMessageBox::warning( parent, tr( "Delete features" ), tr( "Delete %n feature(s)?", "number of features to delete", numberOfSelectedFeatures ), QMessageBox::Ok | QMessageBox::Cancel ) == QMessageBox::Cancel )
  {
    return;
  }

  vlayer->beginEditCommand( tr( "Features deleted" ) );
  int deletedCount = 0;
  if ( !vlayer->deleteSelectedFeatures( &deletedCount ) )
  {
    messageBar()->pushMessage( tr( "Problem deleting features" ),
                               tr( "A problem occurred during deletion of %1 feature(s)" ).arg( numberOfSelectedFeatures - deletedCount ),
                               QgsMessageBar::WARNING );
  }
  else
  {
    showStatusMessage( tr( "%n feature(s) deleted.", "number of features deleted", numberOfSelectedFeatures ) );
  }

  vlayer->endEditCommand();
}


void MainWindow::showLayerProperties( QgsMapLayer *ml )
{/*
  /*
  TODO: Consider reusing the property dialogs again.
  Sometimes around mid 2005, the property dialogs were saved for later reuse;
  this resulted in a time savings when reopening the dialog. The code below
  cannot be used as is, however, simply by saving the dialog pointer here.
  Either the map layer needs to be passed as an argument to sync or else
  a separate copy of the dialog pointer needs to be stored with each layer.
  */

//  if ( !ml )
//    return;

//  if ( !QgsProject::instance()->layerIsEmbedded( ml->id() ).isEmpty() )
//  {
//    return; //don't show properties of embedded layers
//  }

//  if ( ml->type() == QgsMapLayer::RasterLayer )
//  {
//#if 0 // See note above about reusing this
//    QgsRasterLayerProperties *rlp = nullptr;
//    if ( rlp )
//    {
//      rlp->sync();
//    }
//    else
//    {
//      rlp = new QgsRasterLayerProperties( ml, canvas_xy, this );
//      // handled by rendererChanged() connect( rlp, SIGNAL( refreshLegend( QString, bool ) ), mLayerTreeView, SLOT( refreshLayerSymbology( QString ) ) );
//    }
//#else
//    QgsRasterLayerProperties *rlp = new QgsRasterLayerProperties( ml, canvas_xy, this );
//#endif

//    rlp->exec();
//    delete rlp; // delete since dialog cannot be reused without updating code
//  }
//  else if ( ml->type() == QgsMapLayer::VectorLayer ) // VECTOR
//  {
//    QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer *>( ml );

//#if 0 // See note above about reusing this
//    QgsVectorLayerProperties *vlp = nullptr;
//    if ( vlp )
//    {
//      vlp->syncToLayer();
//    }
//    else
//    {
//      vlp = new QgsVectorLayerProperties( vlayer, this );
//      // handled by rendererChanged() connect( vlp, SIGNAL( refreshLegend( QString ) ), mLayerTreeView, SLOT( refreshLayerSymbology( QString ) ) );
//    }
//#else
//    QgsVectorLayerProperties *vlp = new QgsVectorLayerProperties( vlayer, this );
//#endif

//    if ( vlp->exec() )
//    {
//      activateDeactivateLayerRelatedActions( ml );
//    }
//    delete vlp; // delete since dialog cannot be reused without updating code
//  }
//  else if ( ml->type() == QgsMapLayer::PluginLayer )
//  {
//    QgsPluginLayer* pl = qobject_cast<QgsPluginLayer *>( ml );
//    if ( !pl )
//      return;

//    QgsPluginLayerType* plt = QgsPluginLayerRegistry::instance()->pluginLayerType( pl->pluginLayerType() );
//    if ( !plt )
//      return;

//    if ( !plt->showLayerProperties( pl ) )
//    {
//      messageBar()->pushMessage( tr( "Warning" ),
//                                 tr( "This layer doesn't have a properties dialog." ),
//                                 QgsMessageBar::INFO, messageTimeout() );
//    }
//  }*/
}


void MainWindow::saveAsVectorFileGeneral( QgsVectorLayer* vlayer, bool symbologyOption )
{
  if ( !vlayer )
  {
    vlayer = qobject_cast<QgsVectorLayer *>( activeLayer() ); // FIXME: output of multiple layers at once?
  }

  if ( !vlayer )
    return;

  QgsCoordinateReferenceSystem destCRS;

  int options = QgsVectorLayerSaveAsDialog::AllOptions;
  if ( !symbologyOption )
  {
    options &= ~QgsVectorLayerSaveAsDialog::Symbology;
  }

  QgsVectorLayerSaveAsDialog *dialog = new QgsVectorLayerSaveAsDialog( vlayer->crs().srsid(), vlayer->extent(), vlayer->selectedFeatureCount() != 0, options, this );

  dialog->setCanvasExtent( canvas_xy->mapSettings().visibleExtent(), canvas_xy->mapSettings().destinationCrs() );
  dialog->setIncludeZ( QgsWKBTypes::hasZ( QGis::fromOldWkbType( vlayer->wkbType() ) ) );

  if ( dialog->exec() == QDialog::Accepted )
  {
    QString encoding = dialog->encoding();
    QString vectorFilename = dialog->filename();
    QString format = dialog->format();
    QStringList datasourceOptions = dialog->datasourceOptions();
    bool autoGeometryType = dialog->automaticGeometryType();
    QgsWKBTypes::Type forcedGeometryType = dialog->geometryType();

    QgsCoordinateTransform* ct = nullptr;
    destCRS = QgsCoordinateReferenceSystem( dialog->crs(), QgsCoordinateReferenceSystem::InternalCrsId );

    if ( destCRS.isValid() && destCRS != vlayer->crs() )
    {
      ct = new QgsCoordinateTransform( vlayer->crs(), destCRS );

      //ask user about datum transformation
      QSettings settings;
      QList< QList< int > > dt = QgsCoordinateTransform::datumTransformations( vlayer->crs(), destCRS );
      if ( dt.size() > 1 && settings.value( "/Projections/showDatumTransformDialog", false ).toBool() )
      {
        QgsDatumTransformDialog d( vlayer->name(), dt );
        if ( d.exec() == QDialog::Accepted )
        {
          QList< int > sdt = d.selectedDatumTransform();
          if ( !sdt.isEmpty() )
          {
            ct->setSourceDatumTransform( sdt.at( 0 ) );
          }
          if ( sdt.size() > 1 )
          {
            ct->setDestinationDatumTransform( sdt.at( 1 ) );
          }
          ct->initialise();
        }
      }
    }

    // ok if the file existed it should be deleted now so we can continue...
    QApplication::setOverrideCursor( Qt::WaitCursor );

    QgsVectorFileWriter::WriterError error;
    QString errorMessage;
    QString newFilename;
    QgsRectangle filterExtent = dialog->filterExtent();
    error = QgsVectorFileWriter::writeAsVectorFormat(
              vlayer, vectorFilename, encoding, ct, format,
              dialog->onlySelected(),
              &errorMessage,
              datasourceOptions, dialog->layerOptions(),
              dialog->skipAttributeCreation(),
              &newFilename,
              static_cast< QgsVectorFileWriter::SymbologyExport >( dialog->symbologyExport() ),
              dialog->scaleDenominator(),
              dialog->hasFilterExtent() ? &filterExtent : nullptr,
              autoGeometryType ? QgsWKBTypes::Unknown : forcedGeometryType,
              dialog->forceMulti(),
              dialog->includeZ()
            );

    delete ct;

    QApplication::restoreOverrideCursor();

    if ( error == QgsVectorFileWriter::NoError )
    {
      if ( dialog->addToCanvas() )
      {
        addVectorLayers( QStringList( newFilename ), encoding, "file" );
      }
      emit layerSavedAs( vlayer, vectorFilename );
      messageBar()->pushMessage( tr( "Saving done" ),
                                 tr( "Export to vector file has been completed" ),
                                 QgsMessageBar::INFO, messageTimeout() );
    }
    else
    {
      QgsMessageViewer *m = new QgsMessageViewer( nullptr );
      m->setWindowTitle( tr( "Save error" ) );
      m->setMessageAsPlainText( tr( "Export to vector file failed.\nError: %1" ).arg( errorMessage ) );
      m->exec();
    }
  }

  delete dialog;
}



bool MainWindow::addVectorLayers( const QStringList &theLayerQStringList, const QString &enc, const QString &dataSourceType )
{
  bool wasfrozen = canvas_xy->isFrozen();
  QList<QgsMapLayer *> myList;
  Q_FOREACH ( QString src, theLayerQStringList )
  {
    src = src.trimmed();
    QString base;
    if ( dataSourceType == "file" )
    {
      QFileInfo fi( src );
      base = fi.completeBaseName();

//      // if needed prompt for zipitem layers
//      QString vsiPrefix = QgsZipItem::vsiPrefix( src );
//      if ( ! src.startsWith( "/vsi", Qt::CaseInsensitive ) &&
//           ( vsiPrefix == "/vsizip/" || vsiPrefix == "/vsitar/" ) )
//      {
//        if ( askUserForZipItemLayers( src ) )
//          continue;
//      }
    }
    else if ( dataSourceType == "database" )
    {
      base = src;
    }
    else //directory //protocol
    {
      QFileInfo fi( src );
      base = fi.completeBaseName();
    }

    QgsDebugMsg( "completeBaseName: " + base );

    // create the layer

    QgsVectorLayer *layer = new QgsVectorLayer( src, base, "ogr", false );
    Q_CHECK_PTR( layer );

    if ( ! layer )
    {
      canvas_xy->freeze( false );

      // Let render() do its own cursor management
      //      QApplication::restoreOverrideCursor();

      // XXX insert meaningful whine to the user here
      return false;
    }

    if ( layer->isValid() )
    {
      layer->setProviderEncoding( enc );

      QStringList sublayers = layer->dataProvider()->subLayers();
      QgsDebugMsg( QString( "got valid layer with %1 sublayers" ).arg( sublayers.count() ) );

      // If the newly created layer has more than 1 layer of data available, we show the
      // sublayers selection dialog so the user can select the sublayers to actually load.
      if ( sublayers.count() > 1 )
      {
//        askUserForOGRSublayers( layer );

        // The first layer loaded is not useful in that case. The user can select it in
        // the list if he wants to load it.
        delete layer;

      }
      else if ( !sublayers.isEmpty() ) // there is 1 layer of data available
      {
        //set friendly name for datasources with only one layer
        QStringList sublayers = layer->dataProvider()->subLayers();
        QStringList elements = sublayers.at( 0 ).split( ':' );

        Q_ASSERT( elements.size() >= 4 );
        if ( layer->name() != elements.at( 1 ) )
        {
          layer->setLayerName( QString( "%1 %2 %3" ).arg( layer->name(), elements.at( 1 ), elements.at( 3 ) ) );
        }

        myList << layer;
      }
      else
      {
        QString msg = tr( "%1 doesn't have any layers" ).arg( src );
        messageBar()->pushMessage( tr( "Invalid Data Source" ), msg, QgsMessageBar::CRITICAL, messageTimeout() );
        delete layer;
      }
    }
    else
    {
      QString msg = tr( "%1 is not a valid or recognized data source" ).arg( src );
      messageBar()->pushMessage( tr( "Invalid Data Source" ), msg, QgsMessageBar::CRITICAL, messageTimeout() );

      // since the layer is bad, stomp on it
      delete layer;
    }

  }

  // make sure at least one layer was successfully added
  if ( myList.isEmpty() )
  {
    return false;
  }

  // Register this layer with the layers registry
  QgsMapLayerRegistry::instance()->addMapLayers( myList );
  Q_FOREACH ( QgsMapLayer *l, myList )
  {
    bool ok;
    l->loadDefaultStyle( ok );
  }
  activateDeactivateLayerRelatedActions( activeLayer() );

  // Only update the map if we frozen in this method
  // Let the caller do it otherwise
  if ( !wasfrozen )
  {
    canvas_xy->freeze( false );
    canvas_xy->refresh();
  }
// Let render() do its own cursor management
//  QApplication::restoreOverrideCursor();

  // statusBar()->showMessage( canvas_xy->extent().toString( 2 ) );

  return true;
} // MainWindow::addVectorLayer()


void MainWindow::saveActiveLayerEdits()
{
  saveEdits( activeLayer(), true, true );
}

void MainWindow::saveEdits( QgsMapLayer *layer, bool leaveEditable, bool triggerRepaint )
{
  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
  if ( !vlayer || !vlayer->isEditable() || !vlayer->isModified() )
    return;

  if ( vlayer == activeLayer() )
    mSaveRollbackInProgress = true;

  if ( !vlayer->commitChanges() )
  {
    mSaveRollbackInProgress = false;
    commitError( vlayer );
  }

  if ( leaveEditable )
  {
    vlayer->startEditing();
  }
  if ( triggerRepaint )
  {
    vlayer->triggerRepaint();
  }
}

void MainWindow::cancelEdits( QgsMapLayer *layer, bool leaveEditable, bool triggerRepaint )
{
  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
  if ( !vlayer || !vlayer->isEditable() )
    return;

  if ( vlayer == activeLayer() && leaveEditable )
    mSaveRollbackInProgress = true;

//  canvas_xy->freeze( true );
  canvas_xy->freeze( true );
  if ( !vlayer->rollBack( !leaveEditable ) )
  {
    mSaveRollbackInProgress = false;
    QMessageBox::information( nullptr,
                              tr( "Error" ),
                              tr( "Could not %1 changes to layer %2\n\nErrors: %3\n" )
                              .arg( leaveEditable ? tr( "rollback" ) : tr( "cancel" ),
                                    vlayer->name(),
                                    vlayer->commitErrors().join( "\n  " ) ) );
  }
//  canvas_xy->freeze( false );
  canvas_xy->freeze( false );

  if ( leaveEditable )
  {
    vlayer->startEditing();
  }
  if ( triggerRepaint )
  {
    vlayer->triggerRepaint();
  }
}

void MainWindow::saveEdits()
{
  Q_FOREACH ( QgsMapLayer * layer, mLayerTreeView->selectedLayers() )
  {
    saveEdits( layer, true, false );
  }
//  canvas_xy->refresh();
  canvas_xy->refresh();
  activateDeactivateLayerRelatedActions( activeLayer() );
}

void MainWindow::saveAllEdits( bool verifyAction )
{
  if ( verifyAction )
  {
    if ( !verifyEditsActionDialog( tr( "Save" ), tr( "all" ) ) )
      return;
  }

  Q_FOREACH ( QgsMapLayer * layer, editableLayers( true ) )
  {
    saveEdits( layer, true, false );
  }
  canvas_xy->refresh();
  activateDeactivateLayerRelatedActions( activeLayer() );
}

void MainWindow::rollbackEdits()
{
  Q_FOREACH ( QgsMapLayer * layer, mLayerTreeView->selectedLayers() )
  {
    cancelEdits( layer, true, false );
  }
  canvas_xy->refresh();
  activateDeactivateLayerRelatedActions( activeLayer() );
}

void MainWindow::rollbackAllEdits( bool verifyAction )
{
  if ( verifyAction )
  {
    if ( !verifyEditsActionDialog( tr( "Rollback" ), tr( "all" ) ) )
      return;
  }

  Q_FOREACH ( QgsMapLayer * layer, editableLayers( true ) )
  {
    cancelEdits( layer, true, false );
  }
  canvas_xy->refresh();
  activateDeactivateLayerRelatedActions( activeLayer() );
}

void MainWindow::cancelEdits()
{
  Q_FOREACH ( QgsMapLayer * layer, mLayerTreeView->selectedLayers() )
  {
    cancelEdits( layer, false, false );
  }
  canvas_xy->refresh();
  activateDeactivateLayerRelatedActions( activeLayer() );
}

void MainWindow::cancelAllEdits( bool verifyAction )
{
  if ( verifyAction )
  {
    if ( !verifyEditsActionDialog( tr( "Cancel" ), tr( "all" ) ) )
      return;
  }

  Q_FOREACH ( QgsMapLayer * layer, editableLayers() )
  {
    cancelEdits( layer, false, false );
  }
  canvas_xy->refresh();
  activateDeactivateLayerRelatedActions( activeLayer() );
}

bool MainWindow::verifyEditsActionDialog( const QString& act, const QString& upon )
{
  bool res = false;
  switch ( QMessageBox::information( nullptr,
                                     tr( "Current edits" ),
                                     tr( "%1 current changes for %2 layer(s)?" )
                                     .arg( act,
                                           upon ),
                                     QMessageBox::Cancel | QMessageBox::Ok ) )
  {
    case QMessageBox::Ok:
      res = true;
      break;
    default:
      break;
  }
  return res;
}

void MainWindow::updateLayerModifiedActions()
{
  bool enableSaveLayerEdits = false;
  QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer *>( activeLayer() );
  if ( vlayer )
  {
    QgsVectorDataProvider* dprovider = vlayer->dataProvider();
    if ( dprovider )
    {
      enableSaveLayerEdits = ( dprovider->capabilities() & QgsVectorDataProvider::ChangeAttributeValues
                               && vlayer->isEditable()
                               && vlayer->isModified() );
    }
  }
  mActionSaveLayerEdits->setEnabled( enableSaveLayerEdits );

  QList<QgsLayerTreeLayer*> selectedLayerNodes = mLayerTreeView ? mLayerTreeView->selectedLayerNodes() : QList<QgsLayerTreeLayer*>();

  mActionSaveEdits->setEnabled( QgsLayerTreeUtils::layersModified( selectedLayerNodes ) );
  mActionRollbackEdits->setEnabled( QgsLayerTreeUtils::layersModified( selectedLayerNodes ) );
  mActionCancelEdits->setEnabled( QgsLayerTreeUtils::layersEditable( selectedLayerNodes ) );

  bool hasEditLayers = !editableLayers().isEmpty();
  mActionAllEdits->setEnabled( hasEditLayers );
  mActionCancelAllEdits->setEnabled( hasEditLayers );

  bool hasModifiedLayers = !editableLayers( true ).isEmpty();
  mActionSaveAllEdits->setEnabled( hasModifiedLayers );
  mActionRollbackAllEdits->setEnabled( hasModifiedLayers );
}

QList<QgsMapLayer *> MainWindow::editableLayers( bool modified ) const
{
  QList<QgsMapLayer*> editLayers;
  // use legend layers (instead of registry) so QList mirrors its order
  Q_FOREACH ( QgsLayerTreeLayer* nodeLayer, mLayerTreeView->layerTreeModel()->rootGroup()->findLayers() )
  {
    if ( !nodeLayer->layer() )
      continue;

    QgsVectorLayer *vl = qobject_cast<QgsVectorLayer*>( nodeLayer->layer() );
    if ( !vl )
      continue;

    if ( vl->isEditable() && ( !modified || vl->isModified() ) )
      editLayers << vl;
  }
  return editLayers;
}




void MainWindow::removingLayers( const QStringList& theLayers )
{
  Q_FOREACH ( const QString &layerId, theLayers )
  {
    QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer*>(
                               QgsMapLayerRegistry::instance()->mapLayer( layerId ) );
    if ( !vlayer || !vlayer->isEditable() )
      return;

    toggleEditing( vlayer, false );
  }

  if ( autoTransaction() )
  {
    for ( QMap< QPair< QString, QString>, QgsTransactionGroup*>::Iterator tg = mTransactionGroups.begin(); tg != mTransactionGroups.end(); )
    {
      if ( tg.value()->isEmpty() )
      {
        delete tg.value();
        tg = mTransactionGroups.erase( tg );
      }
      else
      {
        ++tg;
      }
    }
  }
}



void MainWindow::identify()
{
  canvas_xy->setMapTool( mMapTools.mIdentify );
}

//void MainWindow::doFeatureAction()
//{
//  canvas_xy->setMapTool( mMapTools.mFeatureAction );
//}







void MainWindow::layerEditStateChanged()
{
  QgsMapLayer* layer = qobject_cast<QgsMapLayer *>( sender() );
  if ( layer && layer == activeLayer() )
  {
    activateDeactivateLayerRelatedActions( layer );
    mSaveRollbackInProgress = false;
  }
}

void MainWindow::activateDeactivateLayerRelatedActions( QgsMapLayer* layer )
{
//  bool enableMove = false, enableRotate = false, enablePin = false, enableShowHide = false, enableChange = false;

//  QMap<QString, QgsMapLayer*> layers = QgsMapLayerRegistry::instance()->mapLayers();
//  for ( QMap<QString, QgsMapLayer*>::iterator it = layers.begin(); it != layers.end(); ++it )
//  {
//    QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( it.value() );
//    if ( !vlayer || !vlayer->isEditable() ||
//         ( !vlayer->diagramsEnabled() && !vlayer->labelsEnabled() ) )
//      continue;

//    int colX, colY, colShow, colAng;
//    enablePin =
//      enablePin ||
//      ( qobject_cast<QgsMapToolPinLabels*>( mMapTools.mPinLabels ) &&
//        qobject_cast<QgsMapToolPinLabels*>( mMapTools.mPinLabels )->layerCanPin( vlayer, colX, colY ) );

//    enableShowHide =
//      enableShowHide ||
//      ( qobject_cast<QgsMapToolShowHideLabels*>( mMapTools.mShowHideLabels ) &&
//        qobject_cast<QgsMapToolShowHideLabels*>( mMapTools.mShowHideLabels )->layerCanShowHide( vlayer, colShow ) );

//    enableMove =
//      enableMove ||
//      ( qobject_cast<QgsMapToolMoveLabel*>( mMapTools.mMoveLabel ) &&
//        ( qobject_cast<QgsMapToolMoveLabel*>( mMapTools.mMoveLabel )->labelMoveable( vlayer, colX, colY )
//          || qobject_cast<QgsMapToolMoveLabel*>( mMapTools.mMoveLabel )->diagramMoveable( vlayer, colX, colY ) )
//      );

//    enableRotate =
//      enableRotate ||
//      ( qobject_cast<QgsMapToolRotateLabel*>( mMapTools.mRotateLabel ) &&
//        qobject_cast<QgsMapToolRotateLabel*>( mMapTools.mRotateLabel )->layerIsRotatable( vlayer, colAng ) );

//    enableChange = true;

//    if ( enablePin && enableShowHide && enableMove && enableRotate && enableChange )
//      break;
//  }

//  mActionPinLabels->setEnabled( enablePin );
//  mActionShowHideLabels->setEnabled( enableShowHide );
//  mActionMoveLabel->setEnabled( enableMove );
//  mActionRotateLabel->setEnabled( enableRotate );
//  mActionChangeLabelProperties->setEnabled( enableChange );

//  mMenuPasteAs->setEnabled( clipboard() && !clipboard()->empty() );
//  mActionPasteAsNewVector->setEnabled( clipboard() && !clipboard()->empty() );
//  mActionPasteAsNewMemoryVector->setEnabled( clipboard() && !clipboard()->empty() );

//  updateLayerModifiedActions();

//  if ( !layer )
//  {
//    mActionSelectFeatures->setEnabled( false );
//    mActionSelectPolygon->setEnabled( false );
//    mActionSelectFreehand->setEnabled( false );
//    mActionSelectRadius->setEnabled( false );
//    mActionIdentify->setEnabled( QSettings().value( "/Map/identifyMode", 0 ).toInt() != 0 );
//    mActionSelectByExpression->setEnabled( false );
//    mActionLabeling->setEnabled( false );
//    mActionOpenTable->setEnabled( false );
//    mActionSelectAll->setEnabled( false );
//    mActionInvertSelection->setEnabled( false );
//    mActionOpenFieldCalc->setEnabled( false );
//    mActionToggleEditing->setEnabled( false );
//    mActionToggleEditing->setChecked( false );
//    mActionSaveLayerEdits->setEnabled( false );
//    mActionSaveLayerDefinition->setEnabled( false );
//    mActionLayerSaveAs->setEnabled( false );
//    mActionLayerProperties->setEnabled( false );
//    mActionLayerSubsetString->setEnabled( false );
//    mActionAddToOverview->setEnabled( false );
//    mActionFeatureAction->setEnabled( false );
//    mActionAddFeature->setEnabled( false );
//    mActionCircularStringCurvePoint->setEnabled( false );
//    mActionCircularStringRadius->setEnabled( false );
//    mActionMoveFeature->setEnabled( false );
//    mActionRotateFeature->setEnabled( false );
//    mActionOffsetCurve->setEnabled( false );
//    mActionNodeTool->setEnabled( false );
//    mActionDeleteSelected->setEnabled( false );
//    mActionCutFeatures->setEnabled( false );
//    mActionCopyFeatures->setEnabled( false );
//    mActionPasteFeatures->setEnabled( false );
//    mActionCopyStyle->setEnabled( false );
//    mActionPasteStyle->setEnabled( false );

//    mUndoWidget->dockContents()->setEnabled( false );
//    mActionUndo->setEnabled( false );
//    mActionRedo->setEnabled( false );
//    mActionSimplifyFeature->setEnabled( false );
//    mActionAddRing->setEnabled( false );
//    mActionFillRing->setEnabled( false );
//    mActionAddPart->setEnabled( false );
//    mActionDeleteRing->setEnabled( false );
//    mActionDeletePart->setEnabled( false );
//    mActionReshapeFeatures->setEnabled( false );
//    mActionOffsetCurve->setEnabled( false );
//    mActionSplitFeatures->setEnabled( false );
//    mActionSplitParts->setEnabled( false );
//    mActionMergeFeatures->setEnabled( false );
//    mActionMergeFeatureAttributes->setEnabled( false );
//    mActionRotatePointSymbols->setEnabled( false );
//    mActionEnableTracing->setEnabled( false );

//    mActionPinLabels->setEnabled( false );
//    mActionShowHideLabels->setEnabled( false );
//    mActionMoveLabel->setEnabled( false );
//    mActionRotateLabel->setEnabled( false );
//    mActionChangeLabelProperties->setEnabled( false );

//    mActionLocalHistogramStretch->setEnabled( false );
//    mActionFullHistogramStretch->setEnabled( false );
//    mActionLocalCumulativeCutStretch->setEnabled( false );
//    mActionFullCumulativeCutStretch->setEnabled( false );
//    mActionIncreaseBrightness->setEnabled( false );
//    mActionDecreaseBrightness->setEnabled( false );
//    mActionIncreaseContrast->setEnabled( false );
//    mActionDecreaseContrast->setEnabled( false );
//    mActionZoomActualSize->setEnabled( false );
//    mActionZoomToLayer->setEnabled( false );
//    return;
//  }

//  mActionLayerProperties->setEnabled( QgsProject::instance()->layerIsEmbedded( layer->id() ).isEmpty() );
//  mActionAddToOverview->setEnabled( true );
//  mActionZoomToLayer->setEnabled( true );

//  mActionCopyStyle->setEnabled( true );
//  mActionPasteStyle->setEnabled( clipboard()->hasFormat( QGSCLIPBOARD_STYLE_MIME ) );

//  /***********Vector layers****************/
//  if ( layer->type() == QgsMapLayer::VectorLayer )
//  {
//    QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer *>( layer );
//    QgsVectorDataProvider* dprovider = vlayer->dataProvider();

//    bool isEditable = vlayer->isEditable();
//    bool layerHasSelection = vlayer->selectedFeatureCount() > 0;
//    bool layerHasActions = vlayer->actions()->size() || !QgsMapLayerActionRegistry::instance()->mapLayerActions( vlayer ).isEmpty();

//    mActionLocalHistogramStretch->setEnabled( false );
//    mActionFullHistogramStretch->setEnabled( false );
//    mActionLocalCumulativeCutStretch->setEnabled( false );
//    mActionFullCumulativeCutStretch->setEnabled( false );
//    mActionIncreaseBrightness->setEnabled( false );
//    mActionDecreaseBrightness->setEnabled( false );
//    mActionIncreaseContrast->setEnabled( false );
//    mActionDecreaseContrast->setEnabled( false );
//    mActionZoomActualSize->setEnabled( false );
//    mActionLabeling->setEnabled( true );

//    mActionSelectFeatures->setEnabled( true );
//    mActionSelectPolygon->setEnabled( true );
//    mActionSelectFreehand->setEnabled( true );
//    mActionSelectRadius->setEnabled( true );
//    mActionIdentify->setEnabled( true );
//    mActionSelectByExpression->setEnabled( true );
//    mActionOpenTable->setEnabled( true );
//    mActionSelectAll->setEnabled( true );
//    mActionInvertSelection->setEnabled( true );
//    mActionSaveLayerDefinition->setEnabled( true );
//    mActionLayerSaveAs->setEnabled( true );
//    mActionCopyFeatures->setEnabled( layerHasSelection );
//    mActionFeatureAction->setEnabled( layerHasActions );

//    if ( !isEditable && canvas_xy && canvas_xy->mapTool()
//         && canvas_xy->mapTool()->isEditTool() && !mSaveRollbackInProgress )
//    {
//      canvas_xy->setMapTool( mNonEditMapTool );
//    }

//    if ( dprovider )
//    {
//      bool canChangeAttributes = dprovider->capabilities() & QgsVectorDataProvider::ChangeAttributeValues;
//      bool canDeleteFeatures = dprovider->capabilities() & QgsVectorDataProvider::DeleteFeatures;
//      bool canAddFeatures = dprovider->capabilities() & QgsVectorDataProvider::AddFeatures;
//      bool canSupportEditing = dprovider->capabilities() & QgsVectorDataProvider::EditingCapabilities;
//      bool canChangeGeometry = dprovider->capabilities() & QgsVectorDataProvider::ChangeGeometries;

//      mActionLayerSubsetString->setEnabled( !isEditable && dprovider->supportsSubsetString() );

//      mActionToggleEditing->setEnabled( canSupportEditing && !vlayer->isReadOnly() );
//      mActionToggleEditing->setChecked( canSupportEditing && isEditable );
//      mActionSaveLayerEdits->setEnabled( canSupportEditing && isEditable && vlayer->isModified() );
//      mUndoWidget->dockContents()->setEnabled( canSupportEditing && isEditable );
//      mActionUndo->setEnabled( canSupportEditing );
//      mActionRedo->setEnabled( canSupportEditing );

//      //start editing/stop editing
//      if ( canSupportEditing )
//      {
//        updateUndoActions();
//      }

//      mActionPasteFeatures->setEnabled( isEditable && canAddFeatures && !clipboard()->empty() );

//      mActionAddFeature->setEnabled( isEditable && canAddFeatures );
//      mActionCircularStringCurvePoint->setEnabled( isEditable && ( canAddFeatures || canChangeGeometry )
//          && ( vlayer->geometryType() == QGis::Line || vlayer->geometryType() == QGis::Polygon ) );
//      mActionCircularStringRadius->setEnabled( isEditable && ( canAddFeatures || canChangeGeometry )
//          && ( vlayer->geometryType() == QGis::Line || vlayer->geometryType() == QGis::Polygon ) );


//      //does provider allow deleting of features?
//      mActionDeleteSelected->setEnabled( isEditable && canDeleteFeatures && layerHasSelection );
//      mActionCutFeatures->setEnabled( isEditable && canDeleteFeatures && layerHasSelection );

//      //merge tool needs editable layer and provider with the capability of adding and deleting features
//      if ( isEditable && canChangeAttributes )
//      {
//        mActionMergeFeatures->setEnabled( layerHasSelection && canDeleteFeatures && canAddFeatures );
//        mActionMergeFeatureAttributes->setEnabled( layerHasSelection );
//      }
//      else
//      {
//        mActionMergeFeatures->setEnabled( false );
//        mActionMergeFeatureAttributes->setEnabled( false );
//      }

//      bool isMultiPart = QGis::isMultiType( vlayer->wkbType() ) || !dprovider->doesStrictFeatureTypeCheck();

//      // moving enabled if geometry changes are supported
//      mActionAddPart->setEnabled( isEditable && canChangeGeometry );
//      mActionDeletePart->setEnabled( isEditable && canChangeGeometry );
//      mActionMoveFeature->setEnabled( isEditable && canChangeGeometry );
//      mActionRotateFeature->setEnabled( isEditable && canChangeGeometry );
//      mActionNodeTool->setEnabled( isEditable && canChangeGeometry );

//      mActionEnableTracing->setEnabled( isEditable && canAddFeatures &&
//                                        ( vlayer->geometryType() == QGis::Line || vlayer->geometryType() == QGis::Polygon ) );

//      if ( vlayer->geometryType() == QGis::Point )
//      {
//        mActionAddFeature->setIcon( QgsApplication::getThemeIcon( "/mActionCapturePoint.svg" ) );

//        mActionAddRing->setEnabled( false );
//        mActionFillRing->setEnabled( false );
//        mActionReshapeFeatures->setEnabled( false );
//        mActionSplitFeatures->setEnabled( false );
//        mActionSplitParts->setEnabled( false );
//        mActionSimplifyFeature->setEnabled( false );
//        mActionDeleteRing->setEnabled( false );
//        mActionRotatePointSymbols->setEnabled( false );
//        mActionOffsetCurve->setEnabled( false );

//        if ( isEditable && canChangeAttributes )
//        {
//          if ( QgsMapToolRotatePointSymbols::layerIsRotatable( vlayer ) )
//          {
//            mActionRotatePointSymbols->setEnabled( true );
//          }
//        }
//      }
//      else if ( vlayer->geometryType() == QGis::Line )
//      {
//        mActionAddFeature->setIcon( QgsApplication::getThemeIcon( "/mActionCaptureLine.svg" ) );

//        mActionReshapeFeatures->setEnabled( isEditable && canChangeGeometry );
//        mActionSplitFeatures->setEnabled( isEditable && canAddFeatures );
//        mActionSplitParts->setEnabled( isEditable && canChangeGeometry && isMultiPart );
//        mActionSimplifyFeature->setEnabled( isEditable && canChangeGeometry );
//        mActionOffsetCurve->setEnabled( isEditable && canAddFeatures && canChangeAttributes );

//        mActionAddRing->setEnabled( false );
//        mActionFillRing->setEnabled( false );
//        mActionDeleteRing->setEnabled( false );
//      }
//      else if ( vlayer->geometryType() == QGis::Polygon )
//      {
//        mActionAddFeature->setIcon( QgsApplication::getThemeIcon( "/mActionCapturePolygon.svg" ) );

//        mActionAddRing->setEnabled( isEditable && canChangeGeometry );
//        mActionFillRing->setEnabled( isEditable && canChangeGeometry );
//        mActionReshapeFeatures->setEnabled( isEditable && canChangeGeometry );
//        mActionSplitFeatures->setEnabled( isEditable && canAddFeatures );
//        mActionSplitParts->setEnabled( isEditable && canChangeGeometry && isMultiPart );
//        mActionSimplifyFeature->setEnabled( isEditable && canChangeGeometry );
//        mActionDeleteRing->setEnabled( isEditable && canChangeGeometry );
//        mActionOffsetCurve->setEnabled( false );
//      }
//      else if ( vlayer->geometryType() == QGis::NoGeometry )
//      {
//        mActionAddFeature->setIcon( QgsApplication::getThemeIcon( "/mActionNewTableRow.png" ) );
//      }

//      mActionOpenFieldCalc->setEnabled( true );

//      return;
//    }
//    else
//    {
//      mUndoWidget->dockContents()->setEnabled( false );
//      mActionUndo->setEnabled( false );
//      mActionRedo->setEnabled( false );
//    }

//    mActionLayerSubsetString->setEnabled( false );
//  } //end vector layer block
//  /*************Raster layers*************/
//  else if ( layer->type() == QgsMapLayer::RasterLayer )
//  {
//    const QgsRasterLayer *rlayer = qobject_cast<const QgsRasterLayer *>( layer );
//    if ( rlayer->dataProvider()->dataType( 1 ) != QGis::ARGB32
//         && rlayer->dataProvider()->dataType( 1 ) != QGis::ARGB32_Premultiplied )
//    {
//      if ( rlayer->dataProvider()->capabilities() & QgsRasterDataProvider::Size )
//      {
//        mActionFullHistogramStretch->setEnabled( true );
//      }
//      else
//      {
//        // it would hang up reading the data for WMS for example
//        mActionFullHistogramStretch->setEnabled( false );
//      }
//      mActionLocalHistogramStretch->setEnabled( true );
//    }
//    else
//    {
//      mActionLocalHistogramStretch->setEnabled( false );
//      mActionFullHistogramStretch->setEnabled( false );
//    }

//    mActionLocalCumulativeCutStretch->setEnabled( true );
//    mActionFullCumulativeCutStretch->setEnabled( true );
//    mActionIncreaseBrightness->setEnabled( true );
//    mActionDecreaseBrightness->setEnabled( true );
//    mActionIncreaseContrast->setEnabled( true );
//    mActionDecreaseContrast->setEnabled( true );

//    mActionLayerSubsetString->setEnabled( false );
//    mActionFeatureAction->setEnabled( false );
//    mActionSelectFeatures->setEnabled( false );
//    mActionSelectPolygon->setEnabled( false );
//    mActionSelectFreehand->setEnabled( false );
//    mActionSelectRadius->setEnabled( false );
//    mActionZoomActualSize->setEnabled( true );
//    mActionOpenTable->setEnabled( false );
//    mActionSelectAll->setEnabled( false );
//    mActionInvertSelection->setEnabled( false );
//    mActionSelectByExpression->setEnabled( false );
//    mActionOpenFieldCalc->setEnabled( false );
//    mActionToggleEditing->setEnabled( false );
//    mActionToggleEditing->setChecked( false );
//    mActionSaveLayerEdits->setEnabled( false );
//    mUndoWidget->dockContents()->setEnabled( false );
//    mActionUndo->setEnabled( false );
//    mActionRedo->setEnabled( false );
//    mActionSaveLayerDefinition->setEnabled( true );
//    mActionLayerSaveAs->setEnabled( true );
//    mActionAddFeature->setEnabled( false );
//    mActionCircularStringCurvePoint->setEnabled( false );
//    mActionCircularStringRadius->setEnabled( false );
//    mActionDeleteSelected->setEnabled( false );
//    mActionAddRing->setEnabled( false );
//    mActionFillRing->setEnabled( false );
//    mActionAddPart->setEnabled( false );
//    mActionNodeTool->setEnabled( false );
//    mActionMoveFeature->setEnabled( false );
//    mActionRotateFeature->setEnabled( false );
//    mActionOffsetCurve->setEnabled( false );
//    mActionCopyFeatures->setEnabled( false );
//    mActionCutFeatures->setEnabled( false );
//    mActionPasteFeatures->setEnabled( false );
//    mActionRotatePointSymbols->setEnabled( false );
//    mActionDeletePart->setEnabled( false );
//    mActionDeleteRing->setEnabled( false );
//    mActionSimplifyFeature->setEnabled( false );
//    mActionReshapeFeatures->setEnabled( false );
//    mActionSplitFeatures->setEnabled( false );
//    mActionSplitParts->setEnabled( false );
//    mActionLabeling->setEnabled( false );

//    //NOTE: This check does not really add any protection, as it is called on load not on layer select/activate
//    //If you load a layer with a provider and idenitfy ability then load another without, the tool would be disabled for both

//    //Enable the Identify tool ( GDAL datasets draw without a provider )
//    //but turn off if data provider exists and has no Identify capabilities
//    mActionIdentify->setEnabled( true );

//    QSettings settings;
//    int identifyMode = settings.value( "/Map/identifyMode", 0 ).toInt();
//    if ( identifyMode == 0 )
//    {
//      const QgsRasterLayer *rlayer = qobject_cast<const QgsRasterLayer *>( layer );
//      const QgsRasterDataProvider* dprovider = rlayer->dataProvider();
//      if ( dprovider )
//      {
//        // does provider allow the identify map tool?
//        if ( dprovider->capabilities() & QgsRasterDataProvider::Identify )
//        {
//          mActionIdentify->setEnabled( true );
//        }
//        else
//        {
//          mActionIdentify->setEnabled( false );
//        }
//      }
//    }
//  }
}



void MainWindow::refreshActionFeatureAction()
{
  QgsMapLayer* layer = activeLayer();

  if ( !layer || layer->type() != QgsMapLayer::VectorLayer )
  {
    return;
  }

  QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer *>( layer );

  bool layerHasActions = vlayer->actions()->size() || !QgsMapLayerActionRegistry::instance()->mapLayerActions( vlayer ).isEmpty();
  mActionFeatureAction->setEnabled( layerHasActions );
}

bool MainWindow::autoTransaction() const
{
  QSettings settings;
  return settings.value( "/qgis/autoTransaction", false ).toBool();
}

void MainWindow::setAutoTransaction( bool state )
{
  QSettings settings;

  if ( settings.value( "/qgis/autoTransaction", false ).toBool() != state )
  {

    settings.setValue( "/qgis/autoTransaction", state );
  }
}


void MainWindow::editCut( QgsMapLayer * layerContainingSelection )
{
  // Test for feature support in this layer
  QgsVectorLayer* selectionVectorLayer = qobject_cast<QgsVectorLayer *>( layerContainingSelection ? layerContainingSelection : activeLayer() );
  if ( !selectionVectorLayer )
    return;

  clipboard()->replaceWithCopyOf( selectionVectorLayer );

  selectionVectorLayer->beginEditCommand( tr( "Features cut" ) );
  selectionVectorLayer->deleteSelectedFeatures();
  selectionVectorLayer->endEditCommand();
}

void MainWindow::editCopy( QgsMapLayer * layerContainingSelection )
{
  QgsVectorLayer* selectionVectorLayer = qobject_cast<QgsVectorLayer *>( layerContainingSelection ? layerContainingSelection : activeLayer() );
  if ( !selectionVectorLayer )
    return;

  // Test for feature support in this layer
  clipboard()->replaceWithCopyOf( selectionVectorLayer );
}

void MainWindow::clipboardChanged()
{
  activateDeactivateLayerRelatedActions( activeLayer() );
}

void MainWindow::editPaste( QgsMapLayer *destinationLayer )
{
  QgsVectorLayer *pasteVectorLayer = qobject_cast<QgsVectorLayer *>( destinationLayer ? destinationLayer : activeLayer() );
  if ( !pasteVectorLayer )
    return;

  pasteVectorLayer->beginEditCommand( tr( "Features pasted" ) );
  QgsFeatureList features;
  if ( canvas_xy->mapSettings().hasCrsTransformEnabled() )
  {
    features = clipboard()->transformedCopyOf( pasteVectorLayer->crs(), pasteVectorLayer->fields() );
  }
  else
  {
    features = clipboard()->copyOf( pasteVectorLayer->fields() );
  }
  int nTotalFeatures = features.count();

  QHash<int, int> remap;
  const QgsFields &fields = clipboard()->fields();
  QgsAttributeList pkAttrList = pasteVectorLayer->pkAttributeList();
  for ( int idx = 0; idx < fields.count(); ++idx )
  {
    int dst = pasteVectorLayer->fieldNameIndex( fields[idx].name() );
    if ( dst < 0 )
      continue;

    remap.insert( idx, dst );
  }

  int dstAttrCount = pasteVectorLayer->fields().count();

  QgsFeatureList::iterator featureIt = features.begin();
  while ( featureIt != features.end() )
  {
    QgsAttributes srcAttr = featureIt->attributes();
    QgsAttributes dstAttr( dstAttrCount );

    for ( int src = 0; src < srcAttr.count(); ++src )
    {
      int dst = remap.value( src, -1 );
      if ( dst < 0 )
        continue;

      // use default value for primary key fields if it's NOT NULL
      if ( pkAttrList.contains( dst ) )
      {
        dstAttr[ dst ] = pasteVectorLayer->dataProvider()->defaultValue( dst );
        if ( !dstAttr.at( dst ).isNull() )
          continue;
        else if ( pasteVectorLayer->providerType() == "spatialite" )
          continue;
      }

      dstAttr[ dst ] = srcAttr.at( src );
    }

    featureIt->setAttributes( dstAttr );

    if ( featureIt->constGeometry() )
    {
      // convert geometry to match destination layer
      QGis::GeometryType destType = pasteVectorLayer->geometryType();
      bool destIsMulti = QGis::isMultiType( pasteVectorLayer->wkbType() );
      if ( pasteVectorLayer->dataProvider() &&
           !pasteVectorLayer->dataProvider()->doesStrictFeatureTypeCheck() )
      {
        // force destination to multi if provider doesn't do a feature strict check
        destIsMulti = true;
      }

      if ( destType != QGis::UnknownGeometry )
      {
        QgsGeometry* newGeometry = featureIt->constGeometry()->convertToType( destType, destIsMulti );
        if ( !newGeometry )
        {
          featureIt = features.erase( featureIt );
          continue;
        }
        featureIt->setGeometry( newGeometry );
      }
      // avoid intersection if enabled in digitize settings
      featureIt->geometry()->avoidIntersections();
    }

    ++featureIt;
  }

  pasteVectorLayer->addFeatures( features );
  pasteVectorLayer->endEditCommand();

  int nCopiedFeatures = features.count();
  if ( nCopiedFeatures == 0 )
  {
    messageBar()->pushMessage( tr( "Paste features" ),
                               tr( "no features could be successfully pasted." ),
                               QgsMessageBar::WARNING, messageTimeout() );

  }
  else if ( nCopiedFeatures == nTotalFeatures )
  {
    messageBar()->pushMessage( tr( "Paste features" ),
                               tr( "%1 features were successfully pasted." ).arg( nCopiedFeatures ),
                               QgsMessageBar::INFO, messageTimeout() );
  }
  else
  {
    messageBar()->pushMessage( tr( "Paste features" ),
                               tr( "%1 of %2 features could be successfully pasted." ).arg( nCopiedFeatures ).arg( nTotalFeatures ),
                               QgsMessageBar::WARNING, messageTimeout() );
  }

  canvas_xy->refresh();
}

void MainWindow::pasteAsNewVector()
{

  QgsVectorLayer *layer = pasteToNewMemoryVector();
  if ( !layer )
    return;

  saveAsVectorFileGeneral( layer, false );

  delete layer;
}

QgsVectorLayer *MainWindow::pasteAsNewMemoryVector( const QString & theLayerName )
{

  QString layerName = theLayerName;

  if ( layerName.isEmpty() )
  {
    bool ok;
    QString defaultName = tr( "Pasted" );
    layerName = QInputDialog::getText( this, tr( "New temporary scratch layer name" ),
                                       tr( "Layer name" ), QLineEdit::Normal,
                                       defaultName, &ok );
    if ( !ok )
      return nullptr;

    if ( layerName.isEmpty() )
    {
      layerName = defaultName;
    }
  }

  QgsVectorLayer *layer = pasteToNewMemoryVector();
  if ( !layer )
    return nullptr;

  layer->setLayerName( layerName );

  canvas_xy->freeze();

  QgsMapLayerRegistry::instance()->addMapLayer( layer );

  canvas_xy->freeze( false );
  canvas_xy->refresh();

  return layer;
}

QgsVectorLayer *MainWindow::pasteToNewMemoryVector()
{
  // Decide geometry type from features, switch to multi type if at least one multi is found
  QMap<QGis::WkbType, int> typeCounts;
  QgsFeatureList features = clipboard()->copyOf();
  for ( int i = 0; i < features.size(); i++ )
  {
    QgsFeature &feature = features[i];
    if ( !feature.constGeometry() )
      continue;

    QGis::WkbType type = QGis::flatType( feature.constGeometry()->wkbType() );

    if ( type == QGis::WKBUnknown || type == QGis::WKBNoGeometry )
      continue;

    if ( QGis::isSingleType( type ) )
    {
      if ( typeCounts.contains( QGis::multiType( type ) ) )
      {
        typeCounts[ QGis::multiType( type )] = typeCounts[ QGis::multiType( type )] + 1;
      }
      else
      {
        typeCounts[ type ] = typeCounts[ type ] + 1;
      }
    }
    else if ( QGis::isMultiType( type ) )
    {
      if ( typeCounts.contains( QGis::singleType( type ) ) )
      {
        // switch to multi type
        typeCounts[type] = typeCounts[ QGis::singleType( type )];
        typeCounts.remove( QGis::singleType( type ) );
      }
      typeCounts[type] = typeCounts[type] + 1;
    }
  }

  QGis::WkbType wkbType = !typeCounts.isEmpty() ? typeCounts.keys().value( 0 ) : QGis::WKBPoint;

  QString typeName = QString( QGis::featureType( wkbType ) ).remove( "WKB" );

  if ( features.isEmpty() )
  {
    // should not happen
    messageBar()->pushMessage( tr( "Paste features" ),
                               tr( "No features in clipboard." ),
                               QgsMessageBar::WARNING, messageTimeout() );
    return nullptr;
  }
  else if ( typeCounts.isEmpty() )
  {
    messageBar()->pushMessage( tr( "Paste features" ),
                               tr( "No features with geometry found, point type layer will be created." ),
                               QgsMessageBar::INFO, messageTimeout() );
  }
  else if ( typeCounts.size() > 1 )
  {
    messageBar()->pushMessage( tr( "Paste features" ),
                               tr( "Multiple geometry types found, features with geometry different from %1 will be created without geometry." ).arg( typeName ),
                               QgsMessageBar::INFO, messageTimeout() );
  }

  typeName += QString( "?memoryid=%1" ).arg( QUuid::createUuid().toString() );
  QgsDebugMsg( QString( "output wkbType = %1 typeName = %2" ).arg( wkbType ).arg( typeName ) );

  QgsVectorLayer *layer = new QgsVectorLayer( typeName, "pasted_features", "memory" );

  if ( !layer->isValid() || !layer->dataProvider() )
  {
    delete layer;
    messageBar()->pushMessage( tr( "Paste features" ),
                               tr( "Cannot create new layer." ),
                               QgsMessageBar::WARNING, messageTimeout() );
    return nullptr;
  }

  layer->startEditing();
  layer->setCrs( clipboard()->crs(), false );

  Q_FOREACH ( QgsField f, clipboard()->fields().toList() )
  {
    QgsDebugMsg( QString( "field %1 (%2)" ).arg( f.name(), QVariant::typeToName( f.type() ) ) );
    if ( !layer->addAttribute( f ) )
    {
      messageBar()->pushMessage( tr( "Paste features" ),
                                 tr( "Cannot create field %1 (%2,%3)" ).arg( f.name(), f.typeName(), QVariant::typeToName( f.type() ) ),
                                 QgsMessageBar::WARNING, messageTimeout() );
      delete layer;
      return nullptr;
    }
  }

  // Convert to multi if necessary
  for ( int i = 0; i < features.size(); i++ )
  {
    QgsFeature &feature = features[i];
    if ( !feature.constGeometry() )
      continue;

    QGis::WkbType type = QGis::flatType( feature.constGeometry()->wkbType() );
    if ( type == QGis::WKBUnknown || type == QGis::WKBNoGeometry )
      continue;

    if ( QGis::singleType( wkbType ) != QGis::singleType( type ) )
    {
      feature.setGeometry( nullptr );
    }

    if ( QGis::isMultiType( wkbType ) &&  QGis::isSingleType( type ) )
    {
      feature.geometry()->convertToMultiType();
    }
  }
  if ( ! layer->addFeatures( features ) || !layer->commitChanges() )
  {
    QgsDebugMsg( "Cannot add features or commit changes" );
    delete layer;
    return nullptr;
  }
  layer->removeSelection();

  QgsDebugMsg( QString( "%1 features pasted to temporary scratch layer" ).arg( layer->featureCount() ) );
  return layer;
}

void MainWindow::copyStyle( QgsMapLayer * sourceLayer )
{
  QgsMapLayer *selectionLayer = sourceLayer ? sourceLayer : activeLayer();

  if ( selectionLayer )
  {
    QString errorMsg;
    QDomDocument doc( "qgis" );
    selectionLayer->exportNamedStyle( doc, errorMsg );


    if ( !errorMsg.isEmpty() )
    {
      messageBar()->pushMessage( tr( "Cannot copy style" ),
                                 errorMsg,
                                 QgsMessageBar::CRITICAL, messageTimeout() );
      return;
    }
    // Copies data in text form as well, so the XML can be pasted into a text editor
    clipboard()->setData( QGSCLIPBOARD_STYLE_MIME, doc.toByteArray(), doc.toString() );
    // Enables the paste menu element
    mActionPasteStyle->setEnabled( true );
  }
}
/**
   \param destinationLayer  The layer that the clipboard will be pasted to
                            (defaults to the active layer on the legend)
 */


void MainWindow::pasteStyle( QgsMapLayer * destinationLayer )
{
  QgsMapLayer *selectionLayer = destinationLayer ? destinationLayer : activeLayer();
  if ( selectionLayer )
  {
    if ( clipboard()->hasFormat( QGSCLIPBOARD_STYLE_MIME ) )
    {
      QDomDocument doc( "qgis" );
      QString errorMsg;
      int errorLine, errorColumn;
      if ( !doc.setContent( clipboard()->data( QGSCLIPBOARD_STYLE_MIME ), false, &errorMsg, &errorLine, &errorColumn ) )
      {

        messageBar()->pushMessage( tr( "Cannot parse style" ),
                                   errorMsg,
                                   QgsMessageBar::CRITICAL, messageTimeout() );
        return;
      }

      bool isVectorStyle = doc.elementsByTagName( "pipe" ).isEmpty();
      if (( selectionLayer->type() == QgsMapLayer::RasterLayer && isVectorStyle ) ||
          ( selectionLayer->type() == QgsMapLayer::VectorLayer && !isVectorStyle ) )
      {
        return;
      }

      if ( !selectionLayer->importNamedStyle( doc, errorMsg ) )
      {
        messageBar()->pushMessage( tr( "Cannot paste style" ),
                                   errorMsg,
                                   QgsMessageBar::CRITICAL, messageTimeout() );
        return;
      }

      mLayerTreeView->refreshLayerSymbology( selectionLayer->id() );
      canvas_xy->clearCache();
      canvas_xy->refresh();
    }
  }
}

void MainWindow::copyFeatures( QgsFeatureStore & featureStore )
{
  clipboard()->replaceWithCopyOf( featureStore );
}

void MainWindow::refreshMapCanvas()
{
  //stop any current rendering
  canvas_xy->stopRendering();

  canvas_xy->refreshAllLayers();
}

void MainWindow::canvasRefreshStarted()
{
  showProgress( -1, 0 ); // trick to make progress bar show busy indicator
}

void MainWindow::canvasRefreshFinished()
{
  showProgress( 0, 0 ); // stop the busy indicator
}

void MainWindow::toggleMapTips()
{
  mMapTipsVisible = !mMapTipsVisible;
  // if off, stop the timer
  if ( !mMapTipsVisible )
  {
    mpMapTipsTimer->stop();
  }
}


void MainWindow::toggleEditing()
{
  QgsVectorLayer *currentLayer = qobject_cast<QgsVectorLayer*>( activeLayer() );
  if ( currentLayer )
  {
    toggleEditing( currentLayer, true );
  }
  else
  {
    // active although there's no layer active!?
    mActionToggleEditing->setChecked( false );
  }
}

bool MainWindow::toggleEditing( QgsMapLayer *layer, bool allowCancel )
{
  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
  if ( !vlayer )
  {
    return false;
  }

  bool res = true;

  QString connString = QgsDataSourceURI( vlayer->source() ).connectionInfo();
  QString key = vlayer->providerType();

  QMap< QPair< QString, QString>, QgsTransactionGroup*>::iterator tIt = mTransactionGroups.find( qMakePair( key, connString ) );
  QgsTransactionGroup* tg = ( tIt != mTransactionGroups.end() ? tIt.value() : nullptr );

  bool isModified = false;

  // Assume changes if: a) the layer reports modifications or b) its transaction group was modified
  if ( vlayer->isModified() || ( tg && tg->layers().contains( vlayer ) && tg->modified() ) )
    isModified  = true;

  if ( !vlayer->isEditable() && !vlayer->isReadOnly() )
  {
    if ( !( vlayer->dataProvider()->capabilities() & QgsVectorDataProvider::EditingCapabilities ) )
    {
      mActionToggleEditing->setChecked( false );
      mActionToggleEditing->setEnabled( false );
      messageBar()->pushMessage( tr( "Start editing failed" ),
                                 tr( "Provider cannot be opened for editing" ),
                                 QgsMessageBar::INFO, messageTimeout() );
      return false;
    }

    vlayer->startEditing();

    QSettings settings;
    QString markerType = settings.value( "/qgis/digitizing/marker_style", "Cross" ).toString();
    bool markSelectedOnly = settings.value( "/qgis/digitizing/marker_only_for_selected", false ).toBool();

    // redraw only if markers will be drawn
    if (( !markSelectedOnly || vlayer->selectedFeatureCount() > 0 ) &&
        ( markerType == "Cross" || markerType == "SemiTransparentCircle" ) )
    {
      vlayer->triggerRepaint();
    }
  }
  else if ( isModified )
  {
    QMessageBox::StandardButtons buttons = QMessageBox::Save | QMessageBox::Discard;
    if ( allowCancel )
      buttons |= QMessageBox::Cancel;

    switch ( QMessageBox::information( nullptr,
                                       tr( "Stop editing" ),
                                       tr( "Do you want to save the changes to layer %1?" ).arg( vlayer->name() ),
                                       buttons ) )
    {
      case QMessageBox::Cancel:
        res = false;
        break;

      case QMessageBox::Save:
        QApplication::setOverrideCursor( Qt::WaitCursor );

        if ( !vlayer->commitChanges() )
        {
          commitError( vlayer );
          // Leave the in-memory editing state alone,
          // to give the user a chance to enter different values
          // and try the commit again later
          res = false;
        }

        vlayer->triggerRepaint();

        QApplication::restoreOverrideCursor();
        break;

      case QMessageBox::Discard:
        QApplication::setOverrideCursor( Qt::WaitCursor );

        canvas_xy->freeze( true );
        if ( !vlayer->rollBack() )
        {
          messageBar()->pushMessage( tr( "Error" ),
                                     tr( "Problems during roll back" ),
                                     QgsMessageBar::CRITICAL );
          res = false;
        }
        canvas_xy->freeze( false );

        vlayer->triggerRepaint();

        QApplication::restoreOverrideCursor();
        break;

      default:
        break;
    }
  }
  else //layer not modified
  {
    canvas_xy->freeze( true );
    vlayer->rollBack();
    canvas_xy->freeze( false );
    res = true;
    vlayer->triggerRepaint();
  }

  if ( !res && layer == activeLayer() )
  {
    // while also called when layer sends editingStarted/editingStopped signals,
    // this ensures correct restoring of gui state if toggling was canceled
    // or layer commit/rollback functions failed
    activateDeactivateLayerRelatedActions( layer );
  }

  return res;
}






// slot to update the progress bar in the status bar
void MainWindow::showProgress( int theProgress, int theTotalSteps )
{
  if ( theProgress == theTotalSteps )
  {
    mProgressBar->reset();
    mProgressBar->hide();
  }
  else
  {
    //only call show if not already hidden to reduce flicker
    if ( !mProgressBar->isVisible() )
    {
      mProgressBar->show();
    }
    mProgressBar->setMaximum( theTotalSteps );
    mProgressBar->setValue( theProgress );

    if ( mProgressBar->maximum() == 0 )
    {
      // for busy indicator (when minimum equals to maximum) the oxygen Qt style (used in KDE)
      // has some issues and does not start busy indicator animation. This is an ugly fix
      // that forces creation of a temporary progress bar that somehow resumes the animations.
      // Caution: looking at the code below may introduce mild pain in stomach.
      if ( strcmp( QApplication::style()->metaObject()->className(), "Oxygen::Style" ) == 0 )
      {
        QProgressBar pb;
        pb.setAttribute( Qt::WA_DontShowOnScreen ); // no visual annoyance
        pb.setMaximum( 0 );
        pb.show();
        qApp->processEvents();
      }
    }

  }
}
//void MainWindow::mapToolChanged( QgsMapTool *newTool, QgsMapTool *oldTool )
//{
//  if ( oldTool )
//  {
//    disconnect( oldTool, SIGNAL( messageEmitted( QString ) ), this, SLOT( displayMapToolMessage( QString ) ) );
//    disconnect( oldTool, SIGNAL( messageEmitted( QString, QgsMessageBar::MessageLevel ) ), this, SLOT( displayMapToolMessage( QString, QgsMessageBar::MessageLevel ) ) );
//    disconnect( oldTool, SIGNAL( messageDiscarded() ), this, SLOT( removeMapToolMessage() ) );
//  }

//  if ( newTool )
//  {
//    if ( !newTool->isEditTool() )
//    {
//      mNonEditMapTool = newTool;
//    }

//    connect( newTool, SIGNAL( messageEmitted( QString ) ), this, SLOT( displayMapToolMessage( QString ) ) );
//    connect( newTool, SIGNAL( messageEmitted( QString, QgsMessageBar::MessageLevel ) ), this, SLOT( displayMapToolMessage( QString, QgsMessageBar::MessageLevel ) ) );
//    connect( newTool, SIGNAL( messageDiscarded() ), this, SLOT( removeMapToolMessage() ) );
//  }
//}

void MainWindow::showMapCanvas()
{
  // Map layers changed -> switch to map canvas
//  if ( mCentralContainer )
//    mCentralContainer->setCurrentIndex( 0 );
}

void MainWindow::markDirty()
{
  // notify the project that there was a change
  QgsProject::instance()->dirty( true );
}

void MainWindow::extentChanged()
{
  // allow symbols in the legend update their preview if they use map units
  mLayerTreeView->layerTreeModel()->setLegendMapViewData( canvas_xy->mapUnitsPerPixel(), canvas_xy->mapSettings().outputDpi(), canvas_xy->scale() );
}

void MainWindow::layersWereAdded( const QList<QgsMapLayer *>& theLayers )
{
  Q_FOREACH ( QgsMapLayer* layer, theLayers )
  {
    QgsDataProvider *provider = nullptr;

    QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
    if ( vlayer )
    {
      // notify user about any font family substitution, but only when rendering labels (i.e. not when opening settings dialog)
      connect( vlayer, SIGNAL( labelingFontNotFound( QgsVectorLayer*, QString ) ), this, SLOT( labelingFontNotFound( QgsVectorLayer*, QString ) ) );

      QgsVectorDataProvider* vProvider = vlayer->dataProvider();
      if ( vProvider && vProvider->capabilities() & QgsVectorDataProvider::EditingCapabilities )
      {
        connect( vlayer, SIGNAL( layerModified() ), this, SLOT( updateLayerModifiedActions() ) );
        connect( vlayer, SIGNAL( editingStarted() ), this, SLOT( layerEditStateChanged() ) );
        connect( vlayer, SIGNAL( editingStopped() ), this, SLOT( layerEditStateChanged() ) );
      }

      if ( autoTransaction() )
      {
        if ( QgsTransaction::supportsTransaction( vlayer ) )
        {
          QString connString = QgsDataSourceURI( vlayer->source() ).connectionInfo();
          QString key = vlayer->providerType();

          QgsTransactionGroup* tg = mTransactionGroups.value( qMakePair( key, connString ) );

          if ( !tg )
          {
            tg = new QgsTransactionGroup();
            mTransactionGroups.insert( qMakePair( key, connString ), tg );

            connect( tg, SIGNAL( commitError( QString ) ), this, SLOT( displayMapToolMessage( QString ) ) );
          }
          tg->addLayer( vlayer );
        }
      }

      connect( vlayer, SIGNAL( raiseError( QString ) ), this, SLOT( onLayerError( QString ) ) );

      provider = vProvider;
    }

    QgsRasterLayer *rlayer = qobject_cast<QgsRasterLayer *>( layer );
    if ( rlayer )
    {
      // connect up any request the raster may make to update the app progress
      connect( rlayer, SIGNAL( drawingProgress( int, int ) ), this, SLOT( showProgress( int, int ) ) );

      // connect up any request the raster may make to update the statusbar message
      connect( rlayer, SIGNAL( statusChanged( QString ) ), this, SLOT( showStatusMessage( QString ) ) );

      provider = rlayer->dataProvider();
    }

    if ( provider )
    {
      connect( provider, SIGNAL( dataChanged() ), layer, SLOT( triggerRepaint() ) );
      connect( provider, SIGNAL( dataChanged() ), canvas_xy, SLOT( refresh() ) );
    }
  }
}

void MainWindow::showRotation()
{
  // update the statusbar with the current rotation.
  double myrotation = canvas_xy->rotation();
//  mRotationEdit->setValue( myrotation );
} // MainWindow::showRotation


void MainWindow::updateMouseCoordinatePrecision()
{
//  mCoordsEdit->setMouseCoordinatesPrecision( QgsCoordinateUtils::calculateCoordinatePrecision( mapCanvas()->mapUnitsPerPixel(), mapCanvas()->mapSettings().destinationCrs() ) );
}

void MainWindow::showStatusMessage( const QString& theMessage )
{
  statusBar()->showMessage( theMessage );
}

void MainWindow::displayMapToolMessage( const QString& message, QgsMessageBar::MessageLevel level )
{
  // remove previous message
//  messageBar()->popWidget( mLastMapToolMessage );

//  QgsMapTool* tool = mapCanvas()->mapTool();

//  if ( tool )
//  {
//    mLastMapToolMessage = new QgsMessageBarItem( tool->toolName(), message, level, messageTimeout() );
//    messageBar()->pushItem( mLastMapToolMessage );
//  }
}

void MainWindow::displayMessage( const QString& title, const QString& message, QgsMessageBar::MessageLevel level )
{
  messageBar()->pushMessage( title, message, level, messageTimeout() );
}

void MainWindow::removeMapToolMessage()
{
  // remove previous message
//  messageBar()->popWidget( mLastMapToolMessage );
}



//// Show the maptip using tooltip
//void MainWindow::showMapTip()
//{
//  // Stop the timer while we look for a maptip
//  mpMapTipsTimer->stop();

//  // Only show tooltip if the mouse is over the canvas
//  if ( canvas_xy->underMouse() )
//  {
//    QPoint myPointerPos = canvas_xy->mouseLastXY();

//    //  Make sure there is an active layer before proceeding
//    QgsMapLayer* mypLayer = canvas_xy->currentLayer();
//    if ( mypLayer )
//    {
//      //QgsDebugMsg("Current layer for maptip display is: " + mypLayer->source());
//      // only process vector layers
//      if ( mypLayer->type() == QgsMapLayer::VectorLayer )
//      {
//        // Show the maptip if the maptips button is depressed
//        if ( mMapTipsVisible )
//        {
//          mpMaptip->showMapTip( mypLayer, mLastMapPosition, myPointerPos, canvas_xy );
//        }
//      }
//    }
//    else
//    {
//      showStatusMessage( tr( "Maptips require an active layer" ) );
//    }
//  }
//}


void MainWindow::commitError( QgsVectorLayer* vlayer )
{
  QgsMessageViewer *mv = new QgsMessageViewer();
  mv->setWindowTitle( tr( "Commit errors" ) );
  mv->setMessageAsPlainText( tr( "Could not commit changes to layer %1" ).arg( vlayer->name() )
                             + "\n\n"
                             + tr( "Errors: %1\n" ).arg( vlayer->commitErrors().join( "\n  " ) )
                           );

  QToolButton *showMore = new QToolButton();
  // store pointer to vlayer in data of QAction
  QAction *act = new QAction( showMore );
  act->setData( QVariant( QMetaType::QObjectStar, &vlayer ) );
  act->setText( tr( "Show more" ) );
  showMore->setStyleSheet( "background-color: rgba(255, 255, 255, 0); color: black; text-decoration: underline;" );
  showMore->setCursor( Qt::PointingHandCursor );
  showMore->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Preferred );
  showMore->addAction( act );
  showMore->setDefaultAction( act );
  connect( showMore, SIGNAL( triggered( QAction* ) ), mv, SLOT( exec() ) );
  connect( showMore, SIGNAL( triggered( QAction* ) ), showMore, SLOT( deleteLater() ) );

  // no timeout set, since notice needs attention and is only shown first time layer is labeled
  QgsMessageBarItem *errorMsg = new QgsMessageBarItem(
    tr( "Commit errors" ),
    tr( "Could not commit changes to layer %1" ).arg( vlayer->name() ),
    showMore,
    QgsMessageBar::WARNING,
    0,
    messageBar() );
  messageBar()->pushItem( errorMsg );
}






//void MainWindow::zoomIn()
//{
//  QgsDebugMsg( "Setting map tool to zoomIn" );

//  canvas_xy->setMapTool( mMapTools.mZoomIn );
//}


//void MainWindow::zoomOut()
//{
//  canvas_xy->setMapTool( mMapTools.mZoomOut );
//}

void MainWindow::zoomToSelected()
{
  canvas_xy->zoomToSelected();
}

void MainWindow::panToSelected()
{
  canvas_xy->panToSelected();
}

//void MainWindow::pan()
//{
//  canvas_xy->setMapTool( mMapTools.mPan );
//}

//#ifdef HAVE_TOUCH
//void MainWindow::touch()
//{
//  canvas_xy->setMapTool( mMapTools.mTouch );
//}
//#endif

void MainWindow::zoomFull()
{
  canvas_xy->zoomToFullExtent();
}

void MainWindow::zoomToPrevious()
{
  canvas_xy->zoomToPreviousExtent();
}

void MainWindow::zoomToNext()
{
  canvas_xy->zoomToNextExtent();
}

//void MainWindow::zoomActualSize()
//{
//  legendLayerZoomNative();
//}

//void MainWindow::identify()
//{
//  canvas_xy->setMapTool( mMapTools.mIdentify );
//}
int MainWindow::messageTimeout()
{
  QSettings settings;
  return settings.value( "/qgis/messageTimeout", 5 ).toInt();
}





void MainWindow::updateUndoActions()
{
  bool canUndo = false, canRedo = false;
  QgsMapLayer* layer = activeLayer();
  if ( layer )
  {
    QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer *>( layer );
    if ( vlayer && vlayer->isEditable() )
    {
      canUndo = vlayer->undoStack()->canUndo();
      canRedo = vlayer->undoStack()->canRedo();
    }
  }
  mActionUndo->setEnabled( canUndo );
  mActionRedo->setEnabled( canRedo );
}

void MainWindow::selectionChanged( QgsMapLayer *layer )
{
  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
  if ( vlayer )
  {
    showStatusMessage( tr( "%n feature(s) selected on layer %1.", "number of selected features", vlayer->selectedFeatureCount() ).arg( vlayer->name() ) );
  }
  if ( layer == activeLayer() )
  {
    activateDeactivateLayerRelatedActions( layer );
  }
}


void MainWindow::doFeatureAction()
{
  canvas_xy->setMapTool( mMapTools.mFeatureAction );
}

void MainWindow::updateDefaultFeatureAction( QAction *action )
{
  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( activeLayer() );
  if ( !vlayer )
    return;

  mFeatureActionMenu->setActiveAction( action );

  int index = mFeatureActionMenu->actions().indexOf( action );

  if ( vlayer->actions()->size() > 0 && index < vlayer->actions()->size() )
  {
    vlayer->actions()->setDefaultAction( index );
    QgsMapLayerActionRegistry::instance()->setDefaultActionForLayer( vlayer, nullptr );
  }
  else
  {
    //action is from QgsMapLayerActionRegistry
    vlayer->actions()->setDefaultAction( -1 );

    QgsMapLayerAction * mapLayerAction = dynamic_cast<QgsMapLayerAction *>( action );
    if ( mapLayerAction )
    {
      QgsMapLayerActionRegistry::instance()->setDefaultActionForLayer( vlayer, mapLayerAction );
    }
    else
    {
      QgsMapLayerActionRegistry::instance()->setDefaultActionForLayer( vlayer, nullptr );
    }
  }

  doFeatureAction();
}

void MainWindow::refreshFeatureActions()
{
  mFeatureActionMenu->clear();

  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( activeLayer() );
  if ( !vlayer )
    return;

  QgsAttributeAction *actions = vlayer->actions();
  for ( int i = 0; i < actions->size(); i++ )
  {
    QAction *action = mFeatureActionMenu->addAction( actions->at( i ).name() );
    if ( i == actions->defaultAction() )
    {
      mFeatureActionMenu->setActiveAction( action );
    }
  }

  //add actions registered in QgsMapLayerActionRegistry
  QList<QgsMapLayerAction *> registeredActions = QgsMapLayerActionRegistry::instance()->mapLayerActions( vlayer );
  if ( actions->size() > 0 && registeredActions.size() > 0 )
  {
    //add a separator between user defined and standard actions
    mFeatureActionMenu->addSeparator();
  }

  for ( int i = 0; i < registeredActions.size(); i++ )
  {
    mFeatureActionMenu->addAction( registeredActions.at( i ) );
    if ( registeredActions.at( i ) == QgsMapLayerActionRegistry::instance()->defaultActionForLayer( vlayer ) )
    {
      mFeatureActionMenu->setActiveAction( registeredActions.at( i ) );
    }
  }

}


QgsClipboard * MainWindow::clipboard()
{
  return mInternalClipboard;
}


