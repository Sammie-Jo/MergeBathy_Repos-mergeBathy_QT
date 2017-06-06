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

#include <commands.h>

#include "qtpropertymanager.h"
#include "qteditorfactory.h"
#include "qttreepropertybrowser.h"
#include "qtbuttonpropertybrowser.h"
#include "qtgroupboxpropertybrowser.h"
#include "qtmbparams.h"
#include "variantmanager.h"
#include "variantfactory.h"
#include "filepathmanager.h"
#include "fileeditfactory.h"

// QGIS Includes
#ifdef Qt5
   #include <qgssinglesymbolrenderer.h>
#else
    #include <qgssinglesymbolrendererv2.h>
#endif

#include <QDockWidget>
#include <QClipboard>
#include <QProcess>

//
// QGIS Specific Includes
//
#include <qgsfeaturerequest.h>
//for check if /Temp directory exists
#include <sys/types.h>
//#include <sys/stat.h>
#include "string"

// basic file operations
#include <iostream>
#include <fstream>
#include <qgsexpression.h>

#include "mbguiapp.h"
#include "mbguiappinterface.h"
//#include "mbguippstylesheet.h"
//#include "qgis.h"
//#include "qgisplugin.h"
//#include "qgsabout.h"
//#include "qgsalignrasterdialog.h"
//#include "qgsapplayertreeviewmenuprovider.h"
#include "qgsapplication.h"
#ifdef Qt5
    #include "qgsactionmanager.h"
#else
    #include "qgsattributeaction.h"
#endif
#include "qgsattributetabledialog.h"
//#include "qgsauthmanager.h"
//#include "qgsauthguiutils.h"
//#ifndef QT_NO_OPENSSL
//#include "qgsauthcertutils.h"
//#include "qgsauthsslerrorsdialog.h"
//#endif
//#include "qgsbookmarks.h"
//#include "qgsbrowserdockwidget.h"
//#include "qgsadvanceddigitizingdockwidget.h"
#include "qgsclipboard.h"
//#include "qgscomposer.h"
//#include "qgscomposermanager.h"
//#include "qgscomposerview.h"
//#include "qgsstatusbarcoordinateswidget.h"
//#include "qgsconfigureshortcutsdialog.h"
//#include "qgscoordinatetransform.h"
#include "qgscoordinateutils.h"
//#include "qgscredentialdialog.h"
//#include "qgscursors.h"
//#include "qgscustomization.h"
#include "qgscustomlayerorderwidget.h"
//#include "qgscustomprojectiondialog.h"
//#include "qgsdatasourceuri.h"
#include "qgsdatumtransformdialog.h"
#include "qgsdoublespinbox.h"
//#include "qgsdxfexport.h"
//#include "qgsdxfexportdialog.h"
//#include "qgsdecorationcopyright.h"
//#include "qgsdecorationnortharrow.h"
//#include "qgsdecorationscalebar.h"
//#include "qgsdecorationgrid.h"
//#include "qgsencodingfiledialog.h"
//#include "qgserror.h"
//#include "qgserrordialog.h"
//#include "qgsexception.h"
#include "qgsexpressionselectiondialog.h"
//#include "qgsfeature.h"
//#include "qgsformannotationitem.h"
#include "qgsfieldcalculator.h"
//#include "qgshtmlannotationitem.h"
//#include "qgsgenericprojectionselector.h"
//#include "qgsgpsinformationwidget.h"
//#include "qgsguivectorlayertools.h"
//#include "qgslabelingwidget.h"
//#include "qgslayerdefinition.h"
#include "qgslayertree.h"
#include "qgslayertreemapcanvasbridge.h"
#include "qgslayertreemodel.h"
#include "qgslayertreemodellegendnode.h"
#include "qgslayertreeregistrybridge.h"
#include "qgslayertreeutils.h"
//#include "qgslayertreeview.h"
#include "qgslayertreeviewdefaultactions.h"
//#include "qgslogger.h"
#include "qgsmapcanvas.h"
//#include "qgsmapcanvassnappingutils.h"
//#include "qgsmapcanvastracer.h"
//#include "qgsmaplayer.h"
#ifndef Qt5
    #include "qgsmaplayerregistry.h"
#endif
#include "qgsmaplayerstyleguiutils.h"
#include "qgsmapoverviewcanvas.h"
//#include "qgsmaprenderer.h"
//#include "qgsmapsettings.h"
//#include "qgsmaptip.h"
//#include "qgsmergeattributesdialog.h"
#include "qgsmessageviewer.h"
#include "qgsmessagebar.h"
#include "qgsmessagebaritem.h"
#include "qgsmimedatautils.h"
//#include "qgsmessagelog.h"
//#include "qgsmultibandcolorrenderer.h"
//#include "qgsnewvectorlayerdialog.h"
//#include "qgsnewmemorylayerdialog.h"
//#include "qgsoptions.h"
//#include "qgspluginlayer.h"
//#include "qgspluginlayerregistry.h"
//#include "qgspluginmanager.h"
//#include "qgspluginregistry.h"
//#include "qgspoint.h"
#ifndef Qt5
    #include "qgshandlebadlayers.h"
#endif
#include "qgsproject.h"
//#include "qgsprojectlayergroupdialog.h"
//#include "qgsprojectproperties.h"
#include "qgsproviderregistry.h"
//#include "qgspythonrunner.h"
//#include "qgsquerybuilder.h"
//#include "qgsrastercalcdialog.h"
//#include "qgsrasterfilewriter.h"
//#include "qgsrasteriterator.h"
#include "qgsrasterlayer.h"
#include "qgsrasterlayerproperties.h"
//#include "qgsrasternuller.h"
//#include "qgsbrightnesscontrastfilter.h"
#include "qgsrasterrenderer.h"
//#include "qgsrasterlayersaveasdialog.h"
//#include "qgsrectangle.h"
#include "qgsscalecombobox.h"
//#include "qgsscalevisibilitydialog.h"
//#include "qgsgroupwmsdatadialog.h"
//#include "qgsshortcutsmanager.h"
//#include "qgssinglebandgrayrenderer.h"
//#include "qgssnappingdialog.h"
//#include "qgssponsors.h"
#include "qgsstatisticalsummarydockwidget.h"
#ifdef Qt5
    #include "qgssymbolselectordialog.h"
    #include "qgsstyle.h"
#else
    #include "qgssymbolv2selectordialog.h"
    #include "qgsstylev2.h"
#endif
//#include "qgssvgannotationitem.h"
//#include "qgstextannotationitem.h"
//#include "qgstipgui.h"
#include "qgsundowidget.h"
//#include "qgsuserinputdockwidget.h"
#include "qgsvectordataprovider.h"
//#include "qgsvectorfilewriter.h"
#include "qgsvectorlayer.h"
//#include "qgsvectorlayerproperties.h"
#include "qgsvisibilitypresets.h"
//#include "qgsmessagelogviewer.h"
#include "qgsdataitem.h"
#include "qgsmaplayeractionregistry.h"
#include "qgswelcomepage.h"
//#include "qgsmaprendererparalleljob.h"
//#include "qgsversioninfo.h"
//#include "qgslegendfilterbutton.h"
//#include "qgsvirtuallayerdefinition.h"
//#include "qgsvirtuallayerdefinitionutils.h"
#include "qgstransaction.h"
#include "qgstransactiongroup.h"

#include "qgssublayersdialog.h"
#include "ogr/qgsopenvectorlayerdialog.h"
#include "ogr/qgsvectorlayersaveasdialog.h"

//#include "qgsosmdownloaddialog.h"
//#include "qgsosmimportdialog.h"
//#include "qgsosmexportdialog.h"

//
// QGIS Map tools
//
//#include "qgsmaptooladdfeature.h"
//#include "qgsmaptooladdpart.h"
//#include "qgsmaptooladdring.h"
//#include "qgsmaptoolfillring.h"
//#include "qgsmaptoolannotation.h"
//#include "qgsmaptoolcircularstringcurvepoint.h"
//#include "qgsmaptoolcircularstringradius.h"
//#include "qgsmaptooldeletering.h"
//#include "qgsmaptooldeletepart.h"
//#include "qgsmaptoolfeatureaction.h"
//#include "qgsmaptoolformannotation.h"
//#include "qgsmaptoolhtmlannotation.h"
#include "qgsmaptoolidentifyaction.h"
//#include "qgsmaptoolmeasureangle.h"
//#include "qgsmaptoolmovefeature.h"
//#include "qgsmaptoolrotatefeature.h"
//#include "qgsmaptooloffsetcurve.h"
#include "qgsmaptoolpan.h"
#include "qgsmaptoolselect.h"
#include "qgsmaptoolselectrectangle.h"
#include "qgsmaptoolselectfreehand.h"
#include "qgsmaptoolselectpolygon.h"
#include "qgsmaptoolselectradius.h"
//#include "qgsmaptoolsvgannotation.h"
//#include "qgsmaptoolreshape.h"
#include "qgsmaptoolrotatepointsymbols.h"
//#include "qgsmaptoolsplitfeatures.h"
//#include "qgsmaptoolsplitparts.h"
//#include "qgsmaptooltextannotation.h"
#include "qgsmaptoolzoom.h"
//#include "qgsmaptoolsimplify.h"
//#include "qgsmeasuretool.h"
//#include "qgsmaptoolpinlabels.h"
//#include "qgsmaptoolshowhidelabels.h"
//#include "qgsmaptoolmovelabel.h"
//#include "qgsmaptoolrotatelabel.h"
//#include "qgsmaptoolchangelabelproperties.h"

#include "nodetool/qgsmaptoolnodetool.h"

// Editor widgets
//#include "qgseditorwidgetregistry.h"

#ifdef Qt5
//    #include <Q3DSurface> //Qt 5.8
//    #include <QtCanvas3D> //Qt 5.5
#else
//    #include <qwt3d_surfaceplot.h>
//    #include <qwt3d_function.h>
#endif

//using namespace Qwt3D;

/** Set the application title bar text

  If the current project title is null
  if the project file is null then
  set title text to just application name and version
  else
  set set title text to the project file name
  else
  set the title text to project title
  */
static void setTitleBarText_( QWidget & MBGuiApp )
{
  QString caption = MBGuiApp::tr( "QGIS " );

  if ( QString( QGis::QGIS_VERSION ).endsWith( "Master" ) )
  {
    caption += QString( "%1" ).arg( QGis::QGIS_DEV_VERSION );
  }
  else
  {
    caption += QGis::QGIS_VERSION;
  }

  if ( QgsProject::instance()->title().isEmpty() )
  {
    if ( QgsProject::instance()->fileName().isEmpty() )
    {
      // no project title nor file name, so just leave caption with
      // application name and version
    }
    else
    {
      QFileInfo projectFileInfo( QgsProject::instance()->fileName() );
      caption += " - " + projectFileInfo.completeBaseName();
    }
  }
  else
  {
    caption += " - " + QgsProject::instance()->title();
  }

  MBGuiApp.setWindowTitle( caption );
}

/* Creator function for output viewer*/
static QgsMessageOutput *messageOutputViewer_()
{
  if ( QThread::currentThread() == qApp->thread() )
    return new QgsMessageViewer( MBGuiApp::instance() );
  else
    return new QgsMessageOutputConsole();
}

static void customSrsValidation_( QgsCoordinateReferenceSystem &srs )
{
  MBGuiApp::instance()->emitCustomSrsValidation( srs );
}

void MBGuiApp::emitCustomSrsValidation( QgsCoordinateReferenceSystem &srs )
{
  emit customSrsValidation( srs );
}

void MBGuiApp::layerTreeViewDoubleClicked( const QModelIndex& index )
{
  Q_UNUSED( index )
  QSettings settings;
  switch ( settings.value( "/qgis/legendDoubleClickAction", 0 ).toInt() )
  {
    case 0:
    {
      //show properties
      if ( mLayerTreeView )
      {
        // if it's a legend node, open symbol editor directly
        if ( QgsSymbolV2LegendNode* node = dynamic_cast<QgsSymbolV2LegendNode*>( mLayerTreeView->currentLegendNode() ) )
        {
          const QgsSymbolV2* originalSymbol = node->symbol();
          if ( !originalSymbol )
            return;

          QScopedPointer< QgsSymbolV2 > symbol( originalSymbol->clone() );
          QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer*>( node->layerNode()->layer() );
          QgsSymbolV2SelectorDialog dlg( symbol.data(), QgsStyleV2::defaultStyle(), vlayer, this );
          dlg.setMapCanvas( canvas_xy );
          if ( dlg.exec() )
          {
            node->setSymbol( symbol.take() );
          }

          return;
        }
      }
      MBGuiApp::instance()->layerProperties();
      break;
    }
    case 1:
      MBGuiApp::instance()->attributeTable();
      break;
    default:
      break;
  }
}
//may need to save this in writeSettings and restore from readSetting to restore on openProject
void MBGuiApp::canvasDoubleClicked_depths(QMouseEvent *event)
{
    //show properties
    showLayerProperties(canvas_depths->layer(0));
}
void MBGuiApp::canvasDoubleClicked_error(QMouseEvent *event)
{
    //show properties
    showLayerProperties(canvas_error->layer(0));
}
void MBGuiApp::canvasDoubleClicked_nei(QMouseEvent *event)
{
    //show properties
    showLayerProperties(canvas_nei->layer(0));
}
void MBGuiApp::canvasDoubleClicked_rei(QMouseEvent *event)
{
    //show properties
    showLayerProperties(canvas_rei->layer(0));
}
void MBGuiApp::canvasDoubleClicked_mean(QMouseEvent *event)
{
    //show properties
    showLayerProperties(canvas_mean->layer(0));
}



static bool cmpByText_( QAction* a, QAction* b )
{
  return QString::localeAwareCompare( a->text(), b->text() ) < 0;
}

MBGuiApp *MBGuiApp::smInstance = nullptr;

void MBGuiApp::dragEnterEvent( QDragEnterEvent *event )
{
  if ( event->mimeData()->hasUrls() || event->mimeData()->hasFormat( "application/x-vnd.qgis.qgis.uri" ) )
  {
    event->acceptProposedAction();
  }
}

void MBGuiApp::dropEvent( QDropEvent *event )
{
  canvas_xy->freeze();
  // get the file list
  QList<QUrl>::iterator i;
  QList<QUrl>urls = event->mimeData()->urls();
  for ( i = urls.begin(); i != urls.end(); ++i )
  {
    QString fileName = i->toLocalFile();
#ifdef Q_OS_MAC
    // Mac OS X 10.10, under Qt4.8 ,changes dropped URL format
    // https://bugreports.qt.io/browse/QTBUG-40449
    // [pzion 20150805] Work around
    if ( fileName.startsWith( "/.file/id=" ) )
    {
      QgsDebugMsg( "Mac dropped URL with /.file/id= (converting)" );
      CFStringRef relCFStringRef =
        CFStringCreateWithCString(
          kCFAllocatorDefault,
          fileName.toUtf8().constData(),
          kCFStringEncodingUTF8
        );
      CFURLRef relCFURL =
        CFURLCreateWithFileSystemPath(
          kCFAllocatorDefault,
          relCFStringRef,
          kCFURLPOSIXPathStyle,
          false // isDirectory
        );
      CFErrorRef error = 0;
      CFURLRef absCFURL =
        CFURLCreateFilePathURL(
          kCFAllocatorDefault,
          relCFURL,
          &error
        );
      if ( !error )
      {
        static const CFIndex maxAbsPathCStrBufLen = 4096;
        char absPathCStr[maxAbsPathCStrBufLen];
        if ( CFURLGetFileSystemRepresentation(
               absCFURL,
               true, // resolveAgainstBase
               reinterpret_cast<UInt8 *>( &absPathCStr[0] ),
               maxAbsPathCStrBufLen ) )
        {
          fileName = QString( absPathCStr );
        }
      }
      CFRelease( absCFURL );
      CFRelease( relCFURL );
      CFRelease( relCFStringRef );
    }
#endif
    // seems that some drag and drop operations include an empty url
    // so we test for length to make sure we have something
    if ( !fileName.isEmpty() )
    {
      openFile( fileName );
    }
  }

  if ( QgsMimeDataUtils::isUriList( event->mimeData() ) )
  {
    QgsMimeDataUtils::UriList lst = QgsMimeDataUtils::decodeUriList( event->mimeData() );
    Q_FOREACH ( const QgsMimeDataUtils::Uri& u, lst )
    {
      QString uri = crsAndFormatAdjustedLayerUri( u.uri, u.supportedCrs, u.supportedFormats );

      if ( u.layerType == "vector" )
      {
        addVectorLayer( uri, u.name, u.providerKey );
      }
      else if ( u.layerType == "raster" )
      {
        addRasterLayer( uri, u.name, u.providerKey );
      }
//      else if ( u.layerType == "plugin" )
//      {
//        addPluginLayer( uri, u.name, u.providerKey );
//      }
    }
  }
  canvas_xy->freeze( false );
  canvas_xy->refresh();
  event->acceptProposedAction();
}

bool MBGuiApp::event( QEvent * event )
{
  bool done = false;
  if ( event->type() == QEvent::FileOpen )
  {
    // handle FileOpen event (double clicking a file icon in Mac OS X Finder)
    QFileOpenEvent *foe = static_cast<QFileOpenEvent *>( event );
    openFile( foe->file() );
    done = true;
  }
  else
  {
    // pass other events to base class
    done = QMainWindow::event( event );
  }
  return done;
}

MBGuiApp::~MBGuiApp()
{
    qDebug() << "~MBGuiApp()" << endl;
    delete checkBox_lock_canvas_xy;
    delete checkBox_lock_canvas_yz;
    delete checkBox_lock_canvas_depths;
    delete checkBox_lock_canvas_error;
    delete checkBox_lock_canvas_nei;
    delete checkBox_lock_canvas_rei;
    delete checkBox_lock_canvas_mean;

//    delete mpZoomInTool;
//    delete mpZoomOutTool;
//    delete mpPanTool;

    delete mpZoomInTool_xy;
    delete mpZoomOutTool_xy;
    delete mpPanTool_xy;
    delete mpZoomInTool_yz;
    delete mpZoomOutTool_yz;
    delete mpPanTool_yz;
    delete mpZoomInTool_depths;
    delete mpZoomOutTool_depths;
    delete mpPanTool_depths;
    delete mpZoomInTool_error;
    delete mpZoomOutTool_error;
    delete mpPanTool_error;
    delete mpZoomInTool_nei;
    delete mpZoomOutTool_nei;
    delete mpPanTool_nei;
    delete mpZoomInTool_rei;
    delete mpZoomOutTool_rei;
    delete mpPanTool_rei;
    delete mpZoomInTool_mean;
    delete mpZoomOutTool_mean;
    delete mpPanTool_mean;



    delete mpMapToolBar;

    delete canvas_xy;
    delete canvas_yz;
    delete canvas_depths;
    delete canvas_error;
    delete canvas_nei;
    delete canvas_rei;
    delete canvas_mean;

//    delete mpLayout;

    delete model;
    delete model_yz;
//    delete view;
//    delete mLayerTreeModel;
    delete mLayerTreeView;
    delete mLayerTreeView_yz;
    delete undoStack;
    delete undoView;

    delete deleteAction;
    delete addLayerAction;
    delete addGroupAction;
    delete undoAction;
    delete redoAction;
    delete exitAction;
    delete aboutAction;
    delete rasterPropertiesAction;

}
MBGuiApp::MBGuiApp(QWidget* parent, Qt::WFlags fl)
    : QMainWindow(parent,fl)
//    , mNonEditMapTool( nullptr )
    , mScaleLabel( nullptr )
    , mScaleEdit( nullptr )
    , mScaleEditValidator( nullptr )
    , mCoordsEdit( nullptr )
    , mRotationLabel( nullptr )
    , mRotationEdit( nullptr )
    , mRotationEditValidator( nullptr )
    , mProgressBar( nullptr )
    , mRenderSuppressionCBox( nullptr )
    , mOnTheFlyProjectionStatusLabel( nullptr )
    , mOnTheFlyProjectionStatusButton( nullptr )
    , mMessageButton( nullptr )
    , mFeatureActionMenu( nullptr )
    , mPopupMenu( nullptr )
//    , mDatabaseMenu( nullptr )
//    , mWebMenu( nullptr )
    , mToolPopupOverviews( nullptr )
    , mToolPopupDisplay( nullptr )
    , mLayerTreeCanvasBridge( nullptr )
//    , mSplash( splash )
    , mInternalClipboard( nullptr )
//    , mShowProjectionTab( false )
//    , mPythonUtils( nullptr )
//    , mComposerManager( nullptr )
//    , mpTileScaleWidget( nullptr )
//    , mpGpsWidget( nullptr )
//    , mTracer( nullptr )
//    , mSnappingUtils( nullptr )
    , mProjectLastModified()
    , mWelcomePage( nullptr )
//    , mCentralContainer( nullptr )
{
    smInstance = this;
//    mInternalClipboard = new QgsClipboard; // create clipboard
//    connect( mInternalClipboard, SIGNAL( changed() ), this, SLOT( clipboardChanged() ) );

    //required by Qt4 to initialise the ui
    setupUi(this);
    undoStack = new QUndoStack(this);


    //QGIS


    QSettings settings;
//    // set canvas color right away
//    int myRed = settings.value( "/qgis/default_canvas_color_red", 255 ).toInt();
//    int myGreen = settings.value( "/qgis/default_canvas_color_green", 255 ).toInt();
//    int myBlue = settings.value( "/qgis/default_canvas_color_blue", 255 ).toInt();
//    canvas_xy->setCanvasColor( QColor( myRed, myGreen, myBlue ) );

    // what type of project to auto-open
    mProjOpen = settings.value( "/qgis/projOpenAtLaunch", 0 ).toInt();

    mWelcomePage = new QgsWelcomePage( TRUE ); //skipVersionCheck

//    // User Input Dock Widget //NEEDED TO ROTATE FEATURE-
//    mUserInputDockWidget = new QgsUserInputDockWidget( this );
//    mUserInputDockWidget->setObjectName( "UserInputDockWidget" );

    mLayerTreeView = new QgsLayerTreeView( this );
    mLayerTreeView->setObjectName( "theLayerTreeView" ); // "theLayerTreeView" used to find this canonical instance later



    // create undo widget
    mUndoWidget = new QgsUndoWidget( nullptr, canvas_xy );
    mUndoWidget->setObjectName( "Undo" );

    // Statistical Summary dock
    mStatisticalSummaryDockWidget = new QgsStatisticalSummaryDockWidget( this );
    mStatisticalSummaryDockWidget->setObjectName( "StatistalSummaryDockWidget" );
    mStatisticalSummaryDockWidget->setFloating(true);

    createActions();
    createActionGroups();
    createMenus();
    createSubMenus();
//    createStatusBar();
    selectedMap = new QMap<QString,QStack<QgsFeatureIds>*>();
    selectedMap_yz = new QMap<QString,QStack<QgsFeatureIds>*>();
    layerSuffixes = new QMap<QString,QString>();

   //these are sams
    createUndoView();

    setWindowTitle(tr("MergeBathy GUI: Untitled Project"));

    readSettings();

    updateRecentProjectPaths();
//    updateProjectFromTemplates();
//    legendLayerSelectionChanged();
    mSaveRollbackInProgress = false;


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
//    QgsLayerTreeGroup *
    root = QgsProject::instance()->layerTreeRoot();
    root_yz = new QgsLayerTreeGroup("root_yz");


    QDockWidget *dock = new QDockWidget(tr("layerTree_yz"), this);
    mLayerTreeView_yz = new QgsLayerTreeView( dock );
    mLayerTreeView_yz->setObjectName( "theLayerTreeViewYZ" );
//    mLayerTreeView_yz->hide();
    dock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
//    dock->setFloating( true );
    dock->setWidget(mLayerTreeView_yz);
    //to use this overloaded function place after canvas_xy is instantiated
//    addDockWidget(Qt::LeftDockWidgetArea, dock);
    //else use this one
    QMainWindow::addDockWidget(Qt::LeftDockWidgetArea, dock);



    model = new QgsLayerTreeModel( root, this );
    model->setFlag( QgsLayerTreeModel::AllowNodeReorder );
    model->setFlag( QgsLayerTreeModel::AllowNodeRename );
    model->setFlag( QgsLayerTreeModel::AllowNodeChangeVisibility );
    model->setFlag( QgsLayerTreeModel::ShowLegendAsTree );
    model->setAutoCollapseLegendNodes( 10 );
    mLayerTreeView->setModel( model );
    //this ones too fancy! let's use sams
//    mLayerTreeView->setMenuProvider( new QgsAppLayerTreeViewMenuProvider( mLayerTreeView, canvas_xy ) );

    model_yz = new QgsLayerTreeModel( root_yz, this );
    model_yz = new QgsLayerTreeModel(root_yz);
    model_yz->setFlag( QgsLayerTreeModel::AllowNodeReorder );
    model_yz->setFlag( QgsLayerTreeModel::AllowNodeRename );
    model_yz->setFlag( QgsLayerTreeModel::AllowNodeChangeVisibility );
    model_yz->setFlag( QgsLayerTreeModel::ShowLegendAsTree );
    model_yz->setAutoCollapseLegendNodes( 10 );
    mLayerTreeView_yz->setModel( model_yz );

//    mLayerTreeView_yz->setSelectionModel(mLayerTreeView->selectionModel());
//    QItemSelectionModel *selection = new QItemSelectionModel(model);
//    mLayerTreeView->setSelectionModel(selection);
//    mLayerTreeView_yz->setSelectionModel(selection);


    //Setup main layer view to show
    mLayerTreeView->setWhatsThis( tr( "Map legend that displays all the layers currently on the map canvas. Click on the check box to turn a layer on or off. Double click on a layer in the legend to customize its appearance and set other properties." ) );

    mLayerTreeDock = new QDockWidget( tr( "Layers Panel" ), this );
    mLayerTreeDock->setObjectName( "Layers" );
    mLayerTreeDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );

    layerTreeRegistryBridge = new QgsLayerTreeRegistryBridge(root,this);
    layerTreeRegistryBridge_yz = new QgsLayerTreeRegistryBridge(root_yz,this);

    // Create provider to context menu
    myProvider = new MyMenuProvider(mLayerTreeView,undoStack);
    mLayerTreeView->setMenuProvider(myProvider);

    // Create the Map Canvas
    canvas_xy = new QgsMapCanvas(0,"Canvas XY");
    mLayerTreeCanvasBridge = new QgsLayerTreeMapCanvasBridge(root, canvas_xy, this);

    canvas_xy->enableAntiAliasing(true);
    canvas_xy->setCanvasColor(QColor(255, 255, 255));
//    canvas_xy->freeze(false);
    canvas_xy->setVisible(true);
    canvas_xy->zoomToFullExtent();
    connect( canvas_xy, SIGNAL( messageEmitted( const QString&, const QString&, QgsMessageBar::MessageLevel ) ),
             this, SLOT( displayMessage( const QString&, const QString&, QgsMessageBar::MessageLevel ) ) );
    canvas_xy->setWhatsThis( tr( "XY-Map canvas. This is where the XY-plots of vector "
                                  "layers are displayed when added to the map" ) );

    canvas_yz = new QgsMapCanvas(0,"Canvas YZ");
    QgsLayerTreeMapCanvasBridge * bridge_canvas_yz = new QgsLayerTreeMapCanvasBridge(root_yz, canvas_yz);
    canvas_yz->zoomToFullExtent();
    connect( canvas_yz, SIGNAL( messageEmitted( const QString&, const QString&, QgsMessageBar::MessageLevel ) ),
             this, SLOT( displayMessage( const QString&, const QString&, QgsMessageBar::MessageLevel ) ) );
    canvas_yz->setWhatsThis( tr( "YZ-Map canvas. This is where the YZ-plots of vector "
                                  "layers are displayed when added to the map" ) );

    canvas_depths = new QgsMapCanvas(0,"Canvas Depths");
//    QgsLayerTreeMapCanvasBridge * bridge_canvas_depths = new QgsLayerTreeMapCanvasBridge(root, canvas_depths);
    canvas_depths->zoomToFullExtent();
    connect( canvas_depths, SIGNAL( messageEmitted( const QString&, const QString&, QgsMessageBar::MessageLevel ) ),
             this, SLOT( displayMessage( const QString&, const QString&, QgsMessageBar::MessageLevel ) ) );
    canvas_depths->setWhatsThis( tr( "Depth-Map canvas. This is where the Depth-plots of vector "
                                  "layers are displayed when added to the map" ) );

    canvas_error = new QgsMapCanvas(0,"Canvas Error");
//    QgsLayerTreeMapCanvasBridge * bridge_canvas_error = new QgsLayerTreeMapCanvasBridge(root, canvas_error);
    canvas_error->zoomToFullExtent();
    connect( canvas_error, SIGNAL( messageEmitted( const QString&, const QString&, QgsMessageBar::MessageLevel ) ),
             this, SLOT( displayMessage( const QString&, const QString&, QgsMessageBar::MessageLevel ) ) );
    canvas_error->setWhatsThis( tr( "Error-Map canvas. This is where the Error-plots of vector "
                                  "layers are displayed when added to the map" ) );

    canvas_nei = new QgsMapCanvas(0,"Canvas NEI");
//    QgsLayerTreeMapCanvasBridge * bridge_canvas_nei = new QgsLayerTreeMapCanvasBridge(root, canvas_nei);
    canvas_nei->zoomToFullExtent();
    canvas_nei->show();
    connect( canvas_nei, SIGNAL( messageEmitted( const QString&, const QString&, QgsMessageBar::MessageLevel ) ),
             this, SLOT( displayMessage( const QString&, const QString&, QgsMessageBar::MessageLevel ) ) );
    canvas_nei->setWhatsThis( tr( "NormalizedError-Map canvas. This is where the NormalizedError-plots of vector "
                                  "layers are displayed when added to the map" ) );

    canvas_rei = new QgsMapCanvas(0,"Canvas REI");
//    QgsLayerTreeMapCanvasBridge * bridge_canvas_rei = new QgsLayerTreeMapCanvasBridge(root, canvas_rei);
    canvas_rei->zoomToFullExtent();
    canvas_rei->show();
    connect( canvas_rei, SIGNAL( messageEmitted( const QString&, const QString&, QgsMessageBar::MessageLevel ) ),
             this, SLOT( displayMessage( const QString&, const QString&, QgsMessageBar::MessageLevel ) ) );
    canvas_rei->setWhatsThis( tr( "ResidualError-Map canvas. This is where the ResidualError-plots of vector "
                                  "layers are displayed when added to the map" ) );

    canvas_mean = new QgsMapCanvas(0,"Canvas Mean");
//    QgsLayerTreeMapCanvasBridge * bridge_canvas_mean = new QgsLayerTreeMapCanvasBridge(root, canvas_mean);
    canvas_mean->zoomToFullExtent();
    canvas_mean->show();
    connect( canvas_mean, SIGNAL( messageEmitted( const QString&, const QString&, QgsMessageBar::MessageLevel ) ),
             this, SLOT( displayMessage( const QString&, const QString&, QgsMessageBar::MessageLevel ) ) );
    canvas_mean->setWhatsThis( tr( "Mean-Map canvas. This is where the Mean-plots of vector "
                                  "layers are displayed when added to the map" ) );

    resultCanvases = new QList<QgsMapCanvas*>();
    resultCanvases->append(canvas_depths);
    resultCanvases->append(canvas_error);
    resultCanvases->append(canvas_nei);
    resultCanvases->append(canvas_rei);
    resultCanvases->append(canvas_mean);

    //this is sams i think
//    QDockWidget * legendDock = new QDockWidget("Layers",0);
//    legendDock->setObjectName("Layers");
//    legendDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
//    legendDock->setWidget(mLayerTreeView);
//    addDockWidget(Qt::LeftDockWidgetArea, legendDock);

    createMBParamsGroupBox();
//    createMapTools();
    createToolBars();
//    readSettings2();//sams

    checkBox_lock_canvas_xy = new QCheckBox("Lock");
    checkBox_lock_canvas_yz = new QCheckBox("Lock");
    checkBox_lock_canvas_depths = new QCheckBox("Lock");
    checkBox_lock_canvas_error = new QCheckBox("Lock");
    checkBox_lock_canvas_nei = new QCheckBox("Lock");
    checkBox_lock_canvas_rei = new QCheckBox("Lock");
    checkBox_lock_canvas_mean = new QCheckBox("Lock");
    checkBox_lock_canvas_xy->setChecked(true);
    checkBox_lock_canvas_yz->setChecked(true);
    checkBox_lock_canvas_depths->setChecked(true);
    checkBox_lock_canvas_error->setChecked(true);
    checkBox_lock_canvas_nei->setChecked(true);
    checkBox_lock_canvas_rei->setChecked(true);
    checkBox_lock_canvas_mean->setChecked(true);

    //These are not working yet! They need to update the maptool for the canvas
//    connect(checkBox_lock_canvas_xy, SIGNAL(toggled()),this,SLOT(refreshMapTools_xy()));
//    connect(checkBox_lock_canvas_yz, SIGNAL(toggled()),this,SLOT(refreshMapTools_yz()));
//    connect(checkBox_lock_canvas_depths, SIGNAL(toggled()),this,SLOT(refreshMapTools_depths()));
//    connect(checkBox_lock_canvas_error, SIGNAL(toggled()),this,SLOT(refreshMapTools_error()));
//    connect(checkBox_lock_canvas_nei, SIGNAL(toggled()),this,SLOT(refreshMapTools_nei()));
//    connect(checkBox_lock_canvas_rei, SIGNAL(toggled()),this,SLOT(refreshMapTools_rei()));
//    connect(checkBox_lock_canvas_mean, SIGNAL(toggled()),this,SLOT(refreshMapTools_mean()));

//    connect(canvas_xy, SIGNAL(mapCanvasRefreshed()),this,SLOT(refreshMapTools()));


    QLabel * label_title_canvas_xy = new QLabel("XY-Plot");
    QLabel * label_title_canvas_yz = new QLabel("YZ-Plot");
    QLabel * label_title_canvas_depths = new QLabel("Depths");
    QLabel * label_title_canvas_error = new QLabel("Error");
    QLabel * label_title_canvas_nei = new QLabel("Normalized Error");
    QLabel * label_title_canvas_rei = new QLabel("Residual Error");
    QLabel * label_title_canvas_mean = new QLabel("Mean");

    // Lay our widgets out in the main window
    hGroupBox_CanvasEdits = new QGroupBox(tr("Edit Layers"),frameMap);
    hGroupBox_CanvasResults = new QGroupBox(tr("Results"),frameMap);

    vGroupBox_canvas_xy = new QGroupBox(tr("Canvas XY Vertical layout"),frameMap);
    vGroupBox_canvas_yz = new QGroupBox(tr("Canvas YZ Vertical layout"),frameMap);
    vGroupBox_canvas_depths = new QGroupBox(tr("Canvas Depths Vertical layout"),frameMap);
    vGroupBox_canvas_error = new QGroupBox(tr("Canvas Error Vertical layout"),frameMap);
    vGroupBox_canvas_nei = new QGroupBox(tr("Canvas NE Vertical layout"),frameMap);
    vGroupBox_canvas_rei = new QGroupBox(tr("Canvas RE Vertical layout"),frameMap);
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
    mpLayout_canvas_depths = new QVBoxLayout(frameMap);
    mpLayout_canvas_depths->addWidget(label_title_canvas_depths);
    mpLayout_canvas_depths->addWidget(canvas_depths);
    mpLayout_canvas_depths->addWidget(checkBox_lock_canvas_depths);
    vGroupBox_canvas_depths->setLayout(mpLayout_canvas_depths);

    mpLayout_canvas_error = new QVBoxLayout(frameMap);
    mpLayout_canvas_error->addWidget(label_title_canvas_error);
    mpLayout_canvas_error->addWidget(canvas_error);
    mpLayout_canvas_error->addWidget(checkBox_lock_canvas_error);
    vGroupBox_canvas_error->setLayout(mpLayout_canvas_error);

    mpLayout_canvas_nei = new QVBoxLayout(frameMap);
    mpLayout_canvas_nei->addWidget(label_title_canvas_nei);
    mpLayout_canvas_nei->addWidget(canvas_nei);
    mpLayout_canvas_nei->addWidget(checkBox_lock_canvas_nei);
    vGroupBox_canvas_nei->setLayout(mpLayout_canvas_nei);

    mpLayout_canvas_rei = new QVBoxLayout(frameMap);
    mpLayout_canvas_rei->addWidget(label_title_canvas_rei);
    mpLayout_canvas_rei->addWidget(canvas_rei);
    mpLayout_canvas_rei->addWidget(checkBox_lock_canvas_rei);
    vGroupBox_canvas_rei->setLayout(mpLayout_canvas_rei);

    mpLayout_canvas_mean = new QVBoxLayout(frameMap);
    mpLayout_canvas_mean->addWidget(label_title_canvas_mean);
    mpLayout_canvas_mean->addWidget(canvas_mean);
    mpLayout_canvas_mean->addWidget(checkBox_lock_canvas_mean);
    vGroupBox_canvas_mean->setLayout(mpLayout_canvas_mean);

    mpLayout_CanvasResults = new QHBoxLayout(frameMap);
    mpLayout_CanvasResults->addWidget(vGroupBox_canvas_depths);
    mpLayout_CanvasResults->addWidget(vGroupBox_canvas_error);
    mpLayout_CanvasResults->addWidget(vGroupBox_canvas_nei);
    mpLayout_CanvasResults->addWidget(vGroupBox_canvas_rei);
    mpLayout_CanvasResults->addWidget(vGroupBox_canvas_mean);
    hGroupBox_CanvasResults->setLayout(mpLayout_CanvasResults);

    // Add Panels to MainLayout
    QVBoxLayout * mainLayout = new QVBoxLayout(frameMap);
    mainLayout->addWidget(hGroupBox_CanvasEdits);
    mainLayout->addWidget(hGroupBox_CanvasResults);

    // a bar to warn the user with non-blocking messages
    mInfoBar = new QgsMessageBar( frameMap );
    mInfoBar->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Fixed );
    mainLayout->addWidget( mInfoBar );

    setLayout(mainLayout);

    createStatusBar();
    createCanvasTools();

    canvas_xy->freeze();
    canvas_yz->freeze();
    canvas_depths->freeze();
    canvas_error->freeze();
    canvas_nei->freeze();
    canvas_rei->freeze();
    canvas_mean->freeze();

    initLayerTreeView();
    createOverview();
    createMapTips();
//    createDecorations();

//    QMainWindow::addDockWidget( Qt::BottomDockWidgetArea, mUserInputDockWidget );
//      mUserInputDockWidget->setFloating( true );

    // now build vector and raster file filters
    mVectorFileFilter = QgsProviderRegistry::instance()->fileVectorFilters();
    mRasterFileFilter = QgsProviderRegistry::instance()->fileRasterFilters();

    // set handler for missing layers (will be owned by QgsProject)
    QgsProject::instance()->setBadLayerHandler( new QgsHandleBadLayersHandler() );
    // Do this last in the ctor to ensure that all members are instantiated properly
    setupConnections();

     mMapTipsVisible = false;
    // setup drag drop
    setAcceptDrops( true );
//    mFullScreenMode = false;
//    mPrevScreenModeMaximized = false;
    show();
    //You can use QApplication.processEvents() to make the UI
    //appear to be alive while the processing is taking place.
    //processEvents() allows all pending UI events to be processed and the display updated.
    qApp->processEvents();

    canvas_xy->freeze( false );
    canvas_yz->freeze( false );
    canvas_depths->freeze( false );
    canvas_error->freeze( false );
    canvas_nei->freeze( false );
    canvas_rei->freeze( false );
    canvas_mean->freeze( false );
    canvas_xy->clearExtentHistory(); // reset zoomnext/zoomlast
    canvas_yz->clearExtentHistory();
    canvas_depths->clearExtentHistory();
    canvas_error->clearExtentHistory();
    canvas_nei->clearExtentHistory();
    canvas_rei->clearExtentHistory();
    canvas_mean->clearExtentHistory();

}

void MBGuiApp::runMB()
{
    qApp->processEvents();
    QApplication::setOverrideCursor( Qt::WaitCursor );

    int status = validateMBInputs();
    if(status == -1)
    {
        int ret = QMessageBox::critical(this, tr("MBGui"),
                                       tr("Tried to set mergeBathy.exe.\n"
                                          "Unable to run."),
                                       QMessageBox::Ok);
        return;
    }

    writeOutputFiles();

    QStringList args;

    QString outputFilenameFullPath = params->getWrkdir();
    outputFilenameFullPath.append("/").append(params->getOutputFilename());

    args << outputFilenameFullPath;
    args << QString::number(params->getGridSpacingX()) << QString::number(params->getGridSpacingY());
    args << params->getSmoothingFilterName();

    QString inputfilelistFullPath = params->getWrkdir();
    inputfilelistFullPath.append("/").append(params->getInputFilelist());

    args << inputfilelistFullPath;
    args << QString::number(params->getRefLL().x()) << QString::number(params->getRefLL().y()) << QString::number(params->getRotAngle());
    args << "-1" << "-nmsei" << "-msri" << "-multiThread" << QString::number(params->getNThreads());

    if(params->getSmoothingFilter() == 4) //kalman
        args << "-kalman" << QString::number(1);
    args << "-mse" << "1";
    if(params->getInputInMeters())
        args << "-inputInMeters";
    if(params->getModelFlag())
        args << "-modelflag";
    if(params->getNoNegDepths())
        args << "-nonegdepths";
    if(params->getUseUnscaledAvgInputs())
        args << "-useUnScaledAvgInputs";
    if(params->getPreSpliner() > 0)
    {
        if(params->getPreSpliner() == 1)
            args << "-GMTSurface";
        else if(params->getPreSpliner() == 2)
            args << "-ZGrid";

        args << QString::number(params->getPreSplinerSpacing().width()) << QString::number(params->getPreSplinerSpacing().height());
        args << QString::number(params->getPreSplinerTensionFactor());

        if(params->getPreSpliner() == 1)
            args << QString::number(params->getPreSplinerScaleFactor()) << QString::number(params->getPreSplinerAlpha());

        int usage;
        if(params->getPreSplinerUseResults())
            usage = 2;
        else usage = 1;
        if(params->getPreSplinerXYZE())
            usage = usage*-1;
        args << QString::number(usage);
    }
    if(params->getComputeOffsets())
        args << "-computeOffset";
    if(params->getKriging())
        args << "-kriging";

    if(params->getUsePreInterpolatedLocs())
    {
        args << "-preInterpolatedLocations" << params->getPreinterpolatedLocsFile();
        if(params->getPreinterpLocsUsage())
            args << "1";
        else args << "-1";
    }
    if(!params->getGridInMeters())
        args << "-llgrid";
    if(params->getSmoothInMeters())
        args << "-msmooth";
    else args << "-llsmooth";
    args << QString::number(params->getSmoothingScaleX()) << QString::number(params->getSmoothingScaleY());
    args << "-outputRasterFile";

    process = new QProcess(this);
    connect( process, SIGNAL(readyReadStandardOutput ()), this, SLOT(logMB()));
    connect( process, SIGNAL( finished(int, QProcess::ExitStatus)), this, SLOT(processExited(int, QProcess::ExitStatus)));
    process->setProcessChannelMode(QProcess::MergedChannels);

    process->start(params->getMBexe(), args);
    process->waitForFinished();

    qDebug() << "args: " << args <<endl;


    QApplication::restoreOverrideCursor();
    canvas_xy->freeze( false );
    canvas_xy->refresh();
    canvas_yz->freeze( false );
    canvas_yz->refresh();
    canvas_depths->freeze( false );
    canvas_depths->refresh();
    canvas_error->freeze( false );
    canvas_error->refresh();
    canvas_nei->freeze( false );
    canvas_nei->refresh();
    canvas_rei->freeze( false );
    canvas_rei->refresh();
    canvas_mean->freeze( false );
    canvas_mean->refresh();

    statusBar()->showMessage( tr( "MergeBathy run finished" ), 3000 );
}
void MBGuiApp::logMB()
{
    qDebug() << "Writing logMB()..." << endl;
    QByteArray data = process->readAllStandardOutput();

     std::ofstream outfile;
     QString logfileFullPath = params->getWrkdir();
     logfileFullPath.append("/").append(params->getLogfile());
     outfile.open (logfileFullPath.toLatin1().data());
     outfile << QString(data).toLatin1().data();
     outfile.close();

     logWindow->appendPlainText(QString(data));
     logWindow->adjustSize();
     docklog->adjustSize();
}

void MBGuiApp::processExited(int exitCode, QProcess::ExitStatus exitStatus)
{
    if(process->exitStatus() == process->NormalExit)
    {
        qDebug() << "Output Logfiles Created." << endl;
        loadMBResults();
    }
    else
    {
        qDebug() << "MB Run Failed!" << endl;
        showMBLog();
    }
    delete process;
    process = 0;
}

int MBGuiApp::validateMBInputs()
{
    // A. Check if local mergeBathy.exe is set
    QString mbpath;
    bool isLocalMBPathSet = QgsProject::instance()->readBoolEntry("myproject", "isLocalMBPath");
    if(isLocalMBPathSet)
        mbpath = localMBPath;//QgsProject::instance()->readEntry("myproject", "LocalMBPath");
    else
        mbpath = globalMBPath;//QgsProject::instance()->readEntry("myproject", "GlobalMBPath");

    QFile file(mbpath.append("/mergeBathy.exe"));
    if(!file.exists())
    {   // If local doesn't exist then try the global mergeBathy.exe
        if(isLocalMBPathSet)
        {
            QString mbpath = globalMBPath;//QgsProject::instance()->readEntry("myproject", "GlobalMBPath");
            QFile file(mbpath.append("/mergeBathy.exe"));
        }
        //Neither exists so let's ask the user for a valid mergeBathy.exe
        if(!file.exists())
        {
            int ret = QMessageBox::critical(this, tr("MBGui"),
                                           tr("mergeBathy.exe not found.\n"
                                              "Set the mergeBathy.exe path."),
                                           QMessageBox::Ok|QMessageBox::Cancel);
            if(ret == QMessageBox::Cancel)
                return -1;
            setLocalMBPath();
            mbpath = localMBPath;//QgsProject::instance()->readEntry("myproject", "LocalMBPath");
        }
        else return -1;
    }
    params->setMBexe(mbpath);

    // B. Check for work directory
    QDir dir;
    QString wrkdir = params->getWrkdir();
    if(wrkdir == "" || !dir.mkpath(wrkdir.toLatin1()))
    {
        qDebug() << "Failed to make directory: " << wrkdir << endl;
//        variantManager->setValue(propertyRunParams_wrkdir, QDir::temp().path().append("/MBGuiApp"));
        filePathManager->setValue(propertyRunParams_wrkdir, QDir::temp().path().append("/MBGuiApp"));
        wrkdir = params->getWrkdir();
        qDebug() << "Saving to temp directory: " << wrkdir << endl;
        if(!dir.mkpath(wrkdir.toLatin1()))
        {
            qDebug() << "Failed to make temp directory: " << wrkdir << endl;
            return -1;
        }
    }

    // C. Check InputFileList Name
    QString inputfilelist  = params->getInputFilelist();
    if(inputfilelist == "")
        inputfilelist = "inputfilelist.txt";
    else
    {
        //check for extension
        QString ext = "";
        int idx = inputfilelist.lastIndexOf(".txt");
        if(idx != inputfilelist.size()-4)
            inputfilelist.append(".txt");
    }
    params->setInputFilelist(inputfilelist);

    // D. Check for outputFilename
    QString outfile = params->getOutputFilename();
    if(outfile == "")
        outfile = "outputfile";
    else
    {
        //check for extension
        int idx = outfile.lastIndexOf(".txt");
        if(idx == outfile.size()-4)
            outfile.truncate(idx);
    }
    params->setOutputFilename(outfile);

    // E. Create log file
    params->setLogfile(params->getOutputFilename().append("_LOG.txt"));
    return 0;
}
void MBGuiApp::writeOutputFiles()
{
    std::ofstream outfile; //layer files
    std::ofstream outfilelist; //inputfilelist

//    std::cout.unsetf ( std::ios::floatfield );                // floatfield not set don't buffer with 0s
//    std::cout.setf( std::ios::fixed, std:: ios::floatfield ); // floatfield set to fixed buffers with 0s
//    outfile.precision(6);


    QString vectorFilename = params->getWrkdir().append("/").append(params->getInputFilelist());
    outfilelist.open (vectorFilename.toLatin1().data());

    // Get features to include // Don't return geometry objects
    QgsExpression exp = QgsExpression("exclude ILIKE \'%included%\'");
    QgsFeatureRequest request = QgsFeatureRequest(exp).setFlags( QgsFeatureRequest::NoGeometry );

    QgsFeature feature;
    QgsVectorLayer* mypLayer;
    QgsFeatureIterator fit;
    QStringList attrNames;
    Q_FOREACH ( QgsMapLayer * layer, mLayerTreeView->selectedLayers() )
    {
        mypLayer = qobject_cast<QgsVectorLayer *>( layer );

        //write map layers
        vectorFilename = params->getWrkdir().append("/Temp_").append(mypLayer->name()).append(".").append(layerSuffixes->value(mypLayer->id()));
        outfile.open (vectorFilename.toLatin1().data());

        QgsFields& fields = mypLayer->fields();
        // get indexes for all fields except "exclude"
        for ( int idx = 0; idx < fields.count(); ++idx )
            if ( fields[idx].name() != "exclude" )
            {
                attrNames.append(fields[idx].name());
//                qDebug() << "attrNames:" << fields[idx].name() << endl;
            }

        // Return all fields but "exclude" for each feature
        request.setSubsetOfAttributes(attrNames, mypLayer->pendingFields());
        fit = mypLayer->getFeatures( request );
        while ( fit.nextFeature( feature ) )
        {
            QgsAttributes attrs = feature.attributes();
            for(int idx = 0; idx < feature.attributes().count(); ++idx)
            {
                if ( fields[idx].name() != "exclude" )
                    outfile << attrs[idx].toDouble() << "\t";
//                    fprintf(outfile,"%.6f\t",attrs[idx]);
            }
              outfile << "\n";
        }
        outfile.close();
        outfilelist << vectorFilename.toLatin1().data() << "\n";
    }
    outfilelist.close();
}
void MBGuiApp::loadMBResults()
{
    // Get MB output raster filenames
    QStringList rasterResults = QStringList();
    QString suf = params->getWrkdir().append("/").append(params->getOutputFilename());
    QString temp = suf;
    rasterResults.append(temp.append("_depth.asc"));
    temp = suf;
    rasterResults.append(temp.append("_ei.asc"));
    temp = suf;
    rasterResults.append(temp.append("_nei.asc"));
    temp = suf;
    rasterResults.append(temp.append("_rei.asc"));
    createMeanResultLayer();

    canvas_depths->freeze(true);
    canvas_error->freeze(true);
    canvas_rei->freeze(true);
    canvas_nei->freeze(true);
    canvas_mean->freeze(true);

    //SAMs method
    for(int i = 0; i < resultCanvases->size()-1; ++i)
    {
        addRasterResultLayer(rasterResults[i],resultCanvases->at(i));
    }
    //add the results for canvas_mean
//    createMeanResultLayer();

    canvas_depths->freeze(false);
    canvas_error->freeze(false);
    canvas_rei->freeze(false);
    canvas_nei->freeze(false);
    canvas_mean->freeze(false);
}

void MBGuiApp::addRasterResultLayer(QString f, QgsMapCanvas* canvas)
{
        QFileInfo myRasterFileInfo(f);
        QgsRasterLayer * myRasterLayer = new QgsRasterLayer(myRasterFileInfo.filePath(), f.append(myRasterFileInfo.completeBaseName()));
        if (myRasterLayer->isValid())
        {
          qDebug("Layer is valid");
        }
        else
        {
          qDebug("Layer is NOT valid");
          return;
        }

        // Add the Vector Layer to the Layer Registry
        QgsMapLayerRegistry::instance()->addMapLayer(myRasterLayer, FALSE);
        QObject::connect( myRasterLayer, SIGNAL(repaintRequested()), canvas, SLOT(refresh()) );

        //create a layerset
        QList<QgsMapCanvasLayer> myList;
        // Add the layers to the Layer Set
        myList.append(QgsMapCanvasLayer(myRasterLayer, TRUE));//bool visibility
        // set the canvas to the extent of our layer
        canvas->setExtent(myRasterLayer->extent());
        // Set the Map Canvas Layer Set
        canvas->setLayerSet(myList);

//        //set the raster colors to those matching "jet" via custom fullHistogramStretch();
//        bool visibleAreaOnly = false;
//        QgsRaster::ContrastEnhancementLimits theLimits = QgsRaster::ContrastEnhancementMinMax;
////        QgsRasterLayer* myRasterLayer = qobject_cast<QgsRasterLayer *>( myLayer );
//        QgsRectangle myRectangle;
//        if ( visibleAreaOnly )
//          myRectangle = canvas->mapSettings().outputExtentToLayerExtent( myRasterLayer, canvas->extent() );

//        myRasterLayer->setContrastEnhancement( QgsContrastEnhancement::StretchToMinimumMaximum, theLimits, myRectangle );

//        canvas->refresh();

}
void MBGuiApp::createMeanResultLayer()
{
    //create the mean results layer


}
//////////////////////////////////////////////////////////////////////
//            Set Up the gui toolbars, menus, statusbar etc
//////////////////////////////////////////////////////////////////////

void MBGuiApp::createMapTools()
{
    //create the action behaviours
    connect(mpActionPan, SIGNAL(triggered()), this, SLOT(panMode()));
    connect(mpActionZoomIn, SIGNAL(triggered()), this, SLOT(zoomInMode()));
    connect(mpActionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOutMode()));
    connect(mpActionAddLayer, SIGNAL(triggered()), this, SLOT(addLayer()));

    //create a little toolbar
    mpMapToolBar = addToolBar(tr("MapTools"));
    mpMapToolBar->addAction(mpActionAddLayer);
    mpMapToolBar->addAction(mpActionZoomIn);
    mpMapToolBar->addAction(mpActionZoomOut);
    mpMapToolBar->addAction(mpActionPan);
    mpMapToolBar->addAction(mActionIdentify);
    mpMapToolBar->addAction(rasterPropertiesAction);

    //qgis' tools
    //create the maptools
//    mpPanTool = new QgsMapToolPan(canvas_xy);
//    mpPanTool->setAction(mpActionPan);
//    mpZoomInTool = new QgsMapToolZoom(canvas_xy, FALSE); // false = in
//    mpZoomInTool->setAction(mpActionZoomIn);
//    mpZoomOutTool = new QgsMapToolZoom(canvas_xy, TRUE ); //true = out
//    mpZoomOutTool->setAction(mpActionZoomOut);

    mpPanTool_xy = new QgsMapToolPan(canvas_xy);
    mpPanTool_xy->setAction(mpActionPan);
    mpZoomInTool_xy = new QgsMapToolZoom(canvas_xy, FALSE); // false = in
    mpZoomInTool_xy->setAction(mpActionZoomIn);
    mpZoomOutTool_xy = new QgsMapToolZoom(canvas_xy, TRUE ); //true = out
    mpZoomOutTool_xy->setAction(mpActionZoomOut);

    mpPanTool_yz = new QgsMapToolPan(canvas_yz);
    mpPanTool_yz->setAction(mpActionPan);
    mpZoomInTool_yz = new QgsMapToolZoom(canvas_yz, FALSE); // false = in
    mpZoomInTool_yz->setAction(mpActionZoomIn);
    mpZoomOutTool_yz = new QgsMapToolZoom(canvas_yz, TRUE ); //true = out
    mpZoomOutTool_yz->setAction(mpActionZoomOut);

    mpPanTool_depths = new QgsMapToolPan(canvas_depths);
    mpPanTool_depths->setAction(mpActionPan);
    mpZoomInTool_depths = new QgsMapToolZoom(canvas_depths, FALSE); // false = in
    mpZoomInTool_depths->setAction(mpActionZoomIn);
    mpZoomOutTool_depths = new QgsMapToolZoom(canvas_depths, TRUE ); //true = out
    mpZoomOutTool_depths->setAction(mpActionZoomOut);

    mpPanTool_error = new QgsMapToolPan(canvas_error);
    mpPanTool_error->setAction(mpActionPan);
    mpZoomInTool_error = new QgsMapToolZoom(canvas_error, FALSE); // false = in
    mpZoomInTool_error->setAction(mpActionZoomIn);
    mpZoomOutTool_error = new QgsMapToolZoom(canvas_error, TRUE ); //true = out
    mpZoomOutTool_error->setAction(mpActionZoomOut);

    mpPanTool_nei = new QgsMapToolPan(canvas_nei);
    mpPanTool_nei->setAction(mpActionPan);
    mpZoomInTool_nei = new QgsMapToolZoom(canvas_nei, FALSE); // false = in
    mpZoomInTool_nei->setAction(mpActionZoomIn);
    mpZoomOutTool_nei = new QgsMapToolZoom(canvas_nei, TRUE ); //true = out
    mpZoomOutTool_nei->setAction(mpActionZoomOut);

    mpPanTool_rei = new QgsMapToolPan(canvas_rei);
    mpPanTool_rei->setAction(mpActionPan);
    mpZoomInTool_rei = new QgsMapToolZoom(canvas_rei, FALSE); // false = in
    mpZoomInTool_rei->setAction(mpActionZoomIn);
    mpZoomOutTool_rei = new QgsMapToolZoom(canvas_rei, TRUE ); //true = out
    mpZoomOutTool_rei->setAction(mpActionZoomOut);

    mpPanTool_mean = new QgsMapToolPan(canvas_mean);
    mpPanTool_mean->setAction(mpActionPan);
    mpZoomInTool_mean = new QgsMapToolZoom(canvas_mean, FALSE); // false = in
    mpZoomInTool_mean->setAction(mpActionZoomIn);
    mpZoomOutTool_mean = new QgsMapToolZoom(canvas_mean, TRUE ); //true = out
    mpZoomOutTool_mean->setAction(mpActionZoomOut);


//    mMapTools.mSelectFeatures_xy = new QgsMapToolSelectFeatures( canvas_xy );
//    mMapTools.mSelectFeatures_xy->setAction( mActionSelectFeatures );
//    mMapTools.mSelectPolygon_xy = new QgsMapToolSelectPolygon( canvas_xy );
//    mMapTools.mSelectPolygon_xy->setAction( mActionSelectPolygon );
//    mMapTools.mSelectFreehand_xy = new QgsMapToolSelectFreehand( canvas_xy );
//    mMapTools.mSelectFreehand_xy->setAction( mActionSelectFreehand );
//    mMapTools.mSelectRadius_xy = new QgsMapToolSelectRadius( canvas_xy );
//    mMapTools.mSelectRadius_xy->setAction( mActionSelectRadius );


}
void MBGuiApp::createActions()
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
    connect(setLocalDataPathAction, SIGNAL(triggered()), this, SLOT(setLocalDataPath()));

    // Set Global Data Directory Action
    setGlobalDataPathAction = new QAction(tr("&Set Global Data Path..."), this);
    connect(setGlobalDataPathAction, SIGNAL(triggered()), this, SLOT(setGlobalDataPath()));

    // Set Local MB Directory Action
    setLocalMBPathAction = new QAction(tr("&Set Local MergeBathy.exe Path..."), this);
    connect(setLocalMBPathAction, SIGNAL(triggered()), this, SLOT(setLocalMBPath()));

    // Set Global MB Directory Action
    setGlobalMBPathAction = new QAction(tr("&Set Global MergeBathy.exe Path..."), this);
    connect(setGlobalMBPathAction, SIGNAL(triggered()), this, SLOT(setGlobalMBPath()));

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

    // Close Project Action
    newProjectAction = new QAction(tr("New Project"), this);
    newProjectAction->setShortcuts(QKeySequence::New);
    connect(newProjectAction, SIGNAL(triggered(bool)), this, SLOT(closeProject()));


    // Save Action
    cleanAction = new QAction(tr("Clean Temp/MBGuiApp Directory"), this);
    connect(cleanAction, SIGNAL(triggered(bool)), this, SLOT(cleanTempDir()));

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
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about2()));

    rasterPropertiesAction = new QAction(tr("Raster Properties"),this);
    connect(rasterPropertiesAction, SIGNAL(triggered()), this, SLOT(rasterProperties()));

    mActionRasterSeparator = nullptr;   // raster plugins list separator will be created when the first plugin is loaded



    //QGIS
    mActionNewProject = new QAction(tr("New Project"), this);
//    mActionNewBlankProject = new QAction(tr("New Blank Project"), this);
    mActionOpenProject = new QAction(tr("Open Project"), this);
    mActionSaveProject = new QAction(tr("Save Project"), this);
    mActionSaveProjectAs = new QAction(tr("Save Project As"), this);
    mActionSaveMapAsImage = new QAction(tr("Save Map As Image"), this);
//    mActionNewPrintComposer = new QAction(tr("New Print Composer"), this);
//    mActionShowComposerManager = new QAction(tr("Show Composer Manager"), this);
    mActionExit = new QAction(tr("Exit2"), this);
//    mActionDxfExport = new QAction(tr("DxfExport"), this);
    mActionUndo = new QAction(tr("Undo2"), this);
    mActionRedo = new QAction(tr("Redo2"), this);
//    mActionCutFeatures = new QAction(tr("Cut Features"), this);
//    mActionCopyFeatures = new QAction(tr("Copy Features"), this);
//    mActionPasteFeatures = new QAction(tr("Paste Features"), this);
//    mActionPasteAsNewVector = new QAction(tr("Paste As New Vector"), this);
//    mActionPasteAsNewMemoryVector = new QAction(tr("Paste As New Memory Vector"), this);
//    mActionCopyStyle = new QAction(tr("Copy Style"), this);
//    mActionPasteStyle = new QAction(tr("Paste Style"), this);
//    mActionAddFeature = new QAction(tr("Add Feature"), this);
//    mActionCircularStringCurvePoint = new QAction(tr("CircularStringCurvePoint"), this);
//    mActionCircularStringRadius = new QAction(tr("CircularStringRadius"), this);
//    mActionMoveFeature = new QAction(tr("Move Feature"), this);
//    mActionRotateFeature = new QAction(tr("Rotate Feature"), this);
    mActionDeleteSelected = new QAction(tr("Delete Selected"), this);
//    mActionAddRing = new QAction(tr("Add Ring"), this);
//    mActionFillRing = new QAction(tr("Fill Ring"), this);
//    mActionAddPart = new QAction(tr("Add Part"), this);
//    mActionSimplifyFeature = new QAction(tr("Simplify Feature"), this);
//    mActionDeleteRing = new QAction(tr("Delete Ring"), this);
//    mActionDeletePart = new QAction(tr("Delete Part"), this);
//    mActionMergeFeatures = new QAction(tr("Merge Features"), this);
//    mActionMergeFeatureAttributes = new QAction(tr("Merge Feature Attributes"), this);
    mActionNodeTool = new QAction(tr("Node Tool"), this);
//    mActionRotatePointSymbols = new QAction(tr("Rotate Point Symbols"), this);
//    mActionSnappingOptions = new QAction(tr("Snapping Options"), this);
//    mActionOffsetCurve = new QAction(tr("Offset Curve"), this);
    mActionPan = new QAction(tr("Pan2"), this);
    mActionPanToSelected = new QAction(tr("Pan To Selected2"), this);
    mActionZoomIn = new QAction(tr("Zoom In"), this);
    mActionZoomOut = new QAction(tr("Zoom Out"), this);
    mActionSelectFeatures = new QAction(tr("Select Features"), this);
    mActionSelectPolygon = new QAction(tr("Select Polygon"), this);
    mActionSelectFreehand = new QAction(tr("Select Freehand"), this);
    mActionSelectRadius = new QAction(tr("Select Radius"), this);
    mActionDeselectAll = new QAction(tr("Deselect All"), this);
    mActionSelectAll = new QAction(tr("Select All"), this);
    mActionInvertSelection = new QAction(tr("Invert Selection"), this);
    mActionSelectByExpression = new QAction(tr("Select By Expression"), this);
    mActionIdentify = new QAction(tr("Identify"), this);
//    mActionFeatureAction = new QAction(tr("Feature Action"), this);
    mActionMeasure = new QAction(tr("Measure"), this);
    mActionMeasureArea = new QAction(tr("Measure Area"), this);
    mActionMeasureAngle = new QAction(tr("Measure Angle"), this);
    mActionZoomFullExtent = new QAction(tr("Zoom Full Extent"), this);
    mActionZoomToLayer = new QAction(tr("Zoom To Layer"), this);
    mActionZoomToSelected = new QAction(tr("Zoom To Selected"), this);
    mActionZoomLast = new QAction(tr("Zoom Last"), this);
    mActionZoomNext = new QAction(tr("Zoom Next"), this);
    mActionZoomActualSize = new QAction(tr("Zoom Actual Size"), this);
    mActionMapTips = new QAction(tr("Map Tips"), this);
//    mActionNewBookmark = new QAction(tr("New Bookmark"), this);
//    mActionShowBookmarks = new QAction(tr("Show Bookmarks"), this);
//    mActionDraw = new QAction(tr("Draw"), this);
//    mActionTextAnnotation = new QAction(tr("Text Annotation"), this);
//    mActionFormAnnotation = new QAction(tr("Form Annotation"), this);
//    mActionHtmlAnnotation = new QAction(tr("Html Annotation"), this);
//    mActionSvgAnnotation = new QAction(tr("Svg Annotation"), this);
//    mActionAnnotation = new QAction(tr("Annotation"), this);
//    mActionLabeling = new QAction(tr("Labeling"), this);
    mActionStatisticalSummary = new QAction(tr("Statistical Summary"), this);
    mActionNewVectorLayer = new QAction(tr("New Vector Layer"), this);
    mActionNewSpatiaLiteLayer = new QAction(tr("New SpatiaLite Layer"), this);
    mActionNewMemoryLayer = new QAction(tr("New Memory Layer"), this);
    mActionShowRasterCalculator = new QAction(tr("Show Raster Calculator"), this);
    mActionShowAlignRasterTool = new QAction(tr("Align Raster Tool"), this);
    mActionEmbedLayers = new QAction(tr("Embed Layers"), this);
    mActionAddLayerDefinition = new QAction(tr("Add Layer Definition"), this);
    mActionAddOgrLayer = new QAction(tr("Add Ogr Layer"), this);
    mActionAddRasterLayer = new QAction(tr("Add Raster Layer"), this);
    mActionAddPgLayer = new QAction(tr("Add Pg Layer"), this);
    mActionAddSpatiaLiteLayer = new QAction(tr("Add SpatiaLite Layer"), this);
    mActionAddMssqlLayer = new QAction(tr("Add Mssql Layer"), this);
    mActionAddOracleLayer = new QAction(tr("Add Oracle Layer"), this);
    mActionAddWmsLayer = new QAction(tr("Add Wms Layer"), this);
    mActionAddWcsLayer = new QAction(tr("Add Wcs Layer"), this);
    mActionAddWfsLayer = new QAction(tr("Add Wfs Layer"), this);
    mActionAddDelimitedText = new QAction(tr("Add Delimited Text"), this);
    mActionAddVirtualLayer = new QAction(tr("Add Virtual Layer"), this);
    mActionOpenTable = new QAction(tr("Open Table"), this);
    mActionOpenFieldCalc = new QAction(tr("Open Field Calculator"), this);
    mActionToggleEditing = new QAction(tr("Toggle Editing"), this);
    mActionSaveLayerEdits = new QAction(tr("Save Layer Edits"), this);
    mActionSaveEdits = new QAction(tr("Save Edits"), this);
    mActionSaveAllEdits = new QAction(tr("Save All Edits"), this);
    mActionCancelAllEdits = new QAction(tr("Cancel All Edits"), this);
    mActionCancelEdits = new QAction(tr("Cancel Edits"), this);
    mActionRollbackAllEdits = new QAction(tr("Rollback All Edits"), this);
    mActionRollbackEdits = new QAction(tr("Rollback Edits"), this);
    mActionLayerSaveAs = new QAction(tr("Layer Save As"), this);
    mActionLayerSaveAsTemp = new QAction(tr("Layer Save As Temp"), this);
    mActionSaveLayerDefinition = new QAction(tr("Save Layer Definition"), this);
    mActionRemoveLayer = new QAction(tr("Remove Layer"), this);
//    mActionDuplicateLayer = new QAction(tr("Duplicate Layer"), this);
    mActionSetLayerScaleVisibility = new QAction(tr("Set Layer Scale Visibility"), this);
    mActionSetLayerCRS = new QAction(tr("Set Layer CRS"), this);
    mActionSetProjectCRSFromLayer = new QAction(tr("Set Project CRS From Layer"), this);
    mActionLayerProperties = new QAction(tr("Layer Properties"), this);
    mActionLayerSubsetString = new QAction(tr("Layer Subset String"), this);
    mActionAddToOverview = new QAction(tr("Add To Overview"), this);
    mActionAddAllToOverview = new QAction(tr("Add All To Overview"), this);
    mActionRemoveAllFromOverview = new QAction(tr("Remove All From Overview"), this);
    mActionShowAllLayers = new QAction(tr("Show All Layers"), this);
    mActionHideAllLayers = new QAction(tr("Hide All Layers"), this);
    mActionShowSelectedLayers = new QAction(tr("Show Selected Layers"), this);
    mActionHideSelectedLayers = new QAction(tr("Hide Selected Layers"), this);
//    mActionManagePlugins = new QAction(tr("Manage Plugins"), this);
//    mActionShowPythonDialog = new QAction(tr("Python Dialog"), this);
    mActionToggleFullScreen = new QAction(tr("Toggle FullScreen"), this);
    mActionProjectProperties = new QAction(tr("Project Properties"), this);
//    mActionOptions = new QAction(tr("Options"), this);
//    mActionCustomProjection = new QAction(tr("Custom Projection"), this);
//    mActionConfigureShortcuts = new QAction(tr("Configure Shortcuts"), this);
//    mActionStyleManagerV2 = new QAction(tr("Style Manager"), this);
//    mActionCustomization = new QAction(tr("Customization"), this);
    mActionLocalHistogramStretch = new QAction(tr("Local Histogram Stretch"), this);
    mActionFullHistogramStretch = new QAction(tr("Full Histogram Stretch"), this);
    mActionLocalCumulativeCutStretch = new QAction(tr("Local Cumulative Cut Search"), this);
    mActionFullCumulativeCutStretch = new QAction(tr("Full Cumulative Cut Search"), this);
    mActionIncreaseBrightness = new QAction(tr("Increase Brightness"), this);
    mActionDecreaseBrightness = new QAction(tr("Decrease Brightness"), this);
    mActionIncreaseContrast = new QAction(tr("Increase Contrast"), this);
    mActionDecreaseContrast = new QAction(tr("Decrease Contrast"), this);
//    mActionShowPinnedLabels = new QAction(tr("Show Pinned Labels"), this);
//    mActionPinLabels = new QAction(tr("Pin Labels"), this);
//    mActionShowHideLabels = new QAction(tr("Show/Hide Labels"), this);
//    mActionMoveLabel = new QAction(tr("Move Label"), this);
//    mActionRotateLabel = new QAction(tr("Rotate Label"), this);
//    mActionChangeLabelProperties = new QAction(tr("Change Label Properties"), this);
    mActionAllEdits = new QAction(tr("All Edits"), this);
//    mActionReshapeFeatures = new QAction(tr("Reshape Features"), this);
//    mActionSplitFeatures = new QAction(tr("Split Features"), this);
//    mActionSplitParts = new QAction(tr("Split Parts"), this);
//    mActionEnableTracing = new QAction(tr("Enable Tracing"), this);
    mActionAbout = new QAction(tr("About"), this);


    // Project Menu Items
    connect( mActionNewProject, SIGNAL( triggered() ), this, SLOT( fileNew() ) );
//    connect( mActionNewBlankProject, SIGNAL( triggered() ), this, SLOT( fileNewBlank() ) );
    connect( mActionOpenProject, SIGNAL( triggered() ), this, SLOT( fileOpen() ) );
    connect( mActionSaveProject, SIGNAL( triggered() ), this, SLOT( fileSave() ) );
    connect( mActionSaveProjectAs, SIGNAL( triggered() ), this, SLOT( fileSaveAs() ) );

    connect( mActionSaveMapAsImage, SIGNAL( triggered() ), this, SLOT( saveMapAsImage() ) );
//    connect( mActionNewPrintComposer, SIGNAL( triggered() ), this, SLOT( newPrintComposer() ) );
//    connect( mActionShowComposerManager, SIGNAL( triggered() ), this, SLOT( showComposerManager() ) );
    connect( mActionExit, SIGNAL( triggered() ), this, SLOT( fileExit() ) );
//    connect( mActionDxfExport, SIGNAL( triggered() ), this, SLOT( dxfExport() ) );

    // Edit Menu Items
    connect( mActionUndo, SIGNAL( triggered() ), mUndoWidget, SLOT( undo() ) );
    connect( mActionRedo, SIGNAL( triggered() ), mUndoWidget, SLOT( redo() ) );
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
//    connect( mActionDeleteSelected, SIGNAL( triggered() ), this, SLOT( deleteSelected() ) );
//    connect( mActionAddRing, SIGNAL( triggered() ), this, SLOT( addRing() ) );
//    connect( mActionFillRing, SIGNAL( triggered() ), this, SLOT( fillRing() ) );
//    connect( mActionAddPart, SIGNAL( triggered() ), this, SLOT( addPart() ) );
//    connect( mActionSimplifyFeature, SIGNAL( triggered() ), this, SLOT( simplifyFeature() ) );
//    connect( mActionDeleteRing, SIGNAL( triggered() ), this, SLOT( deleteRing() ) );
//    connect( mActionDeletePart, SIGNAL( triggered() ), this, SLOT( deletePart() ) );
//    connect( mActionMergeFeatures, SIGNAL( triggered() ), this, SLOT( mergeSelectedFeatures() ) );
//    connect( mActionMergeFeatureAttributes, SIGNAL( triggered() ), this, SLOT( mergeAttributesOfSelectedFeatures() ) );
    connect( mActionNodeTool, SIGNAL( triggered() ), this, SLOT( nodeTool() ) );
//    connect( mActionRotatePointSymbols, SIGNAL( triggered() ), this, SLOT( rotatePointSymbols() ) );
//    connect( mActionSnappingOptions, SIGNAL( triggered() ), this, SLOT( snappingOptions() ) );
//    connect( mActionOffsetCurve, SIGNAL( triggered() ), this, SLOT( offsetCurve() ) );

    // View Menu Items
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
//    connect( mActionSelectByExpression, SIGNAL( triggered() ), this, SLOT( selectByExpression() ) );
    connect( mActionIdentify, SIGNAL( triggered() ), this, SLOT( identify() ) );
//    connect( mActionFeatureAction, SIGNAL( triggered() ), this, SLOT( doFeatureAction() ) );
//    connect( mActionMeasure, SIGNAL( triggered() ), this, SLOT( measure() ) );
//    connect( mActionMeasureArea, SIGNAL( triggered() ), this, SLOT( measureArea() ) );
//    connect( mActionMeasureAngle, SIGNAL( triggered() ), this, SLOT( measureAngle() ) );
    connect( mActionZoomFullExtent, SIGNAL( triggered() ), this, SLOT( zoomFull() ) );
    connect( mActionZoomToLayer, SIGNAL( triggered() ), this, SLOT( zoomToLayerExtent() ) );
    connect( mActionZoomToSelected, SIGNAL( triggered() ), this, SLOT( zoomToSelected() ) );
    connect( mActionZoomLast, SIGNAL( triggered() ), this, SLOT( zoomToPrevious() ) );
    connect( mActionZoomNext, SIGNAL( triggered() ), this, SLOT( zoomToNext() ) );
    connect( mActionZoomActualSize, SIGNAL( triggered() ), this, SLOT( zoomActualSize() ) );
    connect( mActionMapTips, SIGNAL( triggered() ), this, SLOT( toggleMapTips() ) );
//    connect( mActionNewBookmark, SIGNAL( triggered() ), this, SLOT( newBookmark() ) );
//    connect( mActionShowBookmarks, SIGNAL( triggered() ), this, SLOT( showBookmarks() ) );
//    connect( mActionDraw, SIGNAL( triggered() ), this, SLOT( refreshMapCanvas() ) );
//    connect( mActionTextAnnotation, SIGNAL( triggered() ), this, SLOT( addTextAnnotation() ) );
//    connect( mActionFormAnnotation, SIGNAL( triggered() ), this, SLOT( addFormAnnotation() ) );
//    connect( mActionHtmlAnnotation, SIGNAL( triggered() ), this, SLOT( addHtmlAnnotation() ) );
//    connect( mActionSvgAnnotation, SIGNAL( triggered() ), this, SLOT( addSvgAnnotation() ) );
//    connect( mActionAnnotation, SIGNAL( triggered() ), this, SLOT( modifyAnnotation() ) );
//    connect( mActionLabeling, SIGNAL( triggered() ), this, SLOT( labeling() ) );
    connect( mActionStatisticalSummary, SIGNAL( triggered() ), this, SLOT( showStatisticsDockWidget() ) );

    // Layer Menu Items
//    connect( mActionNewVectorLayer, SIGNAL( triggered() ), this, SLOT( newVectorLayer() ) );
//    connect( mActionNewSpatiaLiteLayer, SIGNAL( triggered() ), this, SLOT( newSpatialiteLayer() ) );
//    connect( mActionNewMemoryLayer, SIGNAL( triggered() ), this, SLOT( newMemoryLayer() ) );
//    connect( mActionShowRasterCalculator, SIGNAL( triggered() ), this, SLOT( showRasterCalculator() ) );
//    connect( mActionShowAlignRasterTool, SIGNAL( triggered() ), this, SLOT( showAlignRasterTool() ) );
//    connect( mActionEmbedLayers, SIGNAL( triggered() ), this, SLOT( embedLayers() ) );
//    connect( mActionAddLayerDefinition, SIGNAL( triggered() ), this, SLOT( addLayerDefinition() ) );
//    connect( mActionAddOgrLayer, SIGNAL( triggered() ), this, SLOT( addVectorLayer() ) );
    connect( mActionAddRasterLayer, SIGNAL( triggered() ), this, SLOT( addRasterLayer() ) );
//    connect( mActionAddPgLayer, SIGNAL( triggered() ), this, SLOT( addDatabaseLayer() ) );
//    connect( mActionAddSpatiaLiteLayer, SIGNAL( triggered() ), this, SLOT( addSpatiaLiteLayer() ) );
//    connect( mActionAddMssqlLayer, SIGNAL( triggered() ), this, SLOT( addMssqlLayer() ) );
//    connect( mActionAddOracleLayer, SIGNAL( triggered() ), this, SLOT( addOracleLayer() ) );
//    connect( mActionAddWmsLayer, SIGNAL( triggered() ), this, SLOT( addWmsLayer() ) );
//    connect( mActionAddWcsLayer, SIGNAL( triggered() ), this, SLOT( addWcsLayer() ) );
//    connect( mActionAddWfsLayer, SIGNAL( triggered() ), this, SLOT( addWfsLayer() ) );
    connect( mActionAddDelimitedText, SIGNAL( triggered() ), this, SLOT( addDelimitedTextLayer() ) );
//    connect( mActionAddVirtualLayer, SIGNAL( triggered() ), this, SLOT( addVirtualLayer() ) );
    connect( mActionOpenTable, SIGNAL( triggered() ), this, SLOT( attributeTable() ) );
    connect( mActionOpenFieldCalc, SIGNAL( triggered() ), this, SLOT( fieldCalculator() ) );
//    connect( mActionToggleEditing, SIGNAL( triggered() ), this, SLOT( toggleEditing() ) );
//    connect( mActionSaveLayerEdits, SIGNAL( triggered() ), this, SLOT( saveActiveLayerEdits() ) );
//    connect( mActionSaveEdits, SIGNAL( triggered() ), this, SLOT( saveEdits() ) );
//    connect( mActionSaveAllEdits, SIGNAL( triggered() ), this, SLOT( saveAllEdits() ) );
//    connect( mActionRollbackEdits, SIGNAL( triggered() ), this, SLOT( rollbackEdits() ) );
//    connect( mActionRollbackAllEdits, SIGNAL( triggered() ), this, SLOT( rollbackAllEdits() ) );
//    connect( mActionCancelEdits, SIGNAL( triggered() ), this, SLOT( cancelEdits() ) );
//    connect( mActionCancelAllEdits, SIGNAL( triggered() ), this, SLOT( cancelAllEdits() ) );
    connect( mActionLayerSaveAs, SIGNAL( triggered() ), this, SLOT( saveAsFile() ) );
    connect( mActionLayerSaveAsTemp, SIGNAL( triggered() ), this, SLOT( saveTempFile() ) );
//    connect( mActionSaveLayerDefinition, SIGNAL( triggered() ), this, SLOT( saveAsLayerDefinition() ) );
    connect( mActionRemoveLayer, SIGNAL( triggered() ), this, SLOT( removeLayer() ) );
//    connect( mActionDuplicateLayer, SIGNAL( triggered() ), this, SLOT( duplicateLayers() ) );
//    connect( mActionSetLayerScaleVisibility, SIGNAL( triggered() ), this, SLOT( setLayerScaleVisibility() ) );
//    connect( mActionSetLayerCRS, SIGNAL( triggered() ), this, SLOT( setLayerCRS() ) );
//    connect( mActionSetProjectCRSFromLayer, SIGNAL( triggered() ), this, SLOT( setProjectCRSFromLayer() ) );
    connect( mActionLayerProperties, SIGNAL( triggered() ), this, SLOT( layerProperties() ) );
//    connect( mActionLayerSubsetString, SIGNAL( triggered() ), this, SLOT( layerSubsetString() ) );
    connect( mActionAddToOverview, SIGNAL( triggered() ), this, SLOT( isInOverview() ) );
    connect( mActionAddAllToOverview, SIGNAL( triggered() ), this, SLOT( addAllToOverview() ) );
    connect( mActionRemoveAllFromOverview, SIGNAL( triggered() ), this, SLOT( removeAllFromOverview() ) );
    connect( mActionShowAllLayers, SIGNAL( triggered() ), this, SLOT( showAllLayers() ) );
    connect( mActionHideAllLayers, SIGNAL( triggered() ), this, SLOT( hideAllLayers() ) );
    connect( mActionShowSelectedLayers, SIGNAL( triggered() ), this, SLOT( showSelectedLayers() ) );
    connect( mActionHideSelectedLayers, SIGNAL( triggered() ), this, SLOT( hideSelectedLayers() ) );

//    // Plugin Menu Items
//    connect( mActionManagePlugins, SIGNAL( triggered() ), this, SLOT( showPluginManager() ) );
//    connect( mActionShowPythonDialog, SIGNAL( triggered() ), this, SLOT( showPythonDialog() ) );

//    // Settings Menu Items
    connect( mActionToggleFullScreen, SIGNAL( triggered() ), this, SLOT( toggleFullScreen() ) );
//    connect( mActionProjectProperties, SIGNAL( triggered() ), this, SLOT( projectProperties() ) );
//    connect( mActionOptions, SIGNAL( triggered() ), this, SLOT( options() ) );
//    connect( mActionCustomProjection, SIGNAL( triggered() ), this, SLOT( customProjection() ) );
//    connect( mActionConfigureShortcuts, SIGNAL( triggered() ), this, SLOT( configureShortcuts() ) );
//    connect( mActionStyleManagerV2, SIGNAL( triggered() ), this, SLOT( showStyleManagerV2() ) );
//    connect( mActionCustomization, SIGNAL( triggered() ), this, SLOT( customize() ) );


    // Raster toolbar items
    connect( mActionLocalHistogramStretch, SIGNAL( triggered() ), this, SLOT( localHistogramStretch() ) );
    connect( mActionFullHistogramStretch, SIGNAL( triggered() ), this, SLOT( fullHistogramStretch() ) );
    connect( mActionLocalCumulativeCutStretch, SIGNAL( triggered() ), this, SLOT( localCumulativeCutStretch() ) );
    connect( mActionFullCumulativeCutStretch, SIGNAL( triggered() ), this, SLOT( fullCumulativeCutStretch() ) );
    connect( mActionIncreaseBrightness, SIGNAL( triggered() ), this, SLOT( increaseBrightness() ) );
    connect( mActionDecreaseBrightness, SIGNAL( triggered() ), this, SLOT( decreaseBrightness() ) );
    connect( mActionIncreaseContrast, SIGNAL( triggered() ), this, SLOT( increaseContrast() ) );
    connect( mActionDecreaseContrast, SIGNAL( triggered() ), this, SLOT( decreaseContrast() ) );
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
    connect( mActionAbout, SIGNAL( triggered() ), this, SLOT( about2() ) );
//    connect( mActionSponsors, SIGNAL( triggered() ), this, SLOT( sponsors() ) );

//    connect( mActionShowPinnedLabels, SIGNAL( toggled( bool ) ), this, SLOT( showPinnedLabels( bool ) ) );
//    connect( mActionPinLabels, SIGNAL( triggered() ), this, SLOT( pinLabels() ) );
//    connect( mActionShowHideLabels, SIGNAL( triggered() ), this, SLOT( showHideLabels() ) );
//    connect( mActionMoveLabel, SIGNAL( triggered() ), this, SLOT( moveLabel() ) );
//    connect( mActionRotateLabel, SIGNAL( triggered() ), this, SLOT( rotateLabel() ) );
//    connect( mActionChangeLabelProperties, SIGNAL( triggered() ), this, SLOT( changeLabelProperties() ) );
    /*
    #ifndef HAVE_POSTGRESQL
    delete mActionAddPgLayer;
    mActionAddPgLayer = 0;
    #endif

    #ifndef HAVE_MSSQL
    delete mActionAddMssqlLayer;
    mActionAddMssqlLayer = 0;
    #endif

    #ifndef HAVE_ORACLE
    delete mActionAddOracleLayer;
    mActionAddOracleLayer = nullptr;
    #endif
    */
}

void MBGuiApp::createActionGroups()
{
    // Map Tool Group
    mMapToolGroup = new QActionGroup( this );
    mMapToolGroup->addAction( mActionPan );
    mMapToolGroup->addAction( mActionZoomIn );
    mMapToolGroup->addAction( mActionZoomOut );
    mMapToolGroup->addAction( mActionIdentify );
    mMapToolGroup->addAction( mActionSelectFeatures );
    mMapToolGroup->addAction( mActionSelectPolygon );
    mMapToolGroup->addAction( mActionSelectFreehand );
    mMapToolGroup->addAction( mActionSelectRadius );
    mMapToolGroup->addAction( mActionDeselectAll );
    mMapToolGroup->addAction( mActionSelectAll );
    mMapToolGroup->addAction( mActionInvertSelection );
    /*
    mMapToolGroup->addAction( mActionFeatureAction );
    mMapToolGroup->addAction( mActionMeasure );
    mMapToolGroup->addAction( mActionMeasureArea );
    mMapToolGroup->addAction( mActionMeasureAngle );
    mMapToolGroup->addAction( mActionAddFeature );
    mMapToolGroup->addAction( mActionCircularStringCurvePoint );
    mMapToolGroup->addAction( mActionCircularStringRadius );
    mMapToolGroup->addAction( mActionMoveFeature );
//    mMapToolGroup->addAction( mActionRotateFeature );
    #if defined(GEOS_VERSION_MAJOR) && defined(GEOS_VERSION_MINOR) && \
    ((GEOS_VERSION_MAJOR>3) || ((GEOS_VERSION_MAJOR==3) && (GEOS_VERSION_MINOR>=3)))
    mMapToolGroup->addAction( mActionOffsetCurve );
    #endif
    mMapToolGroup->addAction( mActionReshapeFeatures );
    mMapToolGroup->addAction( mActionSplitFeatures );
    mMapToolGroup->addAction( mActionSplitParts );
    mMapToolGroup->addAction( mActionDeleteSelected );
    mMapToolGroup->addAction( mActionAddRing );
    mMapToolGroup->addAction( mActionFillRing );
    mMapToolGroup->addAction( mActionAddPart );
    mMapToolGroup->addAction( mActionSimplifyFeature );
    mMapToolGroup->addAction( mActionDeleteRing );
    mMapToolGroup->addAction( mActionDeletePart );
    mMapToolGroup->addAction( mActionMergeFeatures );
    mMapToolGroup->addAction( mActionMergeFeatureAttributes );
    mMapToolGroup->addAction( mActionNodeTool );
    mMapToolGroup->addAction( mActionRotatePointSymbols );
    mMapToolGroup->addAction( mActionPinLabels );
    mMapToolGroup->addAction( mActionShowHideLabels );
    mMapToolGroup->addAction( mActionMoveLabel );
    mMapToolGroup->addAction( mActionRotateLabel );
    mMapToolGroup->addAction( mActionChangeLabelProperties );

    // Preview Modes Group
    QActionGroup* mPreviewGroup = new QActionGroup( this );
    mPreviewGroup->setExclusive( true );
    mActionPreviewModeOff->setActionGroup( mPreviewGroup );
    mActionPreviewModeGrayscale->setActionGroup( mPreviewGroup );
    mActionPreviewModeMono->setActionGroup( mPreviewGroup );
    mActionPreviewProtanope->setActionGroup( mPreviewGroup );
    mActionPreviewDeuteranope->setActionGroup( mPreviewGroup );
    */

}
void MBGuiApp::createMenus()
{
    // File Tab
    fileMenu2 = menuBar()->addMenu(tr("&File"));
    fileMenu2->addAction(openAction);
    fileMenu2->addAction(saveAction);
    fileMenu2->addAction(saveAsAction);
    fileMenu2->addAction(closeProjectAction);
    fileMenu2->addSeparator();
    fileMenu2->addAction(cleanAction);
    fileMenu2->addSeparator();
    fileMenu2->addAction(exitAction);
    connect(fileMenu2, SIGNAL(aboutToShow()),this, SLOT(fileMenuAboutToShow()));
    connect(fileMenu2, SIGNAL(aboutToHide()),this, SLOT(fileMenuAboutToHide()));

    // Edit Tab
    editMenu2 = menuBar()->addMenu(tr("&Edit"));
    editMenu2->addAction(undoAction);
    editMenu2->addAction(redoAction);
    editMenu2->addSeparator();
    editMenu2->addAction(deleteAction);
    connect(editMenu2, SIGNAL(aboutToShow()),this, SLOT(itemMenuAboutToShow()));
    connect(editMenu2, SIGNAL(aboutToHide()),this, SLOT(itemMenuAboutToHide()));
    editMenu2->addAction(mActionUndo); //qgis
    editMenu2->addAction(mActionRedo); //qgis

    // Item Tab
    itemMenu2 = menuBar()->addMenu(tr("&Item"));
    itemMenu2->addAction(addLayerAction);
    itemMenu2->addAction(addGroupAction);
    itemMenu2->addSeparator();
    itemMenu2->addAction(setLocalDataPathAction);
    itemMenu2->addAction(setGlobalDataPathAction);
    itemMenu2->addSeparator();
    itemMenu2->addAction(setLocalMBPathAction);
    itemMenu2->addAction(setGlobalMBPathAction);

    // Help Tab
    helpMenu2 = menuBar()->addMenu(tr("&About"));
//    helpMenu2->addAction(aboutAction);
    helpMenu2->addAction(mActionAbout);
//    QAction*
    actionWhatsThis = QWhatsThis::createAction( this );
    actionWhatsThis->setIcon( QgsApplication::getThemeIcon( "/mActionWhatsThis.svg" ) );
    helpMenu2->addAction(actionWhatsThis);

    /*
    #ifdef Q_OS_MAC
        // Window Menu Items

        mActionWindowMinimize = new QAction( tr( "Minimize" ), this );
        mActionWindowMinimize->setShortcut( tr( "Ctrl+M", "Minimize Window" ) );
        mActionWindowMinimize->setStatusTip( tr( "Minimizes the active window to the dock" ) );
        connect( mActionWindowMinimize, SIGNAL( triggered() ), this, SLOT( showActiveWindowMinimized() ) );

        mActionWindowZoom = new QAction( tr( "Zoom" ), this );
        mActionWindowZoom->setStatusTip( tr( "Toggles between a predefined size and the window size set by the user" ) );
        connect( mActionWindowZoom, SIGNAL( triggered() ), this, SLOT( toggleActiveWindowMaximized() ) );

        mActionWindowAllToFront = new QAction( tr( "Bring All to Front" ), this );
        mActionWindowAllToFront->setStatusTip( tr( "Bring forward all open windows" ) );
        connect( mActionWindowAllToFront, SIGNAL( triggered() ), this, SLOT( bringAllToFront() ) );

        // list of open windows
        mWindowActions = new QActionGroup( this );
    #endif
    */
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
}
void MBGuiApp::createSubMenus()
{
    // Panel and Toolbar Submenus
    mPanelMenu = new QMenu( tr( "Panels" ), this );
    mPanelMenu->setObjectName( "mPanelMenu" );
    mToolbarMenu = new QMenu( tr( "Toolbars" ), this );
    mToolbarMenu->setObjectName( "mToolbarMenu" );

    mRecentProjectsMenu = new QMenu( tr( "Projects" ), this );
    mRecentProjectsMenu->setObjectName("mRecentProjectsMenu");
    menuBar()->addMenu(mRecentProjectsMenu);

    mViewMenu = new QMenu( tr( "View" ), this );
    mViewMenu->setObjectName("mViewMenu");
    menuBar()->addMenu(mViewMenu);

    /*mSettingsMenu = new QMenu( tr( "Settings@@" ), this );
    mSettingsMenu->setObjectName("mSettingsMenu");
    menuBar()->addMenu(mSettingsMenu);
    menuBar()->addMenu(mEditMenu);
    menuBar()->addMenu(mAddLayerMenu);
    menuBar()->addMenu(mRasterMenu);
    menuBar()->addMenu(mVectorMenu);
    mMenuPasteAs = new QMenu(tr("Paste As...sam"));*/

    // Get platform for menu layout customization (Gnome, Kde, Mac, Win)
    QDialogButtonBox::ButtonLayout layout = QDialogButtonBox::ButtonLayout( style()->styleHint( QStyle::SH_DialogButtonLayout, nullptr, this ) );

    // Project Menu
    // Connect once for the entire submenu.
    connect( mRecentProjectsMenu, SIGNAL( triggered( QAction * ) ), this, SLOT( openProject( QAction * ) ) );
//  connect( mProjectFromTemplateMenu, SIGNAL( triggered( QAction * ) ), this, SLOT( fileNewFromTemplateAction( QAction * ) ) );

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
    /*else
    {
        // on the top of the settings menu
        QAction* before = mActionNewProject; //mActionProjectProperties;
        mSettingsMenu->insertMenu( before, mPanelMenu );
        mSettingsMenu->insertMenu( before, mToolbarMenu );
        mSettingsMenu->insertAction( before, mActionToggleFullScreen );
        mSettingsMenu->insertSeparator( before );
    }*/

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

/*    // Database Menu
    // don't add it yet, wait for a plugin
    mDatabaseMenu = new QMenu( tr( "&Database" ), menuBar() );
    mDatabaseMenu->setObjectName( "mDatabaseMenu" );
    // Web Menu
    // don't add it yet, wait for a plugin
    mWebMenu = new QMenu( tr( "&Web" ), menuBar() );
    mWebMenu->setObjectName( "mWebMenu" );
*/
//    Help menu
//    add What's this button to it
//    QAction* before = mActionHelpAPI;
//    QAction* actionWhatsThis = QWhatsThis::createAction( this );
//    actionWhatsThis->setIcon( QgsApplication::getThemeIcon( "/mActionWhatsThis.svg" ) );
////    mHelpMenu->insertAction( before, actionWhatsThis );
//    helpMenu2->addAction(actionWhatsThis);
}
void MBGuiApp::createToolBars()
{
    QSettings settings;
    // QSize myIconSize ( 32,32 ); //large icons
    // Note: we need to set each object name to ensure that
    // qmbguiapp::saveState and qmbguiapp::restoreState
    // work properly

    mFileToolBar = new QToolBar("File");
    mFileToolBar->setObjectName("mFileToolBar");
    mLayerToolBar = new QToolBar("Layer");
    mLayerToolBar->setObjectName("mLayerToolBar");
    mMapNavToolBar = new QToolBar("Map");
    mMapNavToolBar->setObjectName("mMapNavToolBar");
    mAttributesToolBar = new QToolBar("Attributes");
    mAttributesToolBar->setObjectName("mAttributesToolBar");
//    mAttributesToolBar = new QToolBar("Help");
//    mAttributesToolBar->setObjectName("mHelpToolBar");
    mRasterToolBar = new QToolBar("Raster");
    mRasterToolBar->setObjectName("mRasterToolBar");
    mVectorToolBar = new QToolBar("Vector");
    mVectorToolBar->setObjectName("mVectorToolBar");
    createMapTools();

    //create a File toolbar
    mFileToolBar = addToolBar(tr("File Tools"));
    mFileToolBar->addAction(mActionNewProject);
    mFileToolBar->addAction(mActionOpenProject);
    mFileToolBar->addAction(mActionSaveProject);
    mFileToolBar->addAction(mActionSaveProjectAs);
    //use sams instead
    mFileToolBar->addAction(openAction);
    mFileToolBar->addAction(saveAction);
    mFileToolBar->addAction(saveAsAction);
    mFileToolBar->addAction(undoAction);
    mFileToolBar->addAction(redoAction);
    mFileToolBar->addAction(actionWhatsThis);

    addToolBar(mFileToolBar);
    addToolBar(mpMapToolBar);
    addToolBar(mAttributesToolBar);
//    addToolBar(mHelpToolBar);
    /*addToolBar(mLayerToolBar);
    addToolBar(mMapNavToolBar);
    addToolBar(mRasterToolBar);
    addToolBar(mVectorToolBar);
*/

    QList<QToolBar*> toolbarMenuToolBars = QList<QToolBar*>();
    toolbarMenuToolBars
    << mFileToolBar
    << mAttributesToolBar;
//    << mHelpToolBar;
    /*<< mLayerToolBar
    << mDigitizeToolBar
    << mAdvancedDigitizeToolBar
    << mMapNavToolBar
    << mPluginToolBar
    << mRasterToolBar
    << mVectorToolBar;
    << mDatabaseToolBar
    << mWebToolBar
    << mLabelToolBar;
    */

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

    QToolButton* bt = new QToolButton( mAttributesToolBar );
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

    /*// feature action tool button
  bt = new QToolButton( mAttributesToolBar );
  bt->setPopupMode( QToolButton::MenuButtonPopup );
  bt->setDefaultAction( mActionFeatureAction );
  mFeatureActionMenu = new QMenu( bt );
  connect( mFeatureActionMenu, SIGNAL( triggered( QAction * ) ), this, SLOT( updateDefaultFeatureAction( QAction * ) ) );
  connect( mFeatureActionMenu, SIGNAL( aboutToShow() ), this, SLOT( refreshFeatureActions() ) );
  bt->setMenu( mFeatureActionMenu );
  QAction* featureActionAction = mAttributesToolBar->insertWidget( selectAction, bt );
  featureActionAction->setObjectName( "ActionFeatureAction" );

  mAttributesToolBar->addAction(mActionToggleEditing);
  mAttributesToolBar->addAction(mActionIdentify);
  mAttributesToolBar->addAction(mActionNodeTool);

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

   annotation tool button
  bt = new QToolButton();
  bt->setPopupMode( QToolButton::MenuButtonPopup );
  bt->addAction( mActionTextAnnotation );
  bt->addAction( mActionFormAnnotation );
  bt->addAction( mActionHtmlAnnotation );
  bt->addAction( mActionSvgAnnotation );
  bt->addAction( mActionAnnotation );

  QAction* defAnnotationAction = mActionTextAnnotation;
  switch ( settings.value( "/UI/annotationTool", 0 ).toInt() )
  {
    case 0:
      defAnnotationAction = mActionTextAnnotation;
      break;
    case 1:
      defAnnotationAction = mActionFormAnnotation;
      break;
    case 2:
      defAnnotationAction = mActionHtmlAnnotation;
      break;
    case 3:
      defAnnotationAction = mActionSvgAnnotation;
      break;
    case 4:
      defAnnotationAction =  mActionAnnotation;
      break;

  }
  bt->setDefaultAction( defAnnotationAction );
  QAction* annotationAction = mAttributesToolBar->addWidget( bt );
  annotationAction->setObjectName( "ActionAnnotation" );
  connect( bt, SIGNAL( triggered( QAction * ) ), this, SLOT( toolButtonActionTriggered( QAction * ) ) );

   vector layer edits tool buttons
  QToolButton* tbAllEdits = qobject_cast<QToolButton *>( mDigitizeToolBar->widgetForAction( mActionAllEdits ) );
  tbAllEdits->setPopupMode( QToolButton::InstantPopup );

  // new layer tool button
  bt = new QToolButton();
  bt->setPopupMode( QToolButton::MenuButtonPopup );
  bt->addAction( mActionNewVectorLayer );
  bt->addAction( mActionNewSpatiaLiteLayer );
  bt->addAction( mActionNewMemoryLayer );

  QAction* defNewLayerAction = mActionNewVectorLayer;
  switch ( settings.value( "/UI/defaultNewLayer", 1 ).toInt() )
  {
    case 0:
      defNewLayerAction = mActionNewSpatiaLiteLayer;
      break;
    case 1:
      defNewLayerAction = mActionNewVectorLayer;
      break;
    case 2:
      defNewLayerAction = mActionNewMemoryLayer;
      break;
  }
  bt->setDefaultAction( defNewLayerAction );
  QAction* newLayerAction = mLayerToolBar->addWidget( bt );

  newLayerAction->setObjectName( "ActionNewLayer" );
  connect( bt, SIGNAL( triggered( QAction * ) ), this, SLOT( toolButtonActionTriggered( QAction * ) ) );

  //circular string digitize tool button
  QToolButton* tbAddCircularString = new QToolButton( mDigitizeToolBar );
  tbAddCircularString->setPopupMode( QToolButton::MenuButtonPopup );
  tbAddCircularString->addAction( mActionCircularStringCurvePoint );
  tbAddCircularString->addAction( mActionCircularStringRadius );
  tbAddCircularString->setDefaultAction( mActionCircularStringCurvePoint );
  connect( tbAddCircularString, SIGNAL( triggered( QAction * ) ), this, SLOT( toolButtonActionTriggered( QAction * ) ) );
  mDigitizeToolBar->insertWidget( mActionMoveFeature, tbAddCircularString );

  // Cad toolbar
  mAdvancedDigitizeToolBar->insertAction( mActionEnableTracing, mAdvancedDigitizingDockWidget->enableAction() );

  mTracer = new QgsMapCanvasTracer( canvas_xy, messageBar() );
  mTracer->setActionEnableTracing( mActionEnableTracing );
*/
}
void MBGuiApp::createStatusBar()
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
    connect( canvas_yz, SIGNAL( renderStarting() ), this, SLOT( canvasRefreshStarted() ) );
    connect( canvas_yz, SIGNAL( mapCanvasRefreshed() ), this, SLOT( canvasRefreshFinished() ) );
    connect( canvas_depths, SIGNAL( renderStarting() ), this, SLOT( canvasRefreshStarted() ) );
    connect( canvas_depths, SIGNAL( mapCanvasRefreshed() ), this, SLOT( canvasRefreshFinished() ) );
    connect( canvas_error, SIGNAL( renderStarting() ), this, SLOT( canvasRefreshStarted() ) );
    connect( canvas_error, SIGNAL( mapCanvasRefreshed() ), this, SLOT( canvasRefreshFinished() ) );
    connect( canvas_nei, SIGNAL( renderStarting() ), this, SLOT( canvasRefreshStarted() ) );
    connect( canvas_nei, SIGNAL( mapCanvasRefreshed() ), this, SLOT( canvasRefreshFinished() ) );
    connect( canvas_rei, SIGNAL( renderStarting() ), this, SLOT( canvasRefreshStarted() ) );
    connect( canvas_rei, SIGNAL( mapCanvasRefreshed() ), this, SLOT( canvasRefreshFinished() ) );
    connect( canvas_mean, SIGNAL( renderStarting() ), this, SLOT( canvasRefreshStarted() ) );
    connect( canvas_mean, SIGNAL( mapCanvasRefreshed() ), this, SLOT( canvasRefreshFinished() ) );

    // Bumped the font up one point size since 8 was too
    // small on some platforms. A point size of 9 still provides
    // plenty of display space on 1024x768 resolutions
    QFont myFont( "Arial", 9 );
    statusBar()->setFont( myFont );

    QList<QgsMapCanvas*> *canvases = new QList<QgsMapCanvas*>(*resultCanvases);
    canvases->prepend(canvas_yz);
    canvases->prepend(canvas_xy);

    //coords status bar widget
    mCoordsEdit = new QgsStatusBarCoordinatesWidget( statusBar() );
    mCoordsEdit->setMapCanvases( canvases );
//    mCoordsEdit->setMapCanvas( canvas_yz );
    mCoordsEdit->setFont( myFont );
    statusBar()->addPermanentWidget( mCoordsEdit, 0 );

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

    mScaleEdit = new QgsScaleComboBox( statusBar() );
    mScaleEdit->setObjectName( "mScaleEdit" );
    mScaleEdit->setFont( myFont );
    // seems setFont() change font only for popup not for line edit,
    // so we need to set font for it separately
    mScaleEdit->lineEdit()->setFont( myFont );
    mScaleEdit->setMinimumWidth( 10 );
    mScaleEdit->setContentsMargins( 0, 0, 0, 0 );
    mScaleEdit->setWhatsThis( tr( "Displays the current map scale" ) );
    mScaleEdit->setToolTip( tr( "Current map scale (formatted as x:y)" ) );

    statusBar()->addPermanentWidget( mScaleEdit, 0 );
    connect( mScaleEdit, SIGNAL( scaleChanged() ), this, SLOT( userScale() ) );

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

        mRotationEdit = new QgsDoubleSpinBox( statusBar() );
        mRotationEdit->setObjectName( "mRotationEdit" );
        mRotationEdit->setClearValue( 0.0 );
        mRotationEdit->setKeyboardTracking( false );
        mRotationEdit->setMaximumWidth( 120 );
        mRotationEdit->setDecimals( 1 );
        mRotationEdit->setRange( -180.0, 180.0 );
        mRotationEdit->setWrapping( true );
        mRotationEdit->setSingleStep( 5.0 );
        mRotationEdit->setFont( myFont );
        mRotationEdit->setWhatsThis( tr( "Shows the current map clockwise rotation "
                                         "in degrees. It also allows editing to set "
                                         "the rotation" ) );
        mRotationEdit->setToolTip( tr( "Current clockwise map rotation in degrees" ) );
        statusBar()->addPermanentWidget( mRotationEdit, 0 );
        connect( mRotationEdit, SIGNAL( valueChanged( double ) ), this, SLOT( userRotation() ) );

        showRotation();
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
    mOnTheFlyProjectionStatusButton = new QToolButton( statusBar() );
    mOnTheFlyProjectionStatusButton->setAutoRaise( true );
    mOnTheFlyProjectionStatusButton->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
    mOnTheFlyProjectionStatusButton->setObjectName( "mOntheFlyProjectionStatusButton" );
    // Maintain uniform widget height in status bar by setting button height same as labels
    // For Qt/Mac 3.3, the default toolbutton height is 30 and labels were expanding to match
    mOnTheFlyProjectionStatusButton->setMaximumHeight( mScaleLabel->height() );
    mOnTheFlyProjectionStatusButton->setIcon( QgsApplication::getThemeIcon( "mIconProjectionEnabled.png" ) );
    mOnTheFlyProjectionStatusButton->setWhatsThis( tr( "This icon shows whether "
      "on the fly coordinate reference system transformation is enabled or not. "
      "Click the icon to bring up "
      "the project properties dialog to alter this behaviour." ) );
    mOnTheFlyProjectionStatusButton->setToolTip( tr( "CRS status - Click "
      "to open coordinate reference system dialog" ) );
    connect( mOnTheFlyProjectionStatusButton, SIGNAL( clicked() ),
           this, SLOT( projectPropertiesProjections() ) );//bring up the project props dialog when clicked
    statusBar()->addPermanentWidget( mOnTheFlyProjectionStatusButton, 0 );
    statusBar()->showMessage( tr( "Ready" ) );

    mMessageButton = new QToolButton( statusBar() );
    mMessageButton->setAutoRaise( true );
    mMessageButton->setIcon( QgsApplication::getThemeIcon( "/mMessageLogRead.svg" ) );
    mMessageButton->setToolTip( tr( "Messages" ) );
    mMessageButton->setWhatsThis( tr( "Messages" ) );
    mMessageButton->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
    mMessageButton->setObjectName( "mMessageLogViewerButton" );
    mMessageButton->setMaximumHeight( mScaleLabel->height() );
    mMessageButton->setCheckable( true );
    statusBar()->addPermanentWidget( mMessageButton, 0 );
}

///////////// END OF GUI SETUP ROUTINES ///////////////


void MBGuiApp::createMBParamsGroupBox()
{
    //EXAMPLE 1
    doubleManager = new QtDoublePropertyManager(this);
    stringManager = new QtStringPropertyManager(this);
    pointFManager = new QtPointFPropertyManager(this);
    sizeFManager = new QtSizeFPropertyManager(this);
    //EXAMPLE 2
    boolManager = new QtBoolPropertyManager(this);
    intManager = new QtIntPropertyManager(this);
    enumManager = new QtEnumPropertyManager(this);
    groupManager = new QtGroupPropertyManager(this);
//    variantManager = new VariantManager(this); //this can be used for all of them; kept as an example
    filePathManager = new FilePathManager(this);

    //List: Dock
    //EXAMPLE 1
    //CONNECT ACTIONS TO PROPERTIES
    connect(doubleManager, SIGNAL(valueChanged(QtProperty *, double)),
              this, SLOT(valueChanged(QtProperty *, double)));
    connect(stringManager, SIGNAL(valueChanged(QtProperty *, const QString &)),
              this, SLOT(valueChanged(QtProperty *, const QString &)));
    connect(pointFManager, SIGNAL(valueChanged(QtProperty *, const QPointF &)),
              this, SLOT(valueChanged(QtProperty *, const QPointF &)));
    connect(sizeFManager, SIGNAL(valueChanged(QtProperty *, const QSizeF &)),
              this, SLOT(valueChanged(QtProperty *, const QSizeF &)));
    connect(boolManager, SIGNAL(valueChanged(QtProperty *, const bool)),
              this, SLOT(valueChanged(QtProperty *, const bool)));
    connect(enumManager, SIGNAL(valueChanged(QtProperty *, const int)),
              this, SLOT(valueChanged(QtProperty*, const int)));
//    connect(variantManager, SIGNAL(valueChanged(QtProperty *, const QString &)),
//              this, SLOT(valueChanged(QtProperty*, const QString &)));
    connect(intManager, SIGNAL(valueChanged(QtProperty *, int)),
              this, SLOT(valueChanged(QtProperty*, int)));
    connect(filePathManager, SIGNAL(valueChanged(QtProperty *, const QString &)),
              this, SLOT(valueChanged(QtProperty*, const QString &)));



      //FACTORIES: property types
      QtDoubleSpinBoxFactory *doubleSpinBoxFactory = new QtDoubleSpinBoxFactory(this);
      QtCheckBoxFactory *checkBoxFactory = new QtCheckBoxFactory(this);
      QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory(this);
      QtLineEditFactory *lineEditFactory = new QtLineEditFactory(this);
      QtEnumEditorFactory *comboBoxFactory = new QtEnumEditorFactory(this);
      QtSliderFactory *sliderFactory = new QtSliderFactory(this);
      QtScrollBarFactory *scrollBarFactory = new QtScrollBarFactory(this);
      QtVariantEditorFactory *variantFactory = new VariantFactory(this);
      FileEditFactory *fileEditFactory = new FileEditFactory(this);

      //DOCKED WIDGET: Property box container
      dock = new QDockWidget(tr("Run MB Params"),this);
      addDockWidget(Qt::LeftDockWidgetArea, dock);

      //EXAMPLE 1: Instantiate property fields
       //List display: Dock1
       propertyEditor = new QtTreePropertyBrowser(dock);
       propertyEditor->setFactoryForManager(doubleManager, doubleSpinBoxFactory);
       propertyEditor->setFactoryForManager(stringManager, lineEditFactory);
       propertyEditor->setFactoryForManager(pointFManager->subDoublePropertyManager(), doubleSpinBoxFactory);
       propertyEditor->setFactoryForManager(sizeFManager->subDoublePropertyManager(), doubleSpinBoxFactory);
       propertyEditor->setFactoryForManager(enumManager, comboBoxFactory);
       propertyEditor->setFactoryForManager(boolManager, checkBoxFactory);
//       propertyEditor->setFactoryForManager(variantManager, variantFactory);
       propertyEditor->setFactoryForManager(intManager, spinBoxFactory);
       propertyEditor->setFactoryForManager(filePathManager, fileEditFactory);


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
       //Run Params
       QtProperty *propertyRunParams0 = groupManager->addProperty("Run Params");
        propertyRunParams0prespliner = groupManager->addProperty("PreSpline");

//       //Configuration = bitstr
//       QtProperty *propertyRunParams_bitstr = enumManager->addProperty(tr("Configuration"));
//       enumManager->setEnumNames(propertyRunParams_bitstr, params->enumBits);
//       enumManager->setValue(propertyRunParams_bitstr, params->getBitStr());
//       addProperty(propertyRunParams_bitstr, QLatin1String("bitstr"));

       //Output Filename
       propertyRunParams_outfname = stringManager->addProperty(tr("Output Filename"));
       stringManager->setValue(propertyRunParams_outfname, params->getOutputFilename());
       addProperty(propertyRunParams_outfname, QLatin1String("outputfilename"));

       //Input Filelist
       propertyRunParams_ifilelist = stringManager->addProperty(tr("Input Filelist"));
       stringManager->setValue(propertyRunParams_ifilelist, params->getInputFilelist());
       addProperty(propertyRunParams_ifilelist, QLatin1String("inputfilelist"));

       //Grid Spacing (X,Y) (meters)
       propertyRunParams_gspacing = sizeFManager->addProperty(tr("Grid Spacing (X,Y)"));
       sizeFManager->setValue(propertyRunParams_gspacing, params->getGridSpacing());
       addProperty(propertyRunParams_gspacing, QLatin1String("gridspacing"));

       qDebug() << "FILTER: " << params->getSmoothingFilter() << endl;

       //Smoothing Filter
       propertyRunParams_filter = enumManager->addProperty(tr("Smoothing Filter"));
       enumManager->setEnumNames(propertyRunParams_filter, params->enumFilters);
       enumManager->setValue(propertyRunParams_filter, params->getSmoothingFilter());
       addProperty(propertyRunParams_filter, QLatin1String("filter"));

       //Smoothing Scale (X,Y) (meters)
       propertyRunParams_sscales = sizeFManager->addProperty(tr("Smoothing Scale (X,Y)"));
       sizeFManager->setValue(propertyRunParams_sscales, params->getSmoothingScales());
       sizeFManager->setMinimum(propertyRunParams_sscales, params->getGridSpacing());
       addProperty(propertyRunParams_sscales, QLatin1String("smoothingscale"));

       //Reference Lon,Lat (X,Y)
       propertyRunParams_refll = pointFManager->addProperty(tr("Reference Lon,Lat (X,Y)"));
       pointFManager->setValue(propertyRunParams_refll, params->getRefLL());
       addProperty(propertyRunParams_refll, QLatin1String("reflonlat"));

       //Rotation Angle (deg)
       propertyRunParams_rotangle = doubleManager->addProperty(tr("Rotation Angle (deg)"));
       doubleManager->setValue(propertyRunParams_rotangle, params->getRotAngle());
       addProperty(propertyRunParams_rotangle, QLatin1String("rotangle"));

       //Input in Meters
       propertyRunParams_inmeters = boolManager->addProperty(tr("Input in Meters"));
       boolManager->setValue(propertyRunParams_inmeters, params->getInputInMeters());
       addProperty(propertyRunParams_inmeters, QLatin1String("inputinmeters"));

       //Working Directory
//       propertyRunParams_wrkdir = variantManager->addProperty(VariantManager::filePathTypeId(), "Working Directory");
//       variantManager->setValue(propertyRunParams_wrkdir, params->getWrkdir());
//       addProperty(propertyRunParams_wrkdir, QLatin1String("wrkdir"));

       propertyRunParams_wrkdir  = filePathManager->addProperty("Working Directory");
       filePathManager->setValue(propertyRunParams_wrkdir, params->getWrkdir());
       addProperty(propertyRunParams_wrkdir, QLatin1String("wrkdir"));

       //PreInterpolators
       propertyRunParams_prespliner = enumManager->addProperty(tr("Pre-spliner"));
       enumManager->setEnumNames(propertyRunParams_prespliner, params->enumPreSpliners);
       enumManager->setValue(propertyRunParams_prespliner, params->getPreSpliner());
       addProperty(propertyRunParams_prespliner, QLatin1String("prespliner"));

       //PreSpliner Output Filename
       propertyRunParams_presplineroutfname = stringManager->addProperty(tr("Prespliner Output Filename"));
       stringManager->setValue(propertyRunParams_presplineroutfname, params->getPreSplinerOutputFilename());
       addProperty(propertyRunParams_presplineroutfname, QLatin1String("presplineoutputfilename"));

       //PreSpliner: write out xyz or xyze
       propertyRunParams_presplinerxyze = boolManager->addProperty(tr("Output Uncertainty in spline outputfile"));
       boolManager->setValue(propertyRunParams_presplinerxyze, params->getPreSplinerXYZE());
       addProperty(propertyRunParams_presplinerxyze, QLatin1String("presplinerxyze"));

       //PreSpliner: continue through MB with presplined results
       propertyRunParams_presplineruseresults = boolManager->addProperty(tr("Cont. w/ Results in MB"));
       boolManager->setValue(propertyRunParams_presplineruseresults, params->getPreSplinerUseResults());
       addProperty(propertyRunParams_presplineruseresults, QLatin1String("presplineruseresults"));

       //PreSpliner Spacing (X,Y) (meters)
       propertyRunParams_presplinerspacing = sizeFManager->addProperty(tr("Spacing (X,Y) (meters) "));
       sizeFManager->setMinimum(propertyRunParams_presplinerspacing, params->getPreSplinerSpacing());
       addProperty(propertyRunParams_presplinerspacing, QLatin1String("presplinerspacing"));

       //PreSpliner: Tension
       propertyRunParams_presplinertension = doubleManager->addProperty(tr("Tension Factor"));
       doubleManager->setValue(propertyRunParams_presplinertension, params->getPreSplinerTensionFactor());
       addProperty(propertyRunParams_presplinertension, QLatin1String("presplinertensionfactor"));

       //PreSpliner: Scale
       propertyRunParams_presplinerscale = doubleManager->addProperty(tr("Scale Factor"));
       doubleManager->setValue(propertyRunParams_presplinerscale, params->getPreSplinerScaleFactor());
       addProperty(propertyRunParams_presplinerscale, QLatin1String("presplinerscalefactor"));

       //PreSpliner: Alpha
       propertyRunParams_presplineralpha = doubleManager->addProperty(tr("Alpha"));
       doubleManager->setValue(propertyRunParams_presplineralpha, params->getPreSplinerAlpha());
       addProperty(propertyRunParams_presplineralpha, QLatin1String("presplineralpha"));


       //nThreads
       propertyRunParams_nthreads = intManager->addProperty(tr("No. of (Multi-)Threads"));
       intManager->setMaximum(propertyRunParams_nthreads,100);
       intManager->setMinimum(propertyRunParams_nthreads,0);
       intManager->setValue(propertyRunParams_nthreads, params->getNThreads());
       addProperty(propertyRunParams_nthreads, QLatin1String("nthreads"));

       //ComputeOffsets
       propertyRunParams_computeoffsets = boolManager->addProperty(tr("Compute Offsets"));
       boolManager->setValue(propertyRunParams_computeoffsets, params->getComputeOffsets());
       addProperty(propertyRunParams_computeoffsets, QLatin1String("computeoffsets"));

       //Modelflag
       propertyRunParams_modelflag = boolManager->addProperty(tr("Remove Bad Points"));
       boolManager->setValue(propertyRunParams_modelflag, params->getModelFlag());
       addProperty(propertyRunParams_modelflag, QLatin1String("modelflag"));

       //No Negative Depths
       propertyRunParams_nonegdepths = boolManager->addProperty(tr("No Negative Depths"));
       boolManager->setValue(propertyRunParams_nonegdepths, params->getNoNegDepths());
       addProperty(propertyRunParams_nonegdepths, QLatin1String("nonegdepths"));

       //Kriging
       propertyRunParams_kriging = boolManager->addProperty(tr("Kriging"));
       boolManager->setValue(propertyRunParams_kriging, params->getKriging());
       addProperty(propertyRunParams_kriging, QLatin1String("kriging"));

       //Smoothing scales in meters
       propertyRunParams_smoothinmeters = boolManager->addProperty(tr("Smoothing Scales in meters"));
       boolManager->setValue(propertyRunParams_smoothinmeters, params->getSmoothInMeters());
       addProperty(propertyRunParams_smoothinmeters, QLatin1String("smoothinmeters"));

       //Grid spacing in meters
       propertyRunParams_gridinmeters = boolManager->addProperty(tr("Grid Spacing in meters"));
       boolManager->setValue(propertyRunParams_gridinmeters, params->getGridInMeters());
       addProperty(propertyRunParams_gridinmeters, QLatin1String("gridinmeters"));

       //useUnscaledAvgInputs
       propertyRunParams_useunscaled = boolManager->addProperty(tr("Use Unscaled Avg Inputs"));
       boolManager->setValue(propertyRunParams_useunscaled, params->getUseUnscaledAvgInputs());
       addProperty(propertyRunParams_useunscaled, QLatin1String("useunscaled"));

       //PreInterpolatedLocs
       propertyRunParams_preinterplocs = boolManager->addProperty(tr("Use Pre-interpolated Locs"));
       boolManager->setValue(propertyRunParams_preinterplocs, params->getUsePreInterpolatedLocs());
       addProperty(propertyRunParams_preinterplocs, QLatin1String("preinterplocs"));

//       //PreInterpolatedLocs Filename
//       propertyRunParams_preinterplocsfname = variantManager->addProperty(VariantManager::filePathTypeId(), tr("Pre-interpolated Locs File"));
//       variantManager->setValue(propertyRunParams_preinterplocsfname, params->getPreinterpolatedLocsFile());
//       addProperty(propertyRunParams_preinterplocsfname, QLatin1String("preinterpfilename"));

       propertyRunParams_preinterplocsfname  = filePathManager->addProperty("Pre-Interpolated Locs File");
       filePathManager->setValue(propertyRunParams_preinterplocsfname, params->getPreinterpolatedLocsFile());
       //filePathManager->setFilter(preinterplocsfile, "Source files (*.cpp *.c)");
       addProperty(propertyRunParams_preinterplocsfname, QLatin1String("preinterpfilename"));

       //Smoothing scales in meters
       propertyRunParams_preinterplocsusage = boolManager->addProperty(tr("Lat,Lon Order"));
       boolManager->setValue(propertyRunParams_preinterplocsusage, params->getPreinterpLocsUsage());
       addProperty(propertyRunParams_preinterplocsusage, QLatin1String("preinterplocsusage"));






//       propertyRunParams0->addSubProperty(propertyRunParams_bitstr);    //Configuration
       propertyRunParams0->addSubProperty(propertyRunParams_wrkdir);    //Working Directory
       propertyRunParams0->addSubProperty(propertyRunParams_outfname);  //Output Filename
       propertyRunParams0->addSubProperty(propertyRunParams_ifilelist); //Input Filelist
       propertyRunParams0->addSubProperty(propertyRunParams_gspacing);  //Grid Spacing (X,Y)
        propertyRunParams0->addSubProperty(propertyRunParams_gridinmeters); //Grid spacing in meters
       propertyRunParams0->addSubProperty(propertyRunParams_filter);    //Smoothing Filter
       propertyRunParams0->addSubProperty(propertyRunParams_sscales);   //Smoothing Scale (X,Y)
        propertyRunParams0->addSubProperty(propertyRunParams_smoothinmeters); //smoothing scales in meters
       propertyRunParams0->addSubProperty(propertyRunParams_refll);     //Reference Lon,Lat (X,Y)
       propertyRunParams0->addSubProperty(propertyRunParams_rotangle);  //Rotation Angle (deg)
       propertyRunParams0->addSubProperty(propertyRunParams_inmeters);  //Input in Meters
       propertyRunParams0->addSubProperty(propertyRunParams_computeoffsets); //compute offsets
        propertyRunParams0->addSubProperty(propertyRunParams_kriging);
        propertyRunParams0->addSubProperty(propertyRunParams_modelflag);
        propertyRunParams0->addSubProperty(propertyRunParams_nonegdepths);
        propertyRunParams0->addSubProperty(propertyRunParams_useunscaled);
        propertyRunParams0->addSubProperty(propertyRunParams_preinterplocs);
        propertyRunParams_preinterplocsfname->setEnabled(params->getUsePreInterpolatedLocs());
        propertyRunParams_preinterplocsusage->setEnabled(params->getUsePreInterpolatedLocs());
        propertyRunParams0->addSubProperty(propertyRunParams_preinterplocsfname);
        propertyRunParams0->addSubProperty(propertyRunParams_preinterplocsusage);
        propertyRunParams0->addSubProperty(propertyRunParams_nthreads);

        propertyRunParams0->addSubProperty(propertyRunParams_prespliner);
//       propertyRunParams0prespliner->addSubProperty(propertyRunParams_prespliner);
       propertyRunParams0prespliner->addSubProperty(propertyRunParams_presplinerspacing);
       propertyRunParams0prespliner->addSubProperty(propertyRunParams_presplineroutfname);
       propertyRunParams0prespliner->addSubProperty(propertyRunParams_presplinertension);
       propertyRunParams0prespliner->addSubProperty(propertyRunParams_presplinerscale);
       propertyRunParams0prespliner->addSubProperty(propertyRunParams_presplineralpha);
       propertyRunParams_presplinerscale->setEnabled(params->getPreSpliner() == 1);
       propertyRunParams_presplineralpha->setEnabled(params->getPreSpliner() == 1);
       propertyRunParams0prespliner->addSubProperty(propertyRunParams_presplineruseresults);
       propertyRunParams0prespliner->addSubProperty(propertyRunParams_presplinerxyze);
       propertyRunParams0prespliner->setEnabled(params->getPreSpliner()>0);
       propertyRunParams0->addSubProperty(propertyRunParams0prespliner);



       //Dock2: ButtonPropertyBrowser
       QtAbstractPropertyBrowser *editor2 = new QtGroupBoxPropertyBrowser(dock);//QtButtonPropertyBrowser(dock2);
       editor2->setFactoryForManager(boolManager, checkBoxFactory);
       editor2->setFactoryForManager(intManager, spinBoxFactory);//scrollBarFactory);
       editor2->setFactoryForManager(enumManager, comboBoxFactory);
       editor2->setFactoryForManager(doubleManager, doubleSpinBoxFactory);
       editor2->setFactoryForManager(stringManager, lineEditFactory);
       editor2->setFactoryForManager(pointFManager->subDoublePropertyManager(), doubleSpinBoxFactory);
       editor2->setFactoryForManager(sizeFManager->subDoublePropertyManager(), doubleSpinBoxFactory);
//       editor2->setFactoryForManager(variantManager, variantFactory);
       editor2->setFactoryForManager(filePathManager, fileEditFactory);
       editor2->addProperty(propertyRunParams0);
       QScrollArea *scroll2 = new QScrollArea(dock);
       scroll2->setWidgetResizable(true);
       scroll2->setWidget(editor2);
//       dock2->setWidget(scroll2);

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
        QPushButton *runButton = new QPushButton(tr("&Run"));
        connect(runButton, SIGNAL(released()), this, SLOT(runMB()));

        QPushButton *logButton = new QPushButton(tr("&Logfile"));
        connect(logButton, SIGNAL(released()), this, SLOT(showMBLog()));


        //DOCKED WIDGET:logfile container
        docklog = new QDockWidget(tr("MergeBathy Output"));
        docklog->setHidden(true);

        QVBoxLayout * logLayout = new QVBoxLayout(docklog);
        logWindow = new QPlainTextEdit(tr("MergeBathy Output"),docklog);
        logWindow->setReadOnly(true);
        logWindow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred );
        logWindow->adjustSize();

        logLayout->addWidget(logWindow);
        docklog->setLayout(logLayout);
        docklog->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred );
        docklog->adjustSize();


        QHBoxLayout * buttonLayout = new QHBoxLayout();
        QGroupBox * buttonBox = new QGroupBox(tr("Buttons"),dock);
        buttonLayout->addWidget(runButton);
        buttonLayout->addWidget(logButton);
        buttonBox->setLayout(buttonLayout);

        vGroupBox_mbparams_dock = new QGroupBox(tr("MBParams layout"),dock);
        mpLayout_mbparams_dock = new QVBoxLayout(dock);
        mpLayout_mbparams_dock->addWidget(tab);
//        mpLayout_mbparams_dock->addWidget(runButton);
//        mpLayout_mbparams_dock->addWidget(logButton);
        mpLayout_mbparams_dock->addWidget(buttonBox);
        vGroupBox_mbparams_dock->setLayout(mpLayout_mbparams_dock);
        dock->setWidget(vGroupBox_mbparams_dock);
}
void MBGuiApp::showMBLog()
{
    docklog->setHidden(false);
}



/*void MBGuiApp::itemMoved(QtCanvasItem *item)
{
    if (item != currentItem)
        return;

    doubleManager->setValue(idToProperty[QLatin1String("xpos")], item->x());
    doubleManager->setValue(idToProperty[QLatin1String("ypos")], item->y());
    doubleManager->setValue(idToProperty[QLatin1String("zpos")], item->z());
}*/
void MBGuiApp::updateExpandState()
{
    QList<QtBrowserItem *> list = propertyEditor->topLevelItems();
    QListIterator<QtBrowserItem *> it(list);
    while (it.hasNext()) {
        QtBrowserItem *item = it.next();
        QtProperty *prop = item->property();
        idToExpanded[propertyToId[prop]] = propertyEditor->isExpanded(item);
    }
}

void MBGuiApp::addProperty(QtProperty *property, const QString &id)
{
    propertyToId[property] = id;
    idToProperty[id] = property;
    QtBrowserItem *item = propertyEditor->addProperty(property);
    if (idToExpanded.contains(id))
        propertyEditor->setExpanded(item, idToExpanded[id]);
}
void MBGuiApp::valueChanged(QtProperty *property, bool value)
{
    if (!propertyToId.contains(property))
        return;

    if (!currentItem)
        return;

    QString id = propertyToId[property];
   if (id == QLatin1String("inputinmeters")) {
        params->setInputInMeters(value);
    }
   else if (id == QLatin1String("computeoffsets")) {
        params->setComputeOffsets(value);
    }
   else if (id == QLatin1String("modelflag")) {
        params->setModelFlag(value);
    }
   else if (id == QLatin1String("nonegdepths")) {
        params->setNoNegDepths(value);
    }
    else if (id == QLatin1String("kriging")) {
        params->setKriging(value);
    }
   else if (id == QLatin1String("smoothinmeters")) {
        params->setSmoothInMeters(value);
    }
   else if (id == QLatin1String("gridinmeters")) {
        params->setGridInMeters(value);
   }
   else if (id == QLatin1String("useunscaled")) {
        params->setUseUnscaledAvgInputs(value);
    }
   else if (id == QLatin1String("preinterplocs")) {
        params->setUsePreInterpolatedLocs(value);
    }
   else if (id == QLatin1String("presplinerxyze")) {
        params->setPreSplinerXYZE(value);
    }
   else if (id == QLatin1String("presplineruseresults")) {
        params->setPreSplinerUseResults(value);
    }
   else if (id == QLatin1String("preinterplocsusage")) {
        params->setPreinterpLocsUsage(value);
    }

   propertyRunParams_preinterplocsfname->setEnabled(params->getUsePreInterpolatedLocs());
   propertyRunParams_preinterplocsusage->setEnabled(params->getUsePreInterpolatedLocs());

}
void MBGuiApp::valueChanged(QtProperty *property, int value)
{
    if (!propertyToId.contains(property))
        return;

    if (!currentItem)
        return;

    QString id = propertyToId[property];
   if (id == QLatin1String("bitstr")) {
        params->setBitStr(value);
    }
   else if (id == QLatin1String("filter")) {
        params->setSmoothingFilter(value);
    }
   else if (id == QLatin1String("nthreads")) {
        params->setNThreads(value);
    }
   else if (id == QLatin1String("prespliner")) {
        params->setPreSpliner(value);
    }
   propertyRunParams_presplinerscale->setEnabled(params->getPreSpliner() == 1);
   propertyRunParams_presplineralpha->setEnabled(params->getPreSpliner() == 1);
   propertyRunParams0prespliner->setEnabled(params->getPreSpliner()>0);
}

void MBGuiApp::valueChanged(QtProperty *property, double value)
{
    if (!propertyToId.contains(property))
        return;

    if (!currentItem)
        return;

    QString id = propertyToId[property];
   if (id == QLatin1String("rotangle")) {
        params->setRotAngle(value);
    } else if (id == QLatin1String("presplinertensionfactor")) {
        params->setPreSplinerTensionFactor(value);
    } else if (id == QLatin1String("presplinerscalefactor")) {
        params->setPreSplinerScaleFactor(value);
    } else if (id == QLatin1String("presplineralpha")) {
        params->setPreSplinerAlpha(value);
    }
}
void MBGuiApp::valueChanged(QtProperty *property, const QString &value)
{
    if (!propertyToId.contains(property))
        return;

    if (!currentItem)
        return;

    QString id = propertyToId[property];
    if (id == QLatin1String("outputfilename")) {
        params->setOutputFilename(value.trimmed());
    } else if (id == QLatin1String("inputfilelist")) {
        params->setInputFilelist(value.trimmed());
    }else if(id == QLatin1String("wrkdir")) {
        params->setWrkdir(value.trimmed());
    }else if(id == QLatin1String("presplineoutputfilename")) {
        params->setPreSplinerOutputFilename(value.trimmed());
    }else if(id == QLatin1String("preinterpfilename")) {
        params->setPreinterpolatedLocsFile(value.trimmed());
    }
}

void MBGuiApp::valueChanged(QtProperty *property, const QPointF &value)
{
    if (!propertyToId.contains(property))
        return;

    if (!currentItem)
        return;

    QString id = propertyToId[property];
    if (id == QLatin1String("reflonlat")) {
        params->setRefLL(value.x(),value.y());
    }
}
void MBGuiApp::valueChanged(QtProperty *property, const QSizeF &value)
{
    if (!propertyToId.contains(property))
        return;

    if (!currentItem)
        return;

    QString id = propertyToId[property];
    if (id == QLatin1String("gridspacing")) {
        params->setGridSpacing(value.width(),value.height());
        sizeFManager->setMinimum(propertyRunParams_sscales, params->getGridSpacing());
        if(params->getPreSpliner()>0)
            sizeFManager->setMinimum(propertyRunParams_presplinerspacing, params->getGridSpacing());
    }
    else if (id == QLatin1String("smoothingscale")) {
        params->setSmoothingScales(value.width(),value.height());
    }
    else if (id == QLatin1String("presplinerspacing")) {
        params->setPreSplinerSpacing(value.width(),value.height());

    }
}

void MBGuiApp::createUndoView()
{
    undoView = new QUndoView(undoStack);
    undoView->setWindowTitle(tr("Command List"));
    undoView->show();
    undoView->setAttribute(Qt::WA_QuitOnClose, false);
}
void MBGuiApp::refreshMapTools_xy()
{
    //this function is not finished and is not correct
    QgsMapTool *tool = canvas_rei->mapTool();

    if(!checkBox_lock_canvas_rei->isChecked())
        canvas_rei->setMapTool(tool);
    else
        canvas_rei->unsetMapTool(tool);

}
void MBGuiApp::refreshMapTools_yz()
{
    //this function is not finished and is not correct
    QgsMapTool *tool = canvas_yz->mapTool();

    if(!checkBox_lock_canvas_yz->isChecked())
        canvas_yz->setMapTool(tool);
    else
        canvas_yz->unsetMapTool(tool);

}
void MBGuiApp::refreshMapTools_depths()
{
    //this function is not finished and is not correct
    QgsMapTool *tool = canvas_depths->mapTool();

    if(!checkBox_lock_canvas_depths->isChecked())
        canvas_depths->setMapTool(tool);
    else
        canvas_depths->unsetMapTool(tool);

}
void MBGuiApp::refreshMapTools_error()
{
    //this function is not finished and is not correct
    QgsMapTool *tool = canvas_error->mapTool();

    if(!checkBox_lock_canvas_error->isChecked())
        canvas_error->setMapTool(tool);
    else
        canvas_error->unsetMapTool(tool);

}
void MBGuiApp::refreshMapTools_nei()
{
    //this function is not finished and is not correct
    QgsMapTool *tool = canvas_nei->mapTool();

    if(!checkBox_lock_canvas_nei->isChecked())
        canvas_nei->setMapTool(tool);
    else
        canvas_nei->unsetMapTool(tool);

}
void MBGuiApp::refreshMapTools_rei()
{
    //this function is not finished and is not correct
    QgsMapTool *tool = canvas_rei->mapTool();

    if(!checkBox_lock_canvas_rei->isChecked())
        canvas_rei->setMapTool(tool);
    else
        canvas_rei->unsetMapTool(tool);
}
void MBGuiApp::refreshMapTools_mean()
{
    //this function is not finished and is not correct
    QgsMapTool *tool = canvas_mean->mapTool();

    if(!checkBox_lock_canvas_mean->isChecked())
        canvas_mean->setMapTool(tool);
    else
        canvas_mean->unsetMapTool(tool);

}
void MBGuiApp::refreshMapTools()
{
    //this function is not finished and is not correct
    QgsMapTool *tool = canvas_xy->mapTool();

    if(tool == mpPanTool_xy)
        panMode();
    else if(tool == mpZoomInTool_xy)
        zoomInMode();
    else if(tool == mpZoomOutTool_xy)
        zoomOutMode();

    if(!checkBox_lock_canvas_xy->isChecked())
        canvas_xy->setMapTool(tool);
    else
        canvas_xy->unsetMapTool(tool);
    if(!checkBox_lock_canvas_yz->isChecked())
        canvas_yz->setMapTool(tool);
    else
        canvas_yz->unsetMapTool(tool);
    if(!checkBox_lock_canvas_depths->isChecked())
        canvas_depths->setMapTool(tool);
    else
        canvas_depths->unsetMapTool(tool);
    if(!checkBox_lock_canvas_error->isChecked())
        canvas_error->setMapTool(tool);
    else
        canvas_error->unsetMapTool(tool);
    if(!checkBox_lock_canvas_nei->isChecked())
        canvas_nei->setMapTool(tool);
    else
        canvas_nei->unsetMapTool(tool);
    if(!checkBox_lock_canvas_rei->isChecked())
        canvas_rei->setMapTool(tool);
    else
        canvas_rei->unsetMapTool(tool);
    if(!checkBox_lock_canvas_mean->isChecked())
        canvas_mean->setMapTool(tool);
    else
        canvas_mean->unsetMapTool(tool);

}


void MBGuiApp::panMode()
{
//    bool locked_xy = 0;
//    if(checkBox_lock_canvas_xy->isChecked())
//        locked_xy = 1;

    //need to get the current canvas and use that tool for all the canvases that are not locked.

//    if(!checkBox_lock_canvas_xy->isChecked())
//        canvas_xy->setMapTool(mpPanTool);
//    if(!checkBox_lock_canvas_yz->isChecked())
//        canvas_yz->setMapTool(mpPanTool_yz);
//    if(!checkBox_lock_canvas_depths->isChecked())
//        canvas_depths->setMapTool(mpPanTool_depths);
//    if(!checkBox_lock_canvas_error->isChecked())
//        canvas_error->setMapTool(mpPanTool_error);
//    if(!checkBox_lock_canvas_nei->isChecked())
//        canvas_nei->setMapTool(mpPanTool_nei);
//    if(!checkBox_lock_canvas_rei->isChecked())
//        canvas_rei->setMapTool(mpPanTool_rei);
//    if(!checkBox_lock_canvas_mean->isChecked())
//        canvas_mean->setMapTool(mpPanTool_mean);

        canvas_xy->setMapTool(mpPanTool_xy);
        canvas_yz->setMapTool(mpPanTool_yz);
        canvas_depths->setMapTool(mpPanTool_depths);
        canvas_error->setMapTool(mpPanTool_error);
        canvas_nei->setMapTool(mpPanTool_nei);
        canvas_rei->setMapTool(mpPanTool_rei);
        canvas_mean->setMapTool(mpPanTool_mean);

//    if(checkBox_lock_canvas_yz->isChecked())
//        canvas_xy->setMapTool(mpPanTool);
//    else
//        canvas_yz->setMapTool(mpPanTool_yz);

//    if(checkBox_lock_canvas_yz->isChecked())
//        canvas_yz->setMapTool(mpPanTool_yz);
//    else
//        canvas_xy->setMapTool(mpPanTool);
}
void MBGuiApp::zoomInMode()
{
//    if(!checkBox_lock_canvas_xy->isChecked())
//        canvas_xy->setMapTool(mpZoomInTool);
//    if(!checkBox_lock_canvas_yz->isChecked())
//        canvas_yz->setMapTool(mpZoomInTool_yz);
//    if(!checkBox_lock_canvas_depths->isChecked())
//        canvas_depths->setMapTool(mpZoomInTool_depths);
//    if(!checkBox_lock_canvas_error->isChecked())
//        canvas_error->setMapTool(mpZoomInTool_error);
//    if(!checkBox_lock_canvas_nei->isChecked())
//        canvas_nei->setMapTool(mpZoomInTool_nei);
//    if(!checkBox_lock_canvas_rei->isChecked())
//        canvas_rei->setMapTool(mpZoomInTool_rei);
//    if(!checkBox_lock_canvas_mean->isChecked())
//        canvas_mean->setMapTool(mpZoomInTool_mean);



        canvas_xy->setMapTool(mpZoomInTool_xy);
        canvas_yz->setMapTool(mpZoomInTool_yz);
        canvas_depths->setMapTool(mpZoomInTool_depths);
        canvas_error->setMapTool(mpZoomInTool_error);
        canvas_nei->setMapTool(mpZoomInTool_nei);
        canvas_rei->setMapTool(mpZoomInTool_rei);
        canvas_mean->setMapTool(mpZoomInTool_mean);
}
void MBGuiApp::zoomOutMode()
{
    if(checkBox_lock_canvas_xy->isChecked())
        canvas_xy->setMapTool(mpZoomOutTool_xy);
    if(!checkBox_lock_canvas_yz->isChecked())
        canvas_yz->setMapTool(mpZoomOutTool_yz);
    if(!checkBox_lock_canvas_depths->isChecked())
        canvas_depths->setMapTool(mpZoomOutTool_depths);
    if(!checkBox_lock_canvas_error->isChecked())
        canvas_error->setMapTool(mpZoomOutTool_error);
    if(!checkBox_lock_canvas_nei->isChecked())
        canvas_nei->setMapTool(mpZoomOutTool_nei);
    if(!checkBox_lock_canvas_rei->isChecked())
        canvas_rei->setMapTool(mpZoomOutTool_rei);
    if(!checkBox_lock_canvas_mean->isChecked())
        canvas_mean->setMapTool(mpZoomOutTool_mean);
}
void MBGuiApp::onWillAddChildren(QgsLayerTreeNode *node, int indexFrom, int indexTo) {
    qDebug() << "WILL ADD" << node << indexFrom << indexTo << endl;
}
// Access newly added nodes
void MBGuiApp::onAddedChildren(QgsLayerTreeNode *node, int indexFrom, int indexTo) {
    qDebug() << "ADDED" << node << indexFrom << indexTo << endl;
}
// Access nodes being removed
void MBGuiApp::onWillRemoveChildren(QgsLayerTreeNode *node, int indexFrom, int indexTo) {
  qDebug() << "WILL REMOVE" << node << indexFrom << indexTo << endl;
}
void MBGuiApp::onRemovedChildren(QgsLayerTreeNode *node, int indexFrom, int indexTo) {
  qDebug() << "REMOVED" << node << indexFrom << indexTo << endl;
}
void MBGuiApp::onVisibilityChanged(QgsLayerTreeNode *node,Qt::CheckState state) {
    qDebug() << "onVisibilityChanged TOGGLED" << node << " " << state << endl;

//    QgsMapLayerRegistry::instance()->mapLayer(view->currentLayer()->id());
//    qDebug() << "isVisible" << myLayerSet.at(myLayerSet.indexOf(view->currentLayer())).isVisible() << endl;
//    canvas_yz->setLayerSet(myLayerSet);

    //need to get the child visibilities and update the yz layer trees and this needs to move to a seperate slot
    QList<QgsLayerTreeNode*> ch = mLayerTreeView_yz->currentGroupNode()->children();
    QStringList layers = mLayerTreeView->currentLayer()->subLayers();

    mLayerTreeView->currentLayer()->setSubLayerVisibility(layers[0],state);
//    QgsMapCanvasLayer * l = static_cast<QgsMapCanvasLayer *>(layers[0]);



//    mLayerTreeView_yz->currentLayer()->setSubLayerVisibility("included",state);
    mLayerTreeView_yz->currentGroupNode()->setVisible(state);

}
void MBGuiApp::onCustomPropertyChanged(QgsLayerTreeNode *node, const QString &key) {
  qDebug() << "CHANGED" << node << " " << key << endl;

//  QVariant* n = qobject_cast<QVariant *>( node);
//  mLayerTreeView_yz->currentGroupNode()->setCustomProperty(key,*n);

}
void MBGuiApp::onExpandedChanged(QgsLayerTreeNode *node, bool expanded) {
  qDebug() << "FOLDED" << node << " " << expanded << endl;
  mLayerTreeView_yz->currentGroupNode()->setExpanded(expanded);
}
void MBGuiApp::onNameChanged(QgsLayerTreeNode *node, QString name) {
  qDebug() << "RENAMED" << node << " to " << name << endl;
}
void MBGuiApp::onChange(QgsMapLayer *layer) {
    qDebug() << "OnChange THIS HAS CHANGED:" <<endl;//<< "Current Layer: " << layer->name() << endl;
    //QMessageBox::information(nullptr, "Change", "Current Layer: "+layer->name());
//    mLayerTreeView_yz->setCurrentLayer(layer);

}
void MBGuiApp::onLayerSavedAs(QgsMapLayer *l, const QString &path) {
    qDebug() << "SAVEDAS" << l << " to " << path << endl;
}
void MBGuiApp::deleteItem()
{
    if (mLayerTreeView->selectedLayerNodes().isEmpty())
        return;
    if (mLayerTreeView_yz->selectedLayerNodes().isEmpty())
        return;

    QUndoCommand *deleteCommand = new DeleteCommand(root,root_yz,mLayerTreeView,mLayerTreeView_yz);
    undoStack->push(deleteCommand);
}
void MBGuiApp::addLayer()
{
    QUndoCommand *addCommand = new AddCommand(root,root_yz,canvas_xy,canvas_yz,myLayerSet,myLayerSet_yz,mLayerTreeView,mLayerTreeView_yz,selectedMap,selectedMap_yz,layerSuffixes);
    undoStack->push(addCommand);
//    activateDeactivateLayerRelatedActions(activeLayer() );
}
void MBGuiApp::addGroup()
{
    QUndoCommand *addGroupCommand = new AddGroupCommand(root,root_yz,mLayerTreeView,mLayerTreeView_yz);
    undoStack->push(addGroupCommand);
}

void MBGuiApp::itemMenuAboutToHide()

{
    deleteAction->setEnabled(true);
}
// Check if we have files selected in order to enable delete action
void MBGuiApp::itemMenuAboutToShow()
{
    if(mLayerTreeView==nullptr)
        deleteAction->setEnabled(false);
    else
        deleteAction->setEnabled(!mLayerTreeView->selectedLayers().isEmpty());
}
void MBGuiApp::fileMenuAboutToHide()
{
    closeProjectAction->setEnabled(true);
}
void MBGuiApp::fileMenuAboutToShow()
{
    closeProjectAction->setEnabled(!QgsProject::instance()->fileName().isEmpty());
}

void MBGuiApp::about2()
{
    QMessageBox::about(this, tr("MergeBathy GUI"),
                       tr("The <b>MergeBathy GUI </b> was developed by NRL in support of NAVO."
                          "(2017) Version 0.1."));
}
void MBGuiApp::setLocalDataPath()
{
    setDataPath(false);
}
void MBGuiApp::setGlobalDataPath()
{
    setDataPath(true);
}
void MBGuiApp::setLocalMBPath()
{
    setMBPath(false);
}
void MBGuiApp::setGlobalMBPath()
{
    setMBPath(true);
}
void MBGuiApp::setDataPath(bool isGlobal)
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
        msgBox.setWindowTitle(tr("Set Global Data Path"));
    else
        msgBox.setWindowTitle(tr("Set Local Data Path"));
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
                settings.setValue("MBGuiApp/GlobalDataPath", dataPath);
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
void MBGuiApp::setMBPath(bool isGlobal)
{
    QgsProject * project = QgsProject::instance();
    QString mbPath;
    QString oldPath;
    if(isGlobal)
        mbPath = globalMBPath;
    else
        mbPath = localMBPath;
    oldPath = mbPath;

    QMessageBox msgBox;
    if(isGlobal)
        msgBox.setWindowTitle(tr("Set Global mergeBathy.exe Path"));
    else
        msgBox.setWindowTitle(tr("Set Local mergeBathy.exe Path"));
    msgBox.setModal(false);
    msgBox.setText(tr("Current MergeBathy.exe Path set to: "));
    msgBox.setInformativeText(mbPath);
    QPushButton *directoryButton = msgBox.addButton(tr("Browse..."), QMessageBox::ActionRole);
    QPushButton *okButton = msgBox.addButton(QMessageBox::Ok);
    QPushButton *cancelButton = msgBox.addButton(QMessageBox::Cancel);

    msgBox.exec();

    while(true) {
        if(msgBox.clickedButton() == directoryButton) {
                QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
                mbPath = QFileDialog::getExistingDirectory(this, tr("Set mergeBathy.exe Path"), mbPath, options);
                if(mbPath.isEmpty())
                        mbPath = oldPath;
                msgBox.setInformativeText(mbPath);
                msgBox.exec();
        }
        else if(msgBox.clickedButton() == okButton && oldPath != mbPath) {
            if(isGlobal) {
                globalMBPath = mbPath;
                QSettings settings("NRL", "MergeBathy GUI");
                settings.setValue("MBGuiApp/GlobalMBPath", mbPath);
            }
            else {
                localMBPath = mbPath;
                project->writeEntry("myproject", "LocalMBPath", mbPath);
                project->writeEntry("myproject", "isLocalMBPathSet", true);
            }
            break;
        } else break;
    }
}
void MBGuiApp::closeProject()
{
    QgsProject * project = QgsProject::instance();
    if (userReallyWantsToQuit()) {
        project->clear();
        //QGIS
        // clear out any stuff from project
        canvas_xy->freeze( true );
        canvas_yz->freeze( true );
        QList<QgsMapCanvasLayer> emptyList;
        canvas_xy->setLayerSet( emptyList );
        canvas_xy->clearCache();
        canvas_yz->setLayerSet( emptyList );
        canvas_yz->clearCache();

        removeAllLayers();
        if(project->fileName().isEmpty())
        {
            localDataPath = globalDataPath;
            localMBPath   = globalMBPath;
        }
        cleanUp();
        setWindowTitle("MergeBathy GUI: Untitled Project");
    }
}
//sams
void MBGuiApp::open()
{
    if (userReallyWantsToQuit()) {
        QString myProjectName = QFileDialog::getOpenFileName(this, tr("Choose a QGIS project file"),
                                                             QCoreApplication::applicationDirPath () + "./" + QgsProject::instance()->title(),
                                                             tr( "QGIS files" ) + " (*.qgs *.QGS)" );

        // Load another project
        bool isOpen = QgsProject::instance()->read(QFileInfo(myProjectName));
        if(isOpen) {
            readProjectSettings();
            setWindowTitle("MergeBathy GUI: "+ QFileInfo(myProjectName).fileName());
        }
    }
}
bool MBGuiApp::save()
{
    // Get the project instance
    QgsProject * project = QgsProject::instance();


    if(project->fileName().isEmpty())
        saveAs();
    else {
        writeParamSettings();
        project->write(project->fileName());
        statusBar()->showMessage(tr("Project saved"), 2000);
    }
    return true;
}
void MBGuiApp::saveAs()
{
    QString myProjectName = QFileDialog::getSaveFileName(this, tr("Save File As"),QCoreApplication::applicationDirPath () + "./",".qgs");

    // Get the project instance
    QgsProject * project = QgsProject::instance();

    writeParamSettings();

    // Save as another project
    project->write(myProjectName);
    setWindowTitle("MergeBathy GUI: "+ QFileInfo(myProjectName).fileName());
    statusBar()->showMessage(tr("Project saved"), 2000);
}
void MBGuiApp::writeParamSettings()
{
    // Get the project instance
    QgsProject * project = QgsProject::instance();

    project->writeEntry("myproject", "isParamsSet", true);

    project->writeEntry("myproject", "wrkdir", params->getWrkdir());
    project->writeEntry("myproject", "outputfilename", params->getOutputFilename());
    project->writeEntry("myproject", "inputfilelist", params->getInputFilelist());
    project->writeEntry("myproject", "inputinmeters", params->getInputInMeters());
    project->writeEntry("myproject", "smoothingfilter", params->getSmoothingFilter());
    project->writeEntry("myproject", "smoothingscalex", params->getSmoothingScaleX());
    project->writeEntry("myproject", "smoothingscaley", params->getSmoothingScaleY());
    project->writeEntry("myproject", "gridspacingx", params->getGridSpacingX());
    project->writeEntry("myproject", "gridspacingy", params->getGridSpacingY());
//    project->writeEntry("myproject", "bitstr", params->getBitStr());
    project->writeEntry("myproject", "reflonx", params->getRefLL().x());
    project->writeEntry("myproject", "reflaty", params->getRefLL().y());
    project->writeEntry("myproject", "rotangle", params->getRotAngle());

    project->writeEntry("myproject", "prespliner", params->getPreSpliner());
    project->writeEntry("myproject", "presplineoutputfilename", params->getPreSplinerOutputFilename());
    project->writeEntry("myproject", "presplinerxyze", params->getPreSplinerXYZE());
    project->writeEntry("myproject", "presplineruseresults", params->getPreSplinerUseResults());
    project->writeEntry("myproject", "presplinerspacingx", params->getPreSplinerSpacing().width());
    project->writeEntry("myproject", "presplinerspacingy", params->getPreSplinerSpacing().height());
    project->writeEntry("myproject", "presplinertensionfactor", params->getPreSplinerTensionFactor());
    project->writeEntry("myproject", "presplinerscalefactor", params->getPreSplinerScaleFactor());
    project->writeEntry("myproject", "presplineralpha", params->getPreSplinerAlpha());

    project->writeEntry("myproject", "nthreads", params->getNThreads());
    project->writeEntry("myproject", "computeoffsets", params->getComputeOffsets());
    project->writeEntry("myproject", "modelflag", params->getModelFlag());
    project->writeEntry("myproject", "nonegdepths", params->getNoNegDepths());
    project->writeEntry("myproject", "kriging", params->getKriging());
    project->writeEntry("myproject", "smoothinmeters", params->getSmoothInMeters());
    project->writeEntry("myproject", "gridinmeters", params->getGridInMeters());
    project->writeEntry("myproject", "useunscaled", params->getUseUnscaledAvgInputs());
    project->writeEntry("myproject", "preinterplocs", params->getUsePreInterpolatedLocs());
    project->writeEntry("myproject", "preinterpfilename", params->getPreinterpolatedLocsFile());
    project->writeEntry("myproject", "preinterplocsusage", params->getPreinterpLocsUsage());

    //Save layer suffixes so we can write inputFileLists when we run MB
    project->writeEntry("myproject", "layerSuffixes_size",layerSuffixes->size());
    QStringList keys = layerSuffixes->keys();
    for(int i = 0; i < keys.size(); ++i)
    {
        project->writeEntry("myproject", "layerSuffixes/key_"+ QString(i), keys[i]);
        project->writeEntry("myproject", "layerSuffixes/value_"+ QString(i), layerSuffixes->value(keys[i]));
    }
}
//Exit program
void MBGuiApp::closeEvent(QCloseEvent *event)
{
    if (userReallyWantsToQuit()) {
        writeSettings();
        event->accept();
    } else
        event->ignore();

}
//Global settings
void MBGuiApp::writeSettings()
{
    QSettings settings("NRL", "MergeBathy GUI");

    settings.beginGroup("MBGuiApp");
        settings.setValue("size", size());
        settings.setValue("pos", pos());
//        settings.setValue("GlobalDataPath", globalDataPath);
//        settings.setValue("GlobalMBPath", globalMBPath);
    settings.endGroup();
}
void MBGuiApp::readSettings()
{
    // Global Settings
//    QCoreApplication::setOrganizationName("NRL");
//    QCoreApplication::setApplicationName("MergeBathy GUI");

    //Global MBGuiApp Settings
    QSettings settings2("NRL", "MergeBathy GUI");
    settings2.beginGroup("MBGuiApp");
        resize(settings2.value("size", QSize(800, 800)).toSize());
        move(settings2.value("pos", QPoint(400, 400)).toPoint());
        globalDataPath = settings2.value("GlobalDataPath").toString();
        globalMBPath = settings2.value("GlobalMBPath").toString();
    settings2.endGroup();

    //Local Project Settings
    QgsProject * project = QgsProject::instance();
    bool isLocalDataPathSet = project->readBoolEntry("myproject", "isLocalDataPathSet");
    //on startup, an empty project will initialize the localDataPath to the globalDataPath
    localDataPath = globalDataPath;
    project->writeEntry("myproject", "LocalDataPath", globalDataPath);
    project->writeEntry("myproject", "isLocalDataPathSet", false);

    //Do the same for MB exe path
    localMBPath = globalMBPath;
    project->writeEntry("myproject", "LocalMBPath", globalMBPath);
    project->writeEntry("myproject", "isLocalMBPathSet", false);
//}
// restore any application settings stored in QSettings
//void MBGuiApp::readSettings2()
//{
  QSettings settings;
  QString themename = settings.value( "UI/UITheme", "default" ).toString();
  setTheme( themename );

  // Read legacy settings
  mRecentProjects.clear();

  // Migrate old recent projects if first time with new system
  settings.beginGroup( "/UI" );
      if ( !settings.childGroups().contains( "recentProjects" ) )
      {
        QStringList oldRecentProjects = settings.value( "/UI/recentProjectsList" ).toStringList();

        Q_FOREACH ( const QString& project, oldRecentProjects )
        {
          QgsWelcomePageItemsModel::RecentProjectData data;
          data.path = project;
          data.title = project;

          mRecentProjects.append( data );
        }
      }
  settings.endGroup();

  settings.beginGroup( "/UI/recentProjects" );
      QStringList projectKeysList = settings.childGroups();

      //convert list to int values to obtain proper order
      QList<int> projectKeys;
      Q_FOREACH ( const QString& key, projectKeysList )
      {
        projectKeys.append( key.toInt() );
      }
      qSort( projectKeys );

      Q_FOREACH ( int key, projectKeys )
      {
        QgsWelcomePageItemsModel::RecentProjectData data;
        settings.beginGroup( QString::number( key ) );
            data.title = settings.value( "title" ).toString();
            data.path = settings.value( "path" ).toString();
            data.previewImagePath = settings.value( "previewImage" ).toString();
            data.crs = settings.value( "crs" ).toString();
        settings.endGroup();
        mRecentProjects.append( data );
      }
  settings.endGroup();

//  // this is a new session! reset enable macros value to "ask"
//  // whether set to "just for this session"
//  if ( settings.value( "/qgis/enableMacros", 1 ).toInt() == 2 )
//  {
//    settings.setValue( "/qgis/enableMacros", 1 );
//  }
}
void MBGuiApp::readProjectSettings()
{
    QgsProject * project = QgsProject::instance();
    // Check project settings
    bool isLocalDataPathSet = project->readBoolEntry("myproject", "isLocalDataPathSet");
    if(isLocalDataPathSet)
        localDataPath = project->readEntry("myproject", "LocalDataPath");
//    else {
//        localDataPath = globalDataPath;
//        project->writeEntry("myproject", "LocalDataPath", localDataPath);
//        project->writeEntry("myproject", "isLocalDataPathSet", true);
//    }

    bool isLocalMBPathSet = project->readBoolEntry("myproject", "isLocalMBPathSet");
    if(isLocalMBPathSet)
        localMBPath = project->readEntry("myproject", "LocalMBPath");
//    else {
//        localMBPath = globalMBPath;
//        project->writeEntry("myproject", "LocalMBPath", localMBPath);
//        project->writeEntry("myproject", "isLocalMBPathSet", true);
//    }

    int sz = project->readNumEntry("myproject", "layerSuffixes_size");
    layerSuffixes = new QMap<QString,QString>();
    QString key, value;
    //Save layer suffixes so we can write inputFileLists when we run MB
    project->writeEntry("myproject", "layerSuffixes_size",layerSuffixes->size());
    for(int i = 0; i < sz; ++i)
    {
         key = project->readEntry("myproject", "layerSuffixes/key_"+ QString(i));
         value = project->readEntry("myproject", "layerSuffixes/value_"+ QString(i));
         layerSuffixes->insert(key,value);
    }

    bool isParamsSet = project->readBoolEntry("myproject", "isParamsSet");
    if(isParamsSet)
    {
        filePathManager->setValue(propertyRunParams_wrkdir, project->readEntry("myproject", "wrkdir"));
        stringManager->setValue(propertyRunParams_outfname, project->readEntry("myproject", "outputfilename"));
        stringManager->setValue(propertyRunParams_ifilelist, project->readEntry("myproject", "inputfilelist"));
        boolManager->setValue(propertyRunParams_inmeters, project->readBoolEntry("myproject", "inputinmeters"));
        enumManager->setValue(propertyRunParams_filter, project->readNumEntry("myproject", "smoothingfilter"));
        sizeFManager->setValue(propertyRunParams_sscales, QSizeF(project->readDoubleEntry("myproject", "smoothingscalex"), project->readDoubleEntry("myproject", "smoothingscaley")));
        sizeFManager->setValue(propertyRunParams_gspacing, QSizeF(project->readDoubleEntry("myproject", "gridspacingx"), project->readDoubleEntry("myproject", "gridspacingy")));
        pointFManager->setValue(propertyRunParams_refll, QPointF(project->readDoubleEntry("myproject", "reflonx"),project->readDoubleEntry("myproject", "reflaty")));
        doubleManager->setValue(propertyRunParams_rotangle, project->readDoubleEntry("myproject", "rotangle"));

        enumManager->setValue(propertyRunParams_prespliner, project->readNumEntry("myproject", "prespliner"));
        stringManager->setValue(propertyRunParams_presplineroutfname, project->readEntry("myproject", "presplineoutputfilename"));
        boolManager->setValue(propertyRunParams_presplinerxyze, project->readBoolEntry("myproject", "presplinerxyze"));
        boolManager->setValue(propertyRunParams_presplineruseresults, project->readBoolEntry("myproject", "presplineruseresults"));
        sizeFManager->setValue(propertyRunParams_presplinerspacing, QSizeF(project->readDoubleEntry("myproject", "presplinerspacingx"), project->readDoubleEntry("myproject", "presplinerspacingy")));
        doubleManager->setValue(propertyRunParams_presplinertension, project->readDoubleEntry("myproject", "presplinertensionfactor"));
        doubleManager->setValue(propertyRunParams_presplinerscale, project->readDoubleEntry("myproject", "presplinerscalefactor"));
        doubleManager->setValue(propertyRunParams_presplineralpha, project->readDoubleEntry("myproject", "presplineralpha"));

        intManager->setValue(propertyRunParams_nthreads, project->readNumEntry("myproject", "nthreads"));
        boolManager->setValue(propertyRunParams_computeoffsets, project->readBoolEntry("myproject", "computeoffsets"));
        boolManager->setValue(propertyRunParams_modelflag, project->readBoolEntry("myproject", "modelflag"));
        boolManager->setValue(propertyRunParams_nonegdepths, project->readBoolEntry("myproject", "nonegdepths"));
        boolManager->setValue(propertyRunParams_kriging, project->readBoolEntry("myproject", "kriging"));
        boolManager->setValue(propertyRunParams_smoothinmeters, project->readBoolEntry("myproject", "smoothinmeters"));
        boolManager->setValue(propertyRunParams_gridinmeters, project->readBoolEntry("myproject", "gridinmeters"));
        boolManager->setValue(propertyRunParams_useunscaled, project->readBoolEntry("myproject", "useunscaled"));
        boolManager->setValue(propertyRunParams_preinterplocs, project->readBoolEntry("myproject", "preinterplocs"));
        filePathManager->setValue(propertyRunParams_preinterplocsfname, project->readEntry("myproject", "preinterpfilename"));
        boolManager->setValue(propertyRunParams_preinterplocsusage, project->readBoolEntry("myproject", "preinterplocsusage"));
    }
}
bool MBGuiApp::userReallyWantsToQuit()
{
    if (QgsProject::instance()->isDirty()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                     tr("The project has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
        {
            cleanUp();
            return save();
        }
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}
void MBGuiApp::cleanUp()
{
    resetMBParams();

    //reset layerSuffixes - contains file suffixes of loaded layers
    delete layerSuffixes;
}
void MBGuiApp::resetMBParams()
{
    //Reset input boxes
    delete params;
    params = new QtMBParams();
    // Get the project instance
    QgsProject * project = QgsProject::instance();

    filePathManager->setValue(propertyRunParams_wrkdir, params->getWrkdir());
    stringManager->setValue(propertyRunParams_outfname, params->getOutputFilename());
    stringManager->setValue(propertyRunParams_ifilelist, params->getInputFilelist());
    boolManager->setValue(propertyRunParams_inmeters, params->getInputInMeters());
    enumManager->setValue(propertyRunParams_filter, params->getSmoothingFilter());
    sizeFManager->setValue(propertyRunParams_sscales, params->getSmoothingScales());
    sizeFManager->setValue(propertyRunParams_gspacing, params->getGridSpacing());
    pointFManager->setValue(propertyRunParams_refll, params->getRefLL());
    doubleManager->setValue(propertyRunParams_rotangle, params->getRotAngle());

    enumManager->setValue(propertyRunParams_prespliner, params->getPreSpliner());
    stringManager->setValue(propertyRunParams_presplineroutfname, params->getPreSplinerOutputFilename());
    boolManager->setValue(propertyRunParams_presplinerxyze, params->getPreSplinerXYZE());
    boolManager->setValue(propertyRunParams_presplineruseresults, params->getPreSplinerUseResults());
    sizeFManager->setValue(propertyRunParams_presplinerspacing, params->getPreSplinerSpacing());
    doubleManager->setValue(propertyRunParams_presplinertension, params->getPreSplinerTensionFactor());
    doubleManager->setValue(propertyRunParams_presplinerscale, params->getPreSplinerScaleFactor());
    doubleManager->setValue(propertyRunParams_presplineralpha, params->getPreSplinerAlpha());

    intManager->setValue(propertyRunParams_nthreads, params->getNThreads());
    boolManager->setValue(propertyRunParams_computeoffsets, params->getComputeOffsets());
    boolManager->setValue(propertyRunParams_modelflag, params->getModelFlag());
    boolManager->setValue(propertyRunParams_nonegdepths, params->getNoNegDepths());
    boolManager->setValue(propertyRunParams_kriging, params->getKriging());
    boolManager->setValue(propertyRunParams_smoothinmeters, params->getSmoothInMeters());
    boolManager->setValue(propertyRunParams_gridinmeters, params->getGridInMeters());
    boolManager->setValue(propertyRunParams_useunscaled, params->getUseUnscaledAvgInputs());
    boolManager->setValue(propertyRunParams_preinterplocs, params->getUsePreInterpolatedLocs());
    filePathManager->setValue(propertyRunParams_preinterplocsfname, params->getPreinterpolatedLocsFile());
    boolManager->setValue(propertyRunParams_preinterplocsusage, params->getPreinterpLocsUsage());
}
//Clean temp directory slot
void MBGuiApp::cleanTempDir()
    {
    //Clean up temp directory
    QDir dir(QDir::temp().path().append("/MBGuiApp"));
    foreach(QString f, dir.entryList())
    {
        QFile file(f);
        file.remove();
    }
}






//////////////////////////////////////////////////////////////////////
//            Set Up the gui toolbars, menus, statusbar etc
//////////////////////////////////////////////////////////////////////
void MBGuiApp::initLayerTreeView()
{
    //sam moved this to initial function
//    mLayerTreeView->setWhatsThis( tr( "Map legend that displays all the layers currently on the map canvas. Click on the check box to turn a layer on or off. Double click on a layer in the legend to customize its appearance and set other properties." ) );

//    mLayerTreeDock = new QDockWidget( tr( "Layers Panel" ), this );
//    mLayerTreeDock->setObjectName( "Layers" );
//    mLayerTreeDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );

//    QgsLayerTreeModel* model = new QgsLayerTreeModel( QgsProject::instance()->layerTreeRoot(), this );
//    QgsLayerTreeModel* model_yz = new QgsLayerTreeModel( root_yz, this );
//  //  model_yz->setFlag( QgsLayerTreeModel::ShowLegend, FALSE);
//  //  model_yz->setFlag( QgsLayerTreeModel::ShowLegendAsTree, false );

//  //#ifdef ENABLE_MODELTEST
//  //  new ModelTest( model, this );
//  //#endif
//    model->setFlag( QgsLayerTreeModel::AllowNodeReorder );
//    model->setFlag( QgsLayerTreeModel::AllowNodeRename );
//    model->setFlag( QgsLayerTreeModel::AllowNodeChangeVisibility );
//    model->setFlag( QgsLayerTreeModel::ShowLegendAsTree );
//    model->setAutoCollapseLegendNodes( 10 );

//    mLayerTreeView->setModel( model );
//    mLayerTreeView->setMenuProvider( new QgsAppLayerTreeViewMenuProvider( mLayerTreeView, canvas_xy ) );


//  setupLayerTreeViewFromSettings();

    //double click canvas to change raster properties.  To use the same properties for everything, use 1 canvasDoubleClicked and set them the same.
    connect( canvas_depths, SIGNAL( mouseDoubleClickEvent(QMouseEvent *) ), this, SLOT( canvasDoubleClicked_depths( QMouseEvent * ) ) );
    connect( canvas_error, SIGNAL( mouseDoubleClickEvent(QMouseEvent *) ), this, SLOT( canvasDoubleClicked_error( QMouseEvent * ) ) );
    connect( canvas_nei, SIGNAL( mouseDoubleClickEvent(QMouseEvent *) ), this, SLOT( canvasDoubleClicked_nei( QMouseEvent * ) ) );
    connect( canvas_rei, SIGNAL( mouseDoubleClickEvent(QMouseEvent *) ), this, SLOT( canvasDoubleClicked_rei( QMouseEvent * ) ) );
    connect( canvas_mean, SIGNAL( mouseDoubleClickEvent(QMouseEvent *) ), this, SLOT( canvasDoubleClicked_mean( QMouseEvent * ) ) );


  connect( mLayerTreeView, SIGNAL( doubleClicked( QModelIndex ) ), this, SLOT( layerTreeViewDoubleClicked( QModelIndex ) ) );
  connect( mLayerTreeView, SIGNAL( currentLayerChanged( QgsMapLayer* ) ), this, SLOT( activeLayerChanged( QgsMapLayer* ) ) );
  connect( mLayerTreeView->selectionModel(), SIGNAL( currentChanged( QModelIndex, QModelIndex ) ), this, SLOT( updateNewLayerInsertionPoint() ) );
//  connect( QgsProject::instance()->layerTreeRegistryBridge(), SIGNAL( addedLayersToLayerTree( QList<QgsMapLayer*> ) ), this, SLOT( autoSelectAddedLayer( QList<QgsMapLayer*> ) ) );
  connect( layerTreeRegistryBridge, SIGNAL( addedLayersToLayerTree( QList<QgsMapLayer*> ) ), this, SLOT( autoSelectAddedLayer( QList<QgsMapLayer*> ) ) );

  //sams for canvas_yz
//  connect( mLayerTreeView, SIGNAL( currentLayerChanged( QgsMapLayer* ) ), mLayerTreeView_yz, SLOT( activeLayerChanged( QgsMapLayer* ) ) );

  connect( mLayerTreeView_yz, SIGNAL( currentLayerChanged( QgsMapLayer* ) ), this, SLOT( activeLayerChanged_yz( QgsMapLayer* ) ) );
  connect( mLayerTreeView_yz->selectionModel(), SIGNAL( currentChanged( QModelIndex, QModelIndex ) ), this, SLOT( updateNewLayerInsertionPoint_yz() ) );
  connect( layerTreeRegistryBridge_yz, SIGNAL( addedLayersToLayerTree( QList<QgsMapLayer*> ) ), this, SLOT( autoSelectAddedLayer_yz( QList<QgsMapLayer*> ) ) );


  // add group action
  QAction* actionAddGroup = new QAction( tr( "Add Group" ), this );
  actionAddGroup->setIcon( QgsApplication::getThemeIcon( "/mActionAddGroup.svg" ) );
  actionAddGroup->setToolTip( tr( "Add Group" ) );
  connect( actionAddGroup, SIGNAL( triggered( bool ) ), mLayerTreeView->defaultActions(), SLOT( addGroup() ) );

  // visibility groups tool button
  QToolButton* btnVisibilityPresets = new QToolButton;
  btnVisibilityPresets->setAutoRaise( true );
  btnVisibilityPresets->setToolTip( tr( "Manage Layer Visibility" ) );
  btnVisibilityPresets->setIcon( QgsApplication::getThemeIcon( "/mActionShowAllLayers.svg" ) );
  btnVisibilityPresets->setPopupMode( QToolButton::InstantPopup );
  btnVisibilityPresets->setMenu( QgsVisibilityPresets::instance()->menu() );

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
  toolbar->addAction( mActionRemoveLayer );

  QVBoxLayout* vboxLayout = new QVBoxLayout;
  vboxLayout->setMargin( 0 );
  vboxLayout->addWidget( toolbar );
  vboxLayout->addWidget( mLayerTreeView );

  QWidget* w = new QWidget;
  w->setLayout( vboxLayout );
  mLayerTreeDock->setWidget( w );
  addDockWidget( Qt::LeftDockWidgetArea, mLayerTreeDock );

//  mLayerTreeCanvasBridge = new QgsLayerTreeMapCanvasBridge( QgsProject::instance()->layerTreeRoot(), canvas_xy, this );
  connect( QgsProject::instance(), SIGNAL( writeProject( QDomDocument& ) ), mLayerTreeCanvasBridge, SLOT( writeProject( QDomDocument& ) ) );
  connect( QgsProject::instance(), SIGNAL( readProject( QDomDocument ) ), mLayerTreeCanvasBridge, SLOT( readProject( QDomDocument ) ) );

//  mLayerTreeCanvasBridge_yz = new QgsLayerTreeMapCanvasBridge( root_yz, canvas_yz, this );
//  connect( QgsProject::instance(), SIGNAL( writeProject( QDomDocument& ) ), mLayerTreeCanvasBridge_yz, SLOT( writeProject( QDomDocument& ) ) );
//  connect( QgsProject::instance(), SIGNAL( readProject( QDomDocument ) ), mLayerTreeCanvasBridge_yz, SLOT( readProject( QDomDocument ) ) );


//  bool otfTransformAutoEnable = QSettings().value( "/Projections/otfTransformAutoEnable", true ).toBool();
//  mLayerTreeCanvasBridge->setAutoEnableCrsTransform( otfTransformAutoEnable );

  mMapLayerOrder = new QgsCustomLayerOrderWidget( mLayerTreeCanvasBridge, this );
  mMapLayerOrder->setObjectName( "theMapLayerOrder" );

  mMapLayerOrder->setWhatsThis( tr( "Map layer list that displays all layers in drawing order." ) );
  mLayerOrderDock = new QDockWidget( tr( "Layer Order Panel" ), this );
  mLayerOrderDock->setObjectName( "LayerOrder" );
  mLayerOrderDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );

  mLayerOrderDock->setWidget( mMapLayerOrder );
  addDockWidget( Qt::LeftDockWidgetArea, mLayerOrderDock );
  mLayerOrderDock->hide();

//  connect( canvas_xy, SIGNAL( mapCanvasRefreshed() ), this, SLOT( updateFilterLegend() ) );
}

void MBGuiApp::setupConnections()
{
//    QgsLayerTreeGroup * root = QgsProject::instance()->layerTreeRoot();

    // Connect to signals emitted from Layer Tree Sams
    connect(root, SIGNAL(willAddChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onWillAddChildren(QgsLayerTreeNode*,int,int)));
    connect(root, SIGNAL(addedChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onWillAddChildren(QgsLayerTreeNode*,int,int)));
    connect(root, SIGNAL(willRemoveChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onWillRemoveChildren(QgsLayerTreeNode*,int,int)));
    connect(root, SIGNAL(removedChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onRemovedChildren(QgsLayerTreeNode*,int,int)));
    connect(root, SIGNAL(visibilityChanged(QgsLayerTreeNode*,Qt::CheckState)), this, SLOT(onVisibilityChanged(QgsLayerTreeNode*,Qt::CheckState)));
    connect(root, SIGNAL(customPropertyChanged(QgsLayerTreeNode*,QString)), this, SLOT(onCustomPropertyChanged(QgsLayerTreeNode*,QString)));
    connect(root, SIGNAL(expandedChanged(QgsLayerTreeNode*,bool)), this, SLOT(onExpandedChanged(QgsLayerTreeNode*,bool)));
    connect(mLayerTreeView, SIGNAL(currentLayerChanged(QgsMapLayer*)), this, SLOT(onChange(QgsMapLayer*)));
    connect(mLayerTreeView, SIGNAL(activated(QModelIndex)), this, SLOT(onChange(QgsMapLayer*)));
//    connect(mLayerTreeView, SIGNAL( ), this, SLOT(onLayerSavedAs(QgsMapLayer*,QString)));

    // Connect to signals emitted from Layer Tree Sams Trying to get canvas_yz to work
    connect(root_yz, SIGNAL(willAddChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onWillAddChildren(QgsLayerTreeNode*,int,int)));
    connect(root_yz, SIGNAL(addedChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onWillAddChildren(QgsLayerTreeNode*,int,int)));
    connect(root_yz, SIGNAL(willRemoveChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onWillRemoveChildren(QgsLayerTreeNode*,int,int)));
    connect(root_yz, SIGNAL(removedChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onRemovedChildren(QgsLayerTreeNode*,int,int)));
    connect(root_yz, SIGNAL(visibilityChanged(QgsLayerTreeNode*,Qt::CheckState)), this, SLOT(onVisibilityChanged(QgsLayerTreeNode*,Qt::CheckState)));
    connect(root_yz, SIGNAL(customPropertyChanged(QgsLayerTreeNode*,QString)), this, SLOT(onCustomPropertyChanged(QgsLayerTreeNode*,QString)));
    connect(root_yz, SIGNAL(expandedChanged(QgsLayerTreeNode*,bool)), this, SLOT(onExpandedChanged(QgsLayerTreeNode*,bool)));
    connect(mLayerTreeView_yz, SIGNAL(currentLayerChanged(QgsMapLayer*)), this, SLOT(onChange(QgsMapLayer*)));
    connect(mLayerTreeView_yz, SIGNAL(activated(QModelIndex)), this, SLOT(onChange(QgsMapLayer*)));

  // connect the "cleanup" slot
  connect( qApp, SIGNAL( aboutToQuit() ), this, SLOT( saveWindowState() ) );

  // signal when mouse moved over window (coords display in status bar)
//    connect( canvas_xy, SIGNAL( xyCoordinates( const QgsPoint & ) ), this, SLOT( saveLastMousePosition( const QgsPoint & ) ) );
    connect( canvas_xy, SIGNAL( extentsChanged() ), this, SLOT( extentChanged() ) );
    connect( canvas_xy, SIGNAL( scaleChanged( double ) ), this, SLOT( showScale( double ) ) );
    connect( canvas_xy, SIGNAL( rotationChanged( double ) ), this, SLOT( showRotation() ) );
    connect( canvas_xy, SIGNAL( scaleChanged( double ) ), this, SLOT( updateMouseCoordinatePrecision() ) );
    connect( canvas_xy, SIGNAL( mapToolSet( QgsMapTool *, QgsMapTool * ) ), this, SLOT( mapToolChanged( QgsMapTool *, QgsMapTool * ) ) );
//    connect( canvas_xy, SIGNAL( selectionChanged( QgsMapLayer * ) ), this, SLOT( selectionChanged( QgsMapLayer * ) ) );
    connect( canvas_xy, SIGNAL( selectionChanged( QgsMapLayer * ) ), this, SLOT( selectionChanged_xy( QgsMapLayer * ) ) );

    connect( canvas_xy, SIGNAL( extentsChanged() ), this, SLOT( markDirty() ) );
    connect( canvas_xy, SIGNAL( layersChanged() ), this, SLOT( markDirty() ) );

    connect( canvas_xy, SIGNAL( zoomLastStatusChanged( bool ) ), mActionZoomLast, SLOT( setEnabled( bool ) ) );
    connect( canvas_xy, SIGNAL( zoomNextStatusChanged( bool ) ), mActionZoomNext, SLOT( setEnabled( bool ) ) );
    connect( mRenderSuppressionCBox, SIGNAL( toggled( bool ) ), canvas_xy, SLOT( setRenderFlag( bool ) ) );

    //sams connections for canvas_yz
//    connect( canvas_yz, SIGNAL( xyCoordinates( const QgsPoint & ) ), this, SLOT( saveLastMousePosition( const QgsPoint & ) ) );
    connect( canvas_yz, SIGNAL( extentsChanged() ), this, SLOT( extentChanged() ) );
    connect( canvas_yz, SIGNAL( scaleChanged( double ) ), this, SLOT( showScale( double ) ) );
    connect( canvas_yz, SIGNAL( rotationChanged( double ) ), this, SLOT( showRotation() ) );
    connect( canvas_yz, SIGNAL( scaleChanged( double ) ), this, SLOT( updateMouseCoordinatePrecision() ) );
    connect( canvas_yz, SIGNAL( mapToolSet( QgsMapTool *, QgsMapTool * ) ), this, SLOT( mapToolChanged( QgsMapTool *, QgsMapTool * ) ) );
    connect( canvas_yz, SIGNAL( selectionChanged( QgsMapLayer * ) ), this, SLOT( selectionChanged_yz( QgsMapLayer * ) ) );
    connect( canvas_yz, SIGNAL( extentsChanged() ), this, SLOT( markDirty() ) );
    connect( canvas_yz, SIGNAL( layersChanged() ), this, SLOT( markDirty() ) );
    connect( canvas_yz, SIGNAL( zoomLastStatusChanged( bool ) ), mActionZoomLast, SLOT( setEnabled( bool ) ) );
    connect( canvas_yz, SIGNAL( zoomNextStatusChanged( bool ) ), mActionZoomNext, SLOT( setEnabled( bool ) ) );
    connect( mRenderSuppressionCBox, SIGNAL( toggled( bool ) ), canvas_yz, SLOT( setRenderFlag( bool ) ) );

    connect( canvas_xy, SIGNAL( extentsChanged() ), this, SLOT( extentChanged_xy() ) );
    connect( canvas_yz, SIGNAL( extentsChanged() ), this, SLOT( extentChanged_yz() ) );

    //  connect( canvas_xy, SIGNAL( destinationCrsChanged() ), this, SLOT( reprojectAnnotations() ) );

    // connect MapCanvas keyPress event so we can check if selected feature collection must be deleted
    connect( canvas_xy, SIGNAL( keyPressed( QKeyEvent * ) ), this, SLOT( mapCanvas_keyPressed( QKeyEvent * ) ) );
    connect( canvas_yz, SIGNAL( keyPressed( QKeyEvent * ) ), this, SLOT( mapCanvas_keyPressed( QKeyEvent * ) ) );
//    connect( canvas_xy, SIGNAL( keyPressed( QKeyEvent * ) ), canvas_yz, SIGNAL( keyPressed( QKeyEvent * ) ));

    // connect renderer
    //  connect( canvas_xy, SIGNAL( hasCrsTransformEnabledChanged( bool ) ), this, SLOT( hasCrsTransformEnabled( bool ) ) );
    //  connect( canvas_xy, SIGNAL( destinationCrsChanged() ), this, SLOT( destinationCrsChanged() ) );

    // connect legend signals
    connect( mLayerTreeView, SIGNAL( currentLayerChanged( QgsMapLayer * ) ), this, SLOT( activateDeactivateLayerRelatedActions( QgsMapLayer * ) ) );
    connect( mLayerTreeView->selectionModel(), SIGNAL( selectionChanged( QItemSelection, QItemSelection ) ), this, SLOT( legendLayerSelectionChanged() ) );
    connect( mLayerTreeView->layerTreeModel()->rootGroup(), SIGNAL( addedChildren( QgsLayerTreeNode*, int, int ) ), this, SLOT( markDirty() ) );
    connect( mLayerTreeView->layerTreeModel()->rootGroup(), SIGNAL( addedChildren( QgsLayerTreeNode*, int, int ) ), this, SLOT( updateNewLayerInsertionPoint() ) );
    connect( mLayerTreeView->layerTreeModel()->rootGroup(), SIGNAL( removedChildren( QgsLayerTreeNode*, int, int ) ), this, SLOT( markDirty() ) );
    connect( mLayerTreeView->layerTreeModel()->rootGroup(), SIGNAL( removedChildren( QgsLayerTreeNode*, int, int ) ), this, SLOT( updateNewLayerInsertionPoint() ) );
    connect( mLayerTreeView->layerTreeModel()->rootGroup(), SIGNAL( visibilityChanged( QgsLayerTreeNode*, Qt::CheckState ) ), this, SLOT( markDirty() ) );
    connect( mLayerTreeView->layerTreeModel()->rootGroup(), SIGNAL( customPropertyChanged( QgsLayerTreeNode*, QString ) ), this, SLOT( markDirty() ) );

    //sams connections for canvas_yz
    connect( mLayerTreeView_yz, SIGNAL( currentLayerChanged( QgsMapLayer * ) ), this, SLOT( activateDeactivateLayerRelatedActions( QgsMapLayer * ) ) );
    connect( mLayerTreeView_yz->selectionModel(), SIGNAL( selectionChanged( QItemSelection, QItemSelection ) ), this, SLOT( legendLayerSelectionChanged() ) );
    connect( mLayerTreeView_yz->layerTreeModel()->rootGroup(), SIGNAL( addedChildren( QgsLayerTreeNode*, int, int ) ), this, SLOT( markDirty() ) );
    connect( mLayerTreeView_yz->layerTreeModel()->rootGroup(), SIGNAL( addedChildren( QgsLayerTreeNode*, int, int ) ), this, SLOT( updateNewLayerInsertionPoint_yz() ) );
    connect( mLayerTreeView_yz->layerTreeModel()->rootGroup(), SIGNAL( removedChildren( QgsLayerTreeNode*, int, int ) ), this, SLOT( markDirty() ) );
    connect( mLayerTreeView_yz->layerTreeModel()->rootGroup(), SIGNAL( removedChildren( QgsLayerTreeNode*, int, int ) ), this, SLOT( updateNewLayerInsertionPoint_yz() ) );
    connect( mLayerTreeView_yz->layerTreeModel()->rootGroup(), SIGNAL( visibilityChanged( QgsLayerTreeNode*, Qt::CheckState ) ), this, SLOT( markDirty() ) );
    connect( mLayerTreeView_yz->layerTreeModel()->rootGroup(), SIGNAL( customPropertyChanged( QgsLayerTreeNode*, QString ) ), this, SLOT( markDirty() ) );

    // connect map layer registry
    connect( QgsMapLayerRegistry::instance(), SIGNAL( layersAdded( QList<QgsMapLayer *> ) ), this, SLOT( layersWereAdded( QList<QgsMapLayer *> ) ) );
    connect( QgsMapLayerRegistry::instance(), SIGNAL( layersWillBeRemoved( QStringList ) ), this, SLOT( removingLayers( QStringList ) ) );




    // connect initialization signal
  connect( this, SIGNAL( initializationCompleted() ), this, SLOT( fileOpenAfterLaunch() ) );

  // Connect warning dialog from project reading
//  connect( QgsProject::instance(), SIGNAL( oldProjectVersionWarning( QString ) ), this, SLOT( oldProjectVersionWarning( QString ) ) );
  connect( QgsProject::instance(), SIGNAL( layerLoaded( int, int ) ), this, SLOT( showProgress( int, int ) ) );
  connect( QgsProject::instance(), SIGNAL( loadingLayer( QString ) ), this, SLOT( showStatusMessage( QString ) ) );
  connect( QgsProject::instance(), SIGNAL( readProject( const QDomDocument & ) ), this, SLOT( readProject( const QDomDocument & ) ) );
  connect( QgsProject::instance(), SIGNAL( writeProject( QDomDocument & ) ), this, SLOT( writeProject( QDomDocument & ) ) );
//  connect( QgsProject::instance(), SIGNAL( writeProject( QDomDocument& ) ), this, SLOT( writeAnnotationItemsToProject( QDomDocument& ) ) );

//  connect( QgsProject::instance(), SIGNAL( readProject( const QDomDocument & ) ), this, SLOT( loadComposersFromProject( const QDomDocument& ) ) );
//  connect( QgsProject::instance(), SIGNAL( readProject( const QDomDocument & ) ), this, SLOT( loadAnnotationItemsFromProject( const QDomDocument& ) ) );

  connect( this, SIGNAL( projectRead() ), this, SLOT( fileOpenedOKAfterLaunch() ) );

//  // connect preview modes actions
//  connect( mActionPreviewModeOff, SIGNAL( triggered() ), this, SLOT( disablePreviewMode() ) );
//  connect( mActionPreviewModeGrayscale, SIGNAL( triggered() ), this, SLOT( activateGrayscalePreview() ) );
//  connect( mActionPreviewModeMono, SIGNAL( triggered() ), this, SLOT( activateMonoPreview() ) );
//  connect( mActionPreviewProtanope, SIGNAL( triggered() ), this, SLOT( activateProtanopePreview() ) );
//  connect( mActionPreviewDeuteranope, SIGNAL( triggered() ), this, SLOT( activateDeuteranopePreview() ) );

//  // handle deprecated labels in project for QGIS 2.0
//  connect( this, SIGNAL( newProject() ), this, SLOT( checkForDeprecatedLabelsInProject() ) );
//  connect( this, SIGNAL( projectRead() ), this, SLOT( checkForDeprecatedLabelsInProject() ) );

//  // setup undo/redo actions
  connect( mUndoWidget, SIGNAL( undoStackChanged() ), this, SLOT( updateUndoActions() ) );
}

void MBGuiApp::createCanvasTools()
{
    //use mine instead although they are exactly the same so pick whichever form you like
    // create tools
//    mMapTools.mZoomIn = new QgsMapToolZoom( canvas_xy, false /* zoomIn */ );
//    mMapTools.mZoomIn->setAction( mActionZoomIn );
//    mMapTools.mZoomOut = new QgsMapToolZoom( canvas_xy, true /* zoomOut */ );
//    mMapTools.mZoomOut->setAction( mActionZoomOut );
//    mMapTools.mPan = new QgsMapToolPan( canvas_xy );
//    mMapTools.mPan->setAction( mActionPan );
//    mMapTools.mIdentify = new QgsMapToolIdentifyAction( canvas_xy );
//    mMapTools.mIdentify->setAction( mActionIdentify );
//    mMapTools.mSelectFeatures = new QgsMapToolSelectFeatures( canvas_xy );
//    mMapTools.mSelectFeatures->setAction( mActionSelectFeatures );
//    mMapTools.mSelectPolygon = new QgsMapToolSelectPolygon( canvas_xy );
//    mMapTools.mSelectPolygon->setAction( mActionSelectPolygon );
//    mMapTools.mSelectFreehand = new QgsMapToolSelectFreehand( canvas_xy );
//    mMapTools.mSelectFreehand->setAction( mActionSelectFreehand );
//    mMapTools.mSelectRadius = new QgsMapToolSelectRadius( canvas_xy );
//    mMapTools.mSelectRadius->setAction( mActionSelectRadius );

    mMapTools.mZoomIn_xy = new QgsMapToolZoom( canvas_xy, false /* zoomIn */ );
    mMapTools.mZoomIn_xy->setAction( mActionZoomIn );
    mMapTools.mZoomOut_xy = new QgsMapToolZoom( canvas_xy, true /* zoomOut */ );
    mMapTools.mZoomOut_xy->setAction( mActionZoomOut );
    mMapTools.mPan_xy = new QgsMapToolPan( canvas_xy );
    mMapTools.mPan_xy->setAction( mActionPan );
    mMapTools.mIdentify_xy = new QgsMapToolIdentifyAction( canvas_xy );
    mMapTools.mIdentify_xy->setAction( mActionIdentify );
    mMapTools.mSelectFeatures_xy = new QgsMapToolSelectFeatures( canvas_xy );
    mMapTools.mSelectFeatures_xy->setAction( mActionSelectFeatures );
    mMapTools.mSelectPolygon_xy = new QgsMapToolSelectPolygon( canvas_xy );
    mMapTools.mSelectPolygon_xy->setAction( mActionSelectPolygon );
    mMapTools.mSelectFreehand_xy = new QgsMapToolSelectFreehand( canvas_xy );
    mMapTools.mSelectFreehand_xy->setAction( mActionSelectFreehand );
    mMapTools.mSelectRadius_xy = new QgsMapToolSelectRadius( canvas_xy );
    mMapTools.mSelectRadius_xy->setAction( mActionSelectRadius );

    mMapTools.mZoomIn_yz = new QgsMapToolZoom( canvas_yz, false /* zoomIn */ );
    mMapTools.mZoomIn_yz->setAction( mActionZoomIn );
    mMapTools.mZoomOut_yz = new QgsMapToolZoom( canvas_yz, true /* zoomOut */ );
    mMapTools.mZoomOut_yz->setAction( mActionZoomOut );
    mMapTools.mPan_yz = new QgsMapToolPan( canvas_yz );
    mMapTools.mPan_yz->setAction( mActionPan );
    mMapTools.mIdentify_yz = new QgsMapToolIdentifyAction( canvas_yz );
    mMapTools.mIdentify_yz->setAction( mActionIdentify );
    mMapTools.mSelectFeatures_yz = new QgsMapToolSelectFeatures( canvas_yz );
    mMapTools.mSelectFeatures_yz->setAction( mActionSelectFeatures );
    mMapTools.mSelectPolygon_yz = new QgsMapToolSelectPolygon( canvas_yz );
    mMapTools.mSelectPolygon_yz->setAction( mActionSelectPolygon );
    mMapTools.mSelectFreehand_yz = new QgsMapToolSelectFreehand( canvas_yz );
    mMapTools.mSelectFreehand_yz->setAction( mActionSelectFreehand );
    mMapTools.mSelectRadius_yz = new QgsMapToolSelectRadius( canvas_yz );
    mMapTools.mSelectRadius_yz->setAction( mActionSelectRadius );

//    mMapTools.mRotateFeature = new QgsMapToolRotateFeature( canvas_xy );
//    mMapTools.mRotateFeature->setAction( mActionRotateFeature );

   /* connect( mMapTools.mIdentify, SIGNAL( copyToClipboard( QgsFeatureStore & ) ), this, SLOT( copyFeatures( QgsFeatureStore & ) ) );
    mMapTools.mFeatureAction = new QgsMapToolFeatureAction( canvas_xy );
    mMapTools.mFeatureAction->setAction( mActionFeatureAction );*/
    //mMapTools.mMeasureDist = new QgsMeasureTool( canvas_xy, false /* area */ );
    //mMapTools.mMeasureDist->setAction( mActionMeasure );
    //mMapTools.mMeasureArea = new QgsMeasureTool( canvas_xy, true /* area */ );
    /*mMapTools.mMeasureArea->setAction( mActionMeasureArea );
    mMapTools.mMeasureAngle = new QgsMapToolMeasureAngle( canvas_xy );
    mMapTools.mMeasureAngle->setAction( mActionMeasureAngle );
    mMapTools.mTextAnnotation = new QgsMapToolTextAnnotation( canvas_xy );
    mMapTools.mTextAnnotation->setAction( mActionTextAnnotation );
    mMapTools.mFormAnnotation = new QgsMapToolFormAnnotation( canvas_xy );
    mMapTools.mFormAnnotation->setAction( mActionFormAnnotation );
    mMapTools.mHtmlAnnotation = new QgsMapToolHtmlAnnotation( canvas_xy );
    mMapTools.mHtmlAnnotation->setAction( mActionHtmlAnnotation );
    mMapTools.mSvgAnnotation = new QgsMapToolSvgAnnotation( canvas_xy );
    mMapTools.mSvgAnnotation->setAction( mActionSvgAnnotation );
    mMapTools.mAnnotation = new QgsMapToolAnnotation( canvas_xy );
    mMapTools.mAnnotation->setAction( mActionAnnotation );
    mMapTools.mAddFeature = new QgsMapToolAddFeature( canvas_xy );
    mMapTools.mAddFeature->setAction( mActionAddFeature );
    mMapTools.mCircularStringCurvePoint = new QgsMapToolCircularStringCurvePoint( dynamic_cast<QgsMapToolAddFeature*>( mMapTools.mAddFeature ), canvas_xy );
    mMapTools.mCircularStringCurvePoint->setAction( mActionCircularStringCurvePoint );
    mMapTools.mCircularStringRadius = new QgsMapToolCircularStringRadius( dynamic_cast<QgsMapToolAddFeature*>( mMapTools.mAddFeature ), canvas_xy );
    mMapTools.mCircularStringRadius->setAction( mActionCircularStringRadius );
    mMapTools.mMoveFeature = new QgsMapToolMoveFeature( canvas_xy );
    mMapTools.mMoveFeature->setAction( mActionMoveFeature );

    //need at least geos 3.3 for OffsetCurve tool
    #if defined(GEOS_VERSION_MAJOR) && defined(GEOS_VERSION_MINOR) && \
    ((GEOS_VERSION_MAJOR>3) || ((GEOS_VERSION_MAJOR==3) && (GEOS_VERSION_MINOR>=3)))
    mMapTools.mOffsetCurve = new QgsMapToolOffsetCurve( canvas_xy );
    mMapTools.mOffsetCurve->setAction( mActionOffsetCurve );
    #else
    mAdvancedDigitizeToolBar->removeAction( mActionOffsetCurve );
    mEditMenu->removeAction( mActionOffsetCurve );
    mMapTools.mOffsetCurve = 0;
    #endif //GEOS_VERSION
    mMapTools.mReshapeFeatures = new QgsMapToolReshape( canvas_xy );
    mMapTools.mReshapeFeatures->setAction( mActionReshapeFeatures );
    mMapTools.mSplitFeatures = new QgsMapToolSplitFeatures( canvas_xy );
    mMapTools.mSplitFeatures->setAction( mActionSplitFeatures );
    mMapTools.mSplitParts = new QgsMapToolSplitParts( canvas_xy );
    mMapTools.mSplitParts->setAction( mActionSplitParts );
    mMapTools.mAddRing = new QgsMapToolAddRing( canvas_xy );
    mMapTools.mAddRing->setAction( mActionAddRing );
    mMapTools.mFillRing = new QgsMapToolFillRing( canvas_xy );
    mMapTools.mFillRing->setAction( mActionFillRing );
    mMapTools.mAddPart = new QgsMapToolAddPart( canvas_xy );
    mMapTools.mAddPart->setAction( mActionAddPart );
    mMapTools.mSimplifyFeature = new QgsMapToolSimplify( canvas_xy );
    mMapTools.mSimplifyFeature->setAction( mActionSimplifyFeature );
    mMapTools.mDeleteRing = new QgsMapToolDeleteRing( canvas_xy );
    mMapTools.mDeleteRing->setAction( mActionDeleteRing );
    mMapTools.mDeletePart = new QgsMapToolDeletePart( canvas_xy );
    mMapTools.mDeletePart->setAction( mActionDeletePart );
    mMapTools.mNodeTool = new QgsMapToolNodeTool( canvas_xy );
    mMapTools.mNodeTool->setAction( mActionNodeTool );
    mMapTools.mRotatePointSymbolsTool = new QgsMapToolRotatePointSymbols( canvas_xy );
    mMapTools.mRotatePointSymbolsTool->setAction( mActionRotatePointSymbols );
    mMapTools.mPinLabels = new QgsMapToolPinLabels( canvas_xy );
    mMapTools.mPinLabels->setAction( mActionPinLabels );
    mMapTools.mShowHideLabels = new QgsMapToolShowHideLabels( canvas_xy );
    mMapTools.mShowHideLabels->setAction( mActionShowHideLabels );
    mMapTools.mMoveLabel = new QgsMapToolMoveLabel( canvas_xy );
    mMapTools.mMoveLabel->setAction( mActionMoveLabel );

    mMapTools.mRotateLabel = new QgsMapToolRotateLabel( canvas_xy );
    mMapTools.mRotateLabel->setAction( mActionRotateLabel );
    mMapTools.mChangeLabelProperties = new QgsMapToolChangeLabelProperties( canvas_xy );
    mMapTools.mChangeLabelProperties->setAction( mActionChangeLabelProperties );*/

    //this doesn't affect us since we are always editing (we are not using qgis' toggle "editing" to edit framework
    //ensure that non edit tool is initialized or we will get crashes in some situations
//    mNonEditMapTool = mMapTools.mPan_xy;
}

void MBGuiApp::createOverview()
{
  // overview canvas
  mOverviewCanvas = new QgsMapOverviewCanvas( nullptr, canvas_xy );

  //set canvas color to default
  QSettings settings;
  int red = settings.value( "/qgis/default_canvas_color_red", 255 ).toInt();
  int green = settings.value( "/qgis/default_canvas_color_green", 255 ).toInt();
  int blue = settings.value( "/qgis/default_canvas_color_blue", 255 ).toInt();
  mOverviewCanvas->setBackgroundColor( QColor( red, green, blue ) );

  mOverviewCanvas->setWhatsThis( tr( "Map overview canvas. This canvas can be used to display a locator map that shows the current extent of the map canvas. The current extent is shown as a red rectangle. Any layer on the map can be added to the overview canvas." ) );

  mOverviewMapCursor = new QCursor( Qt::OpenHandCursor );
  mOverviewCanvas->setCursor( *mOverviewMapCursor );
  //commented out in qgis
//  QVBoxLayout *myOverviewLayout = new QVBoxLayout;
//  myOverviewLayout->addWidget(overviewCanvas);
//  overviewFrame->setLayout(myOverviewLayout);
  mOverviewDock = new QDockWidget( tr( "Overview Panel" ), this );
  mOverviewDock->setObjectName( "Overview" );
  mOverviewDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
  mOverviewDock->setWidget( mOverviewCanvas );
  addDockWidget( Qt::LeftDockWidgetArea, mOverviewDock );
  // add to the Panel submenu
  mPanelMenu->addAction( mOverviewDock->toggleViewAction() );

  canvas_xy->enableOverviewMode( mOverviewCanvas );

  // moved here to set anti aliasing to both map canvas and overview
  QSettings mySettings;
  // Anti Aliasing enabled by default as of QGIS 1.7
  canvas_xy->enableAntiAliasing( mySettings.value( "/qgis/enable_anti_aliasing", true ).toBool() );

  int action = mySettings.value( "/qgis/wheel_action", 2 ).toInt();
  double zoomFactor = mySettings.value( "/qgis/zoom_factor", 2 ).toDouble();
  canvas_xy->setWheelAction( static_cast< QgsMapCanvas::WheelAction >( action ), zoomFactor );

  canvas_xy->setCachingEnabled( mySettings.value( "/qgis/enable_render_caching", true ).toBool() );

  canvas_xy->setParallelRenderingEnabled( mySettings.value( "/qgis/parallel_rendering", false ).toBool() );

  canvas_xy->setMapUpdateInterval( mySettings.value( "/qgis/map_update_interval", 250 ).toInt() );
}

void MBGuiApp::addDockWidget( Qt::DockWidgetArea theArea, QDockWidget * thepDockWidget )
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

void MBGuiApp::removeDockWidget( QDockWidget * thepDockWidget )
{
  QMainWindow::removeDockWidget( thepDockWidget );
  mPanelMenu->removeAction( thepDockWidget->toggleViewAction() );
}

QToolBar *MBGuiApp::addToolBar( const QString& name )
{
  QToolBar *toolBar = QMainWindow::addToolBar( name );
  // add to the Toolbar submenu
  mToolbarMenu->addAction( toolBar->toggleViewAction() );
  return toolBar;
}

void MBGuiApp::addToolBar( QToolBar* toolBar, Qt::ToolBarArea area )
{
  QMainWindow::addToolBar( area, toolBar );
  // add to the Toolbar submenu
  mToolbarMenu->addAction( toolBar->toggleViewAction() );
}

QgsLayerTreeView* MBGuiApp::layerTreeView()
{
  Q_ASSERT( mLayerTreeView );
  return mLayerTreeView;
}

QgsMapCanvas *MBGuiApp::mapCanvas()
{
  Q_ASSERT( canvas_xy );
  return canvas_xy;
}
QgsMapCanvas *MBGuiApp::mapCanvas_xy()
{
  Q_ASSERT( canvas_xy );
  return canvas_xy;
}

QgsMessageBar* MBGuiApp::messageBar()
{
  Q_ASSERT( mInfoBar );
  return mInfoBar;
}



void MBGuiApp::updateNewLayerInsertionPoint()
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
void MBGuiApp::updateNewLayerInsertionPoint_yz()
{
  // defaults
  QgsLayerTreeGroup* parentGroup = mLayerTreeView_yz->layerTreeModel()->rootGroup();
  int index = 0;
  QModelIndex current = mLayerTreeView_yz->currentIndex();

  if ( current.isValid() )
  {
    if ( QgsLayerTreeNode* currentNode = mLayerTreeView_yz->currentNode() )
    {
      // if the insertion point is actually a group, insert new layers into the group
      if ( QgsLayerTree::isGroup( currentNode ) )
      {
        layerTreeRegistryBridge_yz->setLayerInsertionPoint( QgsLayerTree::toGroup( currentNode ), 0 );
        return;
      }

      // otherwise just set the insertion point in front of the current node
      QgsLayerTreeNode* parentNode = currentNode->parent();
      if ( QgsLayerTree::isGroup( parentNode ) )
        parentGroup = QgsLayerTree::toGroup( parentNode );
    }

    index = current.row();
  }

  layerTreeRegistryBridge_yz->setLayerInsertionPoint( parentGroup, index );
}
void MBGuiApp::autoSelectAddedLayer( QList<QgsMapLayer*> layers )
{
  if ( !layers.isEmpty() )
  {
    QgsLayerTreeLayer* nodeLayer = root->findLayer( layers[0]->id() );

    if ( !nodeLayer )
      return;

    QModelIndex index = mLayerTreeView->layerTreeModel()->node2index( nodeLayer );
    mLayerTreeView->setCurrentIndex( index );
  }
}
void MBGuiApp::autoSelectAddedLayer_yz( QList<QgsMapLayer*> layers )
{
  if ( !layers.isEmpty() )
  {
    QgsLayerTreeLayer* nodeLayer_yz = root_yz->findLayer( layers[0]->id() );

    if ( !nodeLayer_yz )
      return;

    QModelIndex index_yz = mLayerTreeView_yz->layerTreeModel()->node2index( nodeLayer_yz );
    mLayerTreeView_yz->setCurrentIndex( index_yz );
  }
}
void MBGuiApp::createMapTips()
{
  // Set up the timer for maptips. The timer is reset everytime the mouse is moved
  mpMapTipsTimer = new QTimer( canvas_xy );
  // connect the timer to the maptips slot
  connect( mpMapTipsTimer, SIGNAL( timeout() ), this, SLOT( showMapTip() ) );
  // set the interval to 0.850 seconds - timer will be started next time the mouse moves
  mpMapTipsTimer->setInterval( 850 );
  // Create the maptips object
  mpMaptip = new QgsMapTip();
}

// Update project menu with the current list of recently accessed projects
void MBGuiApp::updateRecentProjectPaths()
{
  mRecentProjectsMenu->clear();

  Q_FOREACH ( const QgsWelcomePageItemsModel::RecentProjectData& recentProject, mRecentProjects )
  {
    QAction* action = mRecentProjectsMenu->addAction( QString( "%1 (%2)" ).arg( recentProject.title != recentProject.path ? recentProject.title : QFileInfo( recentProject.path ).baseName(), recentProject.path ) );
    action->setEnabled( QFile::exists(( recentProject.path ) ) );
    action->setData( recentProject.path );
  }

  if ( mWelcomePage )
    mWelcomePage->setRecentProjects( mRecentProjects );
}

// add this file to the recently opened/saved projects list
void MBGuiApp::saveRecentProjectPath( const QString& projectPath, bool savePreviewImage )
{
    QSettings settings;

    // Get canonical absolute path
    QFileInfo myFileInfo( projectPath );
    QgsWelcomePageItemsModel::RecentProjectData projectData;
    projectData.path = myFileInfo.absoluteFilePath();
    projectData.title = QgsProject::instance()->title();
    if ( projectData.title.isEmpty() )
        projectData.title = projectData.path;

    projectData.crs = canvas_xy->mapSettings().destinationCrs().authid();

    if ( savePreviewImage )
    {
        // Generate a unique file name
        QString fileName( QCryptographicHash::hash(( projectData.path.toUtf8() ), QCryptographicHash::Md5 ).toHex() );
        QString previewDir = QString( "%1/previewImages" ).arg( QgsApplication::qgisSettingsDirPath() );
        projectData.previewImagePath = QString( "%1/%2.png" ).arg( previewDir, fileName );
        QDir().mkdir( previewDir );

        // Render the map canvas
        QSize previewSize( 250, 177 ); // h = w / sqrt(2)
        QRect previewRect( QPoint(( canvas_xy->width() - previewSize.width() ) / 2
                                  , ( canvas_xy->height() - previewSize.height() ) / 2 )
                           , previewSize );

        QPixmap previewImage( previewSize );
        QPainter previewPainter( &previewImage );
        canvas_xy->render( &previewPainter, QRect( QPoint(), previewSize ), previewRect );

        // Save
        previewImage.save( projectData.previewImagePath );
    }
    else
    {
        int idx = mRecentProjects.indexOf( projectData );
        if ( idx != -1 )
            projectData.previewImagePath = mRecentProjects.at( idx ).previewImagePath;
    }

    // If this file is already in the list, remove it
    mRecentProjects.removeAll( projectData );

    // Prepend this file to the list
    mRecentProjects.prepend( projectData );

    // Keep the list to 10 items by trimming excess off the bottom
    // And remove the associated image
    while ( mRecentProjects.count() > 10 )
    {
        QFile( mRecentProjects.takeLast().previewImagePath ).remove();
    }

    settings.remove( "/UI/recentProjects" );
    int idx = 0;

    // Persist the list
    Q_FOREACH ( const QgsWelcomePageItemsModel::RecentProjectData& recentProject, mRecentProjects )
    {
        ++idx;
        settings.beginGroup( QString( "/UI/recentProjects/%1" ).arg( idx ) );
        settings.setValue( "title", recentProject.title );
        settings.setValue( "path", recentProject.path );
        settings.setValue( "previewImage", recentProject.previewImagePath );
        settings.setValue( "crs", recentProject.crs );
        settings.endGroup();
    }

      // Update menu list of paths
      updateRecentProjectPaths();
}

void MBGuiApp::saveWindowState()
{
  // store window and toolbar positions
  QSettings settings;
  // store the toolbar/dock widget settings using Qt4 settings API
  settings.setValue( "/UI/state", saveState() );

  // store window geometry
  settings.setValue( "/UI/geometry", saveGeometry() );

//  QgsPluginRegistry::instance()->unloadAll();
}

#include "ui_defaults.h"

void MBGuiApp::restoreWindowState()
{
  // restore the toolbar and dock widgets positions using Qt4 settings API
  QSettings settings;

  if ( !restoreState( settings.value( "/UI/state", QByteArray::fromRawData( reinterpret_cast< const char * >( defaultUIstate ), sizeof defaultUIstate ) ).toByteArray() ) )
  {
    QgsDebugMsg( "restore of UI state failed" );
  }

  // restore window geometry
  if ( !restoreGeometry( settings.value( "/UI/geometry", QByteArray::fromRawData( reinterpret_cast< const char * >( defaultUIgeometry ), sizeof defaultUIgeometry ) ).toByteArray() ) )
  {
    QgsDebugMsg( "restore of UI geometry failed" );
  }

}
void MBGuiApp::activeLayerChanged( QgsMapLayer* layer )
{

  if ( canvas_xy )
    canvas_xy->setCurrentLayer( layer );

  mLayerTreeView_yz->setCurrentLayer(layer);

//  if ( canvas_yz )
//    canvas_yz->setCurrentLayer( layer );
}

void MBGuiApp::activeLayerChanged_yz( QgsMapLayer* layer )
{
  if ( canvas_yz )
    canvas_yz->setCurrentLayer( layer );

}
void MBGuiApp::setTheme( const QString& theThemeName )
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
  QgsDebugMsg("Setting theme to \n" + theThemeName);
  //sams
  newProjectAction->setIcon( QgsApplication::getThemeIcon( "/mActionFileNew.svg" ) );
//  closeProjectAction->setIcon( QgsApplication::getThemeIcon( "/mActionFileNew.svg" ) );
  openAction->setIcon( QgsApplication::getThemeIcon( "/mActionFileOpen.svg" ) );
  saveAction->setIcon( QgsApplication::getThemeIcon( "/mActionFileSave.svg" ) );
  saveAsAction->setIcon( QgsApplication::getThemeIcon( "/mActionFileSaveAs.svg" ) );
  exitAction->setIcon( QgsApplication::getThemeIcon( "/mActionFileExit.png" ) );
  undoAction->setIcon( QgsApplication::getThemeIcon( "/mActionUndo.png" ) );
  redoAction->setIcon( QgsApplication::getThemeIcon( "/mActionRedo.png" ) );
  rasterPropertiesAction->setIcon( QgsApplication::getThemeIcon( "/mActionProjectProperties.png" ) );

  //qgis
  mActionNewProject->setIcon( QgsApplication::getThemeIcon( "/mActionFileNew.svg" ) );
  mActionOpenProject->setIcon( QgsApplication::getThemeIcon( "/mActionFileOpen.svg" ) );
  mActionSaveProject->setIcon( QgsApplication::getThemeIcon( "/mActionFileSave.svg" ) );
  mActionSaveProjectAs->setIcon( QgsApplication::getThemeIcon( "/mActionFileSaveAs.svg" ) );
//  mActionNewPrintComposer->setIcon( QgsApplication::getThemeIcon( "/mActionNewComposer.svg" ) );
//  mActionShowComposerManager->setIcon( QgsApplication::getThemeIcon( "/mActionComposerManager.svg" ) );
  mActionSaveMapAsImage->setIcon( QgsApplication::getThemeIcon( "/mActionSaveMapAsImage.png" ) );
  mActionExit->setIcon( QgsApplication::getThemeIcon( "/mActionFileExit.png" ) );
  mActionAddOgrLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddOgrLayer.svg" ) );
  mActionAddRasterLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddRasterLayer.svg" ) );
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
  mActionRemoveLayer->setIcon( QgsApplication::getThemeIcon( "/mActionRemoveLayer.svg" ) );
//  mActionDuplicateLayer->setIcon( QgsApplication::getThemeIcon( "/mActionDuplicateLayer.svg" ) );
//  mActionSetLayerCRS->setIcon( QgsApplication::getThemeIcon( "/mActionSetLayerCRS.png" ) );
//  mActionSetProjectCRSFromLayer->setIcon( QgsApplication::getThemeIcon( "/mActionSetProjectCRSFromLayer.png" ) );
//  mActionNewVectorLayer->setIcon( QgsApplication::getThemeIcon( "/mActionNewVectorLayer.svg" ) );
//  mActionNewMemoryLayer->setIcon( QgsApplication::getThemeIcon( "/mActionCreateMemory.svg" ) );
  mActionAddAllToOverview->setIcon( QgsApplication::getThemeIcon( "/mActionAddAllToOverview.svg" ) );
  mActionHideAllLayers->setIcon( QgsApplication::getThemeIcon( "/mActionHideAllLayers.png" ) );
  mActionShowAllLayers->setIcon( QgsApplication::getThemeIcon( "/mActionShowAllLayers.png" ) );
  mActionHideSelectedLayers->setIcon( QgsApplication::getThemeIcon( "/mActionHideSelectedLayers.png" ) );
  mActionShowSelectedLayers->setIcon( QgsApplication::getThemeIcon( "/mActionShowSelectedLayers.png" ) );
  mActionRemoveAllFromOverview->setIcon( QgsApplication::getThemeIcon( "/mActionRemoveAllFromOverview.svg" ) );
  mActionToggleFullScreen->setIcon( QgsApplication::getThemeIcon( "/mActionToggleFullScreen.png" ) );
//  mActionProjectProperties->setIcon( QgsApplication::getThemeIcon( "/mActionProjectProperties.png" ) );
//  mActionManagePlugins->setIcon( QgsApplication::getThemeIcon( "/mActionShowPluginManager.svg" ) );
//  mActionShowPythonDialog->setIcon( QgsApplication::getThemeIcon( "console/iconRunConsole.png" ) );
//  mActionCheckQgisVersion->setIcon( QgsApplication::getThemeIcon( "/mActionCheckQgisVersion.png" ) );
//  mActionOptions->setIcon( QgsApplication::getThemeIcon( "/mActionOptions.svg" ) );
//  mActionConfigureShortcuts->setIcon( QgsApplication::getThemeIcon( "/mActionOptions.svg" ) );
//  mActionCustomization->setIcon( QgsApplication::getThemeIcon( "/mActionOptions.svg" ) );
//  mActionHelpContents->setIcon( QgsApplication::getThemeIcon( "/mActionHelpContents.svg" ) );
  mActionLocalHistogramStretch->setIcon( QgsApplication::getThemeIcon( "/mActionLocalHistogramStretch.png" ) );
  mActionFullHistogramStretch->setIcon( QgsApplication::getThemeIcon( "/mActionFullHistogramStretch.png" ) );
  mActionIncreaseBrightness->setIcon( QgsApplication::getThemeIcon( "/mActionIncreaseBrightness.svg" ) );
  mActionDecreaseBrightness->setIcon( QgsApplication::getThemeIcon( "/mActionDecreaseBrightness.svg" ) );
  mActionIncreaseContrast->setIcon( QgsApplication::getThemeIcon( "/mActionIncreaseContrast.svg" ) );
  mActionDecreaseContrast->setIcon( QgsApplication::getThemeIcon( "/mActionDecreaseContrast.svg" ) );
  mActionZoomActualSize->setIcon( QgsApplication::getThemeIcon( "/mActionZoomNative.png" ) );
//  mActionQgisHomePage->setIcon( QgsApplication::getThemeIcon( "/mActionQgisHomePage.png" ) );
  mActionAbout->setIcon( QgsApplication::getThemeIcon( "/mActionHelpAbout.png" ) );
//  mActionSponsors->setIcon( QgsApplication::getThemeIcon( "/mActionHelpSponsors.png" ) );
//  mActionDraw->setIcon( QgsApplication::getThemeIcon( "/mActionDraw.svg" ) );
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
//  mActionDeleteSelected->setIcon( QgsApplication::getThemeIcon( "/mActionDeleteSelected.svg" ) );
//  mActionNodeTool->setIcon( QgsApplication::getThemeIcon( "/mActionNodeTool.png" ) );
//  mActionSimplifyFeature->setIcon( QgsApplication::getThemeIcon( "/mActionSimplify.png" ) );
  mActionUndo->setIcon( QgsApplication::getThemeIcon( "/mActionUndo.png" ) );
  mActionRedo->setIcon( QgsApplication::getThemeIcon( "/mActionRedo.png" ) );
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
  mActionShowRasterCalculator->setIcon( QgsApplication::getThemeIcon( "/mActionShowRasterCalculator.png" ) );
  mActionPan->setIcon( QgsApplication::getThemeIcon( "/mActionPan.svg" ) );
  mActionPanToSelected->setIcon( QgsApplication::getThemeIcon( "/mActionPanToSelected.svg" ) );
  mActionZoomLast->setIcon( QgsApplication::getThemeIcon( "/mActionZoomLast.svg" ) );
  mActionZoomNext->setIcon( QgsApplication::getThemeIcon( "/mActionZoomNext.svg" ) );
  mActionZoomToLayer->setIcon( QgsApplication::getThemeIcon( "/mActionZoomToLayer.svg" ) );
  mActionZoomActualSize->setIcon( QgsApplication::getThemeIcon( "/mActionZoomActual.svg" ) );
  mActionIdentify->setIcon( QgsApplication::getThemeIcon( "/mActionIdentify.svg" ) );
//  mActionFeatureAction->setIcon( QgsApplication::getThemeIcon( "/mAction.svg" ) );
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
//  mActionShowBookmarks->setIcon( QgsApplication::getThemeIcon( "/mActionShowBookmarks.png" ) );
//  mActionNewBookmark->setIcon( QgsApplication::getThemeIcon( "/mActionNewBookmark.png" ) );
//  mActionCustomProjection->setIcon( QgsApplication::getThemeIcon( "/mActionCustomProjection.svg" ) );
//  mActionAddWmsLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddWmsLayer.svg" ) );
//  mActionAddWcsLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddWcsLayer.svg" ) );
//  mActionAddWfsLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddWfsLayer.svg" ) );
  mActionAddToOverview->setIcon( QgsApplication::getThemeIcon( "/mActionInOverview.svg" ) );
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

  emit currentThemeChanged( theThemeName );
}

///////////// END OF GUI SETUP ROUTINES ///////////////
/*void MBGuiApp::sponsors()
{
  QgsSponsors * sponsors = new QgsSponsors( this );
  sponsors->show();
  sponsors->raise();
  sponsors->activateWindow();
}
*/
/*void MBGuiApp::about()
{
  static QgsAbout *abt = nullptr;
  if ( !abt )
  {
    QApplication::setOverrideCursor( Qt::WaitCursor );
    abt = new QgsAbout( this );
    QString versionString = "<html><body><div align='center'><table width='100%'>";

    versionString += "<tr>";
    versionString += "<td>" + tr( "QGIS version" )       + "</td><td>" + QGis::QGIS_VERSION + "</td><td>";


    if ( QString( QGis::QGIS_DEV_VERSION ) == "exported" )
    {
      versionString += tr( "QGIS code branch" ) + QString( "</td><td><a href=\"https://github.com/qgis/QGIS/tree/release-%1_%2\">Release %1.%2</a></td>" )
                       .arg( QGis::QGIS_VERSION_INT / 10000 ).arg( QGis::QGIS_VERSION_INT / 100 % 100 );
    }
    else
    {
      versionString += tr( "QGIS code revision" ) + QString( "</td><td><a href=\"https://github.com/qgis/QGIS/commit/%1\">%1</a></td>" ).arg( QGis::QGIS_DEV_VERSION );
    }

    versionString += "</tr><tr>";

    versionString += "<td>" + tr( "Compiled against Qt" ) + "</td><td>" + QT_VERSION_STR + "</td>";
    versionString += "<td>" + tr( "Running against Qt" )  + "</td><td>" + qVersion() + "</td>";

    versionString += "</tr><tr>";

    versionString += "<td>" + tr( "Compiled against GDAL/OGR" ) + "</td><td>" + GDAL_RELEASE_NAME + "</td>";
    versionString += "<td>" + tr( "Running against GDAL/OGR" )  + "</td><td>" + GDALVersionInfo( "RELEASE_NAME" ) + "</td>";

    versionString += "</tr><tr>";

    versionString += "<td>" + tr( "Compiled against GEOS" ) + "</td><td>" + GEOS_CAPI_VERSION + "</td>";
    versionString += "<td>" + tr( "Running against GEOS" ) + "</td><td>" + GEOSversion() + "</td>";

    versionString += "</tr><tr>";

    versionString += "<td>" + tr( "PostgreSQL Client Version" ) + "</td><td>";
//    #ifdef HAVE_POSTGRESQL
//        versionString += PG_VERSION;
//    #else
//        versionString += tr( "No support." );
//    #endif
    versionString += "</td>";

//    versionString += "<td>" +  tr( "SpatiaLite Version" ) + "</td><td>";
//    versionString += spatialite_version();
//    versionString += "</td>";

    versionString += "</tr><tr>";

//    versionString += "<td>" + tr( "QWT Version" ) + "</td><td>" + QWT_VERSION_STR + "</td>";
//    versionString += "<td>" + tr( "PROJ.4 Version" ) + "</td><td>" + QString::number( PJ_VERSION ) + "</td>";

    versionString += "</tr><tr>";

    versionString += "<td>" + tr( "QScintilla2 Version" ) + "</td><td>" + QSCINTILLA_VERSION_STR + "</td>";

    #ifdef QGISDEBUG
        versionString += "<td colspan=2>" + tr( "This copy of QGIS writes debugging output." ) + "</td>";
    #endif

    versionString += "</tr></table></div></body></html>";

    abt->setVersion( versionString );

    QApplication::restoreOverrideCursor();
  }
  abt->show();
  abt->raise();
  abt->activateWindow();
}*/

QString MBGuiApp::crsAndFormatAdjustedLayerUri( const QString &uri, const QStringList &supportedCrs, const QStringList &supportedFormats ) const
{
  QString newuri = uri;

  // Adjust layer CRS to project CRS
  QgsCoordinateReferenceSystem testCrs;
  Q_FOREACH ( const QString& c, supportedCrs )
  {
    testCrs.createFromOgcWmsCrs( c );
    if ( testCrs == canvas_xy->mapSettings().destinationCrs() )
    {
      newuri.replace( QRegExp( "crs=[^&]+" ), "crs=" + c );
      QgsDebugMsg( QString( "Changing layer crs to %1, new uri: %2" ).arg( c, uri ) );
      break;
    }
  }

  // Use the last used image format
  QString lastImageEncoding = QSettings().value( "/qgis/lastWmsImageEncoding", "image/png" ).toString();
  Q_FOREACH ( const QString& fmt, supportedFormats )
  {
    if ( fmt == lastImageEncoding )
    {
      newuri.replace( QRegExp( "format=[^&]+" ), "format=" + fmt );
      QgsDebugMsg( QString( "Changing layer format to %1, new uri: %2" ).arg( fmt, uri ) );
      break;
    }
  }
  return newuri;
}

/* This method prompts the user for a list of vector file names  with a dialog. */
/*void MBGuiApp::addVectorLayer()
{
  canvas_xy->freeze();
  QgsOpenVectorLayerDialog *ovl = new QgsOpenVectorLayerDialog( this );

  if ( ovl->exec() )
  {
    QStringList selectedSources = ovl->dataSources();
    QString enc = ovl->encoding();
    if ( !selectedSources.isEmpty() )
    {
      addVectorLayers( selectedSources, enc, ovl->dataSourceType() );
    }
  }

  canvas_xy->freeze( false );
  canvas_xy->refresh();

  delete ovl;
}*/

bool MBGuiApp::addVectorLayers( const QStringList &theLayerQStringList, const QString &enc, const QString &dataSourceType )
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

      // if needed prompt for zipitem layers
      QString vsiPrefix = QgsZipItem::vsiPrefix( src );
      if ( ! src.startsWith( "/vsi", Qt::CaseInsensitive ) &&
           ( vsiPrefix == "/vsizip/" || vsiPrefix == "/vsitar/" ) )
      {
        if ( askUserForZipItemLayers( src ) )
          continue;
      }
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
        askUserForOGRSublayers( layer );

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
  activateDeactivateLayerRelatedActions( activeLayer_xy() );

  // Only update the map if we frozen in this method
  // Let the caller do it otherwise
  if ( !wasfrozen )
  {
    canvas_xy->freeze( false );
    canvas_xy->refresh();
  }
// Let render() do its own cursor management
//  QApplication::restoreOverrideCursor();

   statusBar()->showMessage( canvas_xy->extent().toString( 2 ) );

  return true;
}

// present a dialog to choose zipitem layers
bool MBGuiApp::askUserForZipItemLayers( QString path )
{
  bool ok = false;
  QVector<QgsDataItem*> childItems;
  QgsZipItem *zipItem = nullptr;
  QSettings settings;
  int promptLayers = settings.value( "/qgis/promptForRasterSublayers", 1 ).toInt();

  QgsDebugMsg( "askUserForZipItemLayers( " + path + ')' );

  // if scanZipBrowser == no: skip to the next file
  if ( settings.value( "/qgis/scanZipInBrowser2", "basic" ).toString() == "no" )
  {
    return false;
  }

  zipItem = new QgsZipItem( nullptr, path, path );
  if ( ! zipItem )
    return false;

  zipItem->populate();
  QgsDebugMsg( QString( "Path= %1 got zipitem with %2 children" ).arg( path ).arg( zipItem->rowCount() ) );

  // if 1 or 0 child found, exit so a normal item is created by gdal or ogr provider
  if ( zipItem->rowCount() <= 1 )
  {
    delete zipItem;
    return false;
  }

  // if promptLayers=Load all, load all layers without prompting
  if ( promptLayers == 3 )
  {
    childItems = zipItem->children();
  }
  // exit if promptLayers=Never
  else if ( promptLayers == 2 )
  {
    delete zipItem;
    return false;
  }
  else
  {
    // We initialize a selection dialog and display it.
    QgsSublayersDialog chooseSublayersDialog( QgsSublayersDialog::Vsifile, "vsi", this );

    QStringList layers;
    for ( int i = 0; i < zipItem->children().size(); i++ )
    {
      QgsDataItem *item = zipItem->children().at( i );
      QgsLayerItem *layerItem = dynamic_cast<QgsLayerItem *>( item );
      if ( layerItem )
      {
        QgsDebugMsgLevel( QString( "item path=%1 provider=%2" ).arg( item->path(), layerItem->providerKey() ), 2 );
      }
      if ( layerItem && layerItem->providerKey() == "gdal" )
      {
        layers << QString( "%1|%2|%3" ).arg( i ).arg( item->name(), "Raster" );
      }
      else if ( layerItem && layerItem->providerKey() == "ogr" )
      {
        layers << QString( "%1|%2|%3" ).arg( i ).arg( item->name(), tr( "Vector" ) );
      }
    }

    chooseSublayersDialog.populateLayerTable( layers, "|" );

    if ( chooseSublayersDialog.exec() )
    {
      Q_FOREACH ( int i, chooseSublayersDialog.selectionIndexes() )
      {
        childItems << zipItem->children().at( i );
      }
    }
  }

  if ( childItems.isEmpty() )
  {
    // return true so dialog doesn't popup again (#6225) - hopefully this doesn't create other trouble
    ok = true;
  }

  // add childItems
  Q_FOREACH ( QgsDataItem* item, childItems )
  {
    QgsLayerItem *layerItem = dynamic_cast<QgsLayerItem *>( item );
    if ( !layerItem )
      continue;

    QgsDebugMsg( QString( "item path=%1 provider=%2" ).arg( item->path(), layerItem->providerKey() ) );
    if ( layerItem->providerKey() == "gdal" )
    {
      if ( addRasterLayer( item->path(), QFileInfo( item->name() ).completeBaseName() ) )
        ok = true;
    }
    else if ( layerItem->providerKey() == "ogr" )
    {
      if ( addVectorLayers( QStringList( item->path() ), "System", "file" ) )
        ok = true;
    }
  }

  delete zipItem;
  return ok;
}

// present a dialog to choose GDAL raster sublayers
void MBGuiApp::askUserForGDALSublayers( QgsRasterLayer *layer )
{
  if ( !layer )
    return;

  QStringList sublayers = layer->subLayers();
  QgsDebugMsg( QString( "raster has %1 sublayers" ).arg( layer->subLayers().size() ) );

  if ( sublayers.size() < 1 )
    return;

  // if promptLayers=Load all, load all sublayers without prompting
  QSettings settings;
  if ( settings.value( "/qgis/promptForRasterSublayers", 1 ).toInt() == 3 )
  {
    loadGDALSublayers( layer->source(), sublayers );
    return;
  }

  // We initialize a selection dialog and display it.
  QgsSublayersDialog chooseSublayersDialog( QgsSublayersDialog::Gdal, "gdal", this );

  QStringList layers;
  QStringList names;
  for ( int i = 0; i < sublayers.size(); i++ )
  {
    // simplify raster sublayer name - should add a function in gdal provider for this?
    // code is copied from QgsGdalLayerItem::createChildren
    QString name = sublayers[i];
    QString path = layer->source();
    // if netcdf/hdf use all text after filename
    // for hdf4 it would be best to get description, because the subdataset_index is not very practical
    if ( name.startsWith( "netcdf", Qt::CaseInsensitive ) ||
         name.startsWith( "hdf", Qt::CaseInsensitive ) )
      name = name.mid( name.indexOf( path ) + path.length() + 1 );
    else
    {
      // remove driver name and file name
      name.remove( name.split( ':' )[0] );
      name.remove( path );
    }
    // remove any : or " left over
    if ( name.startsWith( ':' ) )
      name.remove( 0, 1 );

    if ( name.startsWith( '\"' ) )
      name.remove( 0, 1 );

    if ( name.endsWith( ':' ) )
      name.chop( 1 );

    if ( name.endsWith( '\"' ) )
      name.chop( 1 );

    names << name;
    layers << QString( "%1|%2" ).arg( i ).arg( name );
  }

  chooseSublayersDialog.populateLayerTable( layers, "|" );

  if ( chooseSublayersDialog.exec() )
  {
    // create more informative layer names, containing filename as well as sublayer name
    QRegExp rx( "\"(.*)\"" );
    QString uri, name;

    Q_FOREACH ( int i, chooseSublayersDialog.selectionIndexes() )
    {
      if ( rx.indexIn( sublayers[i] ) != -1 )
      {
        uri = rx.cap( 1 );
        name = sublayers[i];
        name.replace( uri, QFileInfo( uri ).completeBaseName() );
      }
      else
      {
        name = names[i];
      }

      QgsRasterLayer *rlayer = new QgsRasterLayer( sublayers[i], name );
      if ( rlayer && rlayer->isValid() )
      {
        addRasterLayer( rlayer );
      }
    }
  }
}

// should the GDAL sublayers dialog should be presented to the user?
bool MBGuiApp::shouldAskUserForGDALSublayers( QgsRasterLayer *layer )
{
  // return false if layer is empty or raster has no sublayers
  if ( !layer || layer->providerType() != "gdal" || layer->subLayers().size() < 1 )
    return false;

  QSettings settings;
  int promptLayers = settings.value( "/qgis/promptForRasterSublayers", 1 ).toInt();
  // 0 = Always -> always ask (if there are existing sublayers)
  // 1 = If needed -> ask if layer has no bands, but has sublayers
  // 2 = Never -> never prompt, will not load anything
  // 3 = Load all -> never prompt, but load all sublayers

  return promptLayers == 0 || promptLayers == 3 || ( promptLayers == 1 && layer->bandCount() == 0 );
}

// This method will load with GDAL the layers in parameter.
// It is normally triggered by the sublayer selection dialog.
void MBGuiApp::loadGDALSublayers( const QString& uri, const QStringList& list )
{
  QString path, name;
  QgsRasterLayer *subLayer = nullptr;

  //add layers in reverse order so they appear in the right order in the layer dock
  for ( int i = list.size() - 1; i >= 0 ; i-- )
  {
    path = list[i];
    // shorten name by replacing complete path with filename
    name = path;
    name.replace( uri, QFileInfo( uri ).completeBaseName() );
    subLayer = new QgsRasterLayer( path, name );
    if ( subLayer )
    {
      if ( subLayer->isValid() )
        addRasterLayer( subLayer );
      else
        delete subLayer;
    }

  }
}

// This method is the method that does the real job. If the layer given in
// parameter is nullptr, then the method tries to act on the activeLayer.
void MBGuiApp::askUserForOGRSublayers( QgsVectorLayer *layer )
{
  if ( !layer )
  {
    layer = qobject_cast<QgsVectorLayer *>( activeLayer_xy() );
    if ( !layer || layer->dataProvider()->name() != "ogr" )
      return;
  }

  QStringList sublayers = layer->dataProvider()->subLayers();
  QString layertype = layer->dataProvider()->storageType();

  // We initialize a selection dialog and display it.
  QgsSublayersDialog chooseSublayersDialog( QgsSublayersDialog::Ogr, "ogr", this );
  chooseSublayersDialog.populateLayerTable( sublayers );

  if ( chooseSublayersDialog.exec() )
  {
    QString uri = layer->source();
    //the separator char & was changed to | to be compatible
    //with url for protocol drivers
    if ( uri.contains( '|', Qt::CaseSensitive ) )
    {
      // If we get here, there are some options added to the filename.
      // A valid uri is of the form: filename&option1=value1&option2=value2,...
      // We want only the filename here, so we get the first part of the split.
      QStringList theURIParts = uri.split( '|' );
      uri = theURIParts.at( 0 );
    }
    QgsDebugMsg( "Layer type " + layertype );
    // the user has done his choice
    loadOGRSublayers( layertype, uri, chooseSublayersDialog.selectionNames() );
  }
}

// This method will load with OGR the layers  in parameter.
// This method has been conceived to use the new URI
// format of the ogrprovider so as to give precisions about which
// sublayer to load into QGIS. It is normally triggered by the
// sublayer selection dialog.
void MBGuiApp::loadOGRSublayers( const QString& layertype, const QString& uri, const QStringList& list )
{
  // The uri must contain the actual uri of the vectorLayer from which we are
  // going to load the sublayers.
  QString fileName = QFileInfo( uri ).baseName();
  QList<QgsMapLayer *> myList;
  for ( int i = 0; i < list.size(); i++ )
  {
    QString composedURI;
    QStringList elements = list.at( i ).split( ':' );
    while ( elements.size() > 2 )
    {
      elements[0] += ':' + elements[1];
      elements.removeAt( 1 );
    }

    QString layerName = elements.value( 0 );
    QString layerType = elements.value( 1 );
    if ( layerType == "any" )
    {
      layerType = "";
      elements.removeAt( 1 );
    }

    if ( layertype != "GRASS" )
    {
      composedURI = uri + "|layername=" + layerName;
    }
    else
    {
      composedURI = uri + "|layerindex=" + layerName;
    }

    if ( !layerType.isEmpty() )
    {
      composedURI += "|geometrytype=" + layerType;
    }

    // addVectorLayer( composedURI, list.at( i ), "ogr" );

    QgsDebugMsg( "Creating new vector layer using " + composedURI );
    QString name = list.at( i );
    name.replace( ':', ' ' );
    QgsVectorLayer *layer = new QgsVectorLayer( composedURI, fileName + " " + name, "ogr", false );
    if ( layer && layer->isValid() )
    {
      myList << layer;
    }
    else
    {
      QString msg = tr( "%1 is not a valid or recognized data source" ).arg( composedURI );
      messageBar()->pushMessage( tr( "Invalid Data Source" ), msg, QgsMessageBar::CRITICAL, messageTimeout() );
      if ( layer )
        delete layer;
    }
  }

  if ( ! myList.isEmpty() )
  {
    // Register layer(s) with the layers registry
    QgsMapLayerRegistry::instance()->addMapLayers( myList );
    Q_FOREACH ( QgsMapLayer *l, myList )
    {
      bool ok;
      l->loadDefaultStyle( ok );
    }
  }
}
/*void MBGuiApp::addDatabaseLayer()
{
#ifdef HAVE_POSTGRESQL
  // Fudge for now
  QgsDebugMsg( "about to addRasterLayer" );

  // TODO: QDialog for now, switch to QWidget in future
  QDialog *dbs = dynamic_cast<QDialog*>( QgsProviderRegistry::instance()->selectWidget( "postgres", this ) );
  if ( !dbs )
  {
    QMessageBox::warning( this, tr( "PostgreSQL" ), tr( "Cannot get PostgreSQL select dialog from provider." ) );
    return;
  }
  connect( dbs, SIGNAL( addDatabaseLayers( QStringList const &, QString const & ) ),
           this, SLOT( addDatabaseLayers( QStringList const &, QString const & ) ) );
  connect( dbs, SIGNAL( progress( int, int ) ),
           this, SLOT( showProgress( int, int ) ) );
  connect( dbs, SIGNAL( progressMessage( QString ) ),
           this, SLOT( showStatusMessage( QString ) ) );
  dbs->exec();
  delete dbs;
#endif
} // MBGuiApp::addDatabaseLayer()

void MBGuiApp::addDatabaseLayers( QStringList const & layerPathList, QString const & providerKey )
{
  QList<QgsMapLayer *> myList;

  if ( layerPathList.empty() )
  {
    // no layers to add so bail out, but
    // allow canvas_xy to handle events
    // first
    canvas_xy->freeze( false );
    return;
  }

  canvas_xy->freeze( true );

  QApplication::setOverrideCursor( Qt::WaitCursor );

  Q_FOREACH ( const QString& layerPath, layerPathList )
  {
    // create the layer
    QgsDataSourceURI uri( layerPath );

    QgsVectorLayer *layer = new QgsVectorLayer( uri.uri( false ), uri.table(), providerKey, false );
    Q_CHECK_PTR( layer );

    if ( ! layer )
    {
      canvas_xy->freeze( false );
      QApplication::restoreOverrideCursor();

      // XXX insert meaningful whine to the user here
      return;
    }

    if ( layer->isValid() )
    {
      // add to list of layers to register
      //with the central layers registry
      myList << layer;
    }
    else
    {
      QgsMessageLog::logMessage( tr( "%1 is an invalid layer - not loaded" ).arg( layerPath ) );
      QLabel *msgLabel = new QLabel( tr( "%1 is an invalid layer and cannot be loaded. Please check the <a href=\"#messageLog\">message log</a> for further info." ).arg( layerPath ), messageBar() );
      msgLabel->setWordWrap( true );
      connect( msgLabel, SIGNAL( linkActivated( QString ) ), mLogDock, SLOT( show() ) );
      QgsMessageBarItem *item = new QgsMessageBarItem( msgLabel, QgsMessageBar::WARNING );
      messageBar()->pushItem( item );
      delete layer;
    }
    //qWarning("incrementing iterator");
  }

  QgsMapLayerRegistry::instance()->addMapLayers( myList );

  // load default style after adding to process readCustomSymbology signals
  Q_FOREACH ( QgsMapLayer *l, myList )
  {
    bool ok;
    l->loadDefaultStyle( ok );
  }

  // draw the map
  canvas_xy->freeze( false );
  canvas_xy->refresh();

  QApplication::restoreOverrideCursor();
}

void MBGuiApp::addSpatiaLiteLayer()
{
  // show the SpatiaLite dialog
  QDialog *dbs = dynamic_cast<QDialog*>( QgsProviderRegistry::instance()->selectWidget( "spatialite", this ) );
  if ( !dbs )
  {
    QMessageBox::warning( this, tr( "SpatiaLite" ), tr( "Cannot get SpatiaLite select dialog from provider." ) );
    return;
  }
  connect( dbs, SIGNAL( addDatabaseLayers( QStringList const &, QString const & ) ),
           this, SLOT( addDatabaseLayers( QStringList const &, QString const & ) ) );
  dbs->exec();
  delete dbs;
} // MBGuiApp::addSpatiaLiteLayer()
*/
void MBGuiApp::addDelimitedTextLayer()
{
  // show the Delimited text dialog
  QDialog *dts = dynamic_cast<QDialog*>( QgsProviderRegistry::instance()->selectWidget( "delimitedtext", this ) );
  if ( !dts )
  {
    QMessageBox::warning( this, tr( "Delimited Text" ), tr( "Cannot get Delimited Text select dialog from provider." ) );
    return;
  }
  connect( dts, SIGNAL( addVectorLayer( QString, QString, QString ) ),
           this, SLOT( addSelectedVectorLayer( QString, QString, QString ) ) );
  dts->exec();
  delete dts;
}

/*void MBGuiApp::addVirtualLayer()
{
  // show the Delimited text dialog
  QDialog *dts = dynamic_cast<QDialog*>( QgsProviderRegistry::instance()->selectWidget( "virtual", this ) );
  if ( !dts )
  {
    QMessageBox::warning( this, tr( "Virtual layer" ), tr( "Cannot get virtual layer select dialog from provider." ) );
    return;
  }
  connect( dts, SIGNAL( addVectorLayer( QString, QString, QString ) ),
           this, SLOT( addSelectedVectorLayer( QString, QString, QString ) ) );
  connect( dts, SIGNAL( replaceVectorLayer( QString, QString, QString, QString ) ),
           this, SLOT( replaceSelectedVectorLayer( QString, QString, QString, QString ) ) );
  dts->exec();
  delete dts;
} */
/*void MBGuiApp::addSelectedVectorLayer( const QString& uri, const QString& layerName, const QString& provider )
{
  addVectorLayer( uri, layerName, provider );
} */
/*void MBGuiApp::replaceSelectedVectorLayer( const QString& oldId, const QString& uri, const QString& layerName, const QString& provider )
{
  QgsMapLayer* old = QgsMapLayerRegistry::instance()->mapLayer( oldId );
  if ( !old )
    return;
  QgsVectorLayer* oldLayer = static_cast<QgsVectorLayer*>( old );
  QgsVectorLayer* newLayer = new QgsVectorLayer( uri, layerName, provider );
  if ( !newLayer || !newLayer->isValid() )
    return;

  QgsMapLayerRegistry::instance()->addMapLayer( newLayer, /*addToLegend*/
//false, /*takeOwnership*/
//true );
/*  duplicateVectorStyle( oldLayer, newLayer );

  // insert the new layer just below the old one
  QgsLayerTreeUtils::insertLayerBelow( QgsProject::instance()->layerTreeRoot(), oldLayer, newLayer );
  // and remove the old layer
  QgsMapLayerRegistry::instance()->removeMapLayer( oldLayer );
} // MBGuiApp:replaceSelectedVectorLayer

void MBGuiApp::addMssqlLayer()
{
#ifdef HAVE_MSSQL
  // show the MSSQL dialog
  QDialog *dbs = dynamic_cast<QDialog*>( QgsProviderRegistry::instance()->selectWidget( "mssql", this ) );
  if ( !dbs )
  {
    QMessageBox::warning( this, tr( "MSSQL" ), tr( "Cannot get MSSQL select dialog from provider." ) );
    return;
  }
  connect( dbs, SIGNAL( addDatabaseLayers( QStringList const &, QString const & ) ),
           this, SLOT( addDatabaseLayers( QStringList const &, QString const & ) ) );
  dbs->exec();
  delete dbs;
#endif
} // MBGuiApp::addMssqlLayer()

void MBGuiApp::addOracleLayer()
{
#ifdef HAVE_ORACLE
  // show the Oracle dialog
  QDialog *dbs = dynamic_cast<QDialog*>( QgsProviderRegistry::instance()->selectWidget( "oracle", this ) );
  if ( !dbs )
  {
    QMessageBox::warning( this, tr( "Oracle" ), tr( "Cannot get Oracle select dialog from provider." ) );
    return;
  }
  connect( dbs, SIGNAL( addDatabaseLayers( QStringList const &, QString const & ) ),
           this, SLOT( addDatabaseLayers( QStringList const &, QString const & ) ) );
  connect( dbs, SIGNAL( progress( int, int ) ),
           this, SLOT( showProgress( int, int ) ) );
  connect( dbs, SIGNAL( progressMessage( QString ) ),
           this, SLOT( showStatusMessage( QString ) ) );
  dbs->exec();
  delete dbs;
#endif
} // MBGuiApp::addOracleLayer()

void MBGuiApp::addWmsLayer()
{
  // Fudge for now
  QgsDebugMsg( "about to addRasterLayer" );

  // TODO: QDialog for now, switch to QWidget in future
  QDialog *wmss = dynamic_cast<QDialog*>( QgsProviderRegistry::instance()->selectWidget( QString( "wms" ), this ) );
  if ( !wmss )
  {
    QMessageBox::warning( this, tr( "WMS" ), tr( "Cannot get WMS select dialog from provider." ) );
    return;
  }
  connect( wmss, SIGNAL( addRasterLayer( QString const &, QString const &, QString const & ) ),
           this, SLOT( addRasterLayer( QString const &, QString const &, QString const & ) ) );
  wmss->exec();
  delete wmss;
}

void MBGuiApp::addWcsLayer()
{
  QgsDebugMsg( "about to addWcsLayer" );

  // TODO: QDialog for now, switch to QWidget in future
  QDialog *wcss = dynamic_cast<QDialog*>( QgsProviderRegistry::instance()->selectWidget( QString( "wcs" ), this ) );
  if ( !wcss )
  {
    QMessageBox::warning( this, tr( "WCS" ), tr( "Cannot get WCS select dialog from provider." ) );
    return;
  }
  connect( wcss, SIGNAL( addRasterLayer( QString const &, QString const &, QString const & ) ),
           this, SLOT( addRasterLayer( QString const &, QString const &, QString const & ) ) );
  wcss->exec();
  delete wcss;
}

void MBGuiApp::addWfsLayer()
{
  if ( !canvas_xy )
  {
    return;
  }

  QgsDebugMsg( "about to addWfsLayer" );

  // TODO: QDialog for now, switch to QWidget in future
  QDialog *wfss = dynamic_cast<QDialog*>( QgsProviderRegistry::instance()->selectWidget( QString( "WFS" ), this ) );
  if ( !wfss )
  {
    QMessageBox::warning( this, tr( "WFS" ), tr( "Cannot get WFS select dialog from provider." ) );
    return;
  }
  connect( wfss, SIGNAL( addWfsLayer( QString, QString ) ),
           this, SLOT( addWfsLayer( QString, QString ) ) );

  //re-enable wfs with extent setting: pass canvas info to source select
  wfss->setProperty( "MapExtent", canvas_xy->extent().toString() );
  if ( canvas_xy->mapSettings().hasCrsTransformEnabled() )
  { //if "on the fly" reprojection is active, pass canvas CRS
    wfss->setProperty( "MapCRS", canvas_xy->mapSettings().destinationCrs().authid() );
  }

  bool bkRenderFlag = canvas_xy->renderFlag();
  canvas_xy->setRenderFlag( false );
  wfss->exec();
  canvas_xy->setRenderFlag( bkRenderFlag );
  delete wfss;
}

void MBGuiApp::addWfsLayer( const QString& uri, const QString& typeName )
{
  // TODO: this should be eventually moved to a more reasonable place
  addVectorLayer( uri, typeName, "WFS" );
}
*/

int MBGuiApp::messageTimeout()
{
  QSettings settings;
  return settings.value( "/qgis/messageTimeout", 5 ).toInt();
}

void MBGuiApp::fileExit()
{
  if ( saveDirty() )
  {
    closeProject();
    qApp->exit( 0 );
  }
}

void MBGuiApp::fileNew()
{
  fileNew( true ); // prompts whether to save project
}

void MBGuiApp::fileNewBlank()
{
  fileNew( true, true );
}

//as file new but accepts flags to indicate whether we should prompt to save
void MBGuiApp::fileNew( bool thePromptToSaveFlag, bool forceBlank )
{
    if ( thePromptToSaveFlag )
    {
        if ( !saveDirty() )
        {
            return; //cancel pressed
        }
    }

  mProjectLastModified = QDateTime();

  QSettings settings;

  closeProject();

  QgsProject* prj = QgsProject::instance();
  prj->clear();

  prj->layerTreeRegistryBridge()->setNewLayersVisible( settings.value( "/qgis/new_layers_visible", true ).toBool() );

  mLayerTreeCanvasBridge->clear();

  //set the color for selections
  //the default can be set in qgisoptions
  //use project properties to override the color on a per project basis
  int myRed = settings.value( "/qgis/default_selection_color_red", 255 ).toInt();
  int myGreen = settings.value( "/qgis/default_selection_color_green", 255 ).toInt();
  int myBlue = settings.value( "/qgis/default_selection_color_blue", 0 ).toInt();
  int myAlpha = settings.value( "/qgis/default_selection_color_alpha", 255 ).toInt();
  prj->writeEntry( "Gui", "/SelectionColorRedPart", myRed );
  prj->writeEntry( "Gui", "/SelectionColorGreenPart", myGreen );
  prj->writeEntry( "Gui", "/SelectionColorBluePart", myBlue );
  prj->writeEntry( "Gui", "/SelectionColorAlphaPart", myAlpha );
  canvas_xy->setSelectionColor( QColor( myRed, myGreen, myBlue, myAlpha ) );

  //set the canvas to the default background color
  //the default can be set in qgisoptions
  //use project properties to override the color on a per project basis
  myRed = settings.value( "/qgis/default_canvas_color_red", 255 ).toInt();
  myGreen = settings.value( "/qgis/default_canvas_color_green", 255 ).toInt();
  myBlue = settings.value( "/qgis/default_canvas_color_blue", 255 ).toInt();
  prj->writeEntry( "Gui", "/CanvasColorRedPart", myRed );
  prj->writeEntry( "Gui", "/CanvasColorGreenPart", myGreen );
  prj->writeEntry( "Gui", "/CanvasColorBluePart", myBlue );
  canvas_xy->setCanvasColor( QColor( myRed, myGreen, myBlue ) );
  mOverviewCanvas = new QgsMapOverviewCanvas;
  mOverviewCanvas->setBackgroundColor( QColor( myRed, myGreen, myBlue ) );

  prj->dirty( false );

  setTitleBarText_( *this );

  //QgsDebugMsg("emiting new project signal");

  // emit signal so listeners know we have a new project
  emit newProject();

  canvas_xy->freeze( false );
  canvas_xy->refresh();
  canvas_xy->clearExtentHistory();
  canvas_xy->setRotation( 0.0 );
  canvas_yz->freeze( false );
  canvas_yz->refresh();
  canvas_yz->clearExtentHistory();
  canvas_yz->setRotation( 0.0 );
  canvas_depths->freeze( false );
  canvas_depths->refresh();
  canvas_depths->clearExtentHistory();
  canvas_depths->setRotation( 0.0 );
  canvas_error->freeze( false );
  canvas_error->refresh();
  canvas_error->clearExtentHistory();
  canvas_error->setRotation( 0.0 );
  canvas_nei->freeze( false );
  canvas_nei->refresh();
  canvas_nei->clearExtentHistory();
  canvas_nei->setRotation( 0.0 );
  canvas_rei->freeze( false );
  canvas_rei->refresh();
  canvas_rei->clearExtentHistory();
  canvas_rei->setRotation( 0.0 );
  canvas_mean->freeze( false );
  canvas_mean->refresh();
  canvas_mean->clearExtentHistory();
  canvas_mean->setRotation( 0.0 );

  mScaleEdit->updateScales();

  // set project CRS
  QString defCrs = settings.value( "/Projections/projectDefaultCrs", GEO_EPSG_CRS_AUTHID ).toString();
  QgsCoordinateReferenceSystem srs;
  srs.createFromOgcWmsCrs( defCrs );
  canvas_xy->setDestinationCrs( srs );
  // write the projections _proj string_ to project settings
  prj->writeEntry( "SpatialRefSys", "/ProjectCRSProj4String", srs.toProj4() );
  prj->writeEntry( "SpatialRefSys", "/ProjectCrs", srs.authid() );
  prj->writeEntry( "SpatialRefSys", "/ProjectCRSID", static_cast< int >( srs.srsid() ) );
  prj->dirty( false );
  if ( srs.mapUnits() != QGis::UnknownUnit )
  {
    canvas_xy->setMapUnits( srs.mapUnits() );
    canvas_yz->setMapUnits( srs.mapUnits() );
  }

  // enable OTF CRS transformation if necessary
  canvas_xy->setCrsTransformEnabled( settings.value( "/Projections/otfTransformEnabled", 0 ).toBool() );
  canvas_yz->setCrsTransformEnabled( settings.value( "/Projections/otfTransformEnabled", 0 ).toBool() );

  updateCRSStatusBar();

  /** New Empty Project Created
      (before attempting to load custom project templates/filepaths) */

  // load default template
  /* NOTE: don't open default template on launch until after initialization,
           in case a project was defined via command line */

  // don't open template if last auto-opening of a project failed
  if ( ! forceBlank )
  {
    forceBlank = ! settings.value( "/qgis/projOpenedOKAtLaunch", QVariant( true ) ).toBool();
  }

  if ( ! forceBlank && settings.value( "/qgis/newProjectDefault", QVariant( false ) ).toBool() )
  {
    fileNewFromDefaultTemplate();
  }

  // set the initial map tool
  canvas_xy->setMapTool( mMapTools.mPan_xy );
//  mNonEditMapTool = mMapTools.mPan;  // signals are not yet setup to catch this
}

bool MBGuiApp::fileNewFromTemplate( const QString& fileName )
{
  if ( !saveDirty() )
  {
    return false; //cancel pressed
  }

  QgsDebugMsg( QString( "loading project template: %1" ).arg( fileName ) );
  if ( addProject( fileName ) )
  {
    // set null filename so we don't override the template
    QgsProject::instance()->setFileName( QString() );
    return true;
  }
  return false;
}

void MBGuiApp::fileNewFromDefaultTemplate()
{
  QString projectTemplate = QgsApplication::qgisSettingsDirPath() + QLatin1String( "project_default.qgs" );
  QString msgTxt;
  if ( !projectTemplate.isEmpty() && QFile::exists( projectTemplate ) )
  {
    if ( fileNewFromTemplate( projectTemplate ) )
    {
      return;
    }
    msgTxt = tr( "Default failed to open: %1" );
  }
  else
  {
    msgTxt = tr( "Default not found: %1" );
  }
  messageBar()->pushMessage( tr( "Open Template Project" ),
                             msgTxt.arg( projectTemplate ),
                             QgsMessageBar::WARNING );
}
void MBGuiApp::fileOpenAfterLaunch()
{
  // TODO: move auto-open project options to enums

  // check if a project is already loaded via command line or filesystem
  if ( !QgsProject::instance()->fileName().isNull() )
  {
    return;
  }

  // check if a data source is already loaded via command line or filesystem
  // empty project with layer loaded, but may not trigger a dirty project at this point
  if ( QgsProject::instance() && QgsMapLayerRegistry::instance()->count() > 0 )
  {
    return;
  }

  // fileNewBlank() has already been called in MBGuiApp constructor
  // loaded project is either a new blank one, or one from command line/filesystem
  QSettings settings;
  QString autoOpenMsgTitle = tr( "Auto-open Project" );

  // get path of project file to open, or was attempted
  QString projPath;

  if ( mProjOpen == 0 ) // welcome page
  {
    connect( this, SIGNAL( newProject() ), this, SLOT( showMapCanvas() ) );
    connect( this, SIGNAL( projectRead() ), this, SLOT( showMapCanvas() ) );
    return;
  }
  if ( mProjOpen == 1 && !mRecentProjects.isEmpty() ) // most recent project
  {
    projPath = mRecentProjects.at( 0 ).path;
  }
  if ( mProjOpen == 2 ) // specific project
  {
    projPath = settings.value( "/qgis/projOpenAtLaunchPath" ).toString();
  }

  // whether last auto-opening of a project failed
  bool projOpenedOK = settings.value( "/qgis/projOpenedOKAtLaunch", QVariant( true ) ).toBool();

  // notify user if last attempt at auto-opening a project failed
  // NOTE: Notification will not show if last auto-opened project failed but
  //    next project opened is from command line (minor issue)
  // TODO: Keep projOpenedOKAtLaunch from being reset to true after
  //    reading command line project (which happens before initialization signal)
  if ( !projOpenedOK )
  {
    // only show the following 'auto-open project failed' message once, at launch
    settings.setValue( "/qgis/projOpenedOKAtLaunch", QVariant( true ) );

    // set auto-open project back to 'New' to avoid re-opening bad project
    settings.setValue( "/qgis/projOpenAtLaunch", QVariant( 0 ) );

    messageBar()->pushMessage( autoOpenMsgTitle,
                               tr( "Failed to open: %1" ).arg( projPath ),
                               QgsMessageBar::CRITICAL );
    return;
  }

  if ( mProjOpen == 3 ) // new project
  {
    // open default template, if defined
    if ( settings.value( "/qgis/newProjectDefault", QVariant( false ) ).toBool() )
    {
      fileNewFromDefaultTemplate();
    }
    return;
  }

  if ( projPath.isEmpty() ) // projPath required from here
  {
    return;
  }

  if ( !projPath.endsWith( QLatin1String( "qgs" ), Qt::CaseInsensitive ) )
  {
    messageBar()->pushMessage( autoOpenMsgTitle,
                               tr( "Not valid project file: %1" ).arg( projPath ),
                               QgsMessageBar::WARNING );
    return;
  }

  if ( QFile::exists( projPath ) )
  {
    // set flag to check on next app launch if the following project opened OK
    settings.setValue( "/qgis/projOpenedOKAtLaunch", QVariant( false ) );

    if ( !addProject( projPath ) )
    {
      messageBar()->pushMessage( autoOpenMsgTitle,
                                 tr( "Project failed to open: %1" ).arg( projPath ),
                                 QgsMessageBar::WARNING );
    }

    if ( projPath.endsWith( QLatin1String( "project_default.qgs" ) ) )
    {
      messageBar()->pushMessage( autoOpenMsgTitle,
                                 tr( "Default template has been reopened: %1" ).arg( projPath ),
                                 QgsMessageBar::INFO );
    }
  }
  else
  {
    messageBar()->pushMessage( autoOpenMsgTitle,
                               tr( "File not found: %1" ).arg( projPath ),
                               QgsMessageBar::WARNING );
  }
}

void MBGuiApp::fileOpenedOKAfterLaunch()
{
  QSettings settings;
  settings.setValue( "/qgis/projOpenedOKAtLaunch", QVariant( true ) );
}

void MBGuiApp::fileNewFromTemplateAction( QAction * qAction )
{
  if ( ! qAction )
    return;

  if ( qAction->text() == tr( "< Blank >" ) )
  {
    fileNewBlank();
  }
  else
  {
    QSettings settings;
    QString templateDirName = settings.value( "/qgis/projectTemplateDir",
                              QgsApplication::qgisSettingsDirPath() + "project_templates" ).toString();
    fileNewFromTemplate( templateDirName + QDir::separator() + qAction->text() );
  }
}
/*void MBGuiApp::newVectorLayer()
{
  QString enc;
  QString fileName = QgsNewVectorLayerDialog::runAndCreateLayer( this, &enc );

  if ( !fileName.isEmpty() )
  {
    //then add the layer to the view
    QStringList fileNames;
    fileNames.append( fileName );
    //todo: the last parameter will change accordingly to layer type
    addVectorLayers( fileNames, enc, "file" );
  }
  else if ( fileName.isNull() )
  {
    QLabel *msgLabel = new QLabel( tr( "Layer creation failed. Please check the <a href=\"#messageLog\">message log</a> for further information." ), messageBar() );
    msgLabel->setWordWrap( true );
    connect( msgLabel, SIGNAL( linkActivated( QString ) ), mLogDock, SLOT( show() ) );
    QgsMessageBarItem *item = new QgsMessageBarItem( msgLabel, QgsMessageBar::WARNING );
    messageBar()->pushItem( item );
  }
}
*/
/*void MBGuiApp::newMemoryLayer()
{
  QgsVectorLayer* newLayer = QgsNewMemoryLayerDialog::runAndCreateLayer( this );

  if ( newLayer )
  {
    //then add the layer to the view
    QList< QgsMapLayer* > layers;
    layers << newLayer;

    QgsMapLayerRegistry::instance()->addMapLayers( layers );
    newLayer->startEditing();
  }
}*/

/*void MBGuiApp::newSpatialiteLayer()
{
  QgsNewSpatialiteLayerDialog spatialiteDialog( this );
  spatialiteDialog.exec();
}
*/
/*void MBGuiApp::showRasterCalculator()
{
  QgsRasterCalcDialog d( this );
  if ( d.exec() == QDialog::Accepted )
  {
    //invoke analysis library
    QgsRasterCalculator rc( d.formulaString(), d.outputFile(), d.outputFormat(), d.outputRectangle(), d.outputCrs(), d.numberOfColumns(), d.numberOfRows(), d.rasterEntries() );

    QProgressDialog p( tr( "Calculating..." ), tr( "Abort..." ), 0, 0 );
    p.setWindowModality( Qt::WindowModal );
    QgsRasterCalculator::Result res = static_cast< QgsRasterCalculator::Result >( rc.processCalculation( &p ) );
    switch ( res )
    {
      case QgsRasterCalculator::Success:
        if ( d.addLayerToProject() )
        {
          addRasterLayer( d.outputFile(), QFileInfo( d.outputFile() ).baseName() );
        }
        messageBar()->pushMessage( tr( "Raster calculator" ),
                                   tr( "Calculation complete." ),
                                   QgsMessageBar::INFO, messageTimeout() );
        break;

      case QgsRasterCalculator::CreateOutputError:
        messageBar()->pushMessage( tr( "Raster calculator" ),
                                   tr( "Could not create destination file." ),
                                   QgsMessageBar::CRITICAL );
        break;

      case QgsRasterCalculator::InputLayerError:
        messageBar()->pushMessage( tr( "Raster calculator" ),
                                   tr( "Could not read input layer." ),
                                   QgsMessageBar::CRITICAL );
        break;

      case QgsRasterCalculator::Cancelled:
        break;

      case QgsRasterCalculator::ParserError:
        messageBar()->pushMessage( tr( "Raster calculator" ),
                                   tr( "Could not parse raster formula." ),
                                   QgsMessageBar::CRITICAL );
        break;

      case QgsRasterCalculator::MemoryError:
        messageBar()->pushMessage( tr( "Raster calculator" ),
                                   tr( "Insufficient memory available for operation." ),
                                   QgsMessageBar::CRITICAL );
        break;

    }
  }
}
*/
/*void MBGuiApp::showAlignRasterTool()
{
  QgsAlignRasterDialog dlg( this );
  dlg.exec();
}
*/
void MBGuiApp::fileOpen()
{
  // possibly save any pending work before opening a new project
  if ( saveDirty() )
  {
    // Retrieve last used project dir from persistent settings
    QSettings settings;
    QString lastUsedDir = settings.value( "/UI/lastProjectDir", QDir::homePath() ).toString();
    QString fullPath = QFileDialog::getOpenFileName( this,
                       tr( "Choose a QGIS project file to open" ),
                       lastUsedDir,
                       tr( "QGIS files" ) + " (*.qgs *.QGS)" );
    if ( fullPath.isNull() )
    {
      return;
    }

    // Fix by Tim - getting the dirPath from the dialog
    // directly truncates the last node in the dir path.
    // This is a workaround for that
    QFileInfo myFI( fullPath );
    QString myPath = myFI.path();
    // Persist last used project dir
    settings.setValue( "/UI/lastProjectDir", myPath );

    // open the selected project
    addProject( fullPath );
  }
}

/*void MBGuiApp::enableProjectMacros()
{
  mTrustedMacros = true;

  // load macros
  QgsPythonRunner::run( "qgis.utils.reloadProjectMacros()" );
}
*/
/**
  adds a saved project to qgis, usually called on startup by specifying a
  project file on the command line
  */
bool MBGuiApp::addProject( const QString& projectFile )
{
  QFileInfo pfi( projectFile );
  statusBar()->showMessage( tr( "Loading project: %1" ).arg( pfi.fileName() ) );
  qApp->processEvents();

  QApplication::setOverrideCursor( Qt::WaitCursor );

  // close the previous opened project if any
  closeProject();

  if ( !QgsProject::instance()->read( projectFile ) )
  {
    QString backupFile = projectFile + "~";
    QString loadBackupPrompt;
    QMessageBox::StandardButtons buttons;
    if ( QFile( backupFile ).exists() )
    {
      loadBackupPrompt = "\n\n" + tr( "Do you want to open the backup file\n%1\ninstead?" ).arg( backupFile );
      buttons |= QMessageBox::Yes;
      buttons |= QMessageBox::No;
    }
    else
    {
      buttons |= QMessageBox::Ok;
    }
    QApplication::restoreOverrideCursor();
    statusBar()->clearMessage();

    int r = QMessageBox::critical( this,
                                   tr( "Unable to open project" ),
                                   QgsProject::instance()->error() + loadBackupPrompt,
                                   buttons );

    if ( QMessageBox::Yes == r && addProject( backupFile ) )
    {
      // We loaded data from the backup file, but we pretend to work on the original project file.
      QgsProject::instance()->setFileName( projectFile );
      QgsProject::instance()->setDirty( true );
      mProjectLastModified = pfi.lastModified();
      return true;
    }

    canvas_xy->freeze( false );
    canvas_xy->refresh();
    canvas_yz->freeze( false );
    canvas_yz->refresh();
    canvas_depths->freeze( false );
    canvas_depths->refresh();
    canvas_error->freeze( false );
    canvas_error->refresh();
    canvas_nei->freeze( false );
    canvas_nei->refresh();
    canvas_rei->freeze( false );
    canvas_rei->refresh();
    canvas_mean->freeze( false );
    canvas_mean->refresh();

    return false;
  }

  mProjectLastModified = pfi.lastModified();

  setTitleBarText_( *this );
  int  myRedInt = QgsProject::instance()->readNumEntry( "Gui", "/CanvasColorRedPart", 255 );
  int  myGreenInt = QgsProject::instance()->readNumEntry( "Gui", "/CanvasColorGreenPart", 255 );
  int  myBlueInt = QgsProject::instance()->readNumEntry( "Gui", "/CanvasColorBluePart", 255 );
  QColor myColor = QColor( myRedInt, myGreenInt, myBlueInt );
  canvas_xy->setCanvasColor( myColor ); //this is fill color before rendering starts
  mOverviewCanvas->setBackgroundColor( myColor );

  QgsDebugMsg( "Canvas background color restored..." );
  int myAlphaInt = QgsProject::instance()->readNumEntry( "Gui", "/SelectionColorAlphaPart", 255 );
  myRedInt = QgsProject::instance()->readNumEntry( "Gui", "/SelectionColorRedPart", 255 );
  myGreenInt = QgsProject::instance()->readNumEntry( "Gui", "/SelectionColorGreenPart", 255 );
  myBlueInt = QgsProject::instance()->readNumEntry( "Gui", "/SelectionColorBluePart", 0 );
  myColor = QColor( myRedInt, myGreenInt, myBlueInt, myAlphaInt );
  canvas_xy->setSelectionColor( myColor ); //this is selection color before rendering starts

  //load project scales
  bool projectScales = QgsProject::instance()->readBoolEntry( "Scales", "/useProjectScales" );
  if ( projectScales )
  {
    mScaleEdit->updateScales( QgsProject::instance()->readListEntry( "Scales", "/ScalesList" ) );
  }

  canvas_xy->updateScale();
  QgsDebugMsg( "Scale restored..." );

//  mActionFilterLegend->setChecked( QgsProject::instance()->readBoolEntry( "Legend", "filterByMap" ) );

  QSettings settings;

//  // does the project have any macros?
//  if ( mPythonUtils && mPythonUtils->isEnabled() )
//  {
//    if ( !QgsProject::instance()->readEntry( "Macros", "/pythonCode", QString::null ).isEmpty() )
//    {
//      int enableMacros = settings.value( "/qgis/enableMacros", 1 ).toInt();
//      // 0 = never, 1 = ask, 2 = just for this session, 3 = always

//      if ( enableMacros == 3 || enableMacros == 2 )
//      {
//        enableProjectMacros();
//      }
//      else if ( enableMacros == 1 ) // ask
//      {
//        // create the notification widget for macros


//        QToolButton *btnEnableMacros = new QToolButton();
//        btnEnableMacros->setText( tr( "Enable macros" ) );
//        btnEnableMacros->setStyleSheet( "background-color: rgba(255, 255, 255, 0); color: black; text-decoration: underline;" );
//        btnEnableMacros->setCursor( Qt::PointingHandCursor );
//        btnEnableMacros->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Preferred );
//        connect( btnEnableMacros, SIGNAL( clicked() ), mInfoBar, SLOT( popWidget() ) );
//        connect( btnEnableMacros, SIGNAL( clicked() ), this, SLOT( enableProjectMacros() ) );

//        QgsMessageBarItem *macroMsg = new QgsMessageBarItem(
//          tr( "Security warning" ),
//          tr( "project macros have been disabled." ),
//          btnEnableMacros,
//          QgsMessageBar::WARNING,
//          0,
//          mInfoBar );
//        // display the macros notification widget
//        mInfoBar->pushItem( macroMsg );
//      }
//    }
//  }

//  emit projectRead(); // let plug-ins know that we've read in a new
  // project so that they can check any project
  // specific plug-in state

  // add this to the list of recently used project files
  saveRecentProjectPath( projectFile, false );

  // read settings sam
  readProjectSettings();

  QApplication::restoreOverrideCursor();

  canvas_xy->freeze( false );
  canvas_xy->refresh();
  canvas_yz->freeze( false );
  canvas_yz->refresh();
  canvas_depths->freeze( false );
  canvas_depths->refresh();
  canvas_error->freeze( false );
  canvas_error->refresh();
  canvas_nei->freeze( false );
  canvas_nei->refresh();
  canvas_rei->freeze( false );
  canvas_rei->refresh();
  canvas_mean->freeze( false );
  canvas_mean->refresh();

  statusBar()->showMessage( tr( "Project loaded" ), 3000 );
  return true;
}

bool MBGuiApp::fileSave()
{
  // if we don't have a file name, then obviously we need to get one; note
  // that the project file name is reset to null in fileNew()
  QFileInfo fullPath;

  if ( QgsProject::instance()->fileName().isNull() )
  {
    // Retrieve last used project dir from persistent settings
    QSettings settings;
    QString lastUsedDir = settings.value( "/UI/lastProjectDir", QDir::homePath() ).toString();

    QString path = QFileDialog::getSaveFileName(
                     this,
                     tr( "Choose a QGIS project file" ),
                     lastUsedDir + '/' + QgsProject::instance()->title(),
                     tr( "QGIS files" ) + " (*.qgs *.QGS)" );
    if ( path.isEmpty() )
      return false;

    fullPath.setFile( path );

    // make sure we have the .qgs extension in the file name
    if ( "qgs" != fullPath.suffix().toLower() )
    {
      fullPath.setFile( fullPath.filePath() + ".qgs" );
    }


    QgsProject::instance()->setFileName( fullPath.filePath() );
  }
  else
  {
    QFileInfo fi( QgsProject::instance()->fileName() );
    fullPath = fi.absoluteFilePath();
    if ( fi.exists() && !mProjectLastModified.isNull() && mProjectLastModified != fi.lastModified() )
    {
      if ( QMessageBox::warning( this,
                                 tr( "Project file was changed" ),
                                 tr( "The loaded project file on disk was meanwhile changed.  Do you want to overwrite the changes?\n"
                                     "\nLast modification date on load was: %1"
                                     "\nCurrent last modification date is: %2" )
                                 .arg( mProjectLastModified.toString( Qt::DefaultLocaleLongDate ),
                                       fi.lastModified().toString( Qt::DefaultLocaleLongDate ) ),
                                 QMessageBox::Ok | QMessageBox::Cancel ) == QMessageBox::Cancel )
        return false;
    }

    if ( fi.exists() && ! fi.isWritable() )
    {
      messageBar()->pushMessage( tr( "Insufficient permissions" ),
                                 tr( "The project file is not writable." ),
                                 QgsMessageBar::WARNING );
      return false;
    }
  }

  //save mb params
  writeParamSettings();

  if ( QgsProject::instance()->write() )
  {
    setTitleBarText_( *this ); // update title bar
    statusBar()->showMessage( tr( "Saved project to: %1" ).arg( QgsProject::instance()->fileName() ), 5000 );

    saveRecentProjectPath( fullPath.filePath() );

    QFileInfo fi( QgsProject::instance()->fileName() );
    mProjectLastModified = fi.lastModified();
  }
  else
  {
    QMessageBox::critical( this,
                           tr( "Unable to save project %1" ).arg( QgsProject::instance()->fileName() ),
                           QgsProject::instance()->error() );
    return false;
  }
/*
  // run the saved project macro
  if ( mTrustedMacros )
  {
    QgsPythonRunner::run( "qgis.utils.saveProjectMacro();" );
  }
*/
  return true;
}

void MBGuiApp::fileSaveAs()
{
  // Retrieve last used project dir from persistent settings
  QSettings settings;
  QString lastUsedDir = settings.value( "/UI/lastProjectDir", QDir::homePath() ).toString();

  QString path = QFileDialog::getSaveFileName( this,
                 tr( "Choose a file name to save the QGIS project file as" ),
                 lastUsedDir + '/' + QgsProject::instance()->title(),
                 tr( "QGIS files" ) + " (*.qgs *.QGS)" );
  if ( path.isEmpty() )
    return;

  QFileInfo fullPath( path );

  //save mb params
  writeParamSettings();

  settings.setValue( "/UI/lastProjectDir", fullPath.path() );

  // make sure the .qgs extension is included in the path name. if not, add it...
  if ( "qgs" != fullPath.suffix().toLower() )
  {
    fullPath.setFile( fullPath.filePath() + ".qgs" );
  }

  QgsProject::instance()->setFileName( fullPath.filePath() );

  if ( QgsProject::instance()->write() )
  {
    setTitleBarText_( *this ); // update title bar
    statusBar()->showMessage( tr( "Saved project to: %1" ).arg( QgsProject::instance()->fileName() ), 5000 );
    // add this to the list of recently used project files
    saveRecentProjectPath( fullPath.filePath() );
    mProjectLastModified = fullPath.lastModified();
  }
  else
  {
    QMessageBox::critical( this,
                           tr( "Unable to save project %1" ).arg( QgsProject::instance()->fileName() ),
                           QgsProject::instance()->error(),
                           QMessageBox::Ok,
                           Qt::NoButton );
  }
}

/*void MBGuiApp::dxfExport()
{
  QgsDxfExportDialog d;
  if ( d.exec() == QDialog::Accepted )
  {
    QgsDxfExport dxfExport;
    dxfExport.addLayers( d.layers() );
    dxfExport.setSymbologyScaleDenominator( d.symbologyScale() );
    dxfExport.setSymbologyExport( d.symbologyMode() );
    dxfExport.setLayerTitleAsName( d.layerTitleAsName() );
    if ( mapCanvas() )
    {
      dxfExport.setMapUnits( mapCanvas()->mapUnits() );
      //extent
      if ( d.exportMapExtent() )
      {
        dxfExport.setExtent( mapCanvas()->extent() );
      }
    }

    QString fileName( d.saveFile() );
    if ( !fileName.endsWith( ".dxf", Qt::CaseInsensitive ) )
      fileName += ".dxf";
    QFile dxfFile( fileName );
    QApplication::setOverrideCursor( Qt::BusyCursor );
    if ( dxfExport.writeToFile( &dxfFile, d.encoding() ) == 0 )
    {
      messageBar()->pushMessage( tr( "DXF export completed" ), QgsMessageBar::INFO, 4 );
    }
    else
    {
      messageBar()->pushMessage( tr( "DXF export failed" ), QgsMessageBar::CRITICAL, 4 );
    }
    QApplication::restoreOverrideCursor();
  }
}
*/
/*void MBGuiApp::openLayerDefinition( const QString & path )
{
  QString errorMessage;
  bool loaded = QgsLayerDefinition::loadLayerDefinition( path, QgsProject::instance()->layerTreeRoot(), errorMessage );
  if ( !loaded )
  {
    QgsDebugMsg( errorMessage );
    messageBar()->pushMessage( tr( "Error loading layer definition" ), errorMessage, QgsMessageBar::WARNING );
  }
}*/

// Open the project file corresponding to the
// path at the given index in mRecentProjectPaths
void MBGuiApp::openProject( QAction *action )
{
  // possibly save any pending work before opening a different project
  Q_ASSERT( action );

  QString debugme = action->data().toString();
  if ( saveDirty() )
    addProject( debugme );

  //set the projections enabled icon in the status bar
  int myProjectionEnabledFlag =
    QgsProject::instance()->readNumEntry( "SpatialRefSys", "/ProjectionsEnabled", 0 );
  canvas_xy->setCrsTransformEnabled( myProjectionEnabledFlag );
}
/*void MBGuiApp::runScript( const QString &filePath )
{
  if ( !mPythonUtils || !mPythonUtils->isEnabled() )
    return;

  mPythonUtils->runString(
    QString( "import sys\n"
             "execfile(\"%1\".replace(\"\\\\\", \"/\").encode(sys.getfilesystemencoding()))\n" ).arg( filePath )
    , tr( "Failed to run Python script:" ), false );
}*/

/**
  Open the specified project file; prompt to save previous project if necessary.
  Used to process a commandline argument or OpenDocument AppleEvent.
  */
void MBGuiApp::openProject( const QString & fileName )
{
  // possibly save any pending work before opening a different project
  if ( saveDirty() )
  {
    // error handling and reporting is in addProject() function
    addProject( fileName );
  }
  return;
}

/**
  Open a raster or vector file; ignore other files.
  Used to process a commandline argument or OpenDocument AppleEvent.
  @returns true if the file is successfully opened
  */
bool MBGuiApp::openLayer( const QString & fileName, bool allowInteractive )
{
  QFileInfo fileInfo( fileName );
  bool ok( false );

  //to use see main.cpp #include <cpl_error.h>
//  CPLPushErrorHandler( CPLQuietErrorHandler );

  // if needed prompt for zipitem layers
  QString vsiPrefix = QgsZipItem::vsiPrefix( fileName );
  if ( vsiPrefix == "/vsizip/" || vsiPrefix == "/vsitar/" )
  {
    if ( askUserForZipItemLayers( fileName ) )
    {
//      CPLPopErrorHandler();
      return true;
    }
  }

  // try to load it as raster
  if ( QgsRasterLayer::isValidRasterFileName( fileName ) )
  {
    // open .adf as a directory
    if ( fileName.endsWith( ".adf", Qt::CaseInsensitive ) )
    {
      QString dirName = fileInfo.path();
      ok  = addRasterLayer( dirName, QFileInfo( dirName ).completeBaseName() );
    }
    else
      ok  = addRasterLayer( fileName, fileInfo.completeBaseName() );
  }
  // TODO - should we really call isValidRasterFileName() before addRasterLayer()
  //        this results in 2 calls to GDALOpen()
  // I think (Radim) that it is better to test only first if valid,
  // addRasterLayer() is really trying to add layer and gives error if fails
  //
  // if ( addRasterLayer( fileName, fileInfo.completeBaseName() ) )
  // {
  //   ok  = true );
  // }
  else // nope - try to load it as a shape/ogr
  {
    if ( allowInteractive )
    {
      ok = addVectorLayers( QStringList( fileName ), "System", "file" );
    }
    else
    {
      ok = addVectorLayer( fileName, fileInfo.completeBaseName(), "ogr" );
    }
  }


//  CPLPopErrorHandler();

  if ( !ok )
  {
    // we have no idea what this file is...
//    QgsMessageLog::logMessage( tr( "Unable to load %1" ).arg( fileName ) );
    QMessageBox::critical(this, tr("MBGui: Open File Error"),tr( "Unable to load %1" ).arg( fileName ));
  }

  return ok;
}

// Open a file specified by a commandline argument, Drop or FileOpen event.
void MBGuiApp::openFile( const QString & fileName )
{
  // check to see if we are opening a project file
  QFileInfo fi( fileName );
  if ( fi.completeSuffix() == "qgs" )
  {
    QgsDebugMsg( "Opening project " + fileName );
    openProject( fileName );
  }
//  else if ( fi.completeSuffix() == "qlr" )
//  {
//    openLayerDefinition( fileName );
//  }
//  else if ( fi.completeSuffix() == "py" )
//  {
//    runScript( fileName );
//  }
  else
  {
    QgsDebugMsg( "Adding " + fileName + " to the map canvas" );
    openLayer( fileName, true );
  }
}

/*void MBGuiApp::newPrintComposer()
{
  QString title;
  if ( !uniqueComposerTitle( this, title, true ) )
  {
    return;
  }
  createNewComposer( title );
}*/

/*void MBGuiApp::showComposerManager()
{
  if ( !mComposerManager )
  {
    mComposerManager = new QgsComposerManager( nullptr, Qt::Window );
    connect( mComposerManager, SIGNAL( finished( int ) ), this, SLOT( deleteComposerManager() ) );
  }
  mComposerManager->show();
  mComposerManager->activate();
}*/

/*void MBGuiApp::deleteComposerManager()
{
  mComposerManager->deleteLater();
  mComposerManager = nullptr;
}*/

/*void MBGuiApp::disablePreviewMode()
{
  canvas_xy->setPreviewModeEnabled( false );
}*/

/*void MBGuiApp::activateGrayscalePreview()
{
  canvas_xy->setPreviewModeEnabled( true );
  canvas_xy->setPreviewMode( QgsPreviewEffect::PreviewGrayscale );
}*/

/*void MBGuiApp::activateMonoPreview()
{
  canvas_xy->setPreviewModeEnabled( true );
  canvas_xy->setPreviewMode( QgsPreviewEffect::PreviewMono );
}*/

/*void MBGuiApp::activateProtanopePreview()
{
  canvas_xy->setPreviewModeEnabled( true );
  canvas_xy->setPreviewMode( QgsPreviewEffect::PreviewProtanope );
}*/

/*void MBGuiApp::activateDeuteranopePreview()
{
  canvas_xy->setPreviewModeEnabled( true );
  canvas_xy->setPreviewMode( QgsPreviewEffect::PreviewDeuteranope );
}*/

/*void MBGuiApp::toggleFilterLegendByExpression( bool checked )
{
  QgsLayerTreeNode* node = mLayerTreeView->currentNode();
  if ( ! node )
    return;

  if ( QgsLayerTree::isLayer( node ) )
  {
    QString e = mLegendExpressionFilterButton->expressionText();
    QgsLayerTreeUtils::setLegendFilterByExpression( *QgsLayerTree::toLayer( node ), e, checked );
  }

  updateFilterLegend();
}
*/
/*void MBGuiApp::updateFilterLegend()
{
    if ( mActionFilterLegend->isChecked() )
        layerTreeView()->layerTreeModel()->setLegendFilterByMap( &canvas_xy->mapSettings() );
    else if ( QgsLayerTreeUtils::hasLegendFilterExpression( *mLayerTreeView->layerTreeModel()->rootGroup() ) )*/
   //     layerTreeView()->layerTreeModel()->setLegendFilter( &canvas_xy->mapSettings(), /* useExtent */ false );
   /* else
        layerTreeView()->layerTreeModel()->setLegendFilterByMap( nullptr );
}*/

void MBGuiApp::saveMapAsImage()
{
  QPair< QString, QString> myFileNameAndFilter = QgisGui::getSaveAsImageName( this, tr( "Choose a file name to save the map image as" ) );
  if ( myFileNameAndFilter.first != "" )
  {
    //save the mapview to the selected file
    canvas_xy->saveAsImage( myFileNameAndFilter.first, nullptr, myFileNameAndFilter.second );
    statusBar()->showMessage( tr( "Saved map image to %1" ).arg( myFileNameAndFilter.first ) );
  }

}

//overloaded version of the above function
void MBGuiApp::saveMapAsImage( const QString& theImageFileNameQString, QPixmap * theQPixmap )
{
  if ( theImageFileNameQString == "" )
  {
    //no fileName chosen
    return;
  }
  else
  {
    //force the size of the canvase
    canvas_xy->resize( theQPixmap->width(), theQPixmap->height() );
    //save the mapview to the selected file
    canvas_xy->saveAsImage( theImageFileNameQString, theQPixmap );
  }
}

//reimplements method from base (gui) class
void MBGuiApp::addAllToOverview()
{
  if ( mLayerTreeView )
  {
    Q_FOREACH ( QgsLayerTreeLayer* nodeL, mLayerTreeView->layerTreeModel()->rootGroup()->findLayers() )
      nodeL->setCustomProperty( "overview", 1 );
  }

  markDirty();
}

//reimplements method from base (gui) class
void MBGuiApp::removeAllFromOverview()
{
  if ( mLayerTreeView )
  {
    Q_FOREACH ( QgsLayerTreeLayer* nodeL, mLayerTreeView->layerTreeModel()->rootGroup()->findLayers() )
      nodeL->setCustomProperty( "overview", 0 );
  }

  markDirty();
}

void MBGuiApp::toggleFullScreen()
{
  if ( mFullScreenMode )
  {
    if ( mPrevScreenModeMaximized )
    {
      // Change to maximized state. Just calling showMaximized() results in
      // the window going to the normal state. Calling showNormal() then
      // showMaxmized() is a work-around. Turn off rendering for this as it
      // would otherwise cause two re-renders of the map, which can take a
      // long time.
      bool renderFlag = mapCanvas()->renderFlag();
      if ( renderFlag )
        mapCanvas()->setRenderFlag( false );
      showNormal();
      showMaximized();
      if ( renderFlag )
        mapCanvas()->setRenderFlag( true );
      mPrevScreenModeMaximized = false;
    }
    else
    {
      showNormal();
    }
    mFullScreenMode = false;
  }
  else
  {
    if ( isMaximized() )
    {
      mPrevScreenModeMaximized = true;
    }
    showFullScreen();
    mFullScreenMode = true;
  }
}

void MBGuiApp::showActiveWindowMinimized()
{
  QWidget *window = QApplication::activeWindow();
  if ( window )
  {
    window->showMinimized();
  }
}

void MBGuiApp::toggleActiveWindowMaximized()
{
  QWidget *window = QApplication::activeWindow();
  if ( window )
  {
    if ( window->isMaximized() )
      window->showNormal();
    else
      window->showMaximized();
  }
}

/*void MBGuiApp::activate()
{
  raise();
  setWindowState( windowState() & ~Qt::WindowMinimized );
  activateWindow();
}*/

/*void MBGuiApp::bringAllToFront()
{
#ifdef Q_OS_MAC
  // Bring forward all open windows while maintaining layering order
  ProcessSerialNumber psn;
  GetCurrentProcess( &psn );
  SetFrontProcess( &psn );
#endif
}
*/
/*void MBGuiApp::addWindow( QAction *action )
{
#ifdef Q_OS_MAC
  mWindowActions->addAction( action );
  mWindowMenu->addAction( action );
  action->setCheckable( true );
  action->setChecked( true );
#else
  Q_UNUSED( action );
#endif
}
*/
/*void MBGuiApp::removeWindow( QAction *action )
{
#ifdef Q_OS_MAC
  mWindowActions->removeAction( action );
  mWindowMenu->removeAction( action );
#else
  Q_UNUSED( action );
#endif
}
*/
void MBGuiApp::stopRendering()
{
  if ( canvas_xy )
    canvas_xy->stopRendering();
}

//reimplements method from base (gui) class
void MBGuiApp::hideAllLayers()
{
  QgsDebugMsg( "hiding all layers!" );

  Q_FOREACH ( QgsLayerTreeLayer* nodeLayer, mLayerTreeView->layerTreeModel()->rootGroup()->findLayers() )
    nodeLayer->setVisible( Qt::Unchecked );
}

// reimplements method from base (gui) class
void MBGuiApp::showAllLayers()
{
  QgsDebugMsg( "Showing all layers!" );

  Q_FOREACH ( QgsLayerTreeLayer* nodeLayer, mLayerTreeView->layerTreeModel()->rootGroup()->findLayers() )
    nodeLayer->setVisible( Qt::Checked );
}

//reimplements method from base (gui) class
void MBGuiApp::hideSelectedLayers()
{
  QgsDebugMsg( "hiding selected layers!" );

  Q_FOREACH ( QgsLayerTreeNode* node, mLayerTreeView->selectedNodes() )
  {
    if ( QgsLayerTree::isGroup( node ) )
      QgsLayerTree::toGroup( node )->setVisible( Qt::Unchecked );
    else if ( QgsLayerTree::isLayer( node ) )
      QgsLayerTree::toLayer( node )->setVisible( Qt::Unchecked );
  }
}

// reimplements method from base (gui) class
void MBGuiApp::showSelectedLayers()
{
  QgsDebugMsg( "show selected layers!" );

  Q_FOREACH ( QgsLayerTreeNode* node, mLayerTreeView->selectedNodes() )
  {
    if ( QgsLayerTree::isGroup( node ) )
      QgsLayerTree::toGroup( node )->setVisible( Qt::Checked );
    else if ( QgsLayerTree::isLayer( node ) )
      QgsLayerTree::toLayer( node )->setVisible( Qt::Checked );
  }
}

void MBGuiApp::zoomIn()
{
  QgsDebugMsg( "Setting map tool to zoomIn" );

  canvas_xy->setMapTool( mMapTools.mZoomIn_xy );
  canvas_yz->setMapTool( mMapTools.mZoomIn_yz );
}

void MBGuiApp::zoomOut()
{
  canvas_xy->setMapTool( mMapTools.mZoomOut_xy );
}

void MBGuiApp::zoomToSelected()
{
  canvas_xy->zoomToSelected();
//  canvas_yz->zoomToSelected();
}

void MBGuiApp::panToSelected()
{
  canvas_xy->panToSelected();
//  canvas_yz->panToSelected();
}

void MBGuiApp::pan()
{
  canvas_xy->setMapTool( mMapTools.mPan_xy );
}

void MBGuiApp::zoomFull()
{
  canvas_xy->zoomToFullExtent();
}

void MBGuiApp::zoomToPrevious()
{
  canvas_xy->zoomToPreviousExtent();
}

void MBGuiApp::zoomToNext()
{
  canvas_xy->zoomToNextExtent();
}

void MBGuiApp::zoomActualSize()
{
  legendLayerZoomNative();
}

void MBGuiApp::identify()
{
  canvas_xy->setMapTool( mMapTools.mIdentify_xy );
}

/*void MBGuiApp::doFeatureAction()
{
  canvas_xy->setMapTool( mMapTools.mFeatureAction );
}

/*void MBGuiApp::updateDefaultFeatureAction( QAction *action )
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
}*/

/*void MBGuiApp::refreshFeatureActions()
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

}*/

/*void MBGuiApp::measure()
{
  canvas_xy->setMapTool( mMapTools.mMeasureDist );
}
*/
/*void MBGuiApp::measureArea()
{
  canvas_xy->setMapTool( mMapTools.mMeasureArea );
}
*/
/*void MBGuiApp::measureAngle()
{
  canvas_xy->setMapTool( mMapTools.mMeasureAngle );
}
*/
/*void MBGuiApp::addFormAnnotation()
{
  canvas_xy->setMapTool( mMapTools.mFormAnnotation );
}*/

/*void MBGuiApp::addHtmlAnnotation()
{
  canvas_xy->setMapTool( mMapTools.mHtmlAnnotation );
}*/

/*void MBGuiApp::addTextAnnotation()
{
  canvas_xy->setMapTool( mMapTools.mTextAnnotation );
}*/

/*void MBGuiApp::addSvgAnnotation()
{
  canvas_xy->setMapTool( mMapTools.mSvgAnnotation );
}*/

/*void MBGuiApp::modifyAnnotation()
{
  canvas_xy->setMapTool( mMapTools.mAnnotation );
}*/

/*void MBGuiApp::reprojectAnnotations()
{
  Q_FOREACH ( QgsAnnotationItem * annotation, annotationItems() )
  {
    annotation->updatePosition();
  }
}*/

/*void MBGuiApp::labelingFontNotFound( QgsVectorLayer* vlayer, const QString& fontfamily )
{
  // TODO: update when pref for how to resolve missing family (use matching algorithm or just default font) is implemented
  QString substitute = tr( "Default system font substituted." );

  QToolButton* btnOpenPrefs = new QToolButton();
  btnOpenPrefs->setStyleSheet( "QToolButton{ background-color: rgba(255, 255, 255, 0); color: black; text-decoration: underline; }" );
  btnOpenPrefs->setCursor( Qt::PointingHandCursor );
  btnOpenPrefs->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Preferred );
  btnOpenPrefs->setToolButtonStyle( Qt::ToolButtonTextOnly );

  // store pointer to vlayer in data of QAction
  QAction* act = new QAction( btnOpenPrefs );
  act->setData( QVariant( QMetaType::QObjectStar, &vlayer ) );
  act->setText( tr( "Open labeling dialog" ) );
  btnOpenPrefs->addAction( act );
  btnOpenPrefs->setDefaultAction( act );
  btnOpenPrefs->setToolTip( "" );
  connect( btnOpenPrefs, SIGNAL( triggered( QAction* ) ), this, SLOT( labelingDialogFontNotFound( QAction* ) ) );

  // no timeout set, since notice needs attention and is only shown first time layer is labeled
  QgsMessageBarItem* fontMsg = new QgsMessageBarItem(
    tr( "Labeling" ),
    tr( "Font for layer <b><u>%1</u></b> was not found (<i>%2</i>). %3" ).arg( vlayer->name(), fontfamily, substitute ),
    btnOpenPrefs,
    QgsMessageBar::WARNING,
    0,
    messageBar() );
  messageBar()->pushItem( fontMsg );
}*/

void MBGuiApp::commitError( QgsVectorLayer* vlayer )
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
/*void MBGuiApp::labelingDialogFontNotFound( QAction* act )
{
  if ( !act )
  {
    return;
  }

  // get base pointer to layer
  QObject* obj = qvariant_cast<QObject*>( act->data() );

  // remove calling messagebar widget
  messageBar()->popWidget();

  if ( !obj )
  {
    return;
  }

  QgsMapLayer* layer = qobject_cast<QgsMapLayer*>( obj );
  if ( layer && setActiveLayer( layer ) )
  {
    labeling();
  }
}*/

/*void MBGuiApp::labeling()
{
  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer*>( activeLayer() );
  if ( !vlayer )
  {
    messageBar()->pushMessage( tr( "Labeling Options" ),
                               tr( "Please select a vector layer first" ),
                               QgsMessageBar::INFO,
                               messageTimeout() );
    return;
  }


  QDialog dlg;
  dlg.setWindowTitle( tr( "Layer labeling settings" ) );
  QgsLabelingWidget *labelingGui = new QgsLabelingWidget( vlayer, canvas_xy, &dlg );
  labelingGui->layout()->setContentsMargins( 0, 0, 0, 0 );
  QVBoxLayout *layout = new QVBoxLayout( &dlg );
  layout->addWidget( labelingGui );

  QDialogButtonBox *buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply, Qt::Horizontal, &dlg );
  layout->addWidget( buttonBox );

  dlg.setLayout( layout );

  QSettings settings;
  dlg.restoreGeometry( settings.value( "/Windows/Labeling/geometry" ).toByteArray() );

  connect( buttonBox->button( QDialogButtonBox::Ok ), SIGNAL( clicked() ), &dlg, SLOT( accept() ) );
  connect( buttonBox->button( QDialogButtonBox::Cancel ), SIGNAL( clicked() ), &dlg, SLOT( reject() ) );
  connect( buttonBox->button( QDialogButtonBox::Apply ), SIGNAL( clicked() ), labelingGui, SLOT( apply() ) );

  if ( dlg.exec() )
  {
    labelingGui->writeSettingsToLayer();

    settings.setValue( "/Windows/Labeling/geometry", dlg.saveGeometry() );

    // trigger refresh
    if ( canvas_xy )
    {
      canvas_xy->refresh();
    }
  }

  activateDeactivateLayerRelatedActions( vlayer );
}
*/
/*void MBGuiApp::setCadDockVisible( bool visible )
{
  mAdvancedDigitizingDockWidget->setVisible( visible );
}*/

void MBGuiApp::fieldCalculator()
{
  QgsVectorLayer *myLayer = qobject_cast<QgsVectorLayer *>( activeLayer_xy() );
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

void MBGuiApp::attributeTable()
{
  QgsVectorLayer *myLayer = qobject_cast<QgsVectorLayer *>( activeLayer_xy() );
  if ( !myLayer )
  {
    return;
  }

  QgsAttributeTableDialog *mDialog = new QgsAttributeTableDialog( myLayer );
  mDialog->show();
  // the dialog will be deleted by itself on close
}

/*void MBGuiApp::saveAsRasterFile()
{
  QgsRasterLayer* rasterLayer = qobject_cast<QgsRasterLayer *>( activeLayer() );
  if ( !rasterLayer )
  {
    return;
  }

  QgsRasterLayerSaveAsDialog d( rasterLayer, rasterLayer->dataProvider(),
                                canvas_xy->extent(), rasterLayer->crs(),
                                canvas_xy->mapSettings().destinationCrs(),
                                this );
  if ( d.exec() == QDialog::Rejected )
    return;

  QSettings settings;
  settings.setValue( "/UI/lastRasterFileDir", QFileInfo( d.outputFileName() ).absolutePath() );

  QgsRasterFileWriter fileWriter( d.outputFileName() );
  if ( d.tileMode() )
  {
    fileWriter.setTiledMode( true );
    fileWriter.setMaxTileWidth( d.maximumTileSizeX() );
    fileWriter.setMaxTileHeight( d.maximumTileSizeY() );
  }

  QProgressDialog pd( nullptr, tr( "Abort..." ), 0, 0 );
  // Show the dialo immediately because cloning pipe can take some time (WCS)
  pd.setLabelText( tr( "Reading raster" ) );
  pd.setWindowTitle( tr( "Saving raster" ) );
  pd.show();
  pd.setWindowModality( Qt::WindowModal );

  // TODO: show error dialogs
  // TODO: this code should go somewhere else, but probably not into QgsRasterFileWriter
  // clone pipe/provider is not really necessary, ready for threads
  QScopedPointer<QgsRasterPipe> pipe( nullptr );

  if ( d.mode() == QgsRasterLayerSaveAsDialog::RawDataMode )
  {
    QgsDebugMsg( "Writing raw data" );
    //QgsDebugMsg( QString( "Writing raw data" ).arg( pos ) );
    pipe.reset( new QgsRasterPipe() );
    if ( !pipe->set( rasterLayer->dataProvider()->clone() ) )
    {
      QgsDebugMsg( "Cannot set pipe provider" );
      return;
    }

    QgsRasterNuller *nuller = new QgsRasterNuller();
    for ( int band = 1; band <= rasterLayer->dataProvider()->bandCount(); band ++ )
    {
      nuller->setNoData( band, d.noData() );
    }
    if ( !pipe->insert( 1, nuller ) )
    {
      QgsDebugMsg( "Cannot set pipe nuller" );
      return;
    }

    // add projector if necessary
    if ( d.outputCrs() != rasterLayer->crs() )
    {
      QgsRasterProjector * projector = new QgsRasterProjector;
      projector->setCRS( rasterLayer->crs(), d.outputCrs() );
      if ( !pipe->insert( 2, projector ) )
      {
        QgsDebugMsg( "Cannot set pipe projector" );
        return;
      }
    }
  }
  else // RenderedImageMode
  {
    // clone the whole pipe
    QgsDebugMsg( "Writing rendered image" );
    pipe.reset( new QgsRasterPipe( *rasterLayer->pipe() ) );
    QgsRasterProjector *projector = pipe->projector();
    if ( !projector )
    {
      QgsDebugMsg( "Cannot get pipe projector" );
      return;
    }
    projector->setCRS( rasterLayer->crs(), d.outputCrs() );
  }

  if ( !pipe->last() )
  {
    return;
  }
  fileWriter.setCreateOptions( d.createOptions() );

  fileWriter.setBuildPyramidsFlag( d.buildPyramidsFlag() );
  fileWriter.setPyramidsList( d.pyramidsList() );
  fileWriter.setPyramidsResampling( d.pyramidsResamplingMethod() );
  fileWriter.setPyramidsFormat( d.pyramidsFormat() );
  fileWriter.setPyramidsConfigOptions( d.pyramidsConfigOptions() );

  QgsRasterFileWriter::WriterError err = fileWriter.writeRaster( pipe.data(), d.nColumns(), d.nRows(), d.outputRectangle(), d.outputCrs(), &pd );
  if ( err != QgsRasterFileWriter::NoError )
  {
    QMessageBox::warning( this, tr( "Error" ),
                          tr( "Cannot write raster error code: %1" ).arg( err ),
                          QMessageBox::Ok );

  }
  else
  {
    QString fileName( d.outputFileName() );
    if ( d.tileMode() )
    {
      QFileInfo outputInfo( fileName );
      fileName = QString( "%1/%2.vrt" ).arg( fileName, outputInfo.fileName() );
    }

    if ( d.addToCanvas() )
    {
      addRasterLayers( QStringList( fileName ) );
    }

    emit layerSavedAs( rasterLayer, fileName );
    messageBar()->pushMessage( tr( "Saving done" ),
                               tr( "Export to raster file has been completed" ),
                               QgsMessageBar::INFO, messageTimeout() );
  }
}*/

/*void MBGuiApp::saveAsFile()
{
  QgsMapLayer* layer = activeLayer();
  if ( !layer )
    return;

  QgsMapLayer::LayerType layerType = layer->type();
  if ( layerType == QgsMapLayer::RasterLayer )
  {
    saveAsRasterFile();
  }
  else if ( layerType == QgsMapLayer::VectorLayer )
  {
    saveAsVectorFileGeneral();
  }
}*/

/*void MBGuiApp::saveAsLayerDefinition()
{
  QString path = QFileDialog::getSaveFileName( this, "Save as Layer Definition File", QDir::home().path(), "*.qlr" );
  QgsDebugMsg( path );
  if ( path.isEmpty() )
    return;

  QString errorMessage;
  bool saved = QgsLayerDefinition::exportLayerDefinition( path, mLayerTreeView->selectedNodes(), errorMessage );
  if ( !saved )
  {
    messageBar()->pushMessage( tr( "Error saving layer definintion file" ), errorMessage, QgsMessageBar::WARNING );
  }
}*/

void MBGuiApp::saveAsVectorFileGeneral( QgsVectorLayer* vlayer, bool symbologyOption )
{
  if ( !vlayer )
  {
    vlayer = qobject_cast<QgsVectorLayer *>( activeLayer_xy() ); // FIXME: output of multiple layers at once?
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

/*void MBGuiApp::checkForDeprecatedLabelsInProject()
{
  bool ok;
  QgsProject::instance()->readBoolEntry( "DeprecatedLabels", "/Enabled", false, &ok );
  if ( ok ) // project already flagged (regardless of project property value)
  {
    return;
  }

  if ( QgsMapLayerRegistry::instance()->count() > 0 )
  {
    bool depLabelsUsed = false;
    QMap<QString, QgsMapLayer*> layers = QgsMapLayerRegistry::instance()->mapLayers();
    for ( QMap<QString, QgsMapLayer*>::iterator it = layers.begin(); it != layers.end(); ++it )
    {
      QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( it.value() );
      if ( !vl )
      {
        continue;
      }

      Q_NOWARN_DEPRECATED_PUSH
      depLabelsUsed = vl->hasLabelsEnabled();
      if ( depLabelsUsed )
      {
        break;
      }
      Q_NOWARN_DEPRECATED_POP
    }
    if ( depLabelsUsed )
    {
      QgsProject::instance()->writeEntry( "DeprecatedLabels", "/Enabled", true );
    }
  }
}*/

void MBGuiApp::layerProperties()
{
  showLayerProperties( activeLayer_xy() );
}

void MBGuiApp::deleteSelected( QgsMapLayer *layer, QWidget* parent, bool promptConfirmation )
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

/*void MBGuiApp::moveFeature()
{
  canvas_xy->setMapTool( mMapTools.mMoveFeature );
}*/

/*void MBGuiApp::offsetCurve()
{
  canvas_xy->setMapTool( mMapTools.mOffsetCurve );
}
*/
/*void MBGuiApp::simplifyFeature()
{
  canvas_xy->setMapTool( mMapTools.mSimplifyFeature );
}*/

/*void MBGuiApp::deleteRing()
{
  canvas_xy->setMapTool( mMapTools.mDeleteRing );
}
*/
/*void MBGuiApp::deletePart()
{
  canvas_xy->setMapTool( mMapTools.mDeletePart );
}
*/
/*QgsGeometry* MBGuiApp::unionGeometries( const QgsVectorLayer* vl, QgsFeatureList& featureList, bool& canceled )
{
  canceled = false;
  if ( !vl || featureList.size() < 2 )
  {
    return nullptr;
  }

  QgsGeometry* unionGeom = featureList[0].geometry();
  QgsGeometry* backupPtr = nullptr; //pointer to delete intermediate results
  if ( !unionGeom )
  {
    return nullptr;
  }

  QProgressDialog progress( tr( "Merging features..." ), tr( "Abort" ), 0, featureList.size(), this );
  progress.setWindowModality( Qt::WindowModal );

  QApplication::setOverrideCursor( Qt::WaitCursor );

  for ( int i = 1; i < featureList.size(); ++i )
  {
    if ( progress.wasCanceled() )
    {
      delete unionGeom;
      QApplication::restoreOverrideCursor();
      canceled = true;
      return nullptr;
    }
    progress.setValue( i );
    QgsGeometry* currentGeom = featureList[i].geometry();
    if ( currentGeom )
    {
      backupPtr = unionGeom;
      unionGeom = unionGeom->combine( currentGeom );
      if ( i > 1 ) //delete previous intermediate results
      {
        delete backupPtr;
        backupPtr = nullptr;
      }
      if ( !unionGeom )
      {
        QApplication::restoreOverrideCursor();
        return nullptr;
      }
    }
  }

  //convert unionGeom to a multipart geometry in case it is necessary to match the layer type
  if ( QGis::isMultiType( vl->wkbType() ) && !unionGeom->isMultipart() )
  {
    unionGeom->convertToMultiType();
  }

  QApplication::restoreOverrideCursor();
  progress.setValue( featureList.size() );
  return unionGeom;
}
*/
/*bool MBGuiApp::uniqueComposerTitle( QWidget* parent, QString& composerTitle, bool acceptEmpty, const QString& currentName )
{
  if ( !parent )
  {
    parent = this;
  }
  bool ok = false;
  bool titleValid = false;
  QString newTitle = QString( currentName );
  QString chooseMsg = tr( "Create unique print composer title" );
  if ( acceptEmpty )
  {
    chooseMsg += '\n' + tr( "(title generated if left empty)" );
  }
  QString titleMsg = chooseMsg;

  QStringList cNames;
  cNames << newTitle;
  Q_FOREACH ( QgsComposer* c, printComposers() )
  {
    cNames << c->title();
  }

  while ( !titleValid )
  {
    newTitle = QInputDialog::getItem( parent,
                                      tr( "Composer title" ),
                                      titleMsg,
                                      cNames,
                                      cNames.indexOf( newTitle ),
                                      true,
                                      &ok );
    if ( !ok )
    {
      return false;
    }

    if ( newTitle.isEmpty() )
    {
      if ( !acceptEmpty )
      {
        titleMsg = chooseMsg + "\n\n" + tr( "Title can not be empty!" );
      }
      else
      {
        titleValid = true;
      }
    }
    else if ( cNames.indexOf( newTitle, 1 ) >= 0 )
    {
      cNames[0] = QString(); // clear non-unique name
      titleMsg = chooseMsg + "\n\n" + tr( "Title already exists!" );
    }
    else
    {
      titleValid = true;
    }
  }

  composerTitle = newTitle;

  return true;
}
*/
/*QgsComposer* MBGuiApp::createNewComposer( QString title )
{
  //ask user about name
  mLastComposerId++;
  if ( title.isEmpty() )
  {
    title = tr( "Composer %1" ).arg( mLastComposerId );
  }
  //create new composer object
  QgsComposer* newComposerObject = new QgsComposer( this, title );

  //add it to the map of existing print composers
  mPrintComposers.insert( newComposerObject );
  //and place action into print composers menu
  mPrintComposersMenu->addAction( newComposerObject->windowAction() );
  newComposerObject->open();
  emit composerAdded( newComposerObject->view() );
  connect( newComposerObject, SIGNAL( composerAdded( QgsComposerView* ) ), this, SIGNAL( composerAdded( QgsComposerView* ) ) );
  connect( newComposerObject, SIGNAL( composerWillBeRemoved( QgsComposerView* ) ), this, SIGNAL( composerWillBeRemoved( QgsComposerView* ) ) );
  connect( newComposerObject, SIGNAL( atlasPreviewFeatureChanged() ), this, SLOT( refreshMapCanvas() ) );
  markDirty();
  return newComposerObject;
}
*/
/*void MBGuiApp::deleteComposer( QgsComposer* c )
{
  emit composerWillBeRemoved( c->view() );
  mPrintComposers.remove( c );
  mPrintComposersMenu->removeAction( c->windowAction() );
  markDirty();
  emit composerRemoved( c->view() );

  //save a reference to the composition
  QgsComposition* composition = c->composition();

  //first, delete the composer. This must occur before deleting the composition as some of the cleanup code in
  //composer or in composer item widgets may require the composition to still be around
  delete c;

  //next, delete the composition
  if ( composition )
  {
    delete composition;
  }
}
*/
/*QgsComposer* MBGuiApp::duplicateComposer( QgsComposer* currentComposer, QString title )
{
  QgsComposer* newComposer = nullptr;

  // test that current composer template write is valid
  QDomDocument currentDoc;
  currentComposer->templateXML( currentDoc );
  QDomElement compositionElem = currentDoc.documentElement().firstChildElement( "Composition" );
  if ( compositionElem.isNull() )
  {
    QgsDebugMsg( "selected composer could not be stored as temporary template" );
    return newComposer;
  }

  if ( title.isEmpty() )
  {
    // TODO: inject a bit of randomness in auto-titles?
    title = currentComposer->title() + tr( " copy" );
  }

  newComposer = createNewComposer( title );
  if ( !newComposer )
  {
    QgsDebugMsg( "could not create new composer" );
    return newComposer;
  }

  // hiding composer until template is loaded is much faster, provide feedback to user
  newComposer->hide();
  if ( !newComposer->loadFromTemplate( currentDoc, true ) )
  {
    deleteComposer( newComposer );
    newComposer = nullptr;
    QgsDebugMsg( "Error, composer could not be duplicated" );
    return newComposer;
  }
  newComposer->activate();

  return newComposer;
}
*/
/*bool MBGuiApp::loadComposersFromProject( const QDomDocument& doc )
{
  if ( doc.isNull() )
  {
    return false;
  }

  //restore each composer
  QDomNodeList composerNodes = doc.elementsByTagName( "Composer" );
  for ( int i = 0; i < composerNodes.size(); ++i )
  {
    QString title( composerNodes.at( i ).toElement().attribute( "title" ) );
    showStatusMessage( tr( "Loading composer %1" ).arg( title ) );
    showProgress( i,  composerNodes.size() );
    ++mLastComposerId;

    QTime t;
    t.start();
    QgsComposer* composer = new QgsComposer( this, tr( "Composer %1" ).arg( mLastComposerId ) );
    composer->readXML( composerNodes.at( i ).toElement(), doc );
    mPrintComposers.insert( composer );
    mPrintComposersMenu->addAction( composer->windowAction() );
#ifndef Q_OS_MACX
    composer->setWindowState( Qt::WindowMinimized );
    composer->show();
#endif
    composer->zoomFull();
    QgsComposerView* composerView = composer->view();
    if ( composerView )
    {
      composerView->updateRulers();
    }
    if ( composerNodes.at( i ).toElement().attribute( "visible", "1" ).toInt() < 1 )
    {
      composer->close();
    }
    emit composerAdded( composer->view() );
    connect( composer, SIGNAL( composerAdded( QgsComposerView* ) ), this, SIGNAL( composerAdded( QgsComposerView* ) ) );
    connect( composer, SIGNAL( composerWillBeRemoved( QgsComposerView* ) ), this, SIGNAL( composerWillBeRemoved( QgsComposerView* ) ) );
    connect( composer, SIGNAL( atlasPreviewFeatureChanged() ), this, SLOT( refreshMapCanvas() ) );

    QgsDebugMsg( QString( "Loaded composer %1: %2ms" ).arg( title ).arg( t.elapsed() ) );
  }

  showProgress( 0, 0 );

  return true;
}
*/
/*void MBGuiApp::deletePrintComposers()
{
  QSet<QgsComposer*>::iterator it = mPrintComposers.begin();
  while ( it != mPrintComposers.end() )
  {
    QgsComposer* c = ( *it );
    emit composerWillBeRemoved( c->view() );
    it = mPrintComposers.erase( it );
    emit composerRemoved( c->view() );

    //save a reference to the composition
    QgsComposition* composition = c->composition();

    //first, delete the composer. This must occur before deleting the composition as some of the cleanup code in
    //composer or in composer item widgets may require the composition to still be around
    delete( c );

    //next, delete the composition
    if ( composition )
    {
      delete composition;
    }
  }
  mLastComposerId = 0;
  markDirty();
}
*/
/*void MBGuiApp::on_mPrintComposersMenu_aboutToShow()
{
  QList<QAction*> acts = mPrintComposersMenu->actions();
  mPrintComposersMenu->clear();
  if ( acts.size() > 1 )
  {
    // sort actions by text
    qSort( acts.begin(), acts.end(), cmpByText_ );
  }
  mPrintComposersMenu->addActions( acts );
}
*/
/*bool MBGuiApp::loadAnnotationItemsFromProject( const QDomDocument& doc )
{
  if ( !canvas_xy )
  {
    return false;
  }

  removeAnnotationItems();

  if ( doc.isNull() )
  {
    return false;
  }

  QDomNodeList textItemList = doc.elementsByTagName( "TextAnnotationItem" );
  for ( int i = 0; i < textItemList.size(); ++i )
  {
    QgsTextAnnotationItem* newTextItem = new QgsTextAnnotationItem( canvas_xy );
    newTextItem->readXML( doc, textItemList.at( i ).toElement() );
  }

  QDomNodeList formItemList = doc.elementsByTagName( "FormAnnotationItem" );
  for ( int i = 0; i < formItemList.size(); ++i )
  {
    QgsFormAnnotationItem* newFormItem = new QgsFormAnnotationItem( canvas_xy );
    newFormItem->readXML( doc, formItemList.at( i ).toElement() );
  }

#ifdef WITH_QTWEBKIT
  QDomNodeList htmlItemList = doc.elementsByTagName( "HtmlAnnotationItem" );
  for ( int i = 0; i < htmlItemList.size(); ++i )
  {
    QgsHtmlAnnotationItem* newHtmlItem = new QgsHtmlAnnotationItem( canvas_xy );
    newHtmlItem->readXML( doc, htmlItemList.at( i ).toElement() );
  }
#endif

  QDomNodeList svgItemList = doc.elementsByTagName( "SVGAnnotationItem" );
  for ( int i = 0; i < svgItemList.size(); ++i )
  {
    QgsSvgAnnotationItem* newSvgItem = new QgsSvgAnnotationItem( canvas_xy );
    newSvgItem->readXML( doc, svgItemList.at( i ).toElement() );
  }
  return true;
}
*/
/*void MBGuiApp::showPinnedLabels( bool show )
{
  qobject_cast<QgsMapToolPinLabels*>( mMapTools.mPinLabels )->showPinnedLabels( show );
}*/

/*void MBGuiApp::pinLabels()
{
  mActionShowPinnedLabels->setChecked( true );
  canvas_xy->setMapTool( mMapTools.mPinLabels );
}
*/
/*void MBGuiApp::showHideLabels()
{
  canvas_xy->setMapTool( mMapTools.mShowHideLabels );
}
*/
/*void MBGuiApp::moveLabel()
{
  canvas_xy->setMapTool( mMapTools.mMoveLabel );
}
*/
void MBGuiApp::rotateFeature()
{
  canvas_xy->setMapTool( mMapTools.mRotateFeature );
}

/*void MBGuiApp::rotateLabel()
{
  canvas_xy->setMapTool( mMapTools.mRotateLabel );
}
*/
/*void MBGuiApp::changeLabelProperties()
{
  canvas_xy->setMapTool( mMapTools.mChangeLabelProperties );
}
*/
/*QList<QgsAnnotationItem*> MBGuiApp::annotationItems()
{
  QList<QgsAnnotationItem*> itemList;

  if ( !canvas_xy )
  {
    return itemList;
  }

  if ( canvas_xy )
  {
    QList<QGraphicsItem*> graphicsItems = canvas_xy->items();
    QList<QGraphicsItem*>::iterator gIt = graphicsItems.begin();
    for ( ; gIt != graphicsItems.end(); ++gIt )
    {
      QgsAnnotationItem* currentItem = dynamic_cast<QgsAnnotationItem*>( *gIt );
      if ( currentItem )
      {
        itemList.push_back( currentItem );
      }
    }
  }
  return itemList;
}
*/
/*void MBGuiApp::removeAnnotationItems()
{
  if ( !canvas_xy )
  {
    return;
  }
  QGraphicsScene* scene = canvas_xy->scene();
  if ( !scene )
  {
    return;
  }
  QList<QgsAnnotationItem*> itemList = annotationItems();
  QList<QgsAnnotationItem*>::iterator itemIt = itemList.begin();
  for ( ; itemIt != itemList.end(); ++itemIt )
  {
    if ( *itemIt )
    {
      scene->removeItem( *itemIt );
      delete *itemIt;
    }
  }
}
*/
/*void MBGuiApp::mergeAttributesOfSelectedFeatures()
{
  //get active layer (hopefully vector)
  QgsMapLayer *activeMapLayer = activeLayer();
  if ( !activeMapLayer )
  {
    messageBar()->pushMessage( tr( "No active layer" ),
                               tr( "No active layer found. Please select a layer in the layer list" ),
                               QgsMessageBar::INFO, messageTimeout() );
    return;
  }

  QgsVectorLayer* vl = qobject_cast<QgsVectorLayer *>( activeMapLayer );
  if ( !vl )
  {
    messageBar()->pushMessage(
      tr( "Layer not editable" ),
      tr( "The merge features tool only works on vector layers." ),
      QgsMessageBar::WARNING );
    return;
  }

  if ( !vl->isEditable() )
  {
    messageBar()->pushMessage(
      tr( "Layer not editable" ),
      tr( "Merging features can only be done for layers in editing mode." ),
      QgsMessageBar::WARNING );

    return;
  }

  //get selected feature ids (as a QSet<int> )
  const QgsFeatureIds& featureIdSet = vl->selectedFeaturesIds();
  if ( featureIdSet.size() < 2 )
  {
    messageBar()->pushMessage(
      tr( "Not enough features selected" ),
      tr( "The merge tool requires at least two selected features" ),
      QgsMessageBar::WARNING );
    return;
  }

  //get initial selection (may be altered by attribute merge dialog later)
  QgsFeatureList featureList = vl->selectedFeatures();  //get QList<QgsFeature>

  //merge the attributes together
  QgsMergeAttributesDialog d( featureList, vl, mapCanvas() );
  //intialise dialog with all columns set to skip
  d.setAllToSkip();
  if ( d.exec() == QDialog::Rejected )
  {
    return;
  }

  vl->beginEditCommand( tr( "Merged feature attributes" ) );

  QgsAttributes merged = d.mergedAttributes();
  QSet<int> toSkip = d.skippedAttributeIndexes();

  bool firstFeature = true;
  Q_FOREACH ( QgsFeatureId fid, vl->selectedFeaturesIds() )
  {
    for ( int i = 0; i < merged.count(); ++i )
    {
      if ( toSkip.contains( i ) )
        continue;

      QVariant val = merged.at( i );
      const QgsField &fld( vl->fields().at( i ) );
      bool isDefaultValue = vl->fields().fieldOrigin( i ) == QgsFields::OriginProvider &&
                            vl->dataProvider() &&
                            vl->dataProvider()->defaultValue( vl->fields().fieldOriginIndex( i ) ) == val;

      // convert to destination data type
      if ( !isDefaultValue && !fld.convertCompatible( val ) )
      {
        if ( firstFeature )
        {
          //only warn on first feature
          messageBar()->pushMessage(
            tr( "Invalid result" ),
            tr( "Could not store value '%1' in field of type %2" ).arg( merged.at( i ).toString(), fld.typeName() ),
            QgsMessageBar::WARNING );
        }
      }
      else
      {
        vl->changeAttributeValue( fid, i, val );
      }
    }
    firstFeature = false;
  }

  vl->endEditCommand();

  if ( mapCanvas() )
  {
    mapCanvas()->refresh();
  }
}
*/
/*void MBGuiApp::mergeSelectedFeatures()
{
  //get active layer (hopefully vector)
  QgsMapLayer* activeMapLayer = activeLayer();
  if ( !activeMapLayer )
  {
    messageBar()->pushMessage(
      tr( "No active layer" ),
      tr( "Please select a layer in the layer list" ),
      QgsMessageBar::WARNING );
    return;
  }
  QgsVectorLayer* vl = qobject_cast<QgsVectorLayer *>( activeMapLayer );
  if ( !vl )
  {
    messageBar()->pushMessage(
      tr( "Invalid layer" ),
      tr( "The merge features tool only works on vector layers." ),
      QgsMessageBar::WARNING );
    return;
  }
  if ( !vl->isEditable() )
  {
    messageBar()->pushMessage(
      tr( "Layer not editable" ),
      tr( "Merging features can only be done for layers in editing mode." ),
      QgsMessageBar::WARNING );

    return;
  }

  QgsVectorDataProvider* dp = vl->dataProvider();
  bool providerChecksTypeStrictly = true;
  if ( dp )
  {
    providerChecksTypeStrictly = dp->doesStrictFeatureTypeCheck();
  }

  //get selected feature ids (as a QSet<int> )
  const QgsFeatureIds& featureIdSet = vl->selectedFeaturesIds();
  if ( featureIdSet.size() < 2 )
  {
    messageBar()->pushMessage(
      tr( "Not enough features selected" ),
      tr( "The merge tool requires at least two selected features" ),
      QgsMessageBar::WARNING );
    return;
  }

  //get initial selection (may be altered by attribute merge dialog later)
  QgsFeatureIds featureIds = vl->selectedFeaturesIds();
  QgsFeatureList featureList = vl->selectedFeatures();  //get QList<QgsFeature>
  bool canceled;
  QgsGeometry* unionGeom = unionGeometries( vl, featureList, canceled );
  if ( !unionGeom )
  {
    if ( !canceled )
    {
      messageBar()->pushMessage(
        tr( "Merge failed" ),
        tr( "An error occurred during the merge operation" ),
        QgsMessageBar::CRITICAL );
    }
    return;
  }

  //make a first geometry union and notify the user straight away if the union geometry type does not match the layer one
  if ( providerChecksTypeStrictly && unionGeom->wkbType() != vl->wkbType() )
  {
    messageBar()->pushMessage(
      tr( "Merge cancelled" ),
      tr( "The union operation would result in a geometry type that is not compatible with the current layer." ),
      QgsMessageBar::CRITICAL );

    delete unionGeom;
    return;
  }

  //merge the attributes together
  QgsMergeAttributesDialog d( featureList, vl, mapCanvas() );
  if ( d.exec() == QDialog::Rejected )
  {
    delete unionGeom;
    return;
  }

  QgsFeatureIds featureIdsAfter = vl->selectedFeaturesIds();

  if ( featureIdsAfter.size() < 2 )
  {
    messageBar()->pushMessage(
      tr( "Not enough features selected" ),
      tr( "The merge tool requires at least two selected features" ),
      QgsMessageBar::WARNING );
    delete unionGeom;
    return;
  }

  //if the user changed the feature selection in the merge dialog, we need to repeat the union and check the type
  if ( featureIds.size() != featureIdsAfter.size() )
  {
    delete unionGeom;
    bool canceled;
    QgsFeatureList featureListAfter = vl->selectedFeatures();
    unionGeom = unionGeometries( vl, featureListAfter, canceled );
    if ( !unionGeom )
    {
      if ( !canceled )
      {
        messageBar()->pushMessage(
          tr( "Merge failed" ),
          tr( "An error occurred during the merge operation" ),
          QgsMessageBar::CRITICAL );
      }
      return;
    }

    if ( providerChecksTypeStrictly && unionGeom->wkbType() != vl->wkbType() )
    {
      messageBar()->pushMessage(
        tr( "Merge cancelled" ),
        tr( "The union operation would result in a geometry type that is not compatible with the current layer." ),
        QgsMessageBar::CRITICAL );
      delete unionGeom;
      return;
    }
  }

  vl->beginEditCommand( tr( "Merged features" ) );

  //create new feature
  QgsFeature newFeature;
  newFeature.setGeometry( unionGeom );

  QgsAttributes attrs = d.mergedAttributes();
  for ( int i = 0; i < attrs.count(); ++i )
  {
    QVariant val = attrs.at( i );
    bool isDefaultValue = vl->fields().fieldOrigin( i ) == QgsFields::OriginProvider &&
                          vl->dataProvider() &&
                          vl->dataProvider()->defaultValue( vl->fields().fieldOriginIndex( i ) ) == val;

    // convert to destination data type
    if ( !isDefaultValue && !vl->fields().at( i ).convertCompatible( val ) )
    {
      messageBar()->pushMessage(
        tr( "Invalid result" ),
        tr( "Could not store value '%1' in field of type %2" ).arg( attrs.at( i ).toString(), vl->fields().at( i ).typeName() ),
        QgsMessageBar::WARNING );
    }
    attrs[i] = val;
  }
  newFeature.setAttributes( attrs );

  QgsFeatureIds::const_iterator feature_it = featureIdsAfter.constBegin();
  for ( ; feature_it != featureIdsAfter.constEnd(); ++feature_it )
  {
    vl->deleteFeature( *feature_it );
  }

  vl->addFeature( newFeature, false );

  vl->endEditCommand();

  if ( mapCanvas() )
  {
    mapCanvas()->refresh();
  }
}
*/
void MBGuiApp::nodeTool()
{
  canvas_xy->setMapTool( mMapTools.mNodeTool );
}

/*void MBGuiApp::rotatePointSymbols()
{
  canvas_xy->setMapTool( mMapTools.mRotatePointSymbolsTool );
}
*/
/*void MBGuiApp::snappingOptions()
{
  mSnappingDialog->show();
}
*/
/*void MBGuiApp::splitFeatures()
{
  canvas_xy->setMapTool( mMapTools.mSplitFeatures );
}*/

/*void MBGuiApp::splitParts()
{
  canvas_xy->setMapTool( mMapTools.mSplitParts );
}
*/
/*void MBGuiApp::reshapeFeatures()
{
  canvas_xy->setMapTool( mMapTools.mReshapeFeatures );
}
*/
/*void MBGuiApp::addFeature()
{
  canvas_xy->setMapTool( mMapTools.mAddFeature );
}
*/
/*void MBGuiApp::circularStringCurvePoint()
{
  canvas_xy->setMapTool( mMapTools.mCircularStringCurvePoint );
}
*/
/*void MBGuiApp::circularStringRadius()
{
  canvas_xy->setMapTool( mMapTools.mCircularStringRadius );
}
*/
void MBGuiApp::toggleExcludes(bool isCanvasXY)
{
//    QgsLayerTreeView *view;
//    QMap<QString,QStack<QgsFeatureIds>*> *map;
//    if(isCanvasXY)
//    {
//        view = mLayerTreeView;
//        map = selectedMap;
//    }
//    else
//    {
//        view = mLayerTreeView_yz;
//        map = selectedMap_yz;
//    }
//    QUndoCommand *selectedCommand = new SelectedCommand(view,map);
    QUndoCommand *selectedCommand = new SelectedCommand(mLayerTreeView,mLayerTreeView_yz,selectedMap,selectedMap_yz);
    undoStack->push(selectedCommand);
//    activateDeactivateLayerRelatedActions(activeLayer() );
}
//void MBGuiApp::toggleExcludes()
//{
//    QUndoCommand *selectedCommand = new SelectedCommand(mLayerTreeView,mLayerTreeView_yz,selectedMap,selectedMap_yz);
//    undoStack->push(selectedCommand);
////    activateDeactivateLayerRelatedActions(activeLayer() );
//}
void MBGuiApp::selectFeatures()
{
  canvas_xy->setMapTool( mMapTools.mSelectFeatures_xy );
  canvas_yz->setMapTool( mMapTools.mSelectFeatures_yz );
}
void MBGuiApp::toggleExcludes2(QgsMapLayer *layer, QWidget* parent)
{
  if ( !layer )
  {
    layer = mLayerTreeView->currentLayer();
  }

  if ( !parent )
  {
    parent = this;
  }

  QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer *>( layer );
  if ( !vlayer )
  {
    messageBar()->pushMessage( tr( "No Vector Layer Selected" ),
                               tr( "Toggling Exclude features only works on vector layers" ),
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
  vlayer->beginEditCommand( tr( "Features Toggle Exclude" ) );
  int toggledCount = 0;
  //May not need to do this with the undo but instead just call a function sam
//  QUndoCommand *selectedCommand = new SelectedCommand(mLayerTreeView,selectedMap);
//  undoStack->push(selectedCommand);
//  toggleExcludes();
//  if ( !vlayer->toggleSelectedFeatures( &deletedCount ) )
//  {
//    messageBar()->pushMessage( tr( "Problem toggling exclude features" ),
//                               tr( "A problem occurred during toggling exclude of %1 feature(s)" ).arg( numberOfSelectedFeatures - toggledCount ),
//                               QgsMessageBar::WARNING );
//  }
//  else
//  {
    showStatusMessage( tr( "%n feature(s) toggled exclude.", "number of features deleted", numberOfSelectedFeatures ) );
//  }

  vlayer->endEditCommand();
}

void MBGuiApp::selectByPolygon()
{
  canvas_xy->setMapTool( mMapTools.mSelectPolygon_xy );
  canvas_yz->setMapTool( mMapTools.mSelectPolygon_yz );
}

void MBGuiApp::selectByFreehand()
{
  canvas_xy->setMapTool( mMapTools.mSelectFreehand_xy );
  canvas_yz->setMapTool( mMapTools.mSelectFreehand_yz );
}

void MBGuiApp::selectByRadius()
{
  canvas_xy->setMapTool( mMapTools.mSelectRadius_xy );
  canvas_yz->setMapTool( mMapTools.mSelectRadius_yz );
}

void MBGuiApp::deselectAll()
{
  // Turn off rendering to improve speed.
  bool renderFlagState = canvas_xy->renderFlag();
  bool renderFlagState_yz = canvas_yz->renderFlag();
  if ( renderFlagState )
    canvas_xy->setRenderFlag( false );
  if ( renderFlagState_yz )
      canvas_yz->setRenderFlag( false );

  QMap<QString, QgsMapLayer*> layers = QgsMapLayerRegistry::instance()->mapLayers();
  for ( QMap<QString, QgsMapLayer*>::iterator it = layers.begin(); it != layers.end(); ++it )
  {
    QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( it.value() );
    if ( !vl )
      continue;

    vl->removeSelection();
  }

  // Turn on rendering (if it was on previously)
  if ( renderFlagState )
    canvas_xy->setRenderFlag( true );
  if ( renderFlagState_yz )
    canvas_yz->setRenderFlag( true );
}

void MBGuiApp::invertSelection()
{
  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( canvas_xy->currentLayer() );
  QgsVectorLayer *vlayer_yz = qobject_cast<QgsVectorLayer *>( canvas_yz->currentLayer() );
  if ( !vlayer )
  {
    messageBar()->pushMessage(
      tr( "No active vector layer" ),
      tr( "To invert selection, choose a vector layer in the legend" ),
      QgsMessageBar::INFO,
      messageTimeout() );
    return;
  }

  // Turn off rendering to improve speed.
  bool renderFlagState = canvas_xy->renderFlag();
  bool renderFlagState_yz = canvas_yz->renderFlag();
  if ( renderFlagState )
    canvas_xy->setRenderFlag( false );
  if ( renderFlagState_yz )
    canvas_yz->setRenderFlag( false );

  vlayer->invertSelection();
  vlayer_yz->invertSelection();

  // Turn on rendering (if it was on previously)
  if ( renderFlagState )
    canvas_xy->setRenderFlag( true );
  if ( renderFlagState_yz )
    canvas_yz->setRenderFlag( true );
}

void MBGuiApp::selectAll()
{
  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( canvas_xy->currentLayer() );
  QgsVectorLayer *vlayer_yz = qobject_cast<QgsVectorLayer *>( canvas_yz->currentLayer() );
  if ( !vlayer )
  {
    messageBar()->pushMessage(
      tr( "No active vector layer" ),
      tr( "To select all, choose a vector layer in the legend" ),
      QgsMessageBar::INFO,
      messageTimeout() );
    return;
  }

  // Turn off rendering to improve speed.
  bool renderFlagState = canvas_xy->renderFlag();
  bool renderFlagState_yz = canvas_yz->renderFlag();
  if ( renderFlagState )
    canvas_xy->setRenderFlag( false );
  if ( renderFlagState_yz )
    canvas_yz->setRenderFlag( false );

  vlayer->selectAll();
  vlayer_yz->selectAll();

  // Turn on rendering (if it was on previously)
  if ( renderFlagState )
    canvas_xy->setRenderFlag( true );
  if ( renderFlagState_yz )
    canvas_yz->setRenderFlag( true );
}

void MBGuiApp::selectByExpression()
{
  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( canvas_xy->currentLayer() );
//  QgsVectorLayer *vlayer_yz = qobject_cast<QgsVectorLayer *>( canvas_yz->currentLayer() );
  if ( !vlayer )
  {
    messageBar()->pushMessage(
      tr( "No active vector layer" ),
      tr( "To select features, choose a vector layer in the legend" ),
      QgsMessageBar::INFO,
      messageTimeout() );
    return;
  }

  QgsExpressionSelectionDialog* dlg = new QgsExpressionSelectionDialog( vlayer, QString(), this );
  //if this is used, then need to find all features in canvas_xy that match those selected since the xy fields are not the same for canvas_yz data
  dlg->setAttribute( Qt::WA_DeleteOnClose );
  dlg->show();
}
/*void MBGuiApp::addRing()
{
  canvas_xy->setMapTool( mMapTools.mAddRing );
}*/
/*void MBGuiApp::fillRing()
{
  canvas_xy->setMapTool( mMapTools.mFillRing );
}
*/
/*void MBGuiApp::addPart()
{
  canvas_xy->setMapTool( mMapTools.mAddPart );
}
*/

/*void MBGuiApp::editCut( QgsMapLayer * layerContainingSelection )
{
  // Test for feature support in this layer
  QgsVectorLayer* selectionVectorLayer = qobject_cast<QgsVectorLayer *>( layerContainingSelection ? layerContainingSelection : activeLayer() );
  if ( !selectionVectorLayer )
    return;

  clipboard()->replaceWithCopyOf( selectionVectorLayer );

  selectionVectorLayer->beginEditCommand( tr( "Features cut" ) );
  selectionVectorLayer->deleteSelectedFeatures();
  selectionVectorLayer->endEditCommand();
}*/

/*void MBGuiApp::editCopy( QgsMapLayer * layerContainingSelection )
{
  QgsVectorLayer* selectionVectorLayer = qobject_cast<QgsVectorLayer *>( layerContainingSelection ? layerContainingSelection : activeLayer() );
  if ( !selectionVectorLayer )
    return;

  // Test for feature support in this layer
  clipboard()->replaceWithCopyOf( selectionVectorLayer );
}*/

/*void MBGuiApp::clipboardChanged()
{
  activateDeactivateLayerRelatedActions( activeLayer() );
}*/

/*void MBGuiApp::editPaste( QgsMapLayer *destinationLayer )
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
}*/

void MBGuiApp::pasteAsNewVector()
{

  QgsVectorLayer *layer = pasteToNewMemoryVector();
  if ( !layer )
    return;

  saveAsVectorFileGeneral( layer, false );

  delete layer;
}

QgsVectorLayer *MBGuiApp::pasteAsNewMemoryVector( const QString & theLayerName )
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

QgsVectorLayer *MBGuiApp::pasteToNewMemoryVector()
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
/*void MBGuiApp::copyStyle( QgsMapLayer * sourceLayer )
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
}*/
/**  \param destinationLayer  The layer that the clipboard will be pasted to
                            (defaults to the active layer on the legend)
 */

/*void MBGuiApp::pasteStyle( QgsMapLayer * destinationLayer )
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
}*/

void MBGuiApp::copyFeatures( QgsFeatureStore & featureStore )
{
  clipboard()->replaceWithCopyOf( featureStore );
}

void MBGuiApp::refreshMapCanvas()
{
  //stop any current rendering
  canvas_xy->stopRendering();

  canvas_xy->refreshAllLayers();

  canvas_yz->stopRendering();
  canvas_yz->refreshAllLayers();
}

void MBGuiApp::canvasRefreshStarted()
{
  showProgress( -1, 0 ); // trick to make progress bar show busy indicator
}

void MBGuiApp::canvasRefreshFinished()
{
  showProgress( 0, 0 ); // stop the busy indicator
}

void MBGuiApp::toggleMapTips()
{
  mMapTipsVisible = !mMapTipsVisible;
  // if off, stop the timer
  if ( !mMapTipsVisible )
  {
    mpMapTipsTimer->stop();
  }
}

/*void MBGuiApp::toggleEditing()
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
}*/

/*bool MBGuiApp::toggleEditing( QgsMapLayer *layer, bool allowCancel )
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
qDebug()<<"vlayer->isEditable(): "<<vlayer->isEditable()<<endl;
qDebug()<<"vlayer->isModified(): "<<vlayer->isModified()<<endl;


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
}*/

/*void MBGuiApp::saveActiveLayerEdits()
{
  saveEdits( activeLayer(), true, true );
}*/

/*void MBGuiApp::saveEdits( QgsMapLayer *layer, bool leaveEditable, bool triggerRepaint )
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
}*/

/*void MBGuiApp::cancelEdits( QgsMapLayer *layer, bool leaveEditable, bool triggerRepaint )
{
  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
  if ( !vlayer || !vlayer->isEditable() )
    return;

  if ( vlayer == activeLayer() && leaveEditable )
    mSaveRollbackInProgress = true;

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
  canvas_xy->freeze( false );

  if ( leaveEditable )
  {
    vlayer->startEditing();
  }
  if ( triggerRepaint )
  {
    vlayer->triggerRepaint();
  }
}*/

/*void MBGuiApp::saveEdits()
{
  Q_FOREACH ( QgsMapLayer * layer, mLayerTreeView->selectedLayers() )
  {
    saveEdits( layer, true, false );
  }
  canvas_xy->refresh();
  activateDeactivateLayerRelatedActions( activeLayer() );
}*/

/*void MBGuiApp::saveAllEdits( bool verifyAction )
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
}*/

/*void MBGuiApp::rollbackEdits()
{
  Q_FOREACH ( QgsMapLayer * layer, mLayerTreeView->selectedLayers() )
  {
    cancelEdits( layer, true, false );
  }
  canvas_xy->refresh();
  activateDeactivateLayerRelatedActions( activeLayer() );
}*/

/*void MBGuiApp::rollbackAllEdits( bool verifyAction )
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
}*/

/*void MBGuiApp::cancelEdits()
{
  Q_FOREACH ( QgsMapLayer * layer, mLayerTreeView->selectedLayers() )
  {
    cancelEdits( layer, false, false );
  }
  canvas_xy->refresh();
  activateDeactivateLayerRelatedActions( activeLayer() );
}*/

/*void MBGuiApp::cancelAllEdits( bool verifyAction )
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
}*/

/*bool MBGuiApp::verifyEditsActionDialog( const QString& act, const QString& upon )
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
}*/

/*void MBGuiApp::updateLayerModifiedActions()
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
}*/

/*QList<QgsMapLayer *> MBGuiApp::editableLayers( bool modified ) const
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
}*/
/*void MBGuiApp::duplicateVectorStyle( QgsVectorLayer* srcLayer, QgsVectorLayer* destLayer )
{
  // copy symbology, if possible
  if ( srcLayer->geometryType() == destLayer->geometryType() )
  {
    QDomImplementation DomImplementation;
    QDomDocumentType documentType =
      DomImplementation.createDocumentType(
        "qgis", "http://mrcc.com/qgis.dtd", "SYSTEM" );
    QDomDocument doc( documentType );
    QDomElement rootNode = doc.createElement( "qgis" );
    rootNode.setAttribute( "version", QString( "%1" ).arg( QGis::QGIS_VERSION ) );
    doc.appendChild( rootNode );
    QString errorMsg;
    srcLayer->writeSymbology( rootNode, doc, errorMsg );
    destLayer->readSymbology( rootNode, errorMsg );
  }
}
*/
/*void MBGuiApp::layerSubsetString()
{
  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( activeLayer() );
  if ( !vlayer )
    return;

  if ( !vlayer->vectorJoins().isEmpty() )
  {
    if ( QMessageBox::question( NULL, tr( "Filter on joined fields" ),
                                tr( "You are about to set a subset filter on a layer that has joined fields. "
                                    "Joined fields cannot be filtered, unless you convert the layer to a virtual layer first. "
                                    "Would you like to create a virtual layer out of this layer first?" ),
                                QMessageBox::Yes | QMessageBox::No ) == QMessageBox::Yes )
    {
      QgsVirtualLayerDefinition def = QgsVirtualLayerDefinitionUtils::fromJoinedLayer( vlayer );
      QgsVectorLayer* newLayer = new QgsVectorLayer( def.toString(), vlayer->name() + " (virtual)", "virtual" );
      if ( newLayer->isValid() )
      {
        duplicateVectorStyle( vlayer, newLayer );*/
//        QgsMapLayerRegistry::instance()->addMapLayer( newLayer, /*addToLegend*/ false, /*takeOwnership*/ true );
 /*       QgsLayerTreeUtils::insertLayerBelow( QgsProject::instance()->layerTreeRoot(), vlayer, newLayer );
        mLayerTreeView->setCurrentLayer( newLayer );
        // hide the old layer
        QgsLayerTreeLayer* vLayerTreeLayer = QgsProject::instance()->layerTreeRoot()->findLayer( vlayer->id() );
        if ( vLayerTreeLayer )
          vLayerTreeLayer->setVisible( Qt::Unchecked );
        vlayer = newLayer;
      }
      else
      {
        delete newLayer;
      }
    }
  }

  // launch the query builder
  QgsQueryBuilder *qb = new QgsQueryBuilder( vlayer, this );
  QString subsetBefore = vlayer->subsetString();

  // Set the sql in the query builder to the same in the prop dialog
  // (in case the user has already changed it)
  qb->setSql( vlayer->subsetString() );
  // Open the query builder
  if ( qb->exec() )
  {
    if ( subsetBefore != qb->sql() )
    {
      canvas_xy->refresh();
      if ( mLayerTreeView )
      {
        mLayerTreeView->refreshLayerSymbology( vlayer->id() );
      }
    }
  }

  // delete the query builder object
  delete qb;
}*/

/*void MBGuiApp::saveLastMousePosition( const QgsPoint & p )
{
  if ( mMapTipsVisible )
  {
    // store the point, we need it for when the maptips timer fires
    mLastMapPosition = p;

    // we use this slot to control the timer for maptips since it is fired each time
    // the mouse moves.
    if ( canvas_xy->underMouse() )
    {
      // Clear the maptip (this is done conditionally)
      mpMaptip->clear( canvas_xy );
      // don't start the timer if the mouse is not over the map canvas
      mpMapTipsTimer->start();
      //QgsDebugMsg("Started maptips timer");
    }
  }
}
*/
void MBGuiApp::showScale( double theScale )
{
  // Why has MapCanvas the scale inverted?
  mScaleEdit->setScale( 1.0 / theScale );

  // Not sure if the lines below do anything meaningful /Homann
  if ( mScaleEdit->width() > mScaleEdit->minimumWidth() )
  {
    mScaleEdit->setMinimumWidth( mScaleEdit->width() );
  }
}

/*void MBGuiApp::userScale()
{
  // Why has MapCanvas the scale inverted?
  canvas_xy->zoomScale( 1.0 / mScaleEdit->scale() );
}
*/

/*void MBGuiApp::userRotation()
{
  double degrees = mRotationEdit->value();
  canvas_xy->setRotation( degrees );
  canvas_xy->refresh();
}
*/

// toggle overview status
void MBGuiApp::isInOverview()
{
  mLayerTreeView->defaultActions()->showInOverview();
}

void MBGuiApp::removingLayers( const QStringList& theLayers )
{
  Q_FOREACH ( const QString &layerId, theLayers )
  {
    QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer*>(
                               QgsMapLayerRegistry::instance()->mapLayer( layerId ) );
    if ( !vlayer || !vlayer->isEditable() )
      return;

//    toggleEditing( vlayer, false );
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

void MBGuiApp::removeAllLayers()
{
  QgsMapLayerRegistry::instance()->removeAllMapLayers();
}

void MBGuiApp::removeLayer()
{
  if ( !mLayerTreeView )
  {
    return;
  }

  Q_FOREACH ( QgsMapLayer * layer, mLayerTreeView->selectedLayers() )
  {
    QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer*>( layer );
    if ( vlayer && vlayer->isEditable() )//&& !toggleEditing( vlayer, true ) )
      return;
  }

  QList<QgsLayerTreeNode*> selectedNodes = mLayerTreeView->selectedNodes( true );

  //validate selection
  if ( selectedNodes.isEmpty() )
  {
    messageBar()->pushMessage( tr( "No legend entries selected" ),
                               tr( "Select the layers and groups you want to remove in the legend." ),
                               QgsMessageBar::INFO, messageTimeout() );
    return;
  }

  bool promptConfirmation = QSettings().value( "qgis/askToDeleteLayers", true ).toBool();
  //display a warning
  if ( promptConfirmation && QMessageBox::warning( this, tr( "Remove layers and groups" ), tr( "Remove %n legend entries?", "number of legend items to remove", selectedNodes.count() ), QMessageBox::Ok | QMessageBox::Cancel ) == QMessageBox::Cancel )
  {
    return;
  }

  Q_FOREACH ( QgsLayerTreeNode* node, selectedNodes )
  {
    QgsLayerTreeGroup* parentGroup = qobject_cast<QgsLayerTreeGroup*>( node->parent() );
    if ( parentGroup )
      parentGroup->removeChildNode( node );
  }

  showStatusMessage( tr( "%n legend entries removed.", "number of removed legend entries", selectedNodes.count() ) );

  canvas_xy->refresh();
}

/*void MBGuiApp::duplicateLayers( const QList<QgsMapLayer *>& lyrList )
{
  if ( !mLayerTreeView )
  {
    return;
  }

  const QList<QgsMapLayer *> selectedLyrs = lyrList.empty() ? mLayerTreeView->selectedLayers() : lyrList;
  if ( selectedLyrs.empty() )
  {
    return;
  }

  canvas_xy->freeze();
  QgsMapLayer *dupLayer;
  QString layerDupName, unSppType;
  QList<QgsMessageBarItem *> msgBars;

  Q_FOREACH ( QgsMapLayer * selectedLyr, selectedLyrs )
  {
    dupLayer = nullptr;
    unSppType.clear();
    layerDupName = selectedLyr->name() + ' ' + tr( "copy" );

    if ( selectedLyr->type() == QgsMapLayer::PluginLayer )
    {
      unSppType = tr( "Plugin layer" );
    }

    // duplicate the layer's basic parameters

    if ( unSppType.isEmpty() )
    {
      QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer*>( selectedLyr );
      // TODO: add other layer types that can be duplicated
      // currently memory and plugin layers are skipped
      if ( vlayer && vlayer->storageType() == "Memory storage" )
      {
        unSppType = tr( "Memory layer" );
      }
      else if ( vlayer )
      {
        QgsVectorLayer *dupVLayer = new QgsVectorLayer( vlayer->source(), layerDupName, vlayer->providerType() );
        if ( vlayer->dataProvider() )
        {
          dupVLayer->setProviderEncoding( vlayer->dataProvider()->encoding() );
        }

        //add variables defined in layer properties
        QStringList variableNames = vlayer->customProperty( "variableNames" ).toStringList();
        QStringList variableValues = vlayer->customProperty( "variableValues" ).toStringList();

        int varIndex = 0;
        Q_FOREACH ( const QString& variableName, variableNames )
        {
          if ( varIndex >= variableValues.length() )
          {
            break;
          }

          QVariant varValue = variableValues.at( varIndex );
          varIndex++;
          QgsExpressionContextUtils::setLayerVariable( dupVLayer, variableName, varValue );
        }

        Q_FOREACH ( const QgsVectorJoinInfo& join, vlayer->vectorJoins() )
          dupVLayer->addJoin( join );

        for ( int fld = 0; fld < vlayer->fields().count(); fld++ )
        {
          if ( vlayer->fields().fieldOrigin( fld ) == QgsFields::OriginExpression )
            dupVLayer->addExpressionField( vlayer->expressionField( fld ), vlayer->fields().at( fld ) );
        }

        dupLayer = dupVLayer;
      }
    }

    if ( unSppType.isEmpty() && !dupLayer )
    {
      QgsRasterLayer *rlayer = qobject_cast<QgsRasterLayer*>( selectedLyr );
      if ( rlayer )
      {
        dupLayer = new QgsRasterLayer( rlayer->source(), layerDupName );
      }
    }

    if ( unSppType.isEmpty() && dupLayer && !dupLayer->isValid() )
    {
      msgBars.append( new QgsMessageBarItem(
                        tr( "Duplicate layer: " ),
                        tr( "%1 (duplication resulted in invalid layer)" ).arg( selectedLyr->name() ),
                        QgsMessageBar::WARNING,
                        0,
                        mInfoBar ) );
      continue;
    }

    if ( !unSppType.isEmpty() || !dupLayer )
    {
      msgBars.append( new QgsMessageBarItem(
                        tr( "Duplicate layer: " ),
                        tr( "%1 (%2 type unsupported)" )
                        .arg( selectedLyr->name(),
                              !unSppType.isEmpty() ? QString( "'" ) + unSppType + "' " : "" ),
                        QgsMessageBar::WARNING,
                        0,
                        mInfoBar ) );
      continue;
    }

    // add layer to layer registry and legend
    QList<QgsMapLayer *> myList;
    myList << dupLayer;
    QgsProject::instance()->layerTreeRegistryBridge()->setEnabled( false );
    QgsMapLayerRegistry::instance()->addMapLayers( myList );
    QgsProject::instance()->layerTreeRegistryBridge()->setEnabled( true );

    QgsLayerTreeLayer* nodeSelectedLyr = mLayerTreeView->layerTreeModel()->rootGroup()->findLayer( selectedLyr->id() );
    Q_ASSERT( nodeSelectedLyr );
    Q_ASSERT( QgsLayerTree::isGroup( nodeSelectedLyr->parent() ) );
    QgsLayerTreeGroup* parentGroup = QgsLayerTree::toGroup( nodeSelectedLyr->parent() );

    QgsLayerTreeLayer* nodeDupLayer = parentGroup->insertLayer( parentGroup->children().indexOf( nodeSelectedLyr ) + 1, dupLayer );

    // always set duplicated layers to not visible so layer can be configured before being turned on
    nodeDupLayer->setVisible( Qt::Unchecked );

    // duplicate the layer style
    QString errMsg;
    QDomDocument style;
    selectedLyr->exportNamedStyle( style, errMsg );
    if ( errMsg.isEmpty() )
      dupLayer->importNamedStyle( style, errMsg );
    if ( !errMsg.isEmpty() )
      messageBar()->pushMessage( errMsg,
                                 tr( "Cannot copy style to duplicated layer." ),
                                 QgsMessageBar::CRITICAL, messageTimeout() );
  }

  dupLayer = nullptr;

  canvas_xy->freeze( false );

  // display errors in message bar after duplication of layers
  Q_FOREACH ( QgsMessageBarItem * msgBar, msgBars )
  {
    mInfoBar->pushItem( msgBar );
  }
}*/

/*void MBGuiApp::setLayerScaleVisibility()
{
  if ( !mLayerTreeView )
    return;

  QList<QgsMapLayer*> layers = mLayerTreeView->selectedLayers();

  if ( layers.length() < 1 )
    return;

  QgsScaleVisibilityDialog* dlg = new QgsScaleVisibilityDialog( this, tr( "Set scale visibility for selected layers" ), canvas_xy );
  QgsMapLayer* layer = mLayerTreeView->currentLayer();
  if ( layer )
  {
    dlg->setScaleVisiblity( layer->hasScaleBasedVisibility() );
    dlg->setMinimumScale( 1.0 / layer->maximumScale() );
    dlg->setMaximumScale( 1.0 / layer->minimumScale() );
  }
  if ( dlg->exec() )
  {
    canvas_xy->freeze();
    Q_FOREACH ( QgsMapLayer* layer, layers )
    {
      layer->setScaleBasedVisibility( dlg->hasScaleVisibility() );
      layer->setMinimumScale( 1.0 / dlg->maximumScale() );
      layer->setMaximumScale( 1.0 / dlg->minimumScale() );
    }
    canvas_xy->freeze( false );
    canvas_xy->refresh();
  }
  delete dlg;
}
*/
/*void MBGuiApp::setLayerCRS()
{
  if ( !( mLayerTreeView && mLayerTreeView->currentLayer() ) )
  {
    return;
  }

  QgsGenericProjectionSelector mySelector( this );
  mySelector.setSelectedCrsId( mLayerTreeView->currentLayer()->crs().srsid() );
  mySelector.setMessage();
  if ( !mySelector.exec() )
  {
    QApplication::restoreOverrideCursor();
    return;
  }

  QgsCoordinateReferenceSystem crs( mySelector.selectedCrsId(), QgsCoordinateReferenceSystem::InternalCrsId );

  Q_FOREACH ( QgsLayerTreeNode* node, mLayerTreeView->selectedNodes() )
  {
    if ( QgsLayerTree::isGroup( node ) )
    {
      Q_FOREACH ( QgsLayerTreeLayer* child, QgsLayerTree::toGroup( node )->findLayers() )
      {
        if ( child->layer() )
        {
          child->layer()->setCrs( crs );
          child->layer()->triggerRepaint();
        }
      }
    }
    else if ( QgsLayerTree::isLayer( node ) )
    {
      QgsLayerTreeLayer* nodeLayer = QgsLayerTree::toLayer( node );
      if ( nodeLayer->layer() )
      {
        nodeLayer->layer()->setCrs( crs );
        nodeLayer->layer()->triggerRepaint();
      }
    }
  }

  canvas_xy->refresh();
}
*/
/*void MBGuiApp::setProjectCRSFromLayer()
{
  if ( !( mLayerTreeView && mLayerTreeView->currentLayer() ) )
  {
    return;
  }

  QgsCoordinateReferenceSystem crs = mLayerTreeView->currentLayer()->crs();
  canvas_xy->freeze();
  canvas_xy->setDestinationCrs( crs );
  if ( crs.mapUnits() != QGis::UnknownUnit )
  {
    canvas_xy->setMapUnits( crs.mapUnits() );
  }
  canvas_xy->freeze( false );
  canvas_xy->refresh();
}
*/
void MBGuiApp::legendLayerZoomNative()
{
  if ( !mLayerTreeView )
    return;

  //find current Layer
  QgsMapLayer* currentLayer = mLayerTreeView->currentLayer();
  if ( !currentLayer )
    return;

  QgsRasterLayer *layer =  qobject_cast<QgsRasterLayer *>( currentLayer );
  if ( layer )
  {
    QgsDebugMsg( "Raster units per pixel  : " + QString::number( layer->rasterUnitsPerPixelX() ) );
    QgsDebugMsg( "MapUnitsPerPixel before : " + QString::number( canvas_xy->mapUnitsPerPixel() ) );

    if ( canvas_xy->hasCrsTransformEnabled() )
    {
      // get length of central canvas pixel width in source raster crs
      QgsRectangle e = canvas_xy->extent();
      QSize s = canvas_xy->mapSettings().outputSize();
      QgsPoint p1( e.center().x(), e.center().y() );
      QgsPoint p2( e.center().x() + e.width() / s.width(), e.center().y() + e.height() / s.height() );
      QgsCoordinateTransform ct( canvas_xy->mapSettings().destinationCrs(), layer->crs() );
      p1 = ct.transform( p1 );
      p2 = ct.transform( p2 );
      double width = sqrt( p1.sqrDist( p2 ) ); // width (actually the diagonal) of reprojected pixel
      canvas_xy->zoomByFactor( sqrt( layer->rasterUnitsPerPixelX() * layer->rasterUnitsPerPixelX() + layer->rasterUnitsPerPixelY() * layer->rasterUnitsPerPixelY() ) / width );
    }
    else
    {
      canvas_xy->zoomByFactor( qAbs( layer->rasterUnitsPerPixelX() / canvas_xy->mapUnitsPerPixel() ) );
    }
    canvas_xy->refresh();
    QgsDebugMsg( "MapUnitsPerPixel after  : " + QString::number( canvas_xy->mapUnitsPerPixel() ) );
  }
}

/*void MBGuiApp::legendLayerStretchUsingCurrentExtent()
{
  if ( !mLayerTreeView )
    return;

  //find current Layer
  QgsMapLayer* currentLayer = mLayerTreeView->currentLayer();
  if ( !currentLayer )
    return;

  QgsRasterLayer *layer =  qobject_cast<QgsRasterLayer *>( currentLayer );
  if ( layer )
  {
    QgsContrastEnhancement::ContrastEnhancementAlgorithm contrastEnhancementAlgorithm = QgsContrastEnhancement::StretchToMinimumMaximum;

    QgsRectangle myRectangle;
    myRectangle = canvas_xy->mapSettings().outputExtentToLayerExtent( layer, canvas_xy->extent() );
    layer->setContrastEnhancement( contrastEnhancementAlgorithm, QgsRaster::ContrastEnhancementMinMax, myRectangle );

    mLayerTreeView->refreshLayerSymbology( layer->id() );
    canvas_xy->refresh();
  }
}
*/
/*void MBGuiApp::applyStyleToGroup()
{
  if ( !mLayerTreeView )
    return;

  Q_FOREACH ( QgsLayerTreeNode* node, mLayerTreeView->selectedNodes() )
  {
    if ( QgsLayerTree::isGroup( node ) )
    {
      Q_FOREACH ( QgsLayerTreeLayer* nodeLayer, QgsLayerTree::toGroup( node )->findLayers() )
      {
        if ( nodeLayer->layer() )
        {
          pasteStyle( nodeLayer->layer() );
        }
      }
    }
    else if ( QgsLayerTree::isLayer( node ) )
    {
      QgsLayerTreeLayer* nodeLayer = QgsLayerTree::toLayer( node );
      if ( nodeLayer->layer() )
      {
        pasteStyle( nodeLayer->layer() );
      }
    }
  }
}
*/
/*void MBGuiApp::legendGroupSetCRS()
{
  if ( !canvas_xy )
  {
    return;
  }

  QgsLayerTreeGroup* currentGroup = mLayerTreeView->currentGroupNode();
  if ( !currentGroup )
    return;

  QgsGenericProjectionSelector mySelector( this );
  mySelector.setMessage();
  if ( !mySelector.exec() )
  {
    QApplication::restoreOverrideCursor();
    return;
  }

  QgsCoordinateReferenceSystem crs( mySelector.selectedCrsId(), QgsCoordinateReferenceSystem::InternalCrsId );
  Q_FOREACH ( QgsLayerTreeLayer* nodeLayer, currentGroup->findLayers() )
  {
    if ( nodeLayer->layer() )
    {
      nodeLayer->layer()->setCrs( crs );
      nodeLayer->layer()->triggerRepaint();
    }
  }
}
*/
/*void MBGuiApp::legendGroupSetWMSData()
{
  QgsLayerTreeGroup* currentGroup = mLayerTreeView->currentGroupNode();
  if ( !currentGroup )
    return;
  QgsGroupWMSDataDialog* dlg = new QgsGroupWMSDataDialog( this );
  dlg->setGroupShortName( currentGroup->customProperty( "wmsShortName" ).toString() );
  dlg->setGroupTitle( currentGroup->customProperty( "wmsTitle" ).toString() );
  dlg->setGroupTitle( currentGroup->customProperty( "wmsAbstract" ).toString() );
  if ( dlg->exec() )
  {
    currentGroup->setCustomProperty( "wmsShortName", dlg->groupShortName() );
    currentGroup->setCustomProperty( "wmsTitle", dlg->groupTitle() );
    currentGroup->setCustomProperty( "wmsAbstract", dlg->groupAbstract() );
  }
  delete dlg;
}
*/
void MBGuiApp::zoomToLayerExtent()
{
  mLayerTreeView->defaultActions()->zoomToLayer( canvas_xy );
}

/*void MBGuiApp::showPluginManager()
{

  if ( mPythonUtils && mPythonUtils->isEnabled() )
  {
    // Call pluginManagerInterface()->showPluginManager() as soon as the plugin installer says the remote data is fetched.
    QgsPythonRunner::run( "pyplugin_installer.instance().showPluginManagerWhenReady()" );
  }
  else
  {
    // Call the pluginManagerInterface directly
    mQgisInterface->pluginManagerInterface()->showPluginManager();
  }
}
*/
// implementation of the python runner
/*class QgsPythonRunnerImpl : public QgsPythonRunner
{
  public:
    explicit QgsPythonRunnerImpl( QgsPythonUtils* pythonUtils ) : mPythonUtils( pythonUtils ) {}

    virtual bool runCommand( QString command, QString messageOnError = QString() ) override
    {
      if ( mPythonUtils && mPythonUtils->isEnabled() )
      {
        return mPythonUtils->runString( command, messageOnError, false );
      }
      return false;
    }

    virtual bool evalCommand( QString command, QString &result ) override
    {
      if ( mPythonUtils && mPythonUtils->isEnabled() )
      {
        return mPythonUtils->evalString( command, result );
      }
      return false;
    }

  protected:
    QgsPythonUtils* mPythonUtils;
};*/

/*void MBGuiApp::loadPythonSupport()
{
  QString pythonlibName( "qgispython" );
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
  pythonlibName.prepend( QgsApplication::libraryPath() );
#endif
#ifdef __MINGW32__
  pythonlibName.prepend( "lib" );
#endif
  QString version = QString( "%1.%2.%3" ).arg( QGis::QGIS_VERSION_INT / 10000 ).arg( QGis::QGIS_VERSION_INT / 100 % 100 ).arg( QGis::QGIS_VERSION_INT % 100 );
  QgsDebugMsg( QString( "load library %1 (%2)" ).arg( pythonlibName, version ) );
  QLibrary pythonlib( pythonlibName, version );
  // It's necessary to set these two load hints, otherwise Python library won't work correctly
  // see http://lists.kde.org/?l=pykde&m=117190116820758&w=2
  pythonlib.setLoadHints( QLibrary::ResolveAllSymbolsHint | QLibrary::ExportExternalSymbolsHint );
  if ( !pythonlib.load() )
  {
    pythonlib.setFileName( pythonlibName );
    if ( !pythonlib.load() )
    {
      QgsMessageLog::logMessage( tr( "Couldn't load Python support library: %1" ).arg( pythonlib.errorString() ) );
      return;
    }
  }

  //QgsDebugMsg("Python support library loaded successfully.");
  typedef QgsPythonUtils*( *inst )();
  inst pythonlib_inst = reinterpret_cast< inst >( cast_to_fptr( pythonlib.resolve( "instance" ) ) );
  if ( !pythonlib_inst )
  {
    //using stderr on purpose because we want end users to see this [TS]
    QgsMessageLog::logMessage( tr( "Couldn't resolve python support library's instance() symbol." ) );
    return;
  }

  //QgsDebugMsg("Python support library's instance() symbol resolved.");
  mPythonUtils = pythonlib_inst();
  if ( mPythonUtils )
  {
    mPythonUtils->initPython( mQgisInterface );
  }

  if ( mPythonUtils && mPythonUtils->isEnabled() )
  {
    QgsPluginRegistry::instance()->setPythonUtils( mPythonUtils );

    // init python runner
    QgsPythonRunner::setInstance( new QgsPythonRunnerImpl( mPythonUtils ) );

    QgsMessageLog::logMessage( tr( "Python support ENABLED :-) " ), QString::null, QgsMessageLog::INFO );
  }
}
*/
/*void MBGuiApp::checkQgisVersion()
{
  QgsVersionInfo* versionInfo = new QgsVersionInfo();
  QApplication::setOverrideCursor( Qt::WaitCursor );

  connect( versionInfo, SIGNAL( versionInfoAvailable() ), this, SLOT( versionReplyFinished() ) );
  versionInfo->checkVersion();
}*/

/*void MBGuiApp::versionReplyFinished()
{
  QApplication::restoreOverrideCursor();

  QgsVersionInfo* versionInfo = qobject_cast<QgsVersionInfo*>( sender() );
  Q_ASSERT( versionInfo );

  if ( versionInfo->error() == QNetworkReply::NoError )
  {
    QString info;

    if ( versionInfo->newVersionAvailable() )
    {
      info = tr( "There is a new version of QGIS available" );
    }
    else if ( versionInfo->isDevelopmentVersion() )
    {
      info = tr( "You are running a development version of QGIS" );
    }
    else
    {
      info = tr( "You are running the current version of QGIS" );
    }

    info = QString( "<b>%1</b>" ).arg( info );

    info += "<br>" + versionInfo->downloadInfo();

    QMessageBox mb( QMessageBox::Information, tr( "QGIS Version Information" ), info );
    mb.setInformativeText( versionInfo->html() );
    mb.exec();
  }
  else
  {
    QMessageBox mb( QMessageBox::Warning, tr( "QGIS Version Information" ), tr( "Unable to get current version information from server" ) );
    mb.setDetailedText( versionInfo->errorString() );
    mb.exec();
  }
}
*/
/*void MBGuiApp::configureShortcuts()
{
  QgsConfigureShortcutsDialog dlg;
  dlg.exec();
}
*/
/*void MBGuiApp::customize()
{
  QgsCustomization::instance()->openDialog( this );
}
*/
/*void MBGuiApp::options()
{
  showOptionsDialog( this );
}
*/
/*void MBGuiApp::showOptionsDialog( QWidget *parent, const QString& currentPage )
{
  QSettings mySettings;
  QString oldScales = mySettings.value( "Map/scales", PROJECT_SCALES ).toString();

  bool oldCapitalise = mySettings.value( "/qgis/capitaliseLayerName", QVariant( false ) ).toBool();

  QgsOptions *optionsDialog = new QgsOptions( parent );
  if ( !currentPage.isEmpty() )
  {
    optionsDialog->setCurrentPage( currentPage );
  }

  if ( optionsDialog->exec() )
  {
    QgsProject::instance()->layerTreeRegistryBridge()->setNewLayersVisible( mySettings.value( "/qgis/new_layers_visible", true ).toBool() );

    setupLayerTreeViewFromSettings();

    canvas_xy->enableAntiAliasing( mySettings.value( "/qgis/enable_anti_aliasing" ).toBool() );

    int action = mySettings.value( "/qgis/wheel_action", 2 ).toInt();
    double zoomFactor = mySettings.value( "/qgis/zoom_factor", 2 ).toDouble();
    canvas_xy->setWheelAction( static_cast< QgsMapCanvas::WheelAction >( action ), zoomFactor );

    canvas_xy->setCachingEnabled( mySettings.value( "/qgis/enable_render_caching", true ).toBool() );

    canvas_xy->setParallelRenderingEnabled( mySettings.value( "/qgis/parallel_rendering", false ).toBool() );

    canvas_xy->setMapUpdateInterval( mySettings.value( "/qgis/map_update_interval", 250 ).toInt() );

    if ( oldCapitalise != mySettings.value( "/qgis/capitaliseLayerName", QVariant( false ) ).toBool() )
    {
      // if the layer capitalization has changed, we need to update all layer names
      Q_FOREACH ( QgsMapLayer* layer, QgsMapLayerRegistry::instance()->mapLayers() )
        layer->setLayerName( layer->originalName() );
    }

    //update any open compositions so they reflect new composer settings
    //we have to push the changes to the compositions here, because compositions
    //have no access to MBGuiApp and accordingly can't listen in to changes
    QSet<QgsComposer*> composers = instance()->printComposers();
    QSet<QgsComposer*>::iterator composer_it = composers.begin();
    for ( ; composer_it != composers.end(); ++composer_it )
    {
      QgsComposition* composition = ( *composer_it )->composition();
      composition->updateSettings();
    }

    //do we need this? TS
    canvas_xy->refresh();

    mRasterFileFilter = QgsProviderRegistry::instance()->fileRasterFilters();

    if ( oldScales != mySettings.value( "Map/scales", PROJECT_SCALES ).toString() )
    {
      mScaleEdit->updateScales();
    }

    qobject_cast<QgsMeasureTool*>( mMapTools.mMeasureDist )->updateSettings();
    qobject_cast<QgsMeasureTool*>( mMapTools.mMeasureArea )->updateSettings();
    qobject_cast<QgsMapToolMeasureAngle*>( mMapTools.mMeasureAngle )->updateSettings();

    bool otfTransformAutoEnable = mySettings.value( "/Projections/otfTransformAutoEnable", true ).toBool();
    mLayerTreeCanvasBridge->setAutoEnableCrsTransform( otfTransformAutoEnable );
  }

  delete optionsDialog;
}
*/
void MBGuiApp::fullHistogramStretch()
{
  histogramStretch( false, QgsRaster::ContrastEnhancementMinMax );
}

void MBGuiApp::localHistogramStretch()
{
  histogramStretch( true, QgsRaster::ContrastEnhancementMinMax );
}

void MBGuiApp::fullCumulativeCutStretch()
{
  histogramStretch( false, QgsRaster::ContrastEnhancementCumulativeCut );
}

void MBGuiApp::localCumulativeCutStretch()
{
  histogramStretch( true, QgsRaster::ContrastEnhancementCumulativeCut );
}

void MBGuiApp::histogramStretch( bool visibleAreaOnly, QgsRaster::ContrastEnhancementLimits theLimits )
{
  QgsMapLayer * myLayer = mLayerTreeView->currentLayer();

  if ( !myLayer )
  {
    messageBar()->pushMessage( tr( "No Layer Selected" ),
                               tr( "To perform a full histogram stretch, you need to have a raster layer selected." ),
                               QgsMessageBar::INFO, messageTimeout() );
    return;
  }

  QgsRasterLayer* myRasterLayer = qobject_cast<QgsRasterLayer *>( myLayer );
  if ( !myRasterLayer )
  {
    messageBar()->pushMessage( tr( "No Layer Selected" ),
                               tr( "To perform a full histogram stretch, you need to have a raster layer selected." ),
                               QgsMessageBar::INFO, messageTimeout() );
    return;
  }

  QgsRectangle myRectangle;
  if ( visibleAreaOnly )
    myRectangle = canvas_xy->mapSettings().outputExtentToLayerExtent( myRasterLayer, canvas_xy->extent() );

  myRasterLayer->setContrastEnhancement( QgsContrastEnhancement::StretchToMinimumMaximum, theLimits, myRectangle );

  canvas_xy->refresh();
}

void MBGuiApp::increaseBrightness()
{
  int step = 1;
  if ( QgsApplication::keyboardModifiers() == Qt::ShiftModifier )
  {
    step = 10;
  }
  adjustBrightnessContrast( step );
}

void MBGuiApp::decreaseBrightness()
{
  int step = -1;
  if ( QgsApplication::keyboardModifiers() == Qt::ShiftModifier )
  {
    step = -10;
  }
  adjustBrightnessContrast( step );
}

void MBGuiApp::increaseContrast()
{
  int step = 1;
  if ( QgsApplication::keyboardModifiers() == Qt::ShiftModifier )
  {
    step = 10;
  }
  adjustBrightnessContrast( step, false );
}

void MBGuiApp::decreaseContrast()
{
  int step = -1;
  if ( QgsApplication::keyboardModifiers() == Qt::ShiftModifier )
  {
    step = -10;
  }
  adjustBrightnessContrast( step, false );
}

void MBGuiApp::adjustBrightnessContrast( int delta, bool updateBrightness )
{
  Q_FOREACH ( QgsMapLayer * layer, mLayerTreeView->selectedLayers() )
  {
    if ( !layer )
    {
      messageBar()->pushMessage( tr( "No Layer Selected" ),
                                 tr( "To change brightness or contrast, you need to have a raster layer selected." ),
                                 QgsMessageBar::INFO, messageTimeout() );
      return;
    }

    QgsRasterLayer* rasterLayer = qobject_cast<QgsRasterLayer *>( layer );
    if ( !rasterLayer )
    {
      messageBar()->pushMessage( tr( "No Layer Selected" ),
                                 tr( "To change brightness or contrast, you need to have a raster layer selected." ),
                                 QgsMessageBar::INFO, messageTimeout() );
      return;
    }

    QgsBrightnessContrastFilter* brightnessFilter = rasterLayer->brightnessFilter();

    if ( updateBrightness )
    {
      brightnessFilter->setBrightness( brightnessFilter->brightness() + delta );
    }
    else
    {
      brightnessFilter->setContrast( brightnessFilter->contrast() + delta );
    }

    rasterLayer->triggerRepaint();
  }
}

/*void MBGuiApp::helpContents()
{
  // We should really ship the HTML version of the docs local too.
  openURL( QString( "https://docs.qgis.org/%1.%2/%3/docs/user_manual/" )
           .arg( QGis::QGIS_VERSION_INT / 10000 )
           .arg( QGis::QGIS_VERSION_INT / 100 % 100 )
           .arg( tr( "en", "documentation language" ) ),
           false );
}
*/
/*void MBGuiApp::apiDocumentation()
{
  if ( QFileInfo( QgsApplication::pkgDataPath() + "/doc/api/index.html" ).exists() )
  {
    openURL( "api/index.html" );
  }
  else
  {
    openURL( "https://qgis.org/api/", false );
  }
}
*/
/*void MBGuiApp::reportaBug()
{
  openURL( tr( "https://qgis.org/en/site/getinvolved/development/bugreporting.html" ), false );
}
*/
/*void MBGuiApp::supportProviders()
{
  openURL( tr( "https://qgis.org/en/site/forusers/commercial_support.html" ), false );
}
*/
/*void MBGuiApp::helpQgisHomePage()
{
  openURL( "https://qgis.org", false );
}*/

/*void MBGuiApp::openURL( QString url, bool useQgisDocDirectory )
{
  // open help in user browser
  if ( useQgisDocDirectory )
  {
    url = "file://" + QgsApplication::pkgDataPath() + "/doc/" + url;
  }
#ifdef Q_OS_MACX
  // Use Mac OS X Launch Services which uses the user's default browser
  // and will just open a new window if that browser is already running.
  // QProcess creates a new browser process for each invocation and expects a
  // commandline application rather than a bundled application.
  CFURLRef urlRef = CFURLCreateWithBytes( kCFAllocatorDefault,
                                          reinterpret_cast<const UInt8*>( url.toUtf8().data() ), url.length(),
                                          kCFStringEncodingUTF8, nullptr );
  OSStatus status = LSOpenCFURLRef( urlRef, nullptr );
  status = 0; //avoid compiler warning
  CFRelease( urlRef );
#elif defined(Q_OS_WIN)
  if ( url.startsWith( "file://", Qt::CaseInsensitive ) )
    ShellExecute( 0, 0, url.mid( 7 ).toLocal8Bit().constData(), 0, 0, SW_SHOWNORMAL );
  else
    QDesktopServices::openUrl( url );
#else
  QDesktopServices::openUrl( url );
#endif
}*/

/** Get a pointer to the currently selected map layer */
QgsMapLayer *MBGuiApp::activeLayer_xy()
{
  return mLayerTreeView ? mLayerTreeView->currentLayer() : nullptr;
}
QgsMapLayer *MBGuiApp::activeLayer_yz()
{
  return mLayerTreeView_yz ? mLayerTreeView_yz->currentLayer() : nullptr;
}
/** Set the current layer */
bool MBGuiApp::setActiveLayer_xy( QgsMapLayer *layer )
{
  if ( !layer )
    return false;

  if ( !mLayerTreeView->layerTreeModel()->rootGroup()->findLayer( layer->id() ) )
    return false;

  mLayerTreeView->setCurrentLayer( layer );
  return true;
}
bool MBGuiApp::setActiveLayer_yz( QgsMapLayer *layer )
{
  if ( !layer )
    return false;

  if ( !mLayerTreeView_yz->layerTreeModel()->rootGroup()->findLayer( layer->id() ) )
    return false;

  mLayerTreeView_yz->setCurrentLayer( layer );
  return true;
}
/** Add a vector layer directly without prompting user for location
  The caller must provide information compatible with the provider plugin
  using the vectorLayerPath and baseName. The provider can use these
  parameters in any way necessary to initialize the layer. The baseName
  parameter is used in the Map Legend so it should be formed in a meaningful
  way.
  */
QgsVectorLayer* MBGuiApp::addVectorLayer( const QString& vectorLayerPath, const QString& baseName, const QString& providerKey )
{
  bool wasfrozen = canvas_xy->isFrozen();

  canvas_xy->freeze();

// Let render() do its own cursor management
//  QApplication::setOverrideCursor(Qt::WaitCursor);

  /* Eliminate the need to instantiate the layer based on provider type.
     The caller is responsible for cobbling together the needed information to
     open the layer
     */
  QgsDebugMsg( "Creating new vector layer using " + vectorLayerPath
               + " with baseName of " + baseName
               + " and providerKey of " + providerKey );

//  // if the layer needs authentication, ensure the master password is set
  bool authok = true;
//  QRegExp rx( "authcfg=([a-z]|[A-Z]|[0-9]){7}" );
//  if ( rx.indexIn( vectorLayerPath ) != -1 )
//  {
//    authok = false;
//    if ( !QgsAuthGuiUtils::isDisabled( messageBar(), messageTimeout() ) )
//    {
//      authok = QgsAuthManager::instance()->setMasterPassword( true );
//    }
//  }

  // create the layer
  QgsVectorLayer *layer = new QgsVectorLayer( vectorLayerPath, baseName, providerKey, false );

  if ( authok && layer && layer->isValid() )
  {
    QStringList sublayers = layer->dataProvider()->subLayers();
    QgsDebugMsg( QString( "got valid layer with %1 sublayers" ).arg( sublayers.count() ) );

    // If the newly created layer has more than 1 layer of data available, we show the
    // sublayers selection dialog so the user can select the sublayers to actually load.
    if ( sublayers.count() > 1 )
    {
      askUserForOGRSublayers( layer );

      // The first layer loaded is not useful in that case. The user can select it in
      // the list if he wants to load it.
      delete layer;
      layer = nullptr;
    }
    else
    {
      // Register this layer with the layers registry
      QList<QgsMapLayer *> myList;
      myList << layer;
      QgsMapLayerRegistry::instance()->addMapLayers( myList );
      bool ok;
      layer->loadDefaultStyle( ok );
    }
  }
  else
  {
    QString msg = tr( "The layer %1 is not a valid layer and can not be added to the map" ).arg( vectorLayerPath );
    messageBar()->pushMessage( tr( "Layer is not valid" ), msg, QgsMessageBar::CRITICAL, messageTimeout() );

    delete layer;
    canvas_xy->freeze( false );
    return nullptr;
  }

  // Only update the map if we frozen in this method
  // Let the caller do it otherwise
  if ( !wasfrozen )
  {
    canvas_xy->freeze( false );
    canvas_xy->refresh();
  }

// Let render() do its own cursor management
//  QApplication::restoreOverrideCursor();

  return layer;

}

void MBGuiApp::addMapLayer( QgsMapLayer *theMapLayer )
{
  canvas_xy->freeze();

// Let render() do its own cursor management
//  QApplication::setOverrideCursor(Qt::WaitCursor);

  if ( theMapLayer->isValid() )
  {
    // Register this layer with the layers registry
    QList<QgsMapLayer *> myList;
    myList << theMapLayer;
    QgsMapLayerRegistry::instance()->addMapLayers( myList );
    // add it to the mapcanvas collection
    // not necessary since adding to registry adds to canvas canvas_xy->addLayer(theMapLayer);
  }
  else
  {
    QString msg = tr( "The layer is not a valid layer and can not be added to the map" );
    messageBar()->pushMessage( tr( "Layer is not valid" ), msg, QgsMessageBar::CRITICAL, messageTimeout() );
  }

  // draw the map
  canvas_xy->freeze( false );
  canvas_xy->refresh();

// Let render() do its own cursor management
//  QApplication::restoreOverrideCursor();

}
/*void MBGuiApp::embedLayers()
{
  //dialog to select groups/layers from other project files
  QgsProjectLayerGroupDialog d( this );
  if ( d.exec() == QDialog::Accepted && d.isValid() )
  {
    canvas_xy->freeze( true );

    QString projectFile = d.selectedProjectFile();

    //groups
    QStringList groups = d.selectedGroups();
    QStringList::const_iterator groupIt = groups.constBegin();
    for ( ; groupIt != groups.constEnd(); ++groupIt )
    {
      QgsLayerTreeGroup* newGroup = QgsProject::instance()->createEmbeddedGroup( *groupIt, projectFile, QStringList() );

      if ( newGroup )
        QgsProject::instance()->layerTreeRoot()->addChildNode( newGroup );
    }

    //layer ids
    QList<QDomNode> brokenNodes;
    QList< QPair< QgsVectorLayer*, QDomElement > > vectorLayerList;

    // resolve dependencies
    QgsLayerDefinition::DependencySorter depSorter( projectFile );
    QStringList sortedIds = depSorter.sortedLayerIds();
    QStringList layerIds = d.selectedLayerIds();
    Q_FOREACH ( const QString& id, sortedIds )
    {
      Q_FOREACH ( const QString& selId, layerIds )
      {
        if ( selId == id )
          QgsProject::instance()->createEmbeddedLayer( selId, projectFile, brokenNodes, vectorLayerList );
      }
    }

    canvas_xy->freeze( false );
    if ( !groups.isEmpty() || !layerIds.isEmpty() )
    {
      canvas_xy->refresh();
    }
  }
}
*/
void MBGuiApp::setExtent( const QgsRectangle& theRect )
{
  if(!checkBox_lock_canvas_xy->isChecked())
      canvas_xy->setExtent(theRect);
  if(!checkBox_lock_canvas_yz->isChecked())
      canvas_yz->setExtent(theRect);
  if(!checkBox_lock_canvas_depths->isChecked())
      canvas_depths->setExtent(theRect);
  if(!checkBox_lock_canvas_error->isChecked())
      canvas_error->setExtent(theRect);
  if(!checkBox_lock_canvas_nei->isChecked())
      canvas_nei->setExtent(theRect);
  if(!checkBox_lock_canvas_rei->isChecked())
      canvas_rei->setExtent(theRect);
  if(!checkBox_lock_canvas_mean->isChecked())
      canvas_mean->setExtent(theRect);
}

/**
  Prompt and save if project has been modified.
  @return true if saved or discarded, false if cancelled
 */
bool MBGuiApp::saveDirty()
{
  QString whyDirty = "";
  bool hasUnsavedEdits = false;
  // extra check to see if there are any vector layers with unsaved provider edits
  // to ensure user has opportunity to save any editing
  if ( QgsMapLayerRegistry::instance()->count() > 0 )
  {
    QMap<QString, QgsMapLayer*> layers = QgsMapLayerRegistry::instance()->mapLayers();
    for ( QMap<QString, QgsMapLayer*>::iterator it = layers.begin(); it != layers.end(); ++it )
    {
      QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( it.value() );
      if ( !vl )
      {
        continue;
      }

      hasUnsavedEdits = ( vl->isEditable() && vl->isModified() );
      if ( hasUnsavedEdits )
      {
        break;
      }
    }

    if ( hasUnsavedEdits )
    {
      markDirty();
      whyDirty = "<p style='color:darkred;'>";
      whyDirty += tr( "Project has layer(s) in edit mode with unsaved edits, which will NOT be saved!" );
      whyDirty += "</p>";
    }
  }

  QMessageBox::StandardButton answer( QMessageBox::Discard );
  canvas_xy->freeze( true );

  //QgsDebugMsg(QString("Layer count is %1").arg(canvas_xy->layerCount()));
  //QgsDebugMsg(QString("Project is %1dirty").arg( QgsProject::instance()->isDirty() ? "" : "not "));
  //QgsDebugMsg(QString("Map canvas is %1dirty").arg(canvas_xy->isDirty() ? "" : "not "));

  QSettings settings;
  bool askThem = settings.value( "qgis/askToSaveProjectChanges", true ).toBool();

  if ( askThem && QgsProject::instance()->isDirty() && QgsMapLayerRegistry::instance()->count() > 0 )
  {
    // flag project as dirty since dirty state of canvas is reset if "dirty"
    // is based on a zoom or pan
    markDirty();

    // old code: mProjectIsDirtyFlag = true;

    // prompt user to save
    answer = QMessageBox::information( this, tr( "Save?" ),
                                       tr( "Do you want to save the current project? %1" )
                                       .arg( whyDirty ),
                                       QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Discard,
                                       hasUnsavedEdits ? QMessageBox::Cancel : QMessageBox::Save );
    if ( QMessageBox::Save == answer )
    {
      if ( !fileSave() )
        answer = QMessageBox::Cancel;
    }
  }

  canvas_xy->freeze( false );

  return answer != QMessageBox::Cancel;
}

/*void MBGuiApp::closeProject()
{
  // unload the project macros before changing anything
  if ( mTrustedMacros )
  {
    QgsPythonRunner::run( "qgis.utils.unloadProjectMacros();" );
  }

  mTrustedMacros = false;

  mLegendExpressionFilterButton->setExpressionText( "" );
  mLegendExpressionFilterButton->setChecked( false );
  mActionFilterLegend->setChecked( false );

  deletePrintComposers();
  removeAnnotationItems();
  // clear out any stuff from project
  canvas_xy->freeze( true );
  QList<QgsMapCanvasLayer> emptyList;
  canvas_xy->setLayerSet( emptyList );
  canvas_xy->clearCache();
  removeAllLayers();
}*/

void MBGuiApp::changeEvent( QEvent* event )
{
  QMainWindow::changeEvent( event );
#ifdef Q_OS_MAC
  switch ( event->type() )
  {
    case QEvent::ActivationChange:
      if ( QApplication::activeWindow() == this )
      {
        mWindowAction->setChecked( true );
      }
      // this should not be necessary since the action is part of an action group
      // however this check is not cleared if PrintComposer is closed and reopened
      else
      {
        mWindowAction->setChecked( false );
      }
      break;

    case QEvent::WindowTitleChange:
      mWindowAction->setText( windowTitle() );
      break;

    default:
      break;
  }
#endif
}

/*void MBGuiApp::closeEvent( QCloseEvent* event )
{
  // We'll close in our own good time, thank you very much
  event->ignore();
  // Do the usual checks and ask if they want to save, etc
  fileExit();
}*/

void MBGuiApp::whatsThis()
{
  QWhatsThis::enterWhatsThisMode();
}

/*QMenu* MBGuiApp::getPluginMenu( const QString& menuName )
{
  // Plugin menu items are below the plugin separator (which may not exist yet
   // if no plugins are loaded) and above the python separator. If python is not
   // present, there is no python separator and the plugin list is at the bottom
   // of the menu.
   //

  QString cleanedMenuName = menuName;
#ifdef Q_OS_MAC
  // Mac doesn't have '&' keyboard shortcuts.
  cleanedMenuName.remove( QChar( '&' ) );
#endif
  QAction *before = mActionPluginSeparator2;  // python separator or end of list
  if ( !mActionPluginSeparator1 )
  {
    // First plugin - create plugin list separator
    mActionPluginSeparator1 = mPluginMenu->insertSeparator( before );
  }
  else
  {
    QString dst = cleanedMenuName;
    dst.remove( QChar( '&' ) );

    // Plugins exist - search between plugin separator and python separator or end of list
    QList<QAction*> actions = mPluginMenu->actions();
    int end = mActionPluginSeparator2 ? actions.indexOf( mActionPluginSeparator2 ) : actions.count();
    for ( int i = actions.indexOf( mActionPluginSeparator1 ) + 1; i < end; i++ )
    {
      QString src = actions.at( i )->text();
      src.remove( QChar( '&' ) );

      int comp = dst.localeAwareCompare( src );
      if ( comp < 0 )
      {
        // Add item before this one
        before = actions.at( i );
        break;
      }
      else if ( comp == 0 )
      {
        // Plugin menu item already exists
        return actions.at( i )->menu();
      }
    }
  }
  // It doesn't exist, so create
  QMenu *menu = new QMenu( cleanedMenuName, this );
  menu->setObjectName( normalizedMenuName( cleanedMenuName ) );
  // Where to put it? - we worked that out above...
  mPluginMenu->insertMenu( before, menu );

  return menu;
}*/
/*void MBGuiApp::addPluginToMenu( const QString& name, QAction* action )
{
  QMenu* menu = getPluginMenu( name );
  menu->addAction( action );
}
*/
/*void MBGuiApp::removePluginMenu( const QString& name, QAction* action )
{
  QMenu* menu = getPluginMenu( name );
  menu->removeAction( action );
  if ( menu->actions().isEmpty() )
  {
    mPluginMenu->removeAction( menu->menuAction() );
  }
  // Remove separator above plugins in Plugin menu if no plugins remain
  QList<QAction*> actions = mPluginMenu->actions();
  int end = mActionPluginSeparator2 ? actions.indexOf( mActionPluginSeparator2 ) : actions.count();
  if ( actions.indexOf( mActionPluginSeparator1 ) + 1 == end )
  {
    mPluginMenu->removeAction( mActionPluginSeparator1 );
    mActionPluginSeparator1 = nullptr;
  }
}
*/
/*QMenu* MBGuiApp::getDatabaseMenu( const QString& menuName )
{
  QString cleanedMenuName = menuName;
#ifdef Q_OS_MAC
  // Mac doesn't have '&' keyboard shortcuts.
  cleanedMenuName.remove( QChar( '&' ) );
#endif
  QString dst = cleanedMenuName;
  dst.remove( QChar( '&' ) );

  QAction *before = nullptr;
  QList<QAction*> actions = mDatabaseMenu->actions();
  for ( int i = 0; i < actions.count(); i++ )
  {
    QString src = actions.at( i )->text();
    src.remove( QChar( '&' ) );

    int comp = dst.localeAwareCompare( src );
    if ( comp < 0 )
    {
      // Add item before this one
      before = actions.at( i );
      break;
    }
    else if ( comp == 0 )
    {
      // Plugin menu item already exists
      return actions.at( i )->menu();
    }
  }
  // It doesn't exist, so create
  QMenu *menu = new QMenu( cleanedMenuName, this );
  menu->setObjectName( normalizedMenuName( cleanedMenuName ) );
  if ( before )
    mDatabaseMenu->insertMenu( before, menu );
  else
    mDatabaseMenu->addMenu( menu );

  return menu;
}*/

QMenu* MBGuiApp::getRasterMenu( const QString& menuName )
{
  QString cleanedMenuName = menuName;
#ifdef Q_OS_MAC
  // Mac doesn't have '&' keyboard shortcuts.
  cleanedMenuName.remove( QChar( '&' ) );
#endif

  QAction *before = nullptr;
  if ( !mActionRasterSeparator )
  {
    // First plugin - create plugin list separator
    mActionRasterSeparator = mRasterMenu->insertSeparator( before );
  }
  else
  {
    QString dst = cleanedMenuName;
    dst.remove( QChar( '&' ) );
    // Plugins exist - search between plugin separator and python separator or end of list
    QList<QAction*> actions = mRasterMenu->actions();
    for ( int i = actions.indexOf( mActionRasterSeparator ) + 1; i < actions.count(); i++ )
    {
      QString src = actions.at( i )->text();
      src.remove( QChar( '&' ) );

      int comp = dst.localeAwareCompare( src );
      if ( comp < 0 )
      {
        // Add item before this one
        before = actions.at( i );
        break;
      }
      else if ( comp == 0 )
      {
        // Plugin menu item already exists
        return actions.at( i )->menu();
      }
    }
  }

  // It doesn't exist, so create
  QMenu *menu = new QMenu( cleanedMenuName, this );
  menu->setObjectName( normalizedMenuName( cleanedMenuName ) );
  if ( before )
    mRasterMenu->insertMenu( before, menu );
  else
    mRasterMenu->addMenu( menu );

  return menu;
}

QMenu* MBGuiApp::getVectorMenu( const QString& menuName )
{
  QString cleanedMenuName = menuName;
#ifdef Q_OS_MAC
  // Mac doesn't have '&' keyboard shortcuts.
  cleanedMenuName.remove( QChar( '&' ) );
#endif
  QString dst = cleanedMenuName;
  dst.remove( QChar( '&' ) );

  QAction *before = nullptr;
  QList<QAction*> actions = mVectorMenu->actions();
  for ( int i = 0; i < actions.count(); i++ )
  {
    QString src = actions.at( i )->text();
    src.remove( QChar( '&' ) );

    int comp = dst.localeAwareCompare( src );
    if ( comp < 0 )
    {
      // Add item before this one
      before = actions.at( i );
      break;
    }
    else if ( comp == 0 )
    {
      // Plugin menu item already exists
      return actions.at( i )->menu();
    }
  }
  // It doesn't exist, so create
  QMenu *menu = new QMenu( cleanedMenuName, this );
  menu->setObjectName( normalizedMenuName( cleanedMenuName ) );
  if ( before )
    mVectorMenu->insertMenu( before, menu );
  else
    mVectorMenu->addMenu( menu );

  return menu;
}
/*QMenu* MBGuiApp::getWebMenu( const QString& menuName )
{
  QString cleanedMenuName = menuName;
#ifdef Q_OS_MAC
  // Mac doesn't have '&' keyboard shortcuts.
  cleanedMenuName.remove( QChar( '&' ) );
#endif
  QString dst = cleanedMenuName;
  dst.remove( QChar( '&' ) );

  QAction *before = nullptr;
  QList<QAction*> actions = mWebMenu->actions();
  for ( int i = 0; i < actions.count(); i++ )
  {
    QString src = actions.at( i )->text();
    src.remove( QChar( '&' ) );

    int comp = dst.localeAwareCompare( src );
    if ( comp < 0 )
    {
      // Add item before this one
      before = actions.at( i );
      break;
    }
    else if ( comp == 0 )
    {
      // Plugin menu item already exists
      return actions.at( i )->menu();
    }
  }
  // It doesn't exist, so create
  QMenu *menu = new QMenu( cleanedMenuName, this );
  menu->setObjectName( normalizedMenuName( cleanedMenuName ) );
  if ( before )
    mWebMenu->insertMenu( before, menu );
  else
    mWebMenu->addMenu( menu );

  return menu;
}*/

void MBGuiApp::insertAddLayerAction( QAction *action )
{
  mAddLayerMenu->insertAction( mActionAddLayerSeparator, action );
}

void MBGuiApp::removeAddLayerAction( QAction *action )
{
  mAddLayerMenu->removeAction( action );
}
/*void MBGuiApp::addPluginToDatabaseMenu( const QString& name, QAction* action )
{
  QMenu* menu = getDatabaseMenu( name );
  menu->addAction( action );

  // add the Database menu to the menuBar if not added yet
  if ( mDatabaseMenu->actions().count() != 1 )
    return;

  QAction* before = nullptr;
  QList<QAction*> actions = menuBar()->actions();
  for ( int i = 0; i < actions.count(); i++ )
  {
    if ( actions.at( i )->menu() == mDatabaseMenu )
      return;

    // goes before Web menu, if present
    if ( actions.at( i )->menu() == mWebMenu )
    {
      before = actions.at( i );
      break;
    }
  }
  for ( int i = 0; i < actions.count(); i++ )
  {
    // defaults to after Raster menu, which is already in MBGuiApp.ui
    if ( actions.at( i )->menu() == mRasterMenu )
    {
      if ( !before )
      {
        before = actions.at( i += 1 );
        break;
      }
    }
  }
  if ( before )
    menuBar()->insertMenu( before, mDatabaseMenu );
  else
    // fallback insert
    menuBar()->insertMenu( firstRightStandardMenu()->menuAction(), mDatabaseMenu );
}*/
/*void MBGuiApp::addPluginToRasterMenu( const QString& name, QAction* action )
{
  QMenu* menu = getRasterMenu( name );
  menu->addAction( action );
}
*/
/*void MBGuiApp::addPluginToVectorMenu( const QString& name, QAction* action )
{
  QMenu* menu = getVectorMenu( name );
  menu->addAction( action );
}
*/
/*void MBGuiApp::addPluginToWebMenu( const QString& name, QAction* action )
{
  QMenu* menu = getWebMenu( name );
  menu->addAction( action );

  // add the Vector menu to the menuBar if not added yet
  if ( mWebMenu->actions().count() != 1 )
    return;

  QAction* before = nullptr;
  QList<QAction*> actions = menuBar()->actions();
  for ( int i = 0; i < actions.count(); i++ )
  {
    // goes after Database menu, if present
    if ( actions.at( i )->menu() == mDatabaseMenu )
    {
      before = actions.at( i += 1 );
      // don't break here
    }

    if ( actions.at( i )->menu() == mWebMenu )
      return;
  }
  for ( int i = 0; i < actions.count(); i++ )
  {
    // defaults to after Raster menu, which is already in MBGuiApp.ui
    if ( actions.at( i )->menu() == mRasterMenu )
    {
      if ( !before )
      {
        before = actions.at( i += 1 );
        break;
      }
    }
  }

  if ( before )
    menuBar()->insertMenu( before, mWebMenu );
  else
    // fallback insert
    menuBar()->insertMenu( firstRightStandardMenu()->menuAction(), mWebMenu );
}*/
/*void MBGuiApp::removePluginDatabaseMenu( const QString& name, QAction* action )
{
  QMenu* menu = getDatabaseMenu( name );
  menu->removeAction( action );
  if ( menu->actions().isEmpty() )
  {
    mDatabaseMenu->removeAction( menu->menuAction() );
  }

  // remove the Database menu from the menuBar if there are no more actions
  if ( !mDatabaseMenu->actions().isEmpty() )
    return;

  QList<QAction*> actions = menuBar()->actions();
  for ( int i = 0; i < actions.count(); i++ )
  {
    if ( actions.at( i )->menu() == mDatabaseMenu )
    {
      menuBar()->removeAction( actions.at( i ) );
      return;
    }
  }
}*/
/*void MBGuiApp::removePluginRasterMenu( const QString& name, QAction* action )
{
  QMenu* menu = getRasterMenu( name );
  menu->removeAction( action );
  if ( menu->actions().isEmpty() )
  {
    mRasterMenu->removeAction( menu->menuAction() );
  }

  // Remove separator above plugins in Raster menu if no plugins remain
  QList<QAction*> actions = mRasterMenu->actions();
  if ( actions.indexOf( mActionRasterSeparator ) + 1 == actions.count() )
  {
    mRasterMenu->removeAction( mActionRasterSeparator );
    mActionRasterSeparator = nullptr;
  }
}
*/
/*void MBGuiApp::removePluginVectorMenu( const QString& name, QAction* action )
{
  QMenu* menu = getVectorMenu( name );
  menu->removeAction( action );
  if ( menu->actions().isEmpty() )
  {
    mVectorMenu->removeAction( menu->menuAction() );
  }

  // remove the Vector menu from the menuBar if there are no more actions
  if ( !mVectorMenu->actions().isEmpty() )
    return;

  QList<QAction*> actions = menuBar()->actions();
  for ( int i = 0; i < actions.count(); i++ )
  {
    if ( actions.at( i )->menu() == mVectorMenu )
    {
      menuBar()->removeAction( actions.at( i ) );
      return;
    }
  }
}
*/
/*void MBGuiApp::removePluginWebMenu( const QString& name, QAction* action )
{
  QMenu* menu = getWebMenu( name );
  menu->removeAction( action );
  if ( menu->actions().isEmpty() )
  {
    mWebMenu->removeAction( menu->menuAction() );
  }

  // remove the Web menu from the menuBar if there are no more actions
  if ( !mWebMenu->actions().isEmpty() )
    return;

  QList<QAction*> actions = menuBar()->actions();
  for ( int i = 0; i < actions.count(); i++ )
  {
    if ( actions.at( i )->menu() == mWebMenu )
    {
      menuBar()->removeAction( actions.at( i ) );
      return;
    }
  }
}
*/
/*int MBGuiApp::addPluginToolBarIcon( QAction * qAction )
{
  mPluginToolBar->addAction( qAction );
  return 0;
}
*/
/*QAction*MBGuiApp::addPluginToolBarWidget( QWidget* widget )
{
  return mPluginToolBar->addWidget( widget );
}
*/
/*void MBGuiApp::removePluginToolBarIcon( QAction *qAction )
{
  mPluginToolBar->removeAction( qAction );
}
*/
int MBGuiApp::addRasterToolBarIcon( QAction * qAction )
{
  mRasterToolBar->addAction( qAction );
  return 0;
}

QAction*MBGuiApp::addRasterToolBarWidget( QWidget* widget )
{
  return mRasterToolBar->addWidget( widget );
}

void MBGuiApp::removeRasterToolBarIcon( QAction *qAction )
{
  mRasterToolBar->removeAction( qAction );
}

int MBGuiApp::addVectorToolBarIcon( QAction * qAction )
{
  mVectorToolBar->addAction( qAction );
  return 0;
}

QAction*MBGuiApp::addVectorToolBarWidget( QWidget* widget )
{
  return mVectorToolBar->addWidget( widget );
}

void MBGuiApp::removeVectorToolBarIcon( QAction *qAction )
{
  mVectorToolBar->removeAction( qAction );
}
/*int MBGuiApp::addDatabaseToolBarIcon( QAction * qAction )
{
  mDatabaseToolBar->addAction( qAction );
  return 0;
}
*/
/*QAction*MBGuiApp::addDatabaseToolBarWidget( QWidget* widget )
{
  return mDatabaseToolBar->addWidget( widget );
}
*/
/*void MBGuiApp::removeDatabaseToolBarIcon( QAction *qAction )
{
  mDatabaseToolBar->removeAction( qAction );
}
*/
/*int MBGuiApp::addWebToolBarIcon( QAction * qAction )
{
  mWebToolBar->addAction( qAction );
  return 0;
}
*/
/*QAction*MBGuiApp::addWebToolBarWidget( QWidget* widget )
{
  return mWebToolBar->addWidget( widget );
}*/
/*void MBGuiApp::removeWebToolBarIcon( QAction *qAction )
{
  mWebToolBar->removeAction( qAction );
}*/
void MBGuiApp::updateCRSStatusBar()
{
  mOnTheFlyProjectionStatusButton->setText( canvas_xy->mapSettings().destinationCrs().authid() );

  if ( canvas_xy->mapSettings().hasCrsTransformEnabled() )
  {
    mOnTheFlyProjectionStatusButton->setText( tr( "%1 (OTF)" ).arg( mOnTheFlyProjectionStatusButton->text() ) );
    mOnTheFlyProjectionStatusButton->setToolTip(
      tr( "Current CRS: %1 (OTFR enabled)" ).arg( canvas_xy->mapSettings().destinationCrs().description() ) );
    mOnTheFlyProjectionStatusButton->setIcon( QgsApplication::getThemeIcon( "mIconProjectionEnabled.png" ) );
  }
  else
  {
    mOnTheFlyProjectionStatusButton->setToolTip(
      tr( "Current CRS: %1 (OTFR disabled)" ).arg( canvas_xy->mapSettings().destinationCrs().description() ) );
    mOnTheFlyProjectionStatusButton->setIcon( QgsApplication::getThemeIcon( "mIconProjectionDisabled.png" ) );
  }
}
/*void MBGuiApp::destinationCrsChanged()
{
  updateCRSStatusBar();
}
*/
/*void MBGuiApp::hasCrsTransformEnabled( bool theFlag )
{
  // save this information to project
  QgsProject::instance()->writeEntry( "SpatialRefSys", "/ProjectionsEnabled", ( theFlag ? 1 : 0 ) );
  updateCRSStatusBar();
}
*/
// slot to update the progress bar in the status bar
void MBGuiApp::showProgress( int theProgress, int theTotalSteps )
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

void MBGuiApp::mapToolChanged( QgsMapTool *newTool, QgsMapTool *oldTool )
{
  if ( oldTool )
  {
    disconnect( oldTool, SIGNAL( messageEmitted( QString ) ), this, SLOT( displayMapToolMessage( QString ) ) );
    disconnect( oldTool, SIGNAL( messageEmitted( QString, QgsMessageBar::MessageLevel ) ), this, SLOT( displayMapToolMessage( QString, QgsMessageBar::MessageLevel ) ) );
    disconnect( oldTool, SIGNAL( messageDiscarded() ), this, SLOT( removeMapToolMessage() ) );
  }

  if ( newTool )
  {
//    if ( !newTool->isEditTool() )
//    {
//      mNonEditMapTool = newTool;
//    }

    connect( newTool, SIGNAL( messageEmitted( QString ) ), this, SLOT( displayMapToolMessage( QString ) ) );
    connect( newTool, SIGNAL( messageEmitted( QString, QgsMessageBar::MessageLevel ) ), this, SLOT( displayMapToolMessage( QString, QgsMessageBar::MessageLevel ) ) );
    connect( newTool, SIGNAL( messageDiscarded() ), this, SLOT( removeMapToolMessage() ) );
  }
}

void MBGuiApp::showMapCanvas()
{
  // Map layers changed -> switch to map canvas
//  if ( mCentralContainer )
//    mCentralContainer->setCurrentIndex( 0 );
}

void MBGuiApp::markDirty()
{
  // notify the project that there was a change
  QgsProject::instance()->dirty( true );
}

void MBGuiApp::extentChanged()
{
  // allow symbols in the legend update their preview if they use map units
  mLayerTreeView->layerTreeModel()->setLegendMapViewData( canvas_xy->mapUnitsPerPixel(), canvas_xy->mapSettings().outputDpi(), canvas_xy->scale() );
}
void MBGuiApp::extentChanged_xy()
{
    QgsRectangle rect = canvas_xy->extent();
//    if(!checkBox_lock_canvas_xy->isChecked())
//        canvas_xy->setMapTool(mpZoomInTool);
    if(!checkBox_lock_canvas_yz->isChecked())
        canvas_yz->setExtent(rect);
    if(!checkBox_lock_canvas_depths->isChecked())
        canvas_depths->setExtent(rect);
    if(!checkBox_lock_canvas_error->isChecked())
        canvas_error->setExtent(rect);
    if(!checkBox_lock_canvas_nei->isChecked())
        canvas_nei->setExtent(rect);
    if(!checkBox_lock_canvas_rei->isChecked())
        canvas_rei->setExtent(rect);
    if(!checkBox_lock_canvas_mean->isChecked())
        canvas_mean->setExtent(rect);
}
void MBGuiApp::extentChanged_yz()
{
    QgsRectangle rect = canvas_yz->extent();
    if(!checkBox_lock_canvas_xy->isChecked())
        canvas_xy->setMapTool(mpZoomInTool_xy);
//    if(!checkBox_lock_canvas_yz->isChecked())
//        canvas_yz->setExtent(rect);
    if(!checkBox_lock_canvas_depths->isChecked())
        canvas_depths->setExtent(rect);
    if(!checkBox_lock_canvas_error->isChecked())
        canvas_error->setExtent(rect);
    if(!checkBox_lock_canvas_nei->isChecked())
        canvas_nei->setExtent(rect);
    if(!checkBox_lock_canvas_rei->isChecked())
        canvas_rei->setExtent(rect);
    if(!checkBox_lock_canvas_mean->isChecked())
        canvas_mean->setExtent(rect);
}

void MBGuiApp::layersWereAdded( const QList<QgsMapLayer *>& theLayers )
{
    qDebug() << "Layers were added: " << theLayers << endl;
  Q_FOREACH ( QgsMapLayer* layer, theLayers )
  {
    QgsDataProvider *provider = nullptr;

    QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
    if ( vlayer )
    {
      // notify user about any font family substitution, but only when rendering labels (i.e. not when opening settings dialog)
//      connect( vlayer, SIGNAL( labelingFontNotFound( QgsVectorLayer*, QString ) ), this, SLOT( labelingFontNotFound( QgsVectorLayer*, QString ) ) );

      QgsVectorDataProvider* vProvider = vlayer->dataProvider();
      if ( vProvider && vProvider->capabilities() & QgsVectorDataProvider::EditingCapabilities )
      {
//        connect( vlayer, SIGNAL( layerModified() ), this, SLOT( updateLayerModifiedActions() ) );
//        connect( vlayer, SIGNAL( editingStarted() ), this, SLOT( layerEditStateChanged() ) );
//        connect( vlayer, SIGNAL( editingStopped() ), this, SLOT( layerEditStateChanged() ) );
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
      connect( provider, SIGNAL( dataChanged() ), canvas_yz, SLOT( refresh() ) );
    }
  }

}

void MBGuiApp::showRotation()
{
  // update the statusbar with the current rotation.
  double myrotation = canvas_xy->rotation();
  mRotationEdit->setValue( myrotation );
}

void MBGuiApp::updateMouseCoordinatePrecision()
{
  mCoordsEdit->setMouseCoordinatesPrecision( QgsCoordinateUtils::calculateCoordinatePrecision( mapCanvas()->mapUnitsPerPixel(), mapCanvas()->mapSettings().destinationCrs() ) );
}

void MBGuiApp::showStatusMessage( const QString& theMessage )
{
  statusBar()->showMessage( theMessage );
}

void MBGuiApp::displayMapToolMessage( const QString& message, QgsMessageBar::MessageLevel level )
{
  // remove previous message
  messageBar()->popWidget( mLastMapToolMessage );

  QgsMapTool* tool = mapCanvas()->mapTool();

  if ( tool )
  {
    mLastMapToolMessage = new QgsMessageBarItem( tool->toolName(), message, level, messageTimeout() );
    messageBar()->pushItem( mLastMapToolMessage );
  }
}

void MBGuiApp::displayMessage( const QString& title, const QString& message, QgsMessageBar::MessageLevel level )
{
  messageBar()->pushMessage( title, message, level, messageTimeout() );
}

void MBGuiApp::removeMapToolMessage()
{
  // remove previous message
  messageBar()->popWidget( mLastMapToolMessage );
}

// Show the maptip using tooltip
void MBGuiApp::showMapTip()
{
  // Stop the timer while we look for a maptip
  mpMapTipsTimer->stop();

  // Only show tooltip if the mouse is over the canvas
  if ( canvas_xy->underMouse() )
  {
    QPoint myPointerPos = canvas_xy->mouseLastXY();

    //  Make sure there is an active layer before proceeding
    QgsMapLayer* mypLayer = canvas_xy->currentLayer();
    if ( mypLayer )
    {
      //QgsDebugMsg("Current layer for maptip display is: " + mypLayer->source());
      // only process vector layers
      if ( mypLayer->type() == QgsMapLayer::VectorLayer )
      {
        // Show the maptip if the maptips button is depressed
        if ( mMapTipsVisible )
        {
          mpMaptip->showMapTip( mypLayer, mLastMapPosition, myPointerPos, canvas_xy );
        }
      }
    }
    else
    {
      showStatusMessage( tr( "Maptips require an active layer" ) );
    }
  }
}
/*void MBGuiApp::projectPropertiesProjections()
{
  // Driver to display the project props dialog and switch to the
  // projections tab
  mShowProjectionTab = true;
  projectProperties();
}
*/
/*void MBGuiApp::projectProperties()
{
  // Display the property sheet for the Project
  // set wait cursor since construction of the project properties
  // dialog results in the construction of the spatial reference
  // system QMap
  QApplication::setOverrideCursor( Qt::WaitCursor );
  QgsProjectProperties *pp = new QgsProjectProperties( canvas_xy, this );
  // if called from the status bar, show the projection tab
  if ( mShowProjectionTab )
  {
    pp->showProjectionsTab();
    mShowProjectionTab = false;
  }
  qApp->processEvents();
  // Be told if the mouse display precision may have changed by the user
  // changing things in the project properties dialog box
  connect( pp, SIGNAL( displayPrecisionChanged() ), this,
           SLOT( updateMouseCoordinatePrecision() ) );

  connect( pp, SIGNAL( scalesChanged( const QStringList & ) ), mScaleEdit,
           SLOT( updateScales( const QStringList & ) ) );
  QApplication::restoreOverrideCursor();

  //pass any refresh signals off to canvases
  // Line below was commented out by wonder three years ago (r4949).
  // It is needed to refresh scale bar after changing display units.
  connect( pp, SIGNAL( refresh() ), canvas_xy, SLOT( refresh() ) );

  // Display the modal dialog box.
  pp->exec();

  qobject_cast<QgsMeasureTool*>( mMapTools.mMeasureDist )->updateSettings();
  qobject_cast<QgsMeasureTool*>( mMapTools.mMeasureArea )->updateSettings();
  qobject_cast<QgsMapToolMeasureAngle*>( mMapTools.mMeasureAngle )->updateSettings();

  // Set the window title.
  setTitleBarText_( *this );

  // delete the property sheet object
  delete pp;
}*/

QgsClipboard * MBGuiApp::clipboard()
{
  return mInternalClipboard;
}

//void MBGuiApp::selectionChanged( QgsMapLayer *layer )
//{
//  toggleExcludes();

//  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
//  if ( vlayer )
//  {
//    showStatusMessage( tr( "%n feature(s) selected on layer %1.", "number of selected features", vlayer->selectedFeatureCount() ).arg( vlayer->name() ) );
//  }
//  if ( layer == activeLayer() )
//  {
//    activateDeactivateLayerRelatedActions( layer );
//  }
//}
void MBGuiApp::selectionChanged_xy( QgsMapLayer *layer )
{
  toggleExcludes(true);

  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
  if ( vlayer )
  {
    showStatusMessage( tr( "%n feature(s) selected on layer %1.", "number of selected features", vlayer->selectedFeatureCount() ).arg( vlayer->name() ) );
  }
  if ( layer == activeLayer_xy() )
  {
    activateDeactivateLayerRelatedActions( layer );
  }
}
void MBGuiApp::selectionChanged_yz( QgsMapLayer *layer )
{
  toggleExcludes(false);

  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
  if ( vlayer )
  {
    showStatusMessage( tr( "%n feature(s) selected on layer %1.", "number of selected features", vlayer->selectedFeatureCount() ).arg( vlayer->name() ) );
  }
  if ( layer == activeLayer_yz() )
  {
    activateDeactivateLayerRelatedActions( layer );
  }
}
void MBGuiApp::legendLayerSelectionChanged( void )
{
  QList<QgsLayerTreeLayer*> selectedLayers = mLayerTreeView ? mLayerTreeView->selectedLayerNodes() : QList<QgsLayerTreeLayer*>();
  QList<QgsLayerTreeLayer*> selectedLayers_yz = mLayerTreeView_yz ? mLayerTreeView_yz->selectedLayerNodes() : QList<QgsLayerTreeLayer*>();

//  mActionDuplicateLayer->setEnabled( !selectedLayers.isEmpty() );
/*  mActionSetLayerScaleVisibility->setEnabled( !selectedLayers.isEmpty() );
  mActionSetLayerCRS->setEnabled( !selectedLayers.isEmpty() );
  mActionSetProjectCRSFromLayer->setEnabled( selectedLayers.count() == 1 );
*/
  mActionSaveEdits->setEnabled( QgsLayerTreeUtils::layersModified( selectedLayers ) );
  mActionRollbackEdits->setEnabled( QgsLayerTreeUtils::layersModified( selectedLayers ) );
  mActionCancelEdits->setEnabled( QgsLayerTreeUtils::layersEditable( selectedLayers ) );

  mActionSaveEdits->setEnabled( QgsLayerTreeUtils::layersModified( selectedLayers_yz ) );
  mActionRollbackEdits->setEnabled( QgsLayerTreeUtils::layersModified( selectedLayers_yz ) );
  mActionCancelEdits->setEnabled( QgsLayerTreeUtils::layersEditable( selectedLayers_yz ) );

  /*mLegendExpressionFilterButton->setEnabled( false );
  mLegendExpressionFilterButton->setVectorLayer( nullptr );
  if ( selectedLayers.size() == 1 )
  {
    QgsLayerTreeLayer* l = selectedLayers.front();
    if ( l->layer() && l->layer()->type() == QgsMapLayer::VectorLayer )
    {
      mLegendExpressionFilterButton->setEnabled( true );
      bool exprEnabled;
      QString expr = QgsLayerTreeUtils::legendFilterByExpression( *l, &exprEnabled );
      mLegendExpressionFilterButton->setExpressionText( expr );
      mLegendExpressionFilterButton->setVectorLayer( qobject_cast<QgsVectorLayer*>( l->layer() ) );
      mLegendExpressionFilterButton->setChecked( exprEnabled );
    }
  }*/
}

//void MBGuiApp::layerEditStateChanged()
//{
//  QgsMapLayer* layer = qobject_cast<QgsMapLayer *>( sender() );
//  if ( layer && layer == activeLayer() )
//  {
//    activateDeactivateLayerRelatedActions( layer );
//    mSaveRollbackInProgress = false;
//  }
//}
/*void MBGuiApp::layerEditStateChanged_xy()
{
  QgsMapLayer* layer = qobject_cast<QgsMapLayer *>( sender() );
  if ( layer && layer == activeLayer() )
  {
    activateDeactivateLayerRelatedActions( layer );
    mSaveRollbackInProgress = false;
  }
}*/
/*void MBGuiApp::layerEditStateChanged_yz()
{
  QgsMapLayer* layer = qobject_cast<QgsMapLayer *>( sender() );
  if ( layer && layer == activeLayer_yz() )
  {
    activateDeactivateLayerRelatedActions( layer );
    mSaveRollbackInProgress = false;
  }
}*/
void MBGuiApp::activateDeactivateLayerRelatedActions( QgsMapLayer* layer )
{
  bool enableMove = false, enableRotate = false, enablePin = false, enableShowHide = false, enableChange = false;

  QMap<QString, QgsMapLayer*> layers = QgsMapLayerRegistry::instance()->mapLayers();
  for ( QMap<QString, QgsMapLayer*>::iterator it = layers.begin(); it != layers.end(); ++it )
  {
    QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( it.value() );
    if ( !vlayer || !vlayer->isEditable() ||
         ( !vlayer->diagramsEnabled() && !vlayer->labelsEnabled() ) )
      continue;

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
      break;
  }

//  mActionPinLabels->setEnabled( enablePin );
//  mActionShowHideLabels->setEnabled( enableShowHide );
//  mActionMoveLabel->setEnabled( enableMove );
//  mActionRotateLabel->setEnabled( enableRotate );
//  mActionChangeLabelProperties->setEnabled( enableChange );

//  mMenuPasteAs->setEnabled( clipboard() && !clipboard()->empty() );
//  mActionPasteAsNewVector->setEnabled( clipboard() && !clipboard()->empty() );
//  mActionPasteAsNewMemoryVector->setEnabled( clipboard() && !clipboard()->empty() );

//  updateLayerModifiedActions();

  if ( !layer )
  {
    mActionSelectFeatures->setEnabled( false );
    mActionSelectPolygon->setEnabled( false );
    mActionSelectFreehand->setEnabled( false );
    mActionSelectRadius->setEnabled( false );
    mActionIdentify->setEnabled( QSettings().value( "/Map/identifyMode", 0 ).toInt() != 0 );
    mActionSelectByExpression->setEnabled( false );
//    mActionLabeling->setEnabled( false );
    mActionOpenTable->setEnabled( false );
    mActionSelectAll->setEnabled( false );
    mActionInvertSelection->setEnabled( false );
    mActionOpenFieldCalc->setEnabled( false );
    mActionToggleEditing->setEnabled( false );
    mActionToggleEditing->setChecked( false );
    mActionSaveLayerEdits->setEnabled( false );
//    mActionSaveLayerDefinition->setEnabled( false );
    mActionLayerSaveAs->setEnabled( false );
    mActionLayerSaveAsTemp->setEnabled( false );
    mActionLayerProperties->setEnabled( false );
//    mActionLayerSubsetString->setEnabled( false );
    mActionAddToOverview->setEnabled( false );
//    mActionFeatureAction->setEnabled( false );
//    mActionAddFeature->setEnabled( false );
//    mActionCircularStringCurvePoint->setEnabled( false );
//    mActionCircularStringRadius->setEnabled( false );
//    mActionMoveFeature->setEnabled( false );
//    mActionRotateFeature->setEnabled( false );
//    mActionOffsetCurve->setEnabled( false );
    mActionNodeTool->setEnabled( false );
//    mActionDeleteSelected->setEnabled( false );
//    mActionCutFeatures->setEnabled( false );
//    mActionCopyFeatures->setEnabled( false );
//    mActionPasteFeatures->setEnabled( false );
//    mActionCopyStyle->setEnabled( false );
//    mActionPasteStyle->setEnabled( false );

    mUndoWidget->dockContents()->setEnabled( false );
    mActionUndo->setEnabled( false );
    mActionRedo->setEnabled( false );
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

    mActionLocalHistogramStretch->setEnabled( false );
    mActionFullHistogramStretch->setEnabled( false );
    mActionLocalCumulativeCutStretch->setEnabled( false );
    mActionFullCumulativeCutStretch->setEnabled( false );
    mActionIncreaseBrightness->setEnabled( false );
    mActionDecreaseBrightness->setEnabled( false );
    mActionIncreaseContrast->setEnabled( false );
    mActionDecreaseContrast->setEnabled( false );
    mActionZoomActualSize->setEnabled( false );
    mActionZoomToLayer->setEnabled( false );
    return;
  }

  mActionLayerProperties->setEnabled( QgsProject::instance()->layerIsEmbedded( layer->id() ).isEmpty() );
  mActionAddToOverview->setEnabled( true );
  mActionZoomToLayer->setEnabled( true );

//  mActionCopyStyle->setEnabled( true );
//  mActionPasteStyle->setEnabled( clipboard()->hasFormat( QGSCLIPBOARD_STYLE_MIME ) );

  /***********Vector layers****************/
  if ( layer->type() == QgsMapLayer::VectorLayer )
  {
    QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer *>( layer );
    QgsVectorDataProvider* dprovider = vlayer->dataProvider();

    bool isEditable = vlayer->isEditable();
    bool layerHasSelection = vlayer->selectedFeatureCount() > 0;
    bool layerHasActions = vlayer->actions()->size() || !QgsMapLayerActionRegistry::instance()->mapLayerActions( vlayer ).isEmpty();

    mActionLocalHistogramStretch->setEnabled( false );
    mActionFullHistogramStretch->setEnabled( false );
    mActionLocalCumulativeCutStretch->setEnabled( false );
    mActionFullCumulativeCutStretch->setEnabled( false );
    mActionIncreaseBrightness->setEnabled( false );
    mActionDecreaseBrightness->setEnabled( false );
    mActionIncreaseContrast->setEnabled( false );
    mActionDecreaseContrast->setEnabled( false );
    mActionZoomActualSize->setEnabled( false );
//    mActionLabeling->setEnabled( true );

    mActionSelectFeatures->setEnabled( true );
    mActionSelectPolygon->setEnabled( true );
    mActionSelectFreehand->setEnabled( true );
    mActionSelectRadius->setEnabled( true );
    mActionIdentify->setEnabled( true );
    mActionSelectByExpression->setEnabled( true );
    mActionOpenTable->setEnabled( true );
    mActionSelectAll->setEnabled( true );
    mActionInvertSelection->setEnabled( true );
    mActionSaveLayerDefinition->setEnabled( true );
    mActionLayerSaveAs->setEnabled( true );
    mActionLayerSaveAsTemp->setEnabled( true );
//    mActionCopyFeatures->setEnabled( layerHasSelection );
//    mActionFeatureAction->setEnabled( layerHasActions );

//    if ( !isEditable && canvas_xy && canvas_xy->mapTool()
//         && canvas_xy->mapTool()->isEditTool() && !mSaveRollbackInProgress )
//    {
//      canvas_xy->setMapTool( mNonEditMapTool );
//    }

    if ( dprovider )
    {
      bool canChangeAttributes = dprovider->capabilities() & QgsVectorDataProvider::ChangeAttributeValues;
      bool canDeleteFeatures = dprovider->capabilities() & QgsVectorDataProvider::DeleteFeatures;
      bool canAddFeatures = dprovider->capabilities() & QgsVectorDataProvider::AddFeatures;
      bool canSupportEditing = dprovider->capabilities() & QgsVectorDataProvider::EditingCapabilities;
      bool canChangeGeometry = dprovider->capabilities() & QgsVectorDataProvider::ChangeGeometries;

      mActionLayerSubsetString->setEnabled( !isEditable && dprovider->supportsSubsetString() );

      mActionToggleEditing->setEnabled( canSupportEditing && !vlayer->isReadOnly() );
      mActionToggleEditing->setChecked( canSupportEditing && isEditable );
      mActionSaveLayerEdits->setEnabled( canSupportEditing && isEditable && vlayer->isModified() );
      mUndoWidget->dockContents()->setEnabled( canSupportEditing && isEditable );
      mActionUndo->setEnabled( canSupportEditing );
      mActionRedo->setEnabled( canSupportEditing );

      //start editing/stop editing
      if ( canSupportEditing )
      {
        updateUndoActions();
      }

//      mActionPasteFeatures->setEnabled( isEditable && canAddFeatures && !clipboard()->empty() );

//      mActionAddFeature->setEnabled( isEditable && canAddFeatures );
//      mActionCircularStringCurvePoint->setEnabled( isEditable && ( canAddFeatures || canChangeGeometry )
//          && ( vlayer->geometryType() == QGis::Line || vlayer->geometryType() == QGis::Polygon ) );
//      mActionCircularStringRadius->setEnabled( isEditable && ( canAddFeatures || canChangeGeometry )
//          && ( vlayer->geometryType() == QGis::Line || vlayer->geometryType() == QGis::Polygon ) );


      //does provider allow deleting of features?
//      mActionDeleteSelected->setEnabled( isEditable && canDeleteFeatures && layerHasSelection );
//      mActionCutFeatures->setEnabled( isEditable && canDeleteFeatures && layerHasSelection );

      //merge tool needs editable layer and provider with the capability of adding and deleting features
      if ( isEditable && canChangeAttributes )
      {
//        mActionMergeFeatures->setEnabled( layerHasSelection && canDeleteFeatures && canAddFeatures );
//        mActionMergeFeatureAttributes->setEnabled( layerHasSelection );
      }
      else
      {
//        mActionMergeFeatures->setEnabled( false );
//        mActionMergeFeatureAttributes->setEnabled( false );
      }

      bool isMultiPart = QGis::isMultiType( vlayer->wkbType() ) || !dprovider->doesStrictFeatureTypeCheck();

//      // moving enabled if geometry changes are supported
//      mActionAddPart->setEnabled( isEditable && canChangeGeometry );
//      mActionDeletePart->setEnabled( isEditable && canChangeGeometry );
//      mActionMoveFeature->setEnabled( isEditable && canChangeGeometry );
//      mActionRotateFeature->setEnabled( isEditable && canChangeGeometry );
      mActionNodeTool->setEnabled( isEditable && canChangeGeometry );

//      mActionEnableTracing->setEnabled( isEditable && canAddFeatures &&
//                                        ( vlayer->geometryType() == QGis::Line || vlayer->geometryType() == QGis::Polygon ) );

      if ( vlayer->geometryType() == QGis::Point )
      {
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

        if ( isEditable && canChangeAttributes )
        {
          if ( QgsMapToolRotatePointSymbols::layerIsRotatable( vlayer ) )
          {
//            mActionRotatePointSymbols->setEnabled( true );
          }
        }
      }
      else if ( vlayer->geometryType() == QGis::Line )
      {
//        mActionAddFeature->setIcon( QgsApplication::getThemeIcon( "/mActionCaptureLine.svg" ) );

//        mActionReshapeFeatures->setEnabled( isEditable && canChangeGeometry );
//        mActionSplitFeatures->setEnabled( isEditable && canAddFeatures );
//        mActionSplitParts->setEnabled( isEditable && canChangeGeometry && isMultiPart );
//        mActionSimplifyFeature->setEnabled( isEditable && canChangeGeometry );
//        mActionOffsetCurve->setEnabled( isEditable && canAddFeatures && canChangeAttributes );

//        mActionAddRing->setEnabled( false );
//        mActionFillRing->setEnabled( false );
//        mActionDeleteRing->setEnabled( false );
      }
      else if ( vlayer->geometryType() == QGis::Polygon )
      {
//        mActionAddFeature->setIcon( QgsApplication::getThemeIcon( "/mActionCapturePolygon.svg" ) );

//        mActionAddRing->setEnabled( isEditable && canChangeGeometry );
//        mActionFillRing->setEnabled( isEditable && canChangeGeometry );
//        mActionReshapeFeatures->setEnabled( isEditable && canChangeGeometry );
//        mActionSplitFeatures->setEnabled( isEditable && canAddFeatures );
//        mActionSplitParts->setEnabled( isEditable && canChangeGeometry && isMultiPart );
//        mActionSimplifyFeature->setEnabled( isEditable && canChangeGeometry );
//        mActionDeleteRing->setEnabled( isEditable && canChangeGeometry );
//        mActionOffsetCurve->setEnabled( false );
      }
      else if ( vlayer->geometryType() == QGis::NoGeometry )
      {
//        mActionAddFeature->setIcon( QgsApplication::getThemeIcon( "/mActionNewTableRow.png" ) );
      }

      mActionOpenFieldCalc->setEnabled( true );

      return;
    }
    else
    {
      mUndoWidget->dockContents()->setEnabled( false );
      mActionUndo->setEnabled( false );
      mActionRedo->setEnabled( false );
    }

//    mActionLayerSubsetString->setEnabled( false );
  } //end vector layer block
  /*************Raster layers*************/
  else if ( layer->type() == QgsMapLayer::RasterLayer )
  {
    const QgsRasterLayer *rlayer = qobject_cast<const QgsRasterLayer *>( layer );
    if ( rlayer->dataProvider()->dataType( 1 ) != QGis::ARGB32
         && rlayer->dataProvider()->dataType( 1 ) != QGis::ARGB32_Premultiplied )
    {
      if ( rlayer->dataProvider()->capabilities() & QgsRasterDataProvider::Size )
      {
        mActionFullHistogramStretch->setEnabled( true );
      }
      else
      {
        // it would hang up reading the data for WMS for example
        mActionFullHistogramStretch->setEnabled( false );
      }
      mActionLocalHistogramStretch->setEnabled( true );
    }
    else
    {
      mActionLocalHistogramStretch->setEnabled( false );
      mActionFullHistogramStretch->setEnabled( false );
    }

    mActionLocalCumulativeCutStretch->setEnabled( true );
    mActionFullCumulativeCutStretch->setEnabled( true );
    mActionIncreaseBrightness->setEnabled( true );
    mActionDecreaseBrightness->setEnabled( true );
    mActionIncreaseContrast->setEnabled( true );
    mActionDecreaseContrast->setEnabled( true );

//    mActionLayerSubsetString->setEnabled( false );
//    mActionFeatureAction->setEnabled( false );
    mActionSelectFeatures->setEnabled( false );
    mActionSelectPolygon->setEnabled( false );
    mActionSelectFreehand->setEnabled( false );
    mActionSelectRadius->setEnabled( false );
    mActionZoomActualSize->setEnabled( true );
    mActionOpenTable->setEnabled( false );
    mActionSelectAll->setEnabled( false );
    mActionInvertSelection->setEnabled( false );
    mActionSelectByExpression->setEnabled( false );
    mActionOpenFieldCalc->setEnabled( false );
    mActionToggleEditing->setEnabled( false );
    mActionToggleEditing->setChecked( false );
    mActionSaveLayerEdits->setEnabled( false );
    mUndoWidget->dockContents()->setEnabled( false );
    mActionUndo->setEnabled( false );
    mActionRedo->setEnabled( false );
//    mActionSaveLayerDefinition->setEnabled( true );
    mActionLayerSaveAs->setEnabled( true );
    mActionLayerSaveAsTemp->setEnabled( true );
//    mActionAddFeature->setEnabled( false );
//    mActionCircularStringCurvePoint->setEnabled( false );
//    mActionCircularStringRadius->setEnabled( false );
    mActionDeleteSelected->setEnabled( false );
//    mActionAddRing->setEnabled( false );
//    mActionFillRing->setEnabled( false );
//    mActionAddPart->setEnabled( false );
    mActionNodeTool->setEnabled( false );
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

    //NOTE: This check does not really add any protection, as it is called on load not on layer select/activate
    //If you load a layer with a provider and idenitfy ability then load another without, the tool would be disabled for both

    //Enable the Identify tool ( GDAL datasets draw without a provider )
    //but turn off if data provider exists and has no Identify capabilities
    mActionIdentify->setEnabled( true );

    QSettings settings;
    int identifyMode = settings.value( "/Map/identifyMode", 0 ).toInt();
    if ( identifyMode == 0 )
    {
      const QgsRasterLayer *rlayer = qobject_cast<const QgsRasterLayer *>( layer );
      const QgsRasterDataProvider* dprovider = rlayer->dataProvider();
      if ( dprovider )
      {
        // does provider allow the identify map tool?
        if ( dprovider->capabilities() & QgsRasterDataProvider::Identify )
        {
          mActionIdentify->setEnabled( true );
        }
        else
        {
          mActionIdentify->setEnabled( false );
        }
      }
    }
  }
}

/*void MBGuiApp::refreshActionFeatureAction()
{
  QgsMapLayer* layer = activeLayer();

  if ( !layer || layer->type() != QgsMapLayer::VectorLayer )
  {
    return;
  }

  QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer *>( layer );

  bool layerHasActions = vlayer->actions()->size() || !QgsMapLayerActionRegistry::instance()->mapLayerActions( vlayer ).isEmpty();
  mActionFeatureAction->setEnabled( layerHasActions );
}*/

bool MBGuiApp::autoTransaction() const
{
  QSettings settings;
  return settings.value( "/qgis/autoTransaction", false ).toBool();
}

void MBGuiApp::setAutoTransaction( bool state )
{
  QSettings settings;

  if ( settings.value( "/qgis/autoTransaction", false ).toBool() != state )
  {

    settings.setValue( "/qgis/autoTransaction", state );
  }
}



/////////////////////////////////////////////////////////////////
//
//
// Only functions relating to raster layer management in this
// section (look for a similar comment block to this to find
// the end of this section).
//
// Tim Sutton
//
//
/////////////////////////////////////////////////////////////////


// this is a slot for action from GUI to add raster layer
void MBGuiApp::addRasterLayer()
{
  QString fileFilters;

  QStringList selectedFiles;
  QString e;//only for parameter correctness
  QString title = tr( "Open a GDAL Supported Raster Data Source" );
  QgisGui::openFilesRememberingFilter( "lastRasterFileFilter", mRasterFileFilter, selectedFiles, e,
                                       title );

  if ( selectedFiles.isEmpty() )
  {
    // no files were selected, so just bail
    return;
  }

  addRasterLayers( selectedFiles );

}

//
// This is the method that does the actual work of adding a raster layer - the others
// here simply create a raster layer object and delegate here. It is the responsibility
// of the calling method to manage things such as the frozen state of the mapcanvas and
// using waitcursors etc. - this method won't and SHOULDN'T do it
//
bool MBGuiApp::addRasterLayer( QgsRasterLayer *theRasterLayer )
{
  Q_CHECK_PTR( theRasterLayer );

  if ( ! theRasterLayer )
  {
    // XXX insert meaningful whine to the user here; although be
    // XXX mindful that a null layer may mean exhausted memory resources
    return false;
  }

  if ( !theRasterLayer->isValid() )
  {
    delete theRasterLayer;
    return false;
  }

  // register this layer with the central layers registry
  QList<QgsMapLayer *> myList;
  myList << theRasterLayer;
  QgsMapLayerRegistry::instance()->addMapLayers( myList );

  return true;
}


// Open a raster layer - this is the generic function which takes all parameters
// this method is a blend of addRasterLayer() functions (with and without provider)
// and addRasterLayers()
QgsRasterLayer* MBGuiApp::addRasterLayerPrivate(
  const QString & uri, const QString & baseName, const QString & providerKey,
  bool guiWarning, bool guiUpdate )
{
  if ( guiUpdate )
  {
    // let the user know we're going to possibly be taking a while
    // QApplication::setOverrideCursor( Qt::WaitCursor );
    canvas_error->freeze( true );
    canvas_rei->freeze( true );
  }

  QgsDebugMsg( "Creating new raster layer using " + uri + " with baseName of " + baseName );

  QgsRasterLayer *layer = nullptr;
  // XXX ya know QgsRasterLayer can snip out the basename on its own;
  // XXX why do we have to pass it in for it?
  // ET : we may not be getting "normal" files here, so we still need the baseName argument
  if ( !providerKey.isEmpty() && uri.endsWith( ".adf", Qt::CaseInsensitive ) )
  {
    QFileInfo fileInfo( uri );
    QString dirName = fileInfo.path();
    layer = new QgsRasterLayer( dirName, QFileInfo( dirName ).completeBaseName(), QString( "gdal" ) );
  }
  else if ( providerKey.isEmpty() )
    layer = new QgsRasterLayer( uri, baseName ); // fi.completeBaseName());
  else
    layer = new QgsRasterLayer( uri, baseName, providerKey );

  QgsDebugMsg( "Constructed new layer" );

  QgsError error;
  QString title;
  bool ok = false;

  if ( !layer->isValid() )
  {
    error = layer->error();
    title = tr( "Invalid Layer" );

    if ( shouldAskUserForGDALSublayers( layer ) )
    {
      askUserForGDALSublayers( layer );
      ok = true;

      // The first layer loaded is not useful in that case. The user can select it in
      // the list if he wants to load it.
      delete layer;
      layer = nullptr;
    }
  }
  else
  {
    ok = addRasterLayer( layer );
    if ( !ok )
    {
      error.append( QGS_ERROR_MESSAGE( tr( "Error adding valid layer to map canvas" ),
                                       tr( "Raster layer" ) ) );
      title = tr( "Error" );
    }
  }

  if ( !ok )
  {
    if ( guiUpdate ){
      canvas_error->freeze( false );
      canvas_rei->freeze( false );
    }
    // don't show the gui warning if we are loading from command line
    if ( guiWarning )
    {
      messageBar()->pushMessage( title, error.message( QgsErrorMessage::Text ),
                                 QgsMessageBar::CRITICAL, messageTimeout() );
    }

    if ( layer )
    {
      delete layer;
      layer = nullptr;
    }
  }

  if ( guiUpdate )
  {
    // draw the map
    canvas_error->freeze( false );
    canvas_error->refresh();
    canvas_rei->freeze( false );
    canvas_rei->refresh();
    // Let render() do its own cursor management
    //    QApplication::restoreOverrideCursor();
  }

  return layer;

}


//create a raster layer object and delegate to addRasterLayer(QgsRasterLayer *)
QgsRasterLayer* MBGuiApp::addRasterLayer(
  QString const & rasterFile, QString const & baseName, bool guiWarning )
{
  return addRasterLayerPrivate( rasterFile, baseName, QString(), guiWarning, true );
}


/** Add a raster layer directly without prompting user for location
  The caller must provide information compatible with the provider plugin
  using the uri and baseName. The provider can use these
  parameters in any way necessary to initialize the layer. The baseName
  parameter is used in the Map Legend so it should be formed in a meaningful
  way.

  \note   Copied from the equivalent addVectorLayer function in this file
  */
QgsRasterLayer* MBGuiApp::addRasterLayer(
  QString const &uri, QString const &baseName, QString const &providerKey )
{
  return addRasterLayerPrivate( uri, baseName, providerKey, true, true );
}


//create a raster layer object and delegate to addRasterLayer(QgsRasterLayer *)
bool MBGuiApp::addRasterLayers( QStringList const &theFileNameQStringList, bool guiWarning )
{
  if ( theFileNameQStringList.empty() )
  {
    // no files selected so bail out, but
    // allow canvas_xy to handle events
    // first
    canvas_error->freeze( false );
    canvas_rei->freeze( false );
    return false;
  }

  canvas_error->freeze( true );
  canvas_rei->freeze( true );

  // this is messy since some files in the list may be rasters and others may
  // be ogr layers. We'll set returnValue to false if one or more layers fail
  // to load.
  bool returnValue = true;
  for ( QStringList::ConstIterator myIterator = theFileNameQStringList.begin();
        myIterator != theFileNameQStringList.end();
        ++myIterator )
  {
    QString errMsg;
    bool ok = false;

    // if needed prompt for zipitem layers
    QString vsiPrefix = QgsZipItem::vsiPrefix( *myIterator );
    if ( ! myIterator->startsWith( "/vsi", Qt::CaseInsensitive ) &&
         ( vsiPrefix == "/vsizip/" || vsiPrefix == "/vsitar/" ) )
    {
      if ( askUserForZipItemLayers( *myIterator ) )
        continue;
    }

    if ( QgsRasterLayer::isValidRasterFileName( *myIterator, errMsg ) )
    {
      QFileInfo myFileInfo( *myIterator );

      // try to create the layer
      QgsRasterLayer *layer = addRasterLayerPrivate( *myIterator, myFileInfo.completeBaseName(),
                              QString(), guiWarning, true );
      if ( layer && layer->isValid() )
      {
        //only allow one copy of a ai grid file to be loaded at a
        //time to prevent the user selecting all adfs in 1 dir which
        //actually represent 1 coverate,

        if ( myFileInfo.fileName().toLower().endsWith( ".adf" ) )
        {
          break;
        }
      }
      // if layer is invalid addRasterLayerPrivate() will show the error

    } // valid raster filename
    else
    {
      ok = false;

      // Issue message box warning unless we are loading from cmd line since
      // non-rasters are passed to this function first and then successfully
      // loaded afterwards (see main.cpp)
      if ( guiWarning )
      {
        QString msg = tr( "%1 is not a supported raster data source" ).arg( *myIterator );
        if ( !errMsg.isEmpty() )
          msg += '\n' + errMsg;

        messageBar()->pushMessage( tr( "Unsupported Data Source" ), msg, QgsMessageBar::CRITICAL, messageTimeout() );
      }
    }
    if ( ! ok )
    {
      returnValue = false;
    }
  }

  canvas_error->freeze( false );
  canvas_error->refresh();
  canvas_rei->freeze( false );
  canvas_rei->refresh();

// Let render() do its own cursor management
//  QApplication::restoreOverrideCursor();

  return returnValue;

}




///////////////////////////////////////////////////////////////////
//
//
//
//
//    RASTER ONLY RELATED FUNCTIONS BLOCK ENDS....
//
//
//
//
///////////////////////////////////////////////////////////////////

void MBGuiApp::keyPressEvent( QKeyEvent * e )
{
  // The following statement causes a crash on WIN32 and should be
  // enclosed in an #ifdef QGISDEBUG if its really necessary. Its
  // commented out for now. [gsherman]
  // QgsDebugMsg( QString( "%1 (keypress received)" ).arg( e->text() ) );
  emit keyPressed( e );

  //cancel rendering progress with esc key
  if ( e->key() == Qt::Key_Escape )
  {
    stopRendering();
  }
#if defined(Q_OS_WIN) && defined(QGISDEBUG)
  else if ( e->key() == Qt::Key_Backslash && e->modifiers() & Qt::ControlModifier )
  {
    qgisCrashDump( 0 );
  }
#endif
  else
  {
    e->ignore();
  }
}

void MBGuiApp::mapCanvas_keyPressed( QKeyEvent *e )
{
  // Delete selected features when it is possible and KeyEvent was not managed by current MapTool
  if (( e->key() == Qt::Key_Backspace || e->key() == Qt::Key_Delete ) && e->isAccepted() )
  {
//    deleteSelected();
//    toggleExcludes();
  }
}

void MBGuiApp::updateUndoActions()
{
  bool canUndo = false, canRedo = false;
  QgsMapLayer* layer = activeLayer_xy();
//  QgsMapLayer* layer_yz = activeLayer_yz()
  if ( layer )
  {
    QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer *>( layer );
    if ( vlayer && vlayer->isEditable() )
    {
      canUndo = vlayer->undoStack()->canUndo();
      canRedo = vlayer->undoStack()->canRedo();
    }
  }
//  if ( layer_yz )
//  {
//    QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer *>( layer_yz );
//    if ( vlayer && vlayer->isEditable() )
//    {
//      canUndo = vlayer->undoStack()->canUndo();
//      canRedo = vlayer->undoStack()->canRedo();
//    }
//  }
  mActionUndo->setEnabled( canUndo );
  mActionRedo->setEnabled( canRedo );
}
// add project directory to python path
void MBGuiApp::projectChanged( const QDomDocument &doc )
{
  Q_UNUSED( doc );
  QgsProject *project = qobject_cast<QgsProject*>( sender() );
  if ( !project )
    return;

  QFileInfo fi( project->fileName() );
  if ( !fi.exists() )
    return;

  static QString prevProjectDir = QString::null;

  if ( prevProjectDir == fi.canonicalPath() )
    return;

  QString expr;
  if ( !prevProjectDir.isNull() )
  {
    QString prev = prevProjectDir;
    expr = QString( "sys.path.remove(u'%1'); " ).arg( prev.replace( '\'', "\\'" ) );
  }

  prevProjectDir = fi.canonicalPath();

  QString prev = prevProjectDir;
  expr += QString( "sys.path.append(u'%1')" ).arg( prev.replace( '\'', "\\'" ) );

//  QgsPythonRunner::run( expr );
}

void MBGuiApp::writeProject( QDomDocument &doc )
{
  // QGIS server does not use QgsProject for loading of QGIS project.
  // In order to allow reading of new projects, let's also write the original <legend> tag to the project.
  // Ideally the server should be ported to new layer tree implementation, but that requires
  // non-trivial changes to the server components.
  // The <legend> tag is ignored by QGIS application in >= 2.4 and this way also the new project files
  // can be opened in older versions of QGIS without loosing information about layer groups.

  QgsLayerTreeNode* clonedRoot = QgsProject::instance()->layerTreeRoot()->clone();
  QgsLayerTreeUtils::replaceChildrenOfEmbeddedGroups( QgsLayerTree::toGroup( clonedRoot ) );
  QgsLayerTreeUtils::updateEmbeddedGroupsProjectPath( QgsLayerTree::toGroup( clonedRoot ) ); // convert absolute paths to relative paths if required
  QDomElement oldLegendElem = QgsLayerTreeUtils::writeOldLegend( doc, QgsLayerTree::toGroup( clonedRoot ),
                              mLayerTreeCanvasBridge->hasCustomLayerOrder(), mLayerTreeCanvasBridge->customLayerOrder() );
  delete clonedRoot;
  doc.firstChildElement( "qgis" ).appendChild( oldLegendElem );

  QgsProject::instance()->writeEntry( "Legend", "filterByMap", static_cast< bool >( layerTreeView()->layerTreeModel()->legendFilterMapSettings() ) );

  projectChanged( doc );
}


void MBGuiApp::readProject( const QDomDocument &doc )
{
  projectChanged( doc );

  // force update of canvas, without automatic changes to extent and OTF projections
  bool autoEnableCrsTransform = mLayerTreeCanvasBridge->autoEnableCrsTransform();
  bool autoSetupOnFirstLayer = mLayerTreeCanvasBridge->autoSetupOnFirstLayer();
  mLayerTreeCanvasBridge->setAutoEnableCrsTransform( false );
  mLayerTreeCanvasBridge->setAutoSetupOnFirstLayer( false );

  mLayerTreeCanvasBridge->setCanvasLayers();

  if ( autoEnableCrsTransform )
    mLayerTreeCanvasBridge->setAutoEnableCrsTransform( true );

  if ( autoSetupOnFirstLayer )
    mLayerTreeCanvasBridge->setAutoSetupOnFirstLayer( true );
}

void MBGuiApp::showLayerProperties( QgsMapLayer *ml )
{
  /*
  TODO: Consider reusing the property dialogs again.
  Sometimes around mid 2005, the property dialogs were saved for later reuse;
  this resulted in a time savings when reopening the dialog. The code below
  cannot be used as is, however, simply by saving the dialog pointer here.
  Either the map layer needs to be passed as an argument to sync or else
  a separate copy of the dialog pointer needs to be stored with each layer.
  */

  if ( !ml )
    return;

  if ( !QgsProject::instance()->layerIsEmbedded( ml->id() ).isEmpty() )
  {
    return; //don't show properties of embedded layers
  }

  if ( ml->type() == QgsMapLayer::RasterLayer )
  {
#if 0 // See note above about reusing this
    QgsRasterLayerProperties *rlp = nullptr;
    if ( rlp )
    {
      rlp->sync();
    }
    else
    {
      rlp = new QgsRasterLayerProperties( ml, canvas_depths, this );
//      rlp_error = new QgsRasterLayerProperties( ml, canvas_error, this );
//      rlp_nei = new QgsRasterLayerProperties( ml, canvas_nei, this );
//      rlp_rei = new QgsRasterLayerProperties( ml, canvas_rei, this );
//      rlp_mean = new QgsRasterLayerProperties( ml, canvas_mean, this );
      // handled by rendererChanged() connect( rlp, SIGNAL( refreshLegend( QString, bool ) ), mLayerTreeView, SLOT( refreshLayerSymbology( QString ) ) );
    }
#else
    QgsRasterLayerProperties *rlp = new QgsRasterLayerProperties( ml, canvas_depths, this );
    QgsRasterLayerProperties *rlp_error = new QgsRasterLayerProperties( ml, canvas_error, this );
    QgsRasterLayerProperties *rlp_nei = new QgsRasterLayerProperties( ml, canvas_nei, this );
    QgsRasterLayerProperties *rlp_rei = new QgsRasterLayerProperties( ml, canvas_rei, this );
    QgsRasterLayerProperties *rlp_mean = new QgsRasterLayerProperties( ml, canvas_mean, this );
#endif

    rlp->exec();
    delete rlp; // delete since dialog cannot be reused without updating code
  }
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
//  }
}
/*void MBGuiApp::namSetup()
{
  QgsNetworkAccessManager *nam = QgsNetworkAccessManager::instance();

  connect( nam, SIGNAL( authenticationRequired( QNetworkReply *, QAuthenticator * ) ),
           this, SLOT( namAuthenticationRequired( QNetworkReply *, QAuthenticator * ) ) );

  connect( nam, SIGNAL( proxyAuthenticationRequired( const QNetworkProxy &, QAuthenticator * ) ),
           this, SLOT( namProxyAuthenticationRequired( const QNetworkProxy &, QAuthenticator * ) ) );

  connect( nam, SIGNAL( requestTimedOut( QNetworkReply* ) ),
           this, SLOT( namRequestTimedOut( QNetworkReply* ) ) );

#ifndef QT_NO_OPENSSL
  connect( nam, SIGNAL( sslErrors( QNetworkReply *, const QList<QSslError> & ) ),
           this, SLOT( namSslErrors( QNetworkReply *, const QList<QSslError> & ) ) );
#endif
}
*/
/*void MBGuiApp::namAuthenticationRequired( QNetworkReply *inReply, QAuthenticator *auth )
{
  QPointer<QNetworkReply> reply( inReply );
  Q_ASSERT( qApp->thread() == QThread::currentThread() );

  QString username = auth->user();
  QString password = auth->password();

  if ( username.isEmpty() && password.isEmpty() && reply->request().hasRawHeader( "Authorization" ) )
  {
    QByteArray header( reply->request().rawHeader( "Authorization" ) );
    if ( header.startsWith( "Basic " ) )
    {
      QByteArray auth( QByteArray::fromBase64( header.mid( 6 ) ) );
      int pos = auth.indexOf( ':' );
      if ( pos >= 0 )
      {
        username = auth.left( pos );
        password = auth.mid( pos + 1 );
      }
    }
  }

  for ( ;; )
  {
    bool ok;

    {
      QMutexLocker lock( QgsCredentials::instance()->mutex() );
      ok = QgsCredentials::instance()->get(
             QString( "%1 at %2" ).arg( auth->realm(), reply->url().host() ),
             username, password,
             tr( "Authentication required" ) );
    }
    if ( !ok )
      return;

    if ( reply.isNull() || reply->isFinished() )
      return;

    if ( auth->user() != username || ( password != auth->password() && !password.isNull() ) )
      break;

    // credentials didn't change - stored ones probably wrong? clear password and retry
    {
      QMutexLocker lock( QgsCredentials::instance()->mutex() );
      QgsCredentials::instance()->put(
        QString( "%1 at %2" ).arg( auth->realm(), reply->url().host() ),
        username, QString::null );
    }
  }

  // save credentials
  {
    QMutexLocker lock( QgsCredentials::instance()->mutex() );
    QgsCredentials::instance()->put(
      QString( "%1 at %2" ).arg( auth->realm(), reply->url().host() ),
      username, password
    );
  }

  auth->setUser( username );
  auth->setPassword( password );
}
*/
/*void MBGuiApp::namProxyAuthenticationRequired( const QNetworkProxy &proxy, QAuthenticator *auth )
{
  QSettings settings;
  if ( !settings.value( "proxy/proxyEnabled", false ).toBool() ||
       settings.value( "proxy/proxyType", "" ).toString() == "DefaultProxy" )
  {
    auth->setUser( "" );
    return;
  }

  QString username = auth->user();
  QString password = auth->password();

  for ( ;; )
  {
    bool ok;

    {
      QMutexLocker lock( QgsCredentials::instance()->mutex() );
      ok = QgsCredentials::instance()->get(
             QString( "proxy %1:%2 [%3]" ).arg( proxy.hostName() ).arg( proxy.port() ).arg( auth->realm() ),
             username, password,
             tr( "Proxy authentication required" ) );
    }
    if ( !ok )
      return;

    if ( auth->user() != username || ( password != auth->password() && !password.isNull() ) )
      break;

    // credentials didn't change - stored ones probably wrong? clear password and retry
    {
      QMutexLocker lock( QgsCredentials::instance()->mutex() );
      QgsCredentials::instance()->put(
        QString( "proxy %1:%2 [%3]" ).arg( proxy.hostName() ).arg( proxy.port() ).arg( auth->realm() ),
        username, QString::null );
    }
  }

  {
    QMutexLocker lock( QgsCredentials::instance()->mutex() );
    QgsCredentials::instance()->put(
      QString( "proxy %1:%2 [%3]" ).arg( proxy.hostName() ).arg( proxy.port() ).arg( auth->realm() ),
      username, password
    );
  }

  auth->setUser( username );
  auth->setPassword( password );
}*/
/*#ifndef QT_NO_OPENSSL
void MBGuiApp::namSslErrors( QNetworkReply *reply, const QList<QSslError> &errors )
{
  // stop the timeout timer, or app crashes if the user (or slot) takes longer than
  // singleshot timeout and tries to update the closed QNetworkReply
  QTimer *timer = reply->findChild<QTimer *>( "timeoutTimer" );
  if ( timer )
  {
    QgsDebugMsg( "Stopping network reply timeout" );
    timer->stop();
  }

  QString requesturl = reply->request().url().toString();
  QgsDebugMsg( QString( "SSL errors occurred accessing URL:\n%1" ).arg( requesturl ) );

  QString hostport( QString( "%1:%2" )
                    .arg( reply->url().host() )
                    .arg( reply->url().port() != -1 ? reply->url().port() : 443 )
                    .trimmed() );
  QString digest( QgsAuthCertUtils::shaHexForCert( reply->sslConfiguration().peerCertificate() ) );
  QString dgsthostport( QString( "%1:%2" ).arg( digest, hostport ) );

  const QHash<QString, QSet<QSslError::SslError> > &errscache( QgsAuthManager::instance()->getIgnoredSslErrorCache() );

  if ( errscache.contains( dgsthostport ) )
  {
    QgsDebugMsg( QString( "Ignored SSL errors cahced item found, ignoring errors if they match for %1" ).arg( hostport ) );
    const QSet<QSslError::SslError>& errenums( errscache.value( dgsthostport ) );
    bool ignore = !errenums.isEmpty();
    int errmatched = 0;
    if ( ignore )
    {
      Q_FOREACH ( const QSslError& error, errors )
      {
        if ( error.error() == QSslError::NoError )
          continue;

        bool errmatch = errenums.contains( error.error() );
        ignore = ignore && errmatch;
        errmatched += errmatch ? 1 : 0;
      }
    }

    if ( ignore && errenums.size() == errmatched )
    {
      QgsDebugMsg( QString( "Errors matched cached item's, ignoring all for %1" ).arg( hostport ) );
      reply->ignoreSslErrors();
      return;
    }

    QgsDebugMsg( QString( "Errors %1 for cached item for %2" )
                 .arg( errenums.isEmpty() ? "not found" : "did not match",
                       hostport ) );
  }


  QgsAuthSslErrorsDialog *dlg = new QgsAuthSslErrorsDialog( reply, errors, this, digest, hostport );
  dlg->setWindowModality( Qt::ApplicationModal );
  dlg->resize( 580, 512 );
  if ( dlg->exec() )
  {
    if ( reply )
    {
      QgsDebugMsg( QString( "All SSL errors ignored for %1" ).arg( hostport ) );
      reply->ignoreSslErrors();
    }
  }
  dlg->deleteLater();

  // restart network request timeout timer
  if ( reply )
  {
    QSettings s;
    QTimer *timer = reply->findChild<QTimer *>( "timeoutTimer" );
    if ( timer )
    {
      QgsDebugMsg( "Restarting network reply timeout" );
      timer->setSingleShot( true );
      timer->start( s.value( "/qgis/networkAndProxy/networkTimeout", "60000" ).toInt() );
    }
  }
}
#endif*/
/*void MBGuiApp::namRequestTimedOut( QNetworkReply *reply )
{
  Q_UNUSED( reply );
  QLabel *msgLabel = new QLabel( tr( "A network request timed out, any data received is likely incomplete." ) +
                                 tr( " Please check the <a href=\"#messageLog\">message log</a> for further info." ), messageBar() );
  msgLabel->setWordWrap( true );
  connect( msgLabel, SIGNAL( linkActivated( QString ) ), mLogDock, SLOT( show() ) );
  messageBar()->pushItem( new QgsMessageBarItem( msgLabel, QgsMessageBar::WARNING, messageTimeout() ) );
}*/
/*void MBGuiApp::namUpdate()
{
  QgsNetworkAccessManager::instance()->setupDefaultProxyAndCache();
}*/
/*void MBGuiApp::masterPasswordSetup()
{
  connect( QgsAuthManager::instance(), SIGNAL( messageOut( const QString&, const QString&, QgsAuthManager::MessageLevel ) ),
           this, SLOT( authMessageOut( const QString&, const QString&, QgsAuthManager::MessageLevel ) ) );
  connect( QgsAuthManager::instance(), SIGNAL( authDatabaseEraseRequested() ),
           this, SLOT( eraseAuthenticationDatabase() ) );
}*/
/*void MBGuiApp::eraseAuthenticationDatabase()
{
  // First check if now is a good time to interact with the user, e.g. project is done loading.
  // If not, ask QgsAuthManager to re-emit authDatabaseEraseRequested from the schedule timer.
  // No way to know if user interaction will interfere with plugins loading layers.

  if ( !QgsProject::instance()->fileName().isNull() ) // a non-blank project is loaded
  {
    // Apparently, as of QGIS 2.9, the only way to query that the project is in a
    // layer-loading state is via a custom property of the project's layer tree.
    QgsLayerTreeGroup *layertree( QgsProject::instance()->layerTreeRoot() );
    if ( layertree && layertree->customProperty( "loading" ).toBool() )
    {
      QgsDebugMsg( "Project loading, skipping auth db erase" );
      QgsAuthManager::instance()->setScheduledAuthDbEraseRequestEmitted( false );
      return;
    }
  }

  // TODO: Check is Browser panel is also still loading?
  //       It has auto-connections in parallel (if tree item is expanded), though
  //       such connections with possible master password requests *should* be ignored
  //       when there is an authentication db erase scheduled.

  // This funtion should tell QgsAuthManager to stop any erase db schedule timer,
  // *after* interacting with the user
  QgsAuthGuiUtils::eraseAuthenticationDatabase( messageBar(), messageTimeout(), this );
}*/
/*void MBGuiApp::authMessageOut( const QString& message, const QString& authtag, QgsAuthManager::MessageLevel level )
{
  // only if main window is active window
  if ( qApp->activeWindow() != this )
    return;

  int levelint = static_cast< int >( level );
  messageBar()->pushMessage( authtag, message, static_cast< QgsMessageBar::MessageLevel >( levelint ), 7 );
}*/

void MBGuiApp::completeInitialization()
{
  emit initializationCompleted();
}

void MBGuiApp::toolButtonActionTriggered( QAction *action )
{
  QToolButton *bt = qobject_cast<QToolButton *>( sender() );
  if ( !bt )
    return;

  QSettings settings;
  if ( action == mActionSelectFeatures )
    settings.setValue( "/UI/selectTool", 1 );
  else if ( action == mActionSelectRadius )
    settings.setValue( "/UI/selectTool", 2 );
  else if ( action == mActionSelectPolygon )
    settings.setValue( "/UI/selectTool", 3 );
  else if ( action == mActionSelectFreehand )
    settings.setValue( "/UI/selectTool", 4 );
  else if ( action == mActionMeasure )
    settings.setValue( "/UI/measureTool", 0 );
  else if ( action == mActionMeasureArea )
    settings.setValue( "/UI/measureTool", 1 );
  else if ( action == mActionMeasureAngle )
    settings.setValue( "/UI/measureTool", 2 );
//  else if ( action == mActionTextAnnotation )
//    settings.setValue( "/UI/annotationTool", 0 );
//  else if ( action == mActionFormAnnotation )
//    settings.setValue( "/UI/annotationTool", 1 );
//  else if ( action == mActionHtmlAnnotation )
//    settings.setValue( "/UI/annotationTool", 2 );
//  else if ( action == mActionSvgAnnotation )
//    settings.setValue( "UI/annotationTool", 3 );
//  else if ( action == mActionAnnotation )
//    settings.setValue( "/UI/annotationTool", 4 );
//  else if ( action == mActionNewSpatiaLiteLayer )
//    settings.setValue( "/UI/defaultNewLayer", 0 );
//  else if ( action == mActionNewVectorLayer )
//    settings.setValue( "/UI/defaultNewLayer", 1 );
//  else if ( action == mActionNewMemoryLayer )
//    settings.setValue( "/UI/defaultNewLayer", 2 );
  bt->setDefaultAction( action );
}

QMenu* MBGuiApp::createPopupMenu()
{
  QMenu* menu = QMainWindow::createPopupMenu();
  QList< QAction* > al = menu->actions();
  QList< QAction* > panels, toolbars;

  if ( !al.isEmpty() )
  {
    bool found = false;
    for ( int i = 0; i < al.size(); ++i )
    {
      if ( al[ i ]->isSeparator() )
      {
        found = true;
        continue;
      }

      if ( !found )
      {
        panels.append( al[ i ] );
      }
      else
      {
        toolbars.append( al[ i ] );
      }
    }

    qSort( panels.begin(), panels.end(), cmpByText_ );
    QWidgetAction* panelstitle = new QWidgetAction( menu );
    QLabel* plabel = new QLabel( QString( "<b>%1</b>" ).arg( tr( "Panels" ) ) );
    plabel->setMargin( 3 );
    plabel->setAlignment( Qt::AlignHCenter );
    panelstitle->setDefaultWidget( plabel );
    menu->addAction( panelstitle );
    Q_FOREACH ( QAction* a, panels )
    {
      menu->addAction( a );
    }
    menu->addSeparator();
    QWidgetAction* toolbarstitle = new QWidgetAction( menu );
    QLabel* tlabel = new QLabel( QString( "<b>%1</b>" ).arg( tr( "Toolbars" ) ) );
    tlabel->setMargin( 3 );
    tlabel->setAlignment( Qt::AlignHCenter );
    toolbarstitle->setDefaultWidget( tlabel );
    menu->addAction( toolbarstitle );
    qSort( toolbars.begin(), toolbars.end(), cmpByText_ );
    Q_FOREACH ( QAction* a, toolbars )
    {
      menu->addAction( a );
    }
  }

  return menu;
}

/*void MBGuiApp::osmDownloadDialog()
{
  QgsOSMDownloadDialog dlg;
  dlg.exec();
}
*/
/*void MBGuiApp::osmImportDialog()
{
  QgsOSMImportDialog dlg;
  dlg.exec();
}
*/
/*void MBGuiApp::osmExportDialog()
{
  QgsOSMExportDialog dlg;
  dlg.exec();
}
*/
void MBGuiApp::showStatisticsDockWidget()
{
  mStatisticalSummaryDockWidget->show();
  mStatisticalSummaryDockWidget->raise();
}

void MBGuiApp::onLayerError( const QString& msg )
{
  QgsVectorLayer* layer = qobject_cast<QgsVectorLayer*>( sender() );

  Q_ASSERT( layer );

  mInfoBar->pushCritical( tr( "Layer %1" ).arg( layer->name() ), msg );
}


/*void MBGuiApp::addUserInputWidget( QWidget *widget )
{
  mUserInputDockWidget->addUserInputWidget( widget );
}*/




