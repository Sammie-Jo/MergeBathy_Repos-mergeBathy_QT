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
class QgsMapTool;

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

#include <QDockWidget>
#include <mymenuprovider.h>
#include <mycanvasview.h>

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
#include "qgsfeaturestore.h"
#include "qgsmaptip.h"
#include "attribute_table/qgsstatusbarcoordinateswidget.h"

//QT Includes
#include <QtGui>

//Local Includes
#include <ui_mainwindowbase.h>

class QgsVectorLayerTools;
class QgsTransactionGroup;
class QgsClipboard;
class QgsDoubleSpinBox;



//class DockWidget : public QDockWidget
//{
//    Q_OBJECT

//public:
//    DockWidget(QWidget *parent = 0) : QDockWidget(parent), dock(0) {}

//protected:
//    void contentsMousePressEvent(QMouseEvent *event);

//private:
//    void handleMouseClickEvent(QMouseEvent *event);
//    QDockWidget *dock;
//    QAction *act;
//};

class QtProperty;

//class CanvasView : public QtCanvasView
//{
//    Q_OBJECT
//public:
//    CanvasView(QWidget *parent = 0)
//        : QtCanvasView(parent), moving(0) { }
//    CanvasView(QtCanvas *canvas, QWidget *parent = 0)
//        : QtCanvasView(canvas, parent), moving(0) { }
//signals:
//    void itemClicked(QtCanvasItem *item);
//    void itemMoved(QtCanvasItem *item);
////    void toggleDock1();
////    void toggleDock2();
//protected:
//    void contentsMousePressEvent(QMouseEvent *event);
//    void contentsMouseDoubleClickEvent(QMouseEvent *event);
//    void contentsMouseMoveEvent(QMouseEvent* event);
//private:
//    void handleMouseClickEvent(QMouseEvent *event);
//    QPoint moving_start;
//    QtCanvasItem *moving;
//};
//********************************
// MyMenuProvider
//********************************
//class MyMenuProvider: public QObject, public QgsLayerTreeViewMenuProvider
//{
//    Q_OBJECT;
//public:
//    MyMenuProvider(QgsLayerTreeView *view, QUndoStack *undoStack);
//    ~MyMenuProvider();

//public slots:
//    void showExtent();
////    void rename();
//    void renameLayer();
//    void renameGroup();
//    void rename2();
//    void undoLast();
//    void redoLast();
//    void addLayer();
//    void addGroup();
//    void removeLayer();
//    void removeGroup();
//    void saveLayerAs();
//    void resetLayer();


//private:
//    QAction *zoomtolayerAction;
//    QgsLayerTreeView * view;
//    QMenu * m;
//    QMenu * createContextMenu();
//    QUndoStack *undoStack;
//};






//********************************
// MainWindow
//********************************
class MainWindow : public QMainWindow, private Ui::MainWindowBase
{
    Q_OBJECT;
    public:
        MainWindow(QWidget* parent = 0, Qt::WFlags fl = 0 );
        ~MainWindow();
        /**
         * Add a vector layer to the canvas, returns pointer to it
         */
        QgsVectorLayer *addVectorLayer( const QString& vectorLayerPath, const QString& baseName, const QString& providerKey );

        /** \brief overloaded version of the private addLayer method that takes a list of
         * file names instead of prompting user with a dialog.
         @param enc encoding type for the layer
        @param dataSourceType type of ogr datasource
         @returns true if successfully added layer
         */
        bool addVectorLayers( const QStringList &theLayerQStringList, const QString &enc, const QString &dataSourceType );

        /** Overloaded vesion of the private addRasterLayer()
          Method that takes a list of file names instead of prompting
          user with a dialog.
          @returns true if successfully added layer(s)
          */
        bool addRasterLayers( const QStringList &theLayerQStringList, bool guiWarning = true );

        /** Open a raster layer for the given file
          @returns false if unable to open a raster layer for rasterFile
          @note
          This is essentially a simplified version of the above
          */
        QgsRasterLayer *addRasterLayer( const QString &rasterFile, const QString &baseName, bool guiWarning = true );

