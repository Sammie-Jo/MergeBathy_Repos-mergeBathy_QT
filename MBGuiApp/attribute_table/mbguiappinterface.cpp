/***************************************************************************
                          MBGuiAppInterface.cpp
                          Interface class for accessing exposed functions
                          in MbguiApp
                             -------------------
    copyright            : (C) 2002 by Gary E.Sherman
    email                : sherman at mrcc dot com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QFileInfo>
#include <QString>
#include <QMenu>
#include <QDialog>
#include <QAbstractButton>
#include <QSignalMapper>
#include <QTimer>
//#include <QUiLoader>

#include "mbguiinterface.h"
//#include "qgisappstylesheet.h"
#include "mbguiapp.h"
//#include "qgscomposer.h"
#include "qgscomposerview.h"
#include "qgsmaplayer.h"
#include "qgsmaplayerregistry.h"
//#include "qgsmaptooladvanceddigitizing.h"
#include "qgsmapcanvas.h"
#include "qgsproject.h"
#include "qgslayertreeview.h"
//#include "qgsshortcutsmanager.h"
#include "qgsattributedialog.h"
#include "qgsfield.h"
#include "qgsvectordataprovider.h"
#include "qgsfeatureaction.h"
#include "qgsattributeaction.h"
#include "qgsattributetabledialog.h"


MBGuiAppInterface::MBGuiAppInterface( MBGuiApp * _qgis )
    : qgis( _qgis )
    , mTimer( nullptr )
    , legendIface( _qgis->layerTreeView() )
    , pluginManagerIface( _qgis->pluginManager() )
{
  // connect signals
  connect( qgis->layerTreeView(), SIGNAL( currentLayerChanged( QgsMapLayer * ) ),
           this, SIGNAL( currentLayerChanged( QgsMapLayer * ) ) );
  connect( qgis, SIGNAL( currentThemeChanged( QString ) ),
           this, SIGNAL( currentThemeChanged( QString ) ) );
  connect( qgis, SIGNAL( composerAdded( QgsComposerView* ) ),
           this, SIGNAL( composerAdded( QgsComposerView* ) ) );
  connect( qgis, SIGNAL( composerWillBeRemoved( QgsComposerView* ) ),
           this, SIGNAL( composerWillBeRemoved( QgsComposerView* ) ) );
  connect( qgis, SIGNAL( composerRemoved( QgsComposerView* ) ),
           this, SIGNAL( composerRemoved( QgsComposerView* ) ) );
  connect( qgis, SIGNAL( initializationCompleted() ),
           this, SIGNAL( initializationCompleted() ) );
  connect( qgis, SIGNAL( newProject() ),
           this, SIGNAL( newProjectCreated() ) );
  connect( qgis, SIGNAL( projectRead() ),
           this, SIGNAL( projectRead() ) );
  connect( qgis, SIGNAL( layerSavedAs( QgsMapLayer*, QString ) ),
           this, SIGNAL( layerSavedAs( QgsMapLayer*, QString ) ) );
}

MBGuiAppInterface::~MBGuiAppInterface()
{
}

QgsLegendInterface* MBGuiAppInterface::legendInterface()
{
  return &legendIface;
}

QgsPluginManagerInterface* MBGuiAppInterface::pluginManagerInterface()
{
  return &pluginManagerIface;
}

QgsLayerTreeView*MBGuiAppInterface::layerTreeView()
{
  return qgis->layerTreeView();
}

void MBGuiAppInterface::zoomFull()
{
  qgis->zoomFull();
}

void MBGuiAppInterface::zoomToPrevious()
{
  qgis->zoomToPrevious();
}

void MBGuiAppInterface::zoomToNext()
{
  qgis->zoomToNext();
}

void MBGuiAppInterface::zoomToActiveLayer()
{
  qgis->zoomToLayerExtent();
}

QgsVectorLayer* MBGuiAppInterface::addVectorLayer( const QString& vectorLayerPath, const QString& baseName, const QString& providerKey )
{
  QString nonNullBaseBame = baseName;
  if ( nonNullBaseBame.isEmpty() )
  {
    QFileInfo fi( vectorLayerPath );
    nonNullBaseBame = fi.completeBaseName();
  }
  return qgis->addVectorLayer( vectorLayerPath, nonNullBaseBame, providerKey );
}

QgsRasterLayer* MBGuiAppInterface::addRasterLayer( const QString& rasterLayerPath, const QString& baseName )
{
  QString nonNullBaseBame = baseName;
  if ( nonNullBaseBame.isEmpty() )
  {
    QFileInfo fi( rasterLayerPath );
    nonNullBaseBame = fi.completeBaseName();
  }
  return qgis->addRasterLayer( rasterLayerPath, nonNullBaseBame );
}

QgsRasterLayer* MBGuiAppInterface::addRasterLayer( const QString& url, const QString& baseName, const QString& providerKey )
{
  return qgis->addRasterLayer( url, baseName, providerKey );
}

bool MBGuiAppInterface::addProject( const QString& theProjectName )
{
  return qgis->addProject( theProjectName );
}

void MBGuiAppInterface::newProject( bool thePromptToSaveFlag )
{
  qgis->fileNew( thePromptToSaveFlag );
}

QgsMapLayer *MBGuiAppInterface::activeLayer()
{
  return qgis->activeLayer();
}

bool MBGuiAppInterface::setActiveLayer( QgsMapLayer *layer )
{
  return qgis->setActiveLayer( layer );
}

void MBGuiAppInterface::addPluginToMenu( const QString& name, QAction* action )
{
  qgis->addPluginToMenu( name, action );
}

void MBGuiAppInterface::insertAddLayerAction( QAction *action )
{
  qgis->insertAddLayerAction( action );
}

void MBGuiAppInterface::removeAddLayerAction( QAction *action )
{
  qgis->removeAddLayerAction( action );
}

void MBGuiAppInterface::removePluginMenu( const QString& name, QAction* action )
{
  qgis->removePluginMenu( name, action );
}

void MBGuiAppInterface::addPluginToDatabaseMenu( const QString& name, QAction* action )
{
  qgis->addPluginToDatabaseMenu( name, action );
}

void MBGuiAppInterface::removePluginDatabaseMenu( const QString& name, QAction* action )
{
  qgis->removePluginDatabaseMenu( name, action );
}

void MBGuiAppInterface::addPluginToRasterMenu( const QString& name, QAction* action )
{
  qgis->addPluginToRasterMenu( name, action );
}

void MBGuiAppInterface::removePluginRasterMenu( const QString& name, QAction* action )
{
  qgis->removePluginRasterMenu( name, action );
}

void MBGuiAppInterface::addPluginToVectorMenu( const QString& name, QAction* action )
{
  qgis->addPluginToVectorMenu( name, action );
}

void MBGuiAppInterface::removePluginVectorMenu( const QString& name, QAction* action )
{
  qgis->removePluginVectorMenu( name, action );
}

void MBGuiAppInterface::addPluginToWebMenu( const QString& name, QAction* action )
{
  qgis->addPluginToWebMenu( name, action );
}

void MBGuiAppInterface::removePluginWebMenu( const QString& name, QAction* action )
{
  qgis->removePluginWebMenu( name, action );
}

int MBGuiAppInterface::addToolBarIcon( QAction * qAction )
{
  return qgis->addPluginToolBarIcon( qAction );
}

QAction *MBGuiAppInterface::addToolBarWidget( QWidget* widget )
{
  return qgis->addPluginToolBarWidget( widget );
}

void MBGuiAppInterface::removeToolBarIcon( QAction *qAction )
{
  qgis->removePluginToolBarIcon( qAction );
}

int MBGuiAppInterface::addRasterToolBarIcon( QAction * qAction )
{
  return qgis->addRasterToolBarIcon( qAction );
}

QAction *MBGuiAppInterface::addRasterToolBarWidget( QWidget* widget )
{
  return qgis->addRasterToolBarWidget( widget );
}

void MBGuiAppInterface::removeRasterToolBarIcon( QAction *qAction )
{
  qgis->removeRasterToolBarIcon( qAction );
}

int MBGuiAppInterface::addVectorToolBarIcon( QAction * qAction )
{
  return qgis->addVectorToolBarIcon( qAction );
}

QAction *MBGuiAppInterface::addVectorToolBarWidget( QWidget* widget )
{
  return qgis->addVectorToolBarWidget( widget );
}

void MBGuiAppInterface::removeVectorToolBarIcon( QAction *qAction )
{
  qgis->removeVectorToolBarIcon( qAction );
}

int MBGuiAppInterface::addDatabaseToolBarIcon( QAction * qAction )
{
  return qgis->addDatabaseToolBarIcon( qAction );
}

QAction *MBGuiAppInterface::addDatabaseToolBarWidget( QWidget* widget )
{
  return qgis->addDatabaseToolBarWidget( widget );
}

void MBGuiAppInterface::removeDatabaseToolBarIcon( QAction *qAction )
{
  qgis->removeDatabaseToolBarIcon( qAction );
}

int MBGuiAppInterface::addWebToolBarIcon( QAction * qAction )
{
  return qgis->addWebToolBarIcon( qAction );
}

QAction *MBGuiAppInterface::addWebToolBarWidget( QWidget* widget )
{
  return qgis->addWebToolBarWidget( widget );
}

void MBGuiAppInterface::removeWebToolBarIcon( QAction *qAction )
{
  qgis->removeWebToolBarIcon( qAction );
}

QToolBar* MBGuiAppInterface::addToolBar( const QString& name )
{
  return qgis->addToolBar( name );
}

void MBGuiAppInterface::addToolBar( QToolBar *toolbar, Qt::ToolBarArea area )
{
  return qgis->addToolBar( toolbar, area );
}

void MBGuiAppInterface::openURL( const QString& url, bool useQgisDocDirectory )
{
  qgis->openURL( url, useQgisDocDirectory );
}

QgsMapCanvas * MBGuiAppInterface::mapCanvas()
{
  return qgis->mapCanvas();
}

QgsLayerTreeMapCanvasBridge* MBGuiAppInterface::layerTreeCanvasBridge()
{
  return qgis->layerTreeCanvasBridge();
}

QWidget * MBGuiAppInterface::mainWindow()
{
  return qgis;
}

QgsMessageBar * MBGuiAppInterface::messageBar()
{
  return qgis->messageBar();
}

void MBGuiAppInterface::openMessageLog()
{
  qgis->openMessageLog();
}


void MBGuiAppInterface::addUserInputWidget( QWidget *widget )
{
  qgis->addUserInputWidget( widget );
}

QList<QgsComposerView*> MBGuiAppInterface::activeComposers()
{
  QList<QgsComposerView*> composerViewList;
  if ( qgis )
  {
    const QSet<QgsComposer*> composerList = qgis->printComposers();
    QSet<QgsComposer*>::const_iterator it = composerList.constBegin();
    for ( ; it != composerList.constEnd(); ++it )
    {
      if ( *it )
      {
        QgsComposerView* v = ( *it )->view();
        if ( v )
        {
          composerViewList.push_back( v );
        }
      }
    }
  }
  return composerViewList;
}

QgsComposerView* MBGuiAppInterface::createNewComposer( const QString& title )
{
  QgsComposer* composerObj = nullptr;
  composerObj = qgis->createNewComposer( title );
  if ( composerObj )
  {
    return composerObj->view();
  }
  return nullptr;
}

QgsComposerView* MBGuiAppInterface::duplicateComposer( QgsComposerView* composerView, const QString& title )
{
  QgsComposer* composerObj = nullptr;
  composerObj = qobject_cast<QgsComposer *>( composerView->composerWindow() );
  if ( composerObj )
  {
    QgsComposer* dupComposer = qgis->duplicateComposer( composerObj, title );
    if ( dupComposer )
    {
      return dupComposer->view();
    }
  }
  return nullptr;
}

void MBGuiAppInterface::deleteComposer( QgsComposerView* composerView )
{
  composerView->composerWindow()->close();

  QgsComposer* composerObj = nullptr;
  composerObj = qobject_cast<QgsComposer *>( composerView->composerWindow() );
  if ( composerObj )
  {
    qgis->deleteComposer( composerObj );
  }
}

QMap<QString, QVariant> MBGuiAppInterface::defaultStyleSheetOptions()
{
  return qgis->styleSheetBuilder()->defaultOptions();
}

void MBGuiAppInterface::buildStyleSheet( const QMap<QString, QVariant>& opts )
{
  qgis->styleSheetBuilder()->buildStyleSheet( opts );
}

void MBGuiAppInterface::saveStyleSheetOptions( const QMap<QString, QVariant>& opts )
{
  qgis->styleSheetBuilder()->saveToSettings( opts );
}

QFont MBGuiAppInterface::defaultStyleSheetFont()
{
  return qgis->styleSheetBuilder()->defaultFont();
}

void MBGuiAppInterface::addDockWidget( Qt::DockWidgetArea area, QDockWidget * dockwidget )
{
  qgis->addDockWidget( area, dockwidget );
}

void MBGuiAppInterface::removeDockWidget( QDockWidget * dockwidget )
{
  qgis->removeDockWidget( dockwidget );
}


/*QgsAdvancedDigitizingDockWidget*MBGuiAppInterface::cadDockWidget()
{
  return qgis->cadDockWidget();
}
*/
void MBGuiAppInterface::showLayerProperties( QgsMapLayer *l )
{
  if ( l && qgis )
  {
    qgis->showLayerProperties( l );
  }
}

