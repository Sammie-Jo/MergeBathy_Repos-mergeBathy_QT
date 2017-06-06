#include "mymenuprovider.h"
#include "qtmbparams.h"
#include <commands.h>

//QGIS Includes
//#include <qgsapplication.h>
//#include <qgsproviderregistry.h>
//#include <qgssinglesymbolrendererv2.h>
//#include <qgsmaplayerregistry.h>
#include <qgsvectorlayer.h>
#include <qgsmapcanvas.h>
//#include <qgsproject.h>
//#include <qgslayertree.h>
//#include <qgslayertreemodel.h>
//#include <qgslayertreemapcanvasbridge.h>
//#include <qgsrasterlayer.h>
//#include <qgsvectordataprovider.h>
//#include <qgslayertreeregistrybridge.h>

#include <QMenu>
#include <QMessageBox>

/*******************************************************
 * Class: MyMenuProvider
 ******************************************************/
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