        /** Returns and adjusted uri for the layer based on current and available CRS as well as the last selected image format
         * @note added in 2.8
         */
        QString crsAndFormatAdjustedLayerUri( const QString& uri, const QStringList& supportedCrs, const QStringList& supportedFormats ) const;

        /** Add a 'pre-made' map layer to the project */
        void addMapLayer( QgsMapLayer *theMapLayer );

        /** Set the extents of the map canvas */
        void setExtent( const QgsRectangle& theRect );
        //! Remove all layers from the map and legend - reimplements same method from MBGuiAppbase
        void removeAllLayers();
        /** Open a raster or vector file; ignore other files.
          Used to process a commandline argument, FileOpen or Drop event.
          Set interactive to true if it is ok to ask the user for information (mostly for
          when a vector layer has sublayers and we want to ask which sublayers to use).
          @returns true if the file is successfully opened
          */
        bool openLayer( const QString & fileName, bool allowInteractive = false );
        /** Open the specified project file; prompt to save previous project if necessary.
          Used to process a commandline argument, FileOpen or Drop event.
          */
        void openProject( const QString & fileName );

        void openLayerDefinition( const QString & filename );
        /** Opens a qgis project file
          @returns false if unable to open the project
          */
        bool addProject( const QString& projectFile );
        /** Convenience function to open either a project or a layer file.
          */
        void openFile( const QString & fileName );
        //!Overloaded version of the private function with same name that takes the imagename as a parameter
        void saveMapAsImage( const QString&, QPixmap * );

        /** Get the mapcanvas object from the app */
        QgsMapCanvas *mapCanvas();
        QgsMapCanvas *mapCanvas_xy();

        /** Return the messageBar object which allows displaying unobtrusive messages to the user.*/
        QgsMessageBar *messageBar();

        /** Open the message log dock widget **/
        void openMessageLog();

        /** Adds a widget to the user input tool bar.*/
        void addUserInputWidget( QWidget* widget );

        //! Set theme (icons)
        void setTheme( const QString& themeName = "default" );

        void setIconSizes( int size );

        //! Get stylesheet builder object for app and print composers
//        MBGuiAppStyleSheet *styleSheetBuilder();

        //! Setup the toolbar popup menus for a given theme
        void setupToolbarPopups( QString themeName );

        //! Returns a pointer to the internal clipboard
        QgsClipboard *clipboard();

        static MainWindow *instance() { return smInstance; }

        /** Add a dock widget to the main window. Overloaded from QMainWindow.
        * After adding the dock widget to the ui (by delegating to the QMainWindow
        * parent class, it will also add it to the View menu list of docks.*/
       void addDockWidget( Qt::DockWidgetArea area, QDockWidget *dockwidget );
       void removeDockWidget( QDockWidget *dockwidget );
       /** Add a toolbar to the main window. Overloaded from QMainWindow.
        * After adding the toolbar to the ui (by delegating to the QMainWindow
        * parent class, it will also add it to the View menu list of toolbars.*/
       QToolBar *addToolBar2( const QString& name );

       /** Add a toolbar to the main window. Overloaded from QMainWindow.
        * After adding the toolbar to the ui (by delegating to the QMainWindow
        * parent class, it will also add it to the View menu list of toolbars.
        * @note added in 2.3
        */
       void addToolBar2( QToolBar *toolBar, Qt::ToolBarArea area = Qt::TopToolBarArea );




        /** Get timeout for timed messages: default of 5 seconds */
        int messageTimeout();


    public slots:
        void zoomInMode();
        void zoomOutMode();
        void panMode();
        void selectMode();
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
        void tab1Changed();
        void tab2Changed();

        //! Returns map overview canvas
        QgsMapOverviewCanvas* mapOverviewCanvas() { return mOverviewCanvas; }