QDialog* MBGuiAppInterface::showAttributeTable( QgsVectorLayer *l, const QString& filterExpression )
{
  if ( l )
  {
    QgsAttributeTableDialog *dialog = new QgsAttributeTableDialog( l );
    dialog->setFilterExpression( filterExpression );
    dialog->show();
    return dialog;
  }
  return nullptr;
}

void MBGuiAppInterface::addWindow( QAction *action )
{
  qgis->addWindow( action );
}

void MBGuiAppInterface::removeWindow( QAction *action )
{
  qgis->removeWindow( action );
}

bool MBGuiAppInterface::registerMainWindowAction( QAction* action, const QString& defaultShortcut )
{
  return QgsShortcutsManager::instance()->registerAction( action, defaultShortcut );
}

bool MBGuiAppInterface::unregisterMainWindowAction( QAction* action )
{
  return QgsShortcutsManager::instance()->unregisterAction( action );
}

//! Menus
Q_DECL_DEPRECATED QMenu *MBGuiAppInterface::fileMenu() { return qgis->projectMenu(); }
QMenu *MBGuiAppInterface::projectMenu() { return qgis->projectMenu(); }
QMenu *MBGuiAppInterface::editMenu() { return qgis->editMenu(); }
QMenu *MBGuiAppInterface::viewMenu() { return qgis->viewMenu(); }
QMenu *MBGuiAppInterface::layerMenu() { return qgis->layerMenu(); }
QMenu *MBGuiAppInterface::newLayerMenu() { return qgis->newLayerMenu(); }
QMenu *MBGuiAppInterface::addLayerMenu() { return qgis->addLayerMenu(); }
QMenu *MBGuiAppInterface::settingsMenu() { return qgis->settingsMenu(); }
QMenu *MBGuiAppInterface::pluginMenu() { return qgis->pluginMenu(); }
QMenu *MBGuiAppInterface::rasterMenu() { return qgis->rasterMenu(); }
QMenu *MBGuiAppInterface::vectorMenu() { return qgis->vectorMenu(); }
QMenu *MBGuiAppInterface::databaseMenu() { return qgis->databaseMenu(); }
QMenu *MBGuiAppInterface::webMenu() { return qgis->webMenu(); }
QMenu *MBGuiAppInterface::firstRightStandardMenu() { return qgis->firstRightStandardMenu(); }
QMenu *MBGuiAppInterface::windowMenu() { return qgis->windowMenu(); }
QMenu *MBGuiAppInterface::helpMenu() { return qgis->helpMenu(); }

