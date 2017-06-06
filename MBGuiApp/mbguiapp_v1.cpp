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

// QGIS Map tools
#include "qgsmaptoolpan.h"
#include "qgsmaptoolzoom.h"

// These are the other headers for available map tools (not used in this example)
//#include "qgsmaptoolcapture.h"
//#include "qgsmaptoolidentify.h"
//#include "qgsmaptoolselect.h"
//#include "qgsmaptoolvertexedit.h"
//#include "qgsmeasure.h"

MyMenuProvider::MyMenuProvider(QgsLayerTreeView *view)
{
    this->view =  view;
    //m = createContextMenu();
}
MyMenuProvider::~MyMenuProvider()
{
    //delete view;
    //if(m!=nullptr)
    //    delete m;
}
QMenu * MyMenuProvider::createContextMenu() {
    qDebug()<<"THIS IS HERE2"<<endl;
    qDebug() <<"VIEW: " << this->view->currentLayer()<<endl;
    if(!view->currentLayer())
        return nullptr;
    m = new QMenu();
    m->addAction("Show Extent",this,SLOT(showExtent()));
//        QAction* newAct = new QAction(QObject::tr("&Extent"), this);
//        newAct->setShortcuts(QKeySequence::New);
//        newAct->setStatusTip(QObject::tr("Show Extents"));

    m->addAction("Rename",this,SLOT(rename()),tr("Ctrl+Alt+r"));
    m->addAction("Undo",this,SLOT(undoLast()),QKeySequence::Undo)->setStatusTip(tr("Undo last action only"));
    m->addAction("Redo",this,SLOT(redoLast()),QKeySequence::Redo)->setToolTip(tr("Undo last action only"));
    m->addAction("Add Layer",this,SLOT(addLayer()),QKeySequence::New);
    m->addAction("Add Group",this,SLOT(addGroup()),QKeySequence::New);
    m->addAction("Remove Layer",this,SLOT(removeLayer()),QKeySequence::New);
    m->addAction("Remove Group",this,SLOT(removeGroup()),QKeySequence::New);
    m->addAction("Save as New Layer",this,SLOT(saveLayerAs()),QKeySequence::New);
    m->addAction("Reset Layer",this,SLOT(resetLayer()),QKeySequence::New)->setToolTip(tr("Discards All Changes"));





//      createActions();
//    QgsLayerTreeViewDefaultActions

    return m;
}

void MyMenuProvider::showExtent(){
    QgsRectangle r = view->currentLayer()->extent();
    QMessageBox::information(nullptr, "Extent", r.toString());
}