        //! show layer properties
        void showLayerProperties( QgsMapLayer *ml );

        QgsLayerTreeView *layerTreeView();

        /** Return vector layers in edit mode
         * @param modified whether to return only layers that have been modified
         * @returns list of layers in legend order, or empty list */
        QList<QgsMapLayer *> editableLayers( bool modified = false ) const;


        void initLayerTreeView();
        void createMapTips();

        void layerTreeViewDoubleClicked( const QModelIndex& index );
        //! Make sure the insertion point for new layers is up-to-date with the current item in layer tree view
        void updateNewLayerInsertionPoint();
        //! connected to layer tree registry bridge, selects first of the newly added map layers
        void autoSelectAddedLayer( QList<QgsMapLayer*> layers );
        void activeLayerChanged( QgsMapLayer *layer );
        //! Zoom to full extent
        void zoomFull();
        //! Zoom to the previous extent
        void zoomToPrevious();
        //! Zoom to the forward extent
        void zoomToNext();
        //! Zoom to selected features
        void zoomToSelected();
        //! Pan map to selected features
        void panToSelected();

        //! open the properties dialog for the currently selected layer
        void layerProperties();

        //! show the attribute table for the currently selected layer
        void attributeTable();

        void fieldCalculator();

        //! mark project dirty
        void markDirty();

        void layersWereAdded( const QList<QgsMapLayer*>& );

        /* layer will be removed - changed from removingLayer to removingLayers
           in 1.8.
         */
        void removingLayers( const QStringList& );

        //! starts/stops editing mode of the current layer
        void toggleEditing();

        //! starts/stops editing mode of a layer
        bool toggleEditing( QgsMapLayer *layer, bool allowCancel = true );

        /** Save edits for active vector layer and start new transactions */
        void saveActiveLayerEdits();

        /** Save edits of a layer
         * @param leaveEditable leave the layer in editing mode when done
         * @param triggerRepaint send layer signal to repaint canvas when done
         */
        void saveEdits( QgsMapLayer *layer, bool leaveEditable = true, bool triggerRepaint = true );

        /** Cancel edits for a layer
          * @param leaveEditable leave the layer in editing mode when done
          * @param triggerRepaint send layer signal to repaint canvas when done
          */
        void cancelEdits( QgsMapLayer *layer, bool leaveEditable = true, bool triggerRepaint = true );

        //! Save current edits for selected layer(s) and start new transaction(s)
        void saveEdits();

        /** Save edits for all layers and start new transactions */
        void saveAllEdits( bool verifyAction = true );

        /** Rollback current edits for selected layer(s) and start new transaction(s) */
        void rollbackEdits();

        /** Rollback edits for all layers and start new transactions */
        void rollbackAllEdits( bool verifyAction = true );

        /** Cancel edits for selected layer(s) and toggle off editing */
        void cancelEdits();

        /** Cancel all edits for all layers and toggle off editing */
        void cancelAllEdits( bool verifyAction = true );

        void updateUndoActions();

        //! cuts selected features on the active layer to the clipboard
        /**
           \param layerContainingSelection  The layer that the selection will be taken from
                                            (defaults to the active layer on the legend)
         */
        void editCut( QgsMapLayer *layerContainingSelection = nullptr );
        //! copies selected features on the active layer to the clipboard
        /**
           \param layerContainingSelection  The layer that the selection will be taken from
                                            (defaults to the active layer on the legend)
         */
        void editCopy( QgsMapLayer *layerContainingSelection = nullptr );

        //! copies features on the clipboard to the active layer
        /**
           \param destinationLayer  The layer that the clipboard will be pasted to
                                    (defaults to the active layer on the legend)
         */
        void editPaste( QgsMapLayer *destinationLayer = nullptr );

        //! copies features on the clipboard to a new vector layer
        void pasteAsNewVector();

        //! copies features on the clipboard to a new memory vector layer
        QgsVectorLayer *pasteAsNewMemoryVector( const QString & theLayerName = QString() );