//! ToolBars
QToolBar *MBGuiAppInterface::fileToolBar() { return qgis->fileToolBar(); }
QToolBar *MBGuiAppInterface::layerToolBar() { return qgis->layerToolBar(); }
QToolBar *MBGuiAppInterface::mapNavToolToolBar() { return qgis->mapNavToolToolBar(); }
QToolBar *MBGuiAppInterface::digitizeToolBar() { return qgis->digitizeToolBar(); }
QToolBar *MBGuiAppInterface::advancedDigitizeToolBar() { return qgis->advancedDigitizeToolBar(); }
QToolBar *MBGuiAppInterface::attributesToolBar() { return qgis->attributesToolBar(); }
QToolBar *MBGuiAppInterface::pluginToolBar() { return qgis->pluginToolBar(); }
QToolBar *MBGuiAppInterface::helpToolBar() { return qgis->helpToolBar(); }
QToolBar *MBGuiAppInterface::rasterToolBar() { return qgis->rasterToolBar(); }
QToolBar *MBGuiAppInterface::vectorToolBar() { return qgis->vectorToolBar(); }
QToolBar *MBGuiAppInterface::databaseToolBar() { return qgis->databaseToolBar(); }
QToolBar *MBGuiAppInterface::webToolBar() { return qgis->webToolBar(); }