void MyMenuProvider::rename() {
    bool ok;
    QString str = tr("Rename %1 as:").arg(view->currentLayer()->name());
    QString text = QInputDialog::getText(nullptr, tr("Rename Layer"),
                                             str, QLineEdit::Normal,
                                             QDir::home().dirName(), &ok);
    if (ok && !text.isEmpty())
        view->currentLayer()->setLayerName(text);

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
    undoStack = new QUndoStack(this);

    createActions();
    createMenus();

    createUndoView();

    //required by Qt4 to initialise the ui
    setupUi(this);

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
    //Model-View Paradigm
    QgsLayerTreeModel * model = new QgsLayerTreeModel(root);
    // Modify model
    model->setFlag(QgsLayerTreeModel::AllowNodeReorder);
    model->setFlag(QgsLayerTreeModel::AllowNodeChangeVisibility);
    model->setFlag(QgsLayerTreeModel::AllowNodeRename);
    model->setFlag(QgsLayerTreeModel::AllowLegendChangeState);
    QgsLayerTreeView * view = new QgsLayerTreeView();
    view->setModel(model);
    view->show();

    // Connect to signals emitted from Layer Tree
    connect(root, SIGNAL(willAddChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onWillAddChildren(QgsLayerTreeNode*,int,int)));
    connect(root, SIGNAL(addedChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onWillAddChildren(QgsLayerTreeNode*,int,int)));
    connect(root, SIGNAL(willRemoveChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onWillRemoveChildren(QgsLayerTreeNode*,int,int)));
    connect(root, SIGNAL(removedChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onRemovedChildren(QgsLayerTreeNode*,int,int)));
    connect(root, SIGNAL(visibilityChanged(QgsLayerTreeNode*,Qt::CheckState)), this, SLOT(onVisibilityChanged(QgsLayerTreeNode*)));
    connect(root, SIGNAL(customPropertyChanged(QgsLayerTreeNode*,QString)), this, SLOT(onCustomPropertyChanged(QgsLayerTreeNode*,QString)));
    connect(root, SIGNAL(expandedChanged(QgsLayerTreeNode*,bool)), this, SLOT(onExpandedChanged(QgsLayerTreeNode*,bool)));
    connect(view, SIGNAL(currentLayerChanged(QgsMapLayer*)), this, SLOT(onChange(QgsMapLayer*)));
    connect(view, SIGNAL(), this, SLOT(onLayerSavedAs(QgsMapLayer*,QString)));


    // Create provider to context menu
    MyMenuProvider * provider = new MyMenuProvider(view);
    view->setMenuProvider(provider);


    // Create the Map Canvas
    mpMapCanvas= new QgsMapCanvas(0, 0);
    mpMapCanvas->enableAntiAliasing(true);
    //mpMapCanvas->useImageToRender(false);
    mpMapCanvas->setCanvasColor(QColor(255, 255, 255));
    mpMapCanvas->freeze(false);
    mpMapCanvas->setVisible(true);
    mpMapCanvas->refresh();
    mpMapCanvas->show();

    // Lay our widgets out in the main window
    mpLayout = new QVBoxLayout(frameMap);
    mpLayout->addWidget(mpMapCanvas);

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
    mpPanTool = new QgsMapToolPan(mpMapCanvas);
    mpPanTool->setAction(mpActionPan);
    mpZoomInTool = new QgsMapToolZoom(mpMapCanvas, FALSE); // false = in
    mpZoomInTool->setAction(mpActionZoomIn);
    mpZoomOutTool = new QgsMapToolZoom(mpMapCanvas, TRUE ); //true = out
    mpZoomOutTool->setAction(mpActionZoomOut);
}
void MainWindow::createUndoView()
{
    undoView = new QUndoView(undoStack);
    undoView->setWindowTitle(tr("Command List"));
    undoView->show();
    undoView->setAttribute(Qt::WA_QuitOnClose, false);
}
void MainWindow::createActions()
{
    deleteAction = new QAction(tr("&Delete Item"), this);
    deleteAction->setShortcut(tr("Del"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    undoAction = undoStack->createUndoAction(this, tr("&Undo"));
    undoAction->setShortcuts(QKeySequence::Undo);

    redoAction = undoStack->createRedoAction(this, tr("&Redo"));
    redoAction->setShortcuts(QKeySequence::Redo);

}
void MainWindow::createMenus()
{
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    editMenu->addAction(deleteAction);
    connect(editMenu, SIGNAL(aboutToShow()),
            this, SLOT(itemMenuAboutToShow()));
    connect(editMenu, SIGNAL(aboutToHide()),
            this, SLOT(itemMenuAboutToHide()));
}

MainWindow::~MainWindow()
{
  delete mpZoomInTool;
  delete mpZoomOutTool;
  delete mpPanTool;
  delete mpMapToolBar;
  delete mpMapCanvas;
  delete mpLayout;
}

void MainWindow::panMode()
{
  mpMapCanvas->setMapTool(mpPanTool);

}
void MainWindow::zoomInMode()
{
  mpMapCanvas->setMapTool(mpZoomInTool);
}
void MainWindow::zoomOutMode()
{
  mpMapCanvas->setMapTool(mpZoomOutTool);
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
void MainWindow::onVisibilityChanged(QgsLayerTreeNode *node) {
    view->update();
  qDebug() << "TOGGLED" << node << " " << endl;//state << endl;
//  canvas->update();

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
    qDebug() << "THIS HAS CHANGED" << endl;
  //QMessageBox::information(nullptr, "Change", "Current Layer: "+layer->name());
}
void MainWindow::onLayerSavedAs(QgsMapLayer *l, const QString &path) {
    qDebug() << "SAVEDAS" << l << " to " << path << endl;
}


void MainWindow::addLayer()
{

//    myFileNameList = QFileDialog::getOpenFileNames(this, tr("Open File"),QCoreApplication::applicationDirPath () + "./","");

    QUndoCommand *addCommand = new AddCommand(mpMapCanvas,myLayerSet,view);
    undoStack->push(addCommand);
}
void AddCommand::undo()
{
    QString myFileName;
    foreach(myFileName,myFileNameList) {
    //    root->removeChildren(1,1);
        myLayerSet.removeLast();

    }

//    view->removeItem(myDiagramItem);
    view->update();
}
//void AddCommand::redo()
//{
//    view->addItem(myDiagramItem);
//    myDiagramItem->setPos(initialPosition);
//    view->clearSelection();
//    view->update();
//}
AddCommand::AddCommand(QgsMapCanvas *mpMapCanvas,QList<QgsMapCanvasLayer> myLayerSet, QgsLayerTreeView * view, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    this->myLayerSet = myLayerSet;
    this->mpMapCanvas = mpMapCanvas;
    this->view = view;
//    this->myFileNameList = myFileNameList;
    myFileNameList = QFileDialog::getOpenFileNames(nullptr, tr("Open File"),QCoreApplication::applicationDirPath () + "./","");
    qDebug()<<"ARE WE HERE?"<<endl;

    initialPosition = 0;
//    qDebug() << "CurrIndex " << view->currentIndex() << endl;
//    qDebug() << "CurrNode " << view->currentNode() << endl;
//    qDebug() << "CurrLayer " << view->currentLayer() << endl;

    //QgsLayerTreeGroup * root = QgsProject::instance()->layerTreeRoot();



    setText(QObject::tr("Add %1").arg(createCommandString(myFileNameList,initialPosition)));
}
AddCommand::~AddCommand()
{
//    if (!myDiagramItem->scene())
//        delete myDiagramItem;
}

QString createCommandString(QStringList myFileNameList, const int &initialPosition)
{
    return QObject::tr("%1 at (%2)").arg(myFileNameList.size() > 1 ? "Files" : "File").arg(initialPosition);
}




void AddCommand::redo()
//void MainWindow::addLayer()
{
    // ***
    // Open file dialog
    // ***
    //getOpenFileName
    //QStringList myFileNameList = QFileDialog::getOpenFileNames(this, tr("Open File"),QCoreApplication::applicationDirPath () + "./","");

    QString myFileName;
    QString uri;
    QgsVectorLayer * mypLayer;
    QgsSingleSymbolRendererV2 * mypRenderer;
//    QList<QgsMapCanvasLayer> myLayerSet;
    foreach(myFileName,myFileNameList) {
        QFileInfo myRasterFileInfo(myFileName);
        uri = "file:///" + myFileName + "?&useHeader=yes&type=whitespace&xField=1&yField=2";//crs=epsg:4326&field=id:integer&index=yes";
        mypLayer =  new QgsVectorLayer(uri, myRasterFileInfo.completeBaseName(), "delimitedtext");

        mypRenderer = new QgsSingleSymbolRendererV2(QgsSymbolV2::defaultSymbol(mypLayer->geometryType()));
        mypLayer->setRendererV2(mypRenderer);
        if (mypLayer->isValid())
            qDebug() << "Layer is valid" << endl;
        else
        {
            qDebug() << "Layer is NOT valid" << endl;
            return;
        }

        // Add the Vector Layer to the Layer Registry and implicitly add to Layer Tree
       QgsMapLayerRegistry::instance()->addMapLayer(mypLayer, TRUE);

       // Add the Layer to the Layer Set
        myLayerSet.append(QgsMapCanvasLayer(mypLayer));
        // set the canvas to the extent of our layer
        mpMapCanvas->setExtent(mypLayer->extent());
    }
    qDebug()<<"ARE WE HERE?"<<endl;

    // Set the Map Canvas Layer Set
    mpMapCanvas->setLayerSet(myLayerSet);

    //QMenu *addLayerMenu();

//    //******************************************************
//    // II. Layer Legend Widget
//    //******************************************************
//    // A. Start Layer Tree for Legend Widget
//    // Access project's layer tree
//    QgsLayerTreeGroup * root = QgsProject::instance()->layerTreeRoot();
//    //Model-View Paradigm
//    QgsLayerTreeModel * model = new QgsLayerTreeModel(root);
//    // Modify model
//    model->setFlag(QgsLayerTreeModel::AllowNodeReorder);
//    model->setFlag(QgsLayerTreeModel::AllowNodeChangeVisibility);
//    model->setFlag(QgsLayerTreeModel::AllowNodeRename);
//    model->setFlag(QgsLayerTreeModel::AllowLegendChangeState);
//    QgsLayerTreeView * view = new QgsLayerTreeView();
//    view->setModel(model);
//    view->show();

//    // Connect to signals emitted from Layer Tree
//    connect(root, SIGNAL(willAddChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onWillAddChildren(QgsLayerTreeNode*,int,int)));
//    connect(root, SIGNAL(addedChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onWillAddChildren(QgsLayerTreeNode*,int,int)));
//    connect(root, SIGNAL(willRemoveChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onWillRemoveChildren(QgsLayerTreeNode*,int,int)));
//    connect(root, SIGNAL(removedChildren(QgsLayerTreeNode*,int,int)), this, SLOT(onRemovedChildren(QgsLayerTreeNode*,int,int)));
//    connect(root, SIGNAL(visibilityChanged(QgsLayerTreeNode*,Qt::CheckState)), this, SLOT(onVisibilityChanged(QgsLayerTreeNode*)));
//    connect(root, SIGNAL(customPropertyChanged(QgsLayerTreeNode*,QString)), this, SLOT(onCustomPropertyChanged(QgsLayerTreeNode*,QString)));
//    connect(root, SIGNAL(expandedChanged(QgsLayerTreeNode*,bool)), this, SLOT(onExpandedChanged(QgsLayerTreeNode*,bool)));
//    connect(view, SIGNAL(currentLayerChanged(QgsMapLayer*)), this, SLOT(onChange(QgsMapLayer*)));
//    connect(view, SIGNAL(), this, SLOT(onLayerSavedAs(QgsMapLayer*,QString)));

    // Create new layers
//    QgsVectorLayer * layer1 = new QgsVectorLayer("Point", "Layer 1", "memory");
//    QgsVectorLayer * layer2 = new QgsVectorLayer("Polygon", "Layer 2", "memory");
//    QgsVectorLayer * layer3 = new QgsVectorLayer("Point", "Layer 3", "memory");
//    QgsVectorLayer * layer4 = new QgsVectorLayer("Polygon", "Layer 4", "memory");

//    // B. Add Layers
//    // (1) Explicitly adding layers
//    // step 1: add the layer to the registry, False indicates not to add to the layer tree
//    QgsMapLayerRegistry::instance()->addMapLayer(layer1, FALSE);
//    // step 2: append layer to the root group node
//    QgsLayerTreeLayer * node_layer1 = root->addLayer(layer1);
//    // or insert at a specified index
//    QgsLayerTreeLayer * node_layer2 = root->insertLayer(2,layer2);
//    // (2) Implicitly adding layers
//    QgsMapLayerRegistry::instance()->addMapLayer(layer3);

//    // C. Add groups
//    QgsLayerTreeGroup * node_group1 = root->insertGroup(0, "Group 1");
//    // add another sub-group to group1
//    QgsLayerTreeGroup * node_subgroup1 = node_group1->addGroup("Sub-group 1");

//    // Add existing nodes
//    QgsMapLayerRegistry::instance()->addMapLayer(layer4, FALSE);
//    QgsLayerTreeLayer * node_layer4 = new QgsLayerTreeLayer(layer4);
//    root->insertChildNode(0, node_layer4);
//    QgsLayerTreeGroup * node_group2 = new QgsLayerTreeGroup("Group 2");
//    root->addChildNode(node_group2);
//    QgsLayerTreeGroup * g = root->addGroup("My Group");



//    // Create another Map Canvas and show the same layers
//    QgsMapCanvas * canvas = new QgsMapCanvas();
//    QgsLayerTreeGroup *root = QgsProject::instance()->layerTreeRoot();
//    QgsLayerTreeMapCanvasBridge bridge = QgsLayerTreeMapCanvasBridge(root, canvas);
//    canvas->zoomToFullExtent();
//    canvas->show();

//    view->setCurrentLayer(canvas->layers()[0]);

//    // Create provider to context menu
//    MyMenuProvider * provider = new MyMenuProvider(view);
//    view->setMenuProvider(provider);























    // NOTES
    // Ways to Remove nodes
//    root->removeChildren(1,1);
//    root->removeChildNode(node_group2);
//    root->removeLayer(layer1);
//    QgsMapLayerRegistry::instance()->removeMapLayer(layer1);

//    qDebug()<<"TESTING1"<<endl;
//    qDebug() << root->dump() << endl;


    //    // Moving a node
    //    QgsLayerTreeGroup * cloned_group1 = node_group1->clone();
    //    root->insertChildNode(0, cloned_group1);
    //    root->removeChildNode(node_group1);

    //    qDebug()<<"TESTING2"<<endl;

    //    // Rename a node
    //    node_group1->setName("Group X");
    //    node_layer2->setLayerName("Layer X");

    //    qDebug()<<"TESTING3"<<endl;

    //    // Change Visibility
    //    qDebug() << node_group1->isVisible() << endl;
    //    node_group1->setVisible(Qt::Checked);
    //    node_layer2->setVisible(Qt::Unchecked);

    //    qDebug()<<"TESTING4"<<endl;

    //    // Change Expanded State
    //    qDebug() << node_group1->isExpanded();
    //    node_group1->setExpanded(FALSE);

    //    qDebug()<<"TESTING5"<<endl;

    //    // Change Custom Properties
    //    node_group1->setCustomProperty("test_key", "test_value");
    //    qDebug() << node_group1->customProperties() << endl;
    //    qDebug() << node_group1->customProperty("test_key") << endl;
    //    node_group1->removeCustomProperty("test_key");
    //    qDebug() << node_group1->customProperties() << endl;

    //    qDebug()<<"TESTING6"<<endl;

    // View-Model
//    root = QgsProject::instance()->layerTreeRoot();
//    QgsLayerTreeModel * model = new QgsLayerTreeModel(root);
//    QgsLayerTreeView * view = new QgsLayerTreeView();
//    view->setModel(model);
//    view->show();

    //    root->willAddChildren.connect(onWillAddChildren);
    //    root->addedChildren.connect(onAddedChildren);
    //    root->willRemoveChildren.connect(onWillRemoveChildren);
    //    root->removedChildren.connect(onRemovedChildren);
    //    root->visibilityChanged.connect(onVisibilityChanged);
    //    root->customPropertyChanged.connect(onCustomPropertyChanged);
    //    root->expandedChanged.connect(onExpandedChanged);
    //    view.currentLayerChanged.connect(onChanged);
    //    view.layerSavedAs.connect(onLayerSavedAs);
    //    root->nameChanged.connect(onNameChanged);

    // Test Connections
//    QgsLayerTreeGroup * g = root->addGroup("My Group");
    //root->findGroup("My Group")->removeAllChildren();

    // Modify model
//    model = new QgsLayerTreeModel(root);
//    model->setFlag(QgsLayerTreeModel::AllowNodeReorder);
//    model->setFlag(QgsLayerTreeModel::AllowNodeChangeVisibility);





    // Acces LayerTreeView in QGIS main window
//    view = iface.layerTreeView();

    // change selection to the top-most layer (onChange will be also called)
//    view.setCurrentLayer( iface.mapCanvas().layers()[0] );


    // Converting to/from QModelIndex object
//    index2node();
//    node2index();


//    qDebug() << root << endl;
//    qDebug() << root->children() << endl;

//    child0 = root->children()[0];
//    qDebug() << child0 << endl;
//    qDebug() << type(child0) << endl;
//    qDebug() << isinstance(child0, QgsLayerTreeLayer) << endl;
//    qDebug() << child0.parent() << endl;

//    foreach(child,root.children())
//    {
//        if(isinstance(child, QgsLayerTreeGroup))
//            qDebug() << "- group: " << child.name() << endl;
//        else if(isinstance(child, QgsLayerTreeLayer))
//            qDebug() << "- layer: " << child.layerName() << "  ID: " << child.layerId() << endl;
//    }

//    //Find all layer nodes
//    ids = root->findLayerIds();
//    qDebug() << ids << endl;
//    qDebug() << root->findLayers() << endl;
//    qDebug() << root->findLayer(ids[0]) << endl;
//    //Search for group nodes by name (group names are not necessarily unique)
//    qDebug() << root->findGroup("POI") << endl;


//************************************************
//    //////  QString myLayerPath         = "F:/Sams_Files/FY_2012/MergeBathy_Repos/mergeBathy_Qt/QGIS_SAMPLE_DATA/QGIS-Code-Examples/2_basic_main_window/data/test.shp";
//    //    QString myLayerPath         = "../2_basic_main_window/data/test.shp";
//    //    QString myLayerBaseName     = "test";
//        QString myProviderName      = "ogr";
//    //    QgsVectorLayer * mypLayer ;//= new QgsVectorLayer(myLayerPath, myLayerBaseName, myProviderName);
//        //QFileInfo fileInfo = QFileInfo(myLayerPath);

//        // ***
//        // Open file dialog
//        // ***
//        QString myFileName = QFileDialog::getOpenFileName(this, tr("Open File"),
//                               QCoreApplication::applicationDirPath () + "../4_adding_rasters_to_canvas/data","");
//                               //tr("GeoTiff (*.shp)"));
//        QFileInfo myRasterFileInfo(myFileName);
//        qDebug() << "myFileName =" << myFileName;
//        qDebug() << "fileInfo" << myRasterFileInfo.exists() << endl;
//        QgsVectorLayer * mypLayer =  new QgsVectorLayer(myRasterFileInfo.filePath(), myRasterFileInfo.completeBaseName(), myProviderName);

//        QString uri = "file:///" + myFileName + "?&useHeader=yes&type=whitespace&xField=1&yField=2";//crs=epsg:4326&field=id:integer&index=yes";
//        mypLayer =  new QgsVectorLayer(uri, myRasterFileInfo.completeBaseName(), "delimitedtext");

//        qDebug() << "SOMETHING" << mypLayer->isValid()<<endl;

//************************************************

    //    // **********
    //    // Check provider registry for provider
    //    qDebug() << "Provider List" << endl;
    //    qDebug() << r->providerList() << endl;
    //    //qDebug() << r->pluginList() << endl;

    //    if(r->providerList().contains(myProviderName))
    //        qDebug() << "Provider " << myProviderName << "found!" << endl;
    //    else
    //        qDebug() << "Could not find " << myProviderName << "provider out of " << r->providerList().length() << " providers found!" << endl;
    //    // **********

    //    // Open data file
    //    QgsFeatureIterator features;
    //    if(fileInfo.exists())
    //    {
    //        qDebug() << "File exists; Path is correct" << endl;
    //        qDebug() << myLayerPath << myLayerBaseName << myProviderName << endl;
    //        mypLayer = new QgsVectorLayer(myLayerPath, fileInfo.fileName(), myProviderName);
    //        if(mypLayer->isValid())
    //            qDebug() << "Layer is valid" << endl;
    //        else
    //        {
    //            qDebug() << "Layer is NOT valid" << endl;
    //            return;
    //        }
    //        features = mypLayer->getFeatures();
    //    }
    //    else {
    //        qDebug() << "Check if your path is correct: " << myLayerPath << endl;
    //        return;
    //    }

    //    // Fetch attributes
    //    qDebug() << "FEATURES" << endl;
    //    QgsFeature f = QgsFeature();
    //    QgsAttributes attris;
    //    while(features.nextFeature(f)) {
    //        attris = f.attributes();
    //        qDebug() << attris;
    //    }

//**********************************************************


    //    // create layer
    //    //QgsVectorLayer*
    //            mypLayer = new QgsVectorLayer("Point", "temporary_points", "memory");
    //    QgsVectorDataProvider * pr = mypLayer->dataProvider();

    //    QList<QgsField> attribs = QList<QgsField>();
    //    attribs.push_back(QgsField("name", QVariant::String));
    //    attribs.push_back(QgsField("age",  QVariant::Int));
    //    attribs.push_back(QgsField("size", QVariant::Double));

    //    // add fields
    //    pr->addAttributes(attribs);

    //    //                    QgsField("age",  QVariant.Int),
    //      //                  QgsField("size", QVariant.Double)]);
    //    mypLayer->updateFields(); // tell the vector layer to fetch changes from the provider

    //    // add a feature
    //    QgsFeature * fet = new QgsFeature();
    //    fet->setGeometry(QgsGeometry::fromPoint(QgsPoint(10,10)));
    //    QgsAttributes atts = QgsAttributes();
    //    atts.push_back("Johnny");
    //    atts.push_back("2");
    //    atts.push_back("0.3");
    //    fet->setAttributes(atts);
    //    QgsFeatureList fl= QgsFeatureList();
    //    fl.push_back(*fet);
    //    pr->addFeatures(fl);

    //    //fet->setAttributes(QgsAttributes("Johny", 2, 0.3));
    //    //pr->addFeatures(fet);

    //    // update layer's extent when new features have been added
    //    // because change of extent in provider is not propagated to the layer
    //    mypLayer->updateExtents();
    //    qDebug() << "HELP" << mypLayer->isValid()<<endl;
//mypLayer.commitChanges()
//**************************************************************




}