        //! copies style of the active layer to the clipboard
        /**
           \param sourceLayer  The layer where the style will be taken from
                                            (defaults to the active layer on the legend)
         */
        void copyStyle( QgsMapLayer *sourceLayer = nullptr );

        //! pastes style on the clipboard to the active layer
        /**
           \param destinationLayer  The layer that the clipboard will be pasted to
                                    (defaults to the active layer on the legend)
         */
        void pasteStyle( QgsMapLayer *destinationLayer = nullptr );

        //! copies features to internal clipboard
        void copyFeatures( QgsFeatureStore & featureStore );

        QgsVectorLayerTools *vectorLayerTools() { return mVectorLayerTools; }


        /** Deletes the selected attributes for the currently selected vector layer*/
        void deleteSelected( QgsMapLayer *layer = nullptr, QWidget *parent = nullptr, bool promptConfirmation = false );

        void saveAsVectorFileGeneral( QgsVectorLayer* vlayer = nullptr, bool symbologyOption = true );

        /** Paste features from clipboard into a new memory layer.
         *  If no features are in clipboard an empty layer is returned.
         *  @return pointer to a new layer or 0 if failed
         */
        QgsVectorLayer * pasteToNewMemoryVector();



        /** Refreshes the state of the layer actions toolbar action
          * @note added in 2.1 */
        void refreshActionFeatureAction();

        QMenu *panelMenu() { return mPanelMenu; }

       bool autoTransaction() const;
       void setAutoTransaction( bool state );







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
        void viewMenuAboutToShow1();
        void viewMenuAboutToHide1();
        void viewMenuAboutToShow2();
        void viewMenuAboutToHide2();

        void itemClicked(QtCanvasItem *item);
        void itemMoved(QtCanvasItem *item);
        void valueChanged(QtProperty *property, double value);
        void valueChanged(QtProperty *property, const QString &value);
        void valueChanged(QtProperty *property, const QColor &value);
        void valueChanged(QtProperty *property, const QPoint &value);
        void valueChanged(QtProperty *property, const QSize &value);

        /** Alerts user when commit errors occurred */
        void commitError( QgsVectorLayer *vlayer );

        //! Toggle map tips on/off
        void toggleMapTips();

        //! Return pointer to the active layer
        QgsMapLayer *activeLayer();
        //! set the active layer
        bool setActiveLayer( QgsMapLayer * );

        /** Called when some layer's editing mode was toggled on/off */
            void layerEditStateChanged();

       /** Activates or deactivates actions depending on the current maplayer type.
           Is called from the legend when the current legend item has changed*/
       void activateDeactivateLayerRelatedActions( QgsMapLayer *layer );

    void selectionChanged( QgsMapLayer *layer );

       void showProgress( int theProgress, int theTotalSteps );
       void extentChanged();
       void showRotation();
       void showStatusMessage( const QString& theMessage );
       void displayMapToolMessage( const QString& message, QgsMessageBar::MessageLevel level = QgsMessageBar::INFO );
       void displayMessage( const QString& title, const QString& message, QgsMessageBar::MessageLevel level );
       void removeMapToolMessage();
       void updateMouseCoordinatePrecision();
//       void hasCrsTransformEnabled( bool theFlag );

        //! Identify feature(s) on the currently selected layer
        void identify();

        //! Run the default feature action on the current layer
        void doFeatureAction();
        //! Set the default feature action for the current layer
        void updateDefaultFeatureAction( QAction *action );
        //! Refresh the list of feature actions of the current layer
        void refreshFeatureActions();

        void clipboardChanged();

     signals:

        /** This signal is emitted when a layer has been saved using save as
          @note added in version 2.7
        */
        void layerSavedAs( QgsMapLayer* l, const QString& path );


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
        QAction *tab1ToggleAction;
        QAction *tab2ToggleAction;