//! Project menu actions
QAction *MBGuiAppInterface::actionNewProject() { return qgis->actionNewProject(); }
QAction *MBGuiAppInterface::actionOpenProject() { return qgis->actionOpenProject(); }
QAction *MBGuiAppInterface::actionSaveProject() { return qgis->actionSaveProject(); }
QAction *MBGuiAppInterface::actionSaveProjectAs() { return qgis->actionSaveProjectAs(); }
QAction *MBGuiAppInterface::actionSaveMapAsImage() { return qgis->actionSaveMapAsImage(); }
QAction *MBGuiAppInterface::actionProjectProperties() { return qgis->actionProjectProperties(); }
QAction *MBGuiAppInterface::actionPrintComposer() { return qgis->actionNewPrintComposer(); }
QAction *MBGuiAppInterface::actionShowComposerManager() { return qgis->actionShowComposerManager(); }
QAction *MBGuiAppInterface::actionExit() { return qgis->actionExit(); }

//! Edit menu actions
QAction *MBGuiAppInterface::actionCutFeatures() { return qgis->actionCutFeatures(); }
QAction *MBGuiAppInterface::actionCopyFeatures() { return qgis->actionCopyFeatures(); }
QAction *MBGuiAppInterface::actionPasteFeatures() { return qgis->actionPasteFeatures(); }
QAction *MBGuiAppInterface::actionAddFeature() { return qgis->actionAddFeature(); }
QAction *MBGuiAppInterface::actionDeleteSelected() { return qgis->actionDeleteSelected(); }
QAction *MBGuiAppInterface::actionMoveFeature() { return qgis->actionMoveFeature(); }
QAction *MBGuiAppInterface::actionSplitFeatures() { return qgis->actionSplitFeatures(); }
QAction *MBGuiAppInterface::actionSplitParts() { return qgis->actionSplitParts(); }
QAction *MBGuiAppInterface::actionAddRing() { return qgis->actionAddRing(); }
QAction *MBGuiAppInterface::actionAddPart() { return qgis->actionAddPart(); }
QAction *MBGuiAppInterface::actionSimplifyFeature() { return qgis->actionSimplifyFeature(); }
QAction *MBGuiAppInterface::actionDeleteRing() { return qgis->actionDeleteRing(); }
QAction *MBGuiAppInterface::actionDeletePart() { return qgis->actionDeletePart(); }
QAction *MBGuiAppInterface::actionNodeTool() { return qgis->actionNodeTool(); }

//! View menu actions
QAction *MBGuiAppInterface::actionPan() { return qgis->actionPan(); }
QAction *MBGuiAppInterface::actionTouch() { return qgis->actionTouch(); }
QAction *MBGuiAppInterface::actionPanToSelected() { return qgis->actionPanToSelected(); }
QAction *MBGuiAppInterface::actionZoomIn() { return qgis->actionZoomIn(); }
QAction *MBGuiAppInterface::actionZoomOut() { return qgis->actionZoomOut(); }
QAction *MBGuiAppInterface::actionSelect() { return qgis->actionSelect(); }
QAction *MBGuiAppInterface::actionSelectRectangle() { return qgis->actionSelectRectangle(); }
QAction *MBGuiAppInterface::actionSelectPolygon() { return qgis->actionSelectPolygon(); }
QAction *MBGuiAppInterface::actionSelectFreehand() { return qgis->actionSelectFreehand(); }
QAction *MBGuiAppInterface::actionSelectRadius() { return qgis->actionSelectRadius(); }
QAction *MBGuiAppInterface::actionIdentify() { return qgis->actionIdentify(); }
QAction *MBGuiAppInterface::actionFeatureAction() { return qgis->actionFeatureAction(); }
QAction *MBGuiAppInterface::actionMeasure() { return qgis->actionMeasure(); }
QAction *MBGuiAppInterface::actionMeasureArea() { return qgis->actionMeasureArea(); }
QAction *MBGuiAppInterface::actionZoomFullExtent() { return qgis->actionZoomFullExtent(); }
QAction *MBGuiAppInterface::actionZoomToLayer() { return qgis->actionZoomToLayer(); }
QAction *MBGuiAppInterface::actionZoomToSelected() { return qgis->actionZoomToSelected(); }
QAction *MBGuiAppInterface::actionZoomLast() { return qgis->actionZoomLast(); }
QAction *MBGuiAppInterface::actionZoomNext() { return qgis->actionZoomNext(); }
QAction *MBGuiAppInterface::actionZoomActualSize() { return qgis->actionZoomActualSize(); }
QAction *MBGuiAppInterface::actionMapTips() { return qgis->actionMapTips(); }
QAction *MBGuiAppInterface::actionNewBookmark() { return qgis->actionNewBookmark(); }
QAction *MBGuiAppInterface::actionShowBookmarks() { return qgis->actionShowBookmarks(); }
QAction *MBGuiAppInterface::actionDraw() { return qgis->actionDraw(); }