        QAction *closeProjectAction;
        QAction *openAction;
        QAction *saveAction;
        QAction *saveAsAction;

QAction *mActionPan;
QAction *mActionPanToSelected;
QAction *mActionZoomIn;
QAction *mActionZoomOut;
QAction *mActionSelectFeatures;
QAction *mActionSelectPolygon;
QAction *mActionSelectFreehand;
QAction *mActionSelectRadius;
QAction *mActionDeselectAll;
QAction *mActionSelectAll;
QAction *mActionInvertSelection;
QAction *mActionSelectByExpression;
QAction *mActionIdentify;
QAction *mActionFeatureAction;
QAction *mActionMeasure;
QAction *mActionMeasureArea;
QAction *mActionMeasureAngle;
QAction *mActionZoomFullExtent;
QAction *mActionZoomToLayer;
QAction *mActionZoomToSelected;
QAction *mActionZoomLast;
QAction *mActionZoomNext;
QAction *mActionZoomActualSize;
QAction *mActionMapTips;
QAction *mActionNewBookmark;
QAction *mActionShowBookmarks;
QAction *mActionDraw;

QAction *mActionOpenTable;
QAction *mActionOpenFieldCalc;
QAction *mActionToggleEditing;
QAction *mActionSaveLayerEdits;
QAction *mActionSaveEdits;
QAction *mActionSaveAllEdits;
QAction *mActionRollbackEdits;
QAction *mActionRollbackAllEdits;
QAction *mActionCancelEdits;
QAction *mActionCancelAllEdits;
QAction *mActionLayerSaveAs;
QAction *mActionShowAllLayers;
QAction *mActionHideAllLayers;
QAction *mActionShowSelectedLayers;
QAction *mActionHideSelectedLayers;
QAction *mActionAllEdits;
QAction *mActionDeleteSelected;
QAction *mActionToggleFullScreen;
QAction *mActionPasteStyle;
QAction *mActionUndo;
QAction *mActionRedo;

// action groups ----------------------------------
    QActionGroup *mMapToolGroup;
    // menus ------------------------------------------

   #ifdef Q_OS_MAC
       QMenu *mWindowMenu;
   #endif
       QMenu *mPanelMenu;
       QMenu *mToolbarMenu;
       QToolBar *mFileToolBar;
       QToolBar *mLayerToolBar;
       QToolBar *mMapNavToolBar;
       QToolBar *mAttributesToolBar;
       QToolBar *mVectorToolBar;
       QMenu *mViewMenu;
       bool cmpByText( QAction* a, QAction* b );
       //! Menu that contains the list of actions of the selected vector layer
       QMenu *mFeatureActionMenu;
       //! Widget that will live on the statusbar to display "scale 1:"
       QLabel *mScaleLabel;
       //! Widget that will live on the statusbar to display scale value
//       QgsScaleComboBox *mScaleEdit;
       //! The validator for the mScaleEdit
       QValidator * mScaleEditValidator;

       //! Widget that will live in the statusbar to display and edit coords
       QgsStatusBarCoordinatesWidget *mCoordsEdit;

       //! Widget that will live on the statusbar to display "Rotation"
       QLabel *mRotationLabel;
       //! Widget that will live in the statusbar to display and edit rotation
       QgsDoubleSpinBox *mRotationEdit;
       //! The validator for the mCoordsEdit
       QValidator *mRotationEditValidator;
       //! Widget that will live in the statusbar to show progress of operations
       QProgressBar *mProgressBar;
       //! Widget used to suppress rendering
       QCheckBox *mRenderSuppressionCBox;

       // docks ------------------------------------------
       QDockWidget *mLayerTreeDock;
       QDockWidget *mLayerOrderDock;
       QDockWidget *mOverviewDock;
       QDockWidget *mpGpsDock;
       QDockWidget *mLogDock;

   #ifdef Q_OS_MAC
       //! Window menu action to select this window
       QAction *mWindowAction;
   #endif

       class Tools
       {
         public:

           Tools()
               : mZoomIn( nullptr )
               , mZoomOut( nullptr )
               , mPan( nullptr )
   #ifdef HAVE_TOUCH
               , mTouch( 0 )
   #endif
               , mIdentify( nullptr )
               , mFeatureAction( nullptr )
               , mMeasureDist( nullptr )
               , mMeasureArea( nullptr )
               , mMeasureAngle( nullptr )
               , mAddFeature( nullptr )
               , mCircularStringCurvePoint( nullptr )
               , mCircularStringRadius( nullptr )
               , mMoveFeature( nullptr )
               , mOffsetCurve( nullptr )
               , mReshapeFeatures( nullptr )
               , mSplitFeatures( nullptr )
               , mSplitParts( nullptr )
               , mSelect( nullptr )
               , mSelectFeatures( nullptr )
               , mSelectPolygon( nullptr )
               , mSelectFreehand( nullptr )
               , mSelectRadius( nullptr )
               , mVertexAdd( nullptr )
               , mVertexMove( nullptr )
               , mVertexDelete( nullptr )
               , mAddRing( nullptr )
               , mFillRing( nullptr )
               , mAddPart( nullptr )
               , mSimplifyFeature( nullptr )
               , mDeleteRing( nullptr )
               , mDeletePart( nullptr )
               , mNodeTool( nullptr )
               , mRotatePointSymbolsTool( nullptr )
               , mAnnotation( nullptr )
               , mFormAnnotation( nullptr )
               , mHtmlAnnotation( nullptr )
               , mSvgAnnotation( nullptr )
               , mTextAnnotation( nullptr )
               , mPinLabels( nullptr )
               , mShowHideLabels( nullptr )
               , mMoveLabel( nullptr )
               , mRotateFeature( nullptr )
               , mRotateLabel( nullptr )
               , mChangeLabelProperties( nullptr )
           {}

           QgsMapTool *mZoomIn;
           QgsMapTool *mZoomOut;
           QgsMapTool *mPan;
   #ifdef HAVE_TOUCH
           QgsMapTool *mTouch;
   #endif
           QgsMapTool *mIdentify;
           QgsMapTool *mFeatureAction;
           QgsMapTool *mMeasureDist;
           QgsMapTool *mMeasureArea;
           QgsMapTool *mMeasureAngle;
           QgsMapTool *mAddFeature;
           QgsMapTool *mCircularStringCurvePoint;
           QgsMapTool *mCircularStringRadius;
           QgsMapTool *mMoveFeature;
           QgsMapTool *mOffsetCurve;
           QgsMapTool *mReshapeFeatures;
           QgsMapTool *mSplitFeatures;
           QgsMapTool *mSplitParts;
           QgsMapTool *mSelect;
           QgsMapTool *mSelectFeatures;
           QgsMapTool *mSelectPolygon;
           QgsMapTool *mSelectFreehand;
           QgsMapTool *mSelectRadius;
           QgsMapTool *mVertexAdd;
           QgsMapTool *mVertexMove;
           QgsMapTool *mVertexDelete;
           QgsMapTool *mAddRing;
           QgsMapTool *mFillRing;
           QgsMapTool *mAddPart;
           QgsMapTool *mSimplifyFeature;
           QgsMapTool *mDeleteRing;
           QgsMapTool *mDeletePart;
           QgsMapTool *mNodeTool;
           QgsMapTool *mRotatePointSymbolsTool;
           QgsMapTool *mAnnotation;
           QgsMapTool *mFormAnnotation;
           QgsMapTool *mHtmlAnnotation;
           QgsMapTool *mSvgAnnotation;
           QgsMapTool *mTextAnnotation;
           QgsMapTool *mPinLabels;
           QgsMapTool *mShowHideLabels;
           QgsMapTool *mMoveLabel;
           QgsMapTool *mRotateFeature;
           QgsMapTool *mRotateLabel;
           QgsMapTool *mChangeLabelProperties;
       } mMapTools;