//! Layer menu actions
QAction *MBGuiAppInterface::actionNewVectorLayer() { return qgis->actionNewVectorLayer(); }
QAction *MBGuiAppInterface::actionAddOgrLayer() { return qgis->actionAddOgrLayer(); }
QAction *MBGuiAppInterface::actionAddRasterLayer() { return qgis->actionAddRasterLayer(); }
QAction *MBGuiAppInterface::actionAddPgLayer() { return qgis->actionAddPgLayer(); }
QAction *MBGuiAppInterface::actionAddWmsLayer() { return qgis->actionAddWmsLayer(); }
QAction *MBGuiAppInterface::actionCopyLayerStyle() { return qgis->actionCopyLayerStyle(); }
QAction *MBGuiAppInterface::actionPasteLayerStyle() { return qgis->actionPasteLayerStyle(); }
QAction *MBGuiAppInterface::actionOpenTable() { return qgis->actionOpenTable(); }
QAction *MBGuiAppInterface::actionOpenFieldCalculator() { return qgis->actionOpenFieldCalculator(); }
QAction *MBGuiAppInterface::actionToggleEditing() { return qgis->actionToggleEditing(); }
QAction *MBGuiAppInterface::actionSaveActiveLayerEdits() { return qgis->actionSaveActiveLayerEdits(); }
QAction *MBGuiAppInterface::actionAllEdits() { return qgis->actionAllEdits(); }
QAction *MBGuiAppInterface::actionSaveEdits() { return qgis->actionSaveEdits(); }
QAction *MBGuiAppInterface::actionSaveAllEdits() { return qgis->actionSaveAllEdits(); }
QAction *MBGuiAppInterface::actionRollbackEdits() { return qgis->actionRollbackEdits(); }
QAction *MBGuiAppInterface::actionRollbackAllEdits() { return qgis->actionRollbackAllEdits(); }
QAction *MBGuiAppInterface::actionCancelEdits() { return qgis->actionCancelEdits(); }
QAction *MBGuiAppInterface::actionCancelAllEdits() { return qgis->actionCancelAllEdits(); }
QAction *MBGuiAppInterface::actionLayerSaveAs() { return qgis->actionLayerSaveAs(); }
QAction *MBGuiAppInterface::actionLayerSelectionSaveAs() { return nullptr; }
QAction *MBGuiAppInterface::actionRemoveLayer() { return qgis->actionRemoveLayer(); }
QAction *MBGuiAppInterface::actionDuplicateLayer() { return qgis->actionDuplicateLayer(); }
QAction *MBGuiAppInterface::actionLayerProperties() { return qgis->actionLayerProperties(); }
QAction *MBGuiAppInterface::actionAddToOverview() { return qgis->actionAddToOverview(); }
QAction *MBGuiAppInterface::actionAddAllToOverview() { return qgis->actionAddAllToOverview(); }
QAction *MBGuiAppInterface::actionRemoveAllFromOverview() { return qgis->actionRemoveAllFromOverview(); }
QAction *MBGuiAppInterface::actionHideAllLayers() { return qgis->actionHideAllLayers(); }
QAction *MBGuiAppInterface::actionShowAllLayers() { return qgis->actionShowAllLayers(); }
QAction *MBGuiAppInterface::actionHideSelectedLayers() { return qgis->actionHideSelectedLayers(); }
QAction *MBGuiAppInterface::actionShowSelectedLayers() { return qgis->actionShowSelectedLayers(); }

////! Plugin menu actions
//QAction *MBGuiAppInterface::actionManagePlugins() { return qgis->actionManagePlugins(); }
//QAction *MBGuiAppInterface::actionPluginListSeparator() { return qgis->actionPluginListSeparator(); }
//QAction *MBGuiAppInterface::actionShowPythonDialog() { return qgis->actionShowPythonDialog(); }

//! Settings menu actions
QAction *MBGuiAppInterface::actionToggleFullScreen() { return qgis->actionToggleFullScreen(); }
QAction *MBGuiAppInterface::actionOptions() { return qgis->actionOptions(); }
QAction *MBGuiAppInterface::actionCustomProjection() { return qgis->actionCustomProjection(); }

//! Help menu actions
QAction *MBGuiAppInterface::actionHelpContents() { return qgis->actionHelpContents(); }
QAction *MBGuiAppInterface::actionQgisHomePage() { return qgis->actionQgisHomePage(); }
QAction *MBGuiAppInterface::actionCheckQgisVersion() { return qgis->actionCheckQgisVersion(); }
QAction *MBGuiAppInterface::actionAbout() { return qgis->actionAbout(); }

bool MBGuiAppInterface::openFeatureForm( QgsVectorLayer *vlayer, QgsFeature &f, bool updateFeatureOnly, bool showModal )
{
  Q_UNUSED( updateFeatureOnly );
  if ( !vlayer )
    return false;

  QgsFeatureAction action( tr( "Attributes changed" ), f, vlayer, -1, -1, MbguiApp::instance() );
  if ( vlayer->isEditable() )
  {
    return action.editFeature( showModal );
  }
  else
  {
    action.viewFeatureForm();
    return true;
  }
}

void MBGuiAppInterface::preloadForm( const QString& uifile )
{
  QSignalMapper* signalMapper = new QSignalMapper( this );
  mTimer = new QTimer( this );

  connect( mTimer, SIGNAL( timeout() ), signalMapper, SLOT( map() ) );
  connect( signalMapper, SIGNAL( mapped( QString ) ), mTimer, SLOT( stop() ) );
  connect( signalMapper, SIGNAL( mapped( QString ) ), this, SLOT( cacheloadForm( QString ) ) );

  signalMapper->setMapping( mTimer, uifile );

  mTimer->start( 0 );
}

void MBGuiAppInterface::cacheloadForm( const QString& uifile )
{
  QFile file( uifile );

  if ( file.open( QFile::ReadOnly ) )
  {
    QUiLoader loader;

    QFileInfo fi( uifile );
    loader.setWorkingDirectory( fi.dir() );
    QWidget *myWidget = loader.load( &file );
    file.close();
    delete myWidget;
  }
}

QgsAttributeDialog* MBGuiAppInterface::getFeatureForm( QgsVectorLayer *l, QgsFeature &feature )
{
  QgsDistanceArea myDa;

  myDa.setSourceCrs( l->crs().srsid() );
  myDa.setEllipsoidalMode( MbguiApp::instance()->mapCanvas()->mapSettings().hasCrsTransformEnabled() );
  myDa.setEllipsoid( QgsProject::instance()->readEntry( "Measure", "/Ellipsoid", GEO_NONE ) );

  QgsAttributeEditorContext context;
  context.setDistanceArea( myDa );
  context.setVectorLayerTools( qgis->vectorLayerTools() );
  QgsAttributeDialog *dialog = new QgsAttributeDialog( l, &feature, false, nullptr, true, context );
  if ( !feature.isValid() )
  {
    dialog->setIsAddDialog( true );
  }
  return dialog;
}

QgsVectorLayerTools* MBGuiAppInterface::vectorLayerTools()
{
  return qgis->vectorLayerTools();
}

QList<QgsMapLayer *> MBGuiAppInterface::editableLayers( bool modified ) const
{
  return qgis->editableLayers( modified );
}

int MBGuiAppInterface::messageTimeout()
{
  return qgis->messageTimeout();
}