       //! Flag to indicate an edits save/rollback for active layer is in progress
       bool mSaveRollbackInProgress;
       //! map of transaction group: QPair( providerKey, connString ) -> transactionGroup
       QMap< QPair< QString, QString>, QgsTransactionGroup*> mTransactionGroups;



        QUndoStack *undoStack;
        QUndoView *undoView;

        QgsMapCanvas * canvas_xy;
        QgsMapCanvas * canvas_yz;
        QgsMapCanvas * canvas_error;
        QgsMapCanvas * canvas_rms;
        QgsMapCanvas * canvas_mean;
        QVBoxLayout  * mpLayout_canvas_xy;
        QVBoxLayout  * mpLayout_canvas_yz;
        QVBoxLayout  * mpLayout_canvas_error;
        QVBoxLayout  * mpLayout_canvas_rms;
        QVBoxLayout  * mpLayout_canvas_mean;
        QHBoxLayout  * mpLayout_CanvasEdits;
        QHBoxLayout  * mpLayout_CanvasResults;
        QGroupBox * hGroupBox_CanvasEdits;
        QGroupBox * hGroupBox_CanvasResults;
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

        QGroupBox * vGroupBox_mbparams_dock1;
        QVBoxLayout * mpLayout_mbparams_dock1;
        QGroupBox * vGroupBox_mbparams_dock2;
        QVBoxLayout * mpLayout_mbparams_dock2;
        QGroupBox * vGroupBox_mbparams_dock;
        QVBoxLayout * mpLayout_mbparams_dock;
        QTabWidget *tab;

        QToolBar * mpMapToolBar;
        QgsMapTool * mpPanTool;
        QgsMapTool * mpZoomInTool;
        QgsMapTool * mpZoomOutTool;
        QgsMapTool * mpSelectTool;

//        QgsMapTool * mpPanTool2;
//        QgsMapTool * mpZoomInTool2;
//        QgsMapTool * mpZoomOutTool2;

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
//        QgsLayerTreeGroup * root2;

//         MySetDataPathDialog * setDataPathDialog;
         QString globalDataPath;
         QString localDataPath;

        int openDataFile(QString myLayerPath, QString myLayerBaseName, QString myProviderName, QgsMapLayer &mypLayer, QgsFeatureIterator &features);
        int validateProvider(QString myProviderName);
        void readProjectSettings();

        void createMapTools();

        //MergeBathy Input Dialog Vars
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

        QDockWidget *dock;
        bool dockVis;


QgsVectorLayerTools* mVectorLayerTools;


//MyCanvasView *canvasView;




//! Overview map canvas
    QgsMapOverviewCanvas *mOverviewCanvas;
    //! Table of contents (legend) for the map
    QgsLayerTreeView *mLayerTreeView;
    //! a bar to display warnings in a non-blocker manner
    QgsMessageBar *mInfoBar;

    //! refresh map canvas
    void refreshMapCanvas();

    //! start "busy" progress bar
    void canvasRefreshStarted();

    //! stop "busy" progress bar
    void canvasRefreshFinished();

    /** Dialog for verification of action on many edits */
    bool verifyEditsActionDialog( const QString& act, const QString& upon );

    /** Update gui actions/menus when layers are modified */
    void updateLayerModifiedActions();

    //! map tool changed
    void mapToolChanged( QgsMapTool *newTool, QgsMapTool *oldTool );

    //! map layers changed
    void showMapCanvas();

QStackedWidget* mCentralContainer;


//From QgisApp
static MainWindow *smInstance;
//void attributeTable();
void createActions2();
void createActionGroups();
void createMenus2();
void createToolBars();
void createStatusBar2();
void setupConnections();
void createCanvasTools();
void createOverview();


    /** QGIS-internal vector feature clipboard */
    QgsClipboard *mInternalClipboard;


    //! Timer for map tips
    QTimer *mpMapTipsTimer;

    //! Maptip object
    QgsMapTip *mpMaptip;

    //! Flag to indicate if maptips are on or off
    bool mMapTipsVisible;

};

#endif
