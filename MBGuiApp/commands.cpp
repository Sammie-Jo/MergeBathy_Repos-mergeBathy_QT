#include "commands.h"
#include "mbguiapp.h"
#include <qgslayertree.h>
#include <qinputdialog.h>
#include <QFileDialog>
//#include <qgsapplication.h>
//#include <qgsproviderregistry.h>
#include <qgssinglesymbolrendererv2.h>
#include <qgscategorizedsymbolrendererv2.h>
#include <qgsfillsymbollayerv2.h>
#include <qgsmaplayerregistry.h>
#include <qgsvectorlayer.h>
//#include <qgsmapcanvas.h>
#include <qgsproject.h>
//#include <qgslayertree.h>
//#include <qgslayertreemodel.h>
#include <qgslayertreemapcanvasbridge.h>
//#include <qgsrasterlayer.h>

#include <qgsmarkersymbollayerv2.h>

//#include <qgsvectordataprovider.h>
#include <qgsapplication.h>
#include <qgsfeature.h>
//#include <qgsfeatureattribute.h>
#include <qgsfield.h>
#include <qgsproviderregistry.h>
#include <qgsvectordataprovider.h>
#include <qgsvectorlayer.h>

#include <qgsvectorfilewriter.h>
//for check if /Temp directory exists
#include <sys/types.h>
#include <sys/stat.h>

#include <QDir>





//********************************************
// RENAME COMMAND
//********************************************
RenameCommand::RenameCommand(QgsLayerTreeNode::NodeType nodetype, QgsLayerTreeView * view, QUndoCommand *parent) : QUndoCommand(parent)
{
    this->view = view;
    if(this->view->currentNode()->nodeType()==QgsLayerTreeNode::NodeLayer)
        oldName = this->view->currentLayer()->name();
    else
        oldName = this->view->currentGroupNode()->name();
    QString str = QObject::tr("Rename %1 as:").arg(view->currentLayer()->name());
    newName = QInputDialog::getText(nullptr, QObject::tr("Rename Layer"), str, QLineEdit::Normal, "", &ok);

    setText(QObject::tr("Rename %1").arg(createCommandString(nodetype, this->view)));
}

RenameCommand::~RenameCommand()
{
    qDebug() << "~RenameCommand()" << endl;
}

void RenameCommand::undo()
{
    view->currentLayer()->setLayerName(oldName);
}

void RenameCommand::redo()
{
    if (ok && !newName.isEmpty())
        view->currentLayer()->setLayerName(newName);
}

//******************************************************************
// DELETE COMMAND
//******************************************************************
DeleteCommand::DeleteCommand(QgsLayerTreeGroup *root, QgsLayerTreeGroup* root_yz,QgsLayerTreeView * view,QgsLayerTreeView * view_yz, QUndoCommand *parent) : QUndoCommand(parent)
{
    this->view = view;
    this->view_yz = view_yz;
    this->root = root;
    this->root_yz = root_yz;

//    QList<QgsMapLayer *> list = this->view->selectedLayers();
// //   list.first()->setSelected(false);
//    layer = list.first();
//    layer = static_cast<QgsMapLayer *>(list.first());
//    setText(QObject::tr("Delete %1").arg(createCommandString(layer->shortName(),layer->id())));
}

void DeleteCommand::undo()
{
    myLayerSet.append(layer);
    myLayerSet_yz.append(layer_yz);
    root->addLayer(layer);
    root_yz->addLayer(layer_yz);
}

void DeleteCommand::redo()
{
    layer = view->currentLayer();
    layer_yz = view_yz->currentLayer();
    QList<QgsLayerTreeNode * > nodes = view->selectedNodes();
    QList<QgsLayerTreeNode * > nodes_yz = view_yz->selectedNodes();
    foreach (QgsLayerTreeNode * n, nodes)
        root->removeChildNode(n);
    foreach (QgsLayerTreeNode * n, nodes_yz)
        root_yz->removeChildNode(n);

    view->currentGroupNode()->removeLayer(layer);
    view_yz->currentGroupNode()->removeLayer(layer_yz);

    //QgsMapLayerRegistry::instance()->removeMapLayer(layer1);
}
//******************************************************************
// ADD GROUP COMMAND
//******************************************************************
AddGroupCommand::AddGroupCommand(QgsLayerTreeGroup * root,QgsLayerTreeGroup * root_yz,QgsLayerTreeView * view,QgsLayerTreeView * view_yz, QUndoCommand *parent) : QUndoCommand(parent)
{
    this->view = view;
    this->view_yz = view_yz;
    this->root = root;
    this->root_yz = root_yz;

    if(this->view == nullptr)
        initialPosition = root;//QgsProject::instance()->layerTreeRoot();
    else {
        initialPosition = this->view->currentGroupNode();
        qDebug() << "Internal ID" << this->view->currentIndex().internalId()<<endl;
    }
    if(this->view_yz == nullptr)
        initialPosition_yz = root_yz;//QgsProject::instance()->layerTreeRoot();
    else {
        initialPosition_yz = this->view_yz->currentGroupNode();
        qDebug() << "Internal ID" << this->view_yz->currentIndex().internalId()<<endl;
    }
    QString newName = "New Group";
    setText(QObject::tr("Add %1").arg(createCommandString(newName,initialPosition)));
}
AddGroupCommand::~AddGroupCommand()
{
    qDebug() << "~AddGroupCommand()" << endl;
}
//void AddGroupCommand::undo()
//{
//    newGroupParent->removeChildNode(newGroup);
//}
//void AddGroupCommand::redo()
//{
//    newGroupParent = view->currentGroupNode();
//    newGroup = newGroupParent->insertGroup(0, "New Group");
//}
void AddGroupCommand::undo()
{
    newGroupParent  = (QgsLayerTreeGroup*)newGroup->parent();
    newGroupParent->removeChildNode(newGroup);

    newGroupParent_yz  = (QgsLayerTreeGroup*)newGroup_yz->parent();
    newGroupParent_yz->removeChildNode(newGroup_yz);
}
void AddGroupCommand::redo()
{
    newGroupParent = view->currentGroupNode();
    newGroup = newGroupParent->insertGroup(0, "New Group");

    newGroupParent_yz = view_yz->currentGroupNode();
    newGroup_yz = newGroupParent_yz->insertGroup(0, "New Group");
}

//******************************************************************
// ADD COMMAND
//******************************************************************
AddCommand::AddCommand(QgsLayerTreeGroup * root,QgsLayerTreeGroup * root_yz, QgsMapCanvas * canvas_xy, QgsMapCanvas * canvas_yz,
                       QList<QgsMapCanvasLayer> myLayerSet, QList<QgsMapCanvasLayer> myLayerSet_yz,
                       QgsLayerTreeView * view, QgsLayerTreeView * view_yz,
                       QMap<QString,QStack<QgsFeatureIds>*> *selectedMap, QMap<QString,QStack<QgsFeatureIds>*> *selectedMap_yz,
                       QMap<QString,QString> *layerSuffixes,QUndoCommand *parent) : QUndoCommand(parent)
{
//    root = QgsProject::instance()->layerTreeRoot();
    this->root = root;
    this->root_yz = root_yz;
    this->canvas_xy = canvas_xy;
    this->canvas_yz = canvas_yz;
    this->myLayerSet = myLayerSet;
    this->myLayerSet_yz = myLayerSet_yz;
    this->view = view;
    this->view_yz = view_yz;
    this->selectedMap = selectedMap;
    this->selectedMap_yz = selectedMap_yz;
    this->layerSuffixes = layerSuffixes;
//    renderer = NULL;

    QString dataPath = QgsProject::instance()->readEntry("myproject","LocalDataPath");

    myFileNameList = QFileDialog::getOpenFileNames(nullptr, tr("Open File"),dataPath,"");//QCoreApplication::applicationDirPath () + "./","");

    if(this->view == nullptr)
        initialPosition = QgsProject::instance()->layerTreeRoot();
    else {
        initialPosition = this->view->currentGroupNode();
        qDebug() << "Internal ID" << this->view->currentIndex().internalId()<<endl;
    }
    setText(QObject::tr("Add %1").arg(createCommandString(myFileNameList,initialPosition)));

//    if(this->view_yz == nullptr)
//        initialPosition_yz = root_yz;
//    else {
//        initialPosition_yz = this->view_yz->currentGroupNode();
//        qDebug() << "Internal ID" << this->view_yz->currentIndex().internalId()<<endl;
//    }
//    setText(QObject::tr("Add %1").arg(createCommandString(myFileNameList,initialPosition)));
}
AddCommand::~AddCommand()
{
//    if (!myDiagramItem->scene())
//        delete myDiagramItem;
    qDebug() << "~AddCommand()" << endl;
}
void AddCommand::undo()
{
    QgsVectorLayer * mypLayer;
    foreach(QString myFileName,myFileNameList) {
        mypLayer = (QgsVectorLayer*)myLayerSet.back().layer();
        myLayerSet.removeLast();
        QgsMapLayerRegistry::instance()->removeMapLayer(mypLayer);

        mypLayer = (QgsVectorLayer*)myLayerSet_yz.back().layer();
        myLayerSet_yz.removeLast();
        QgsMapLayerRegistry::instance()->removeMapLayer(mypLayer);
    }
    canvas_xy->setLayerSet(myLayerSet);
    canvas_yz->setLayerSet(myLayerSet_yz);
}
void AddCommand::addField(QgsVectorLayer* mypLayer)
{
    /** Idx of changed attribute*/
    int mAttributeId;
    QgsFeature feature;

    if ( !mypLayer->isEditable() )
      mypLayer->startEditing();

    QApplication::setOverrideCursor( Qt::WaitCursor );

//    mypLayer->beginEditCommand( "Create New Field" );

    //create new field
    QList<QgsField> nfields;
    const QgsField newField = QgsField("exclude", QVariant::String);
    nfields << newField;
    if ( !mypLayer->dataProvider()->addAttributes(nfields) )
    {
      QApplication::restoreOverrideCursor();
      QMessageBox::critical( nullptr, tr( "Provider error" ), tr( "Could not add the new field to the provider." ) );
      mypLayer->destroyEditCommand();
      return;
    }
    mypLayer->commitChanges();
    mypLayer->startEditing();
    //get index of the new field
    const QgsFields& fields = mypLayer->fields();

    for ( int idx = 0; idx < fields.count(); ++idx )
    {
      if ( fields[idx].name() == "exclude" )
      {
        mAttributeId = idx;
        break;
      }
    }

    int rownum = 1;

    QgsField field = mypLayer->fields().at( mAttributeId );

    QVariant emptyAttribute = QVariant( field.type() );

    QgsFeatureRequest req = QgsFeatureRequest().setFlags( QgsFeatureRequest::NoGeometry );

    QgsFeatureIterator fit = mypLayer->getFeatures( req );
    while ( fit.nextFeature( feature ) )
    {
        QVariant value = "included";//0;

        field.convertCompatible( value );
        mypLayer->changeAttributeValue( feature.id(), mAttributeId, value, emptyAttribute  );
        QVariant t = feature.attribute("exclude");

        rownum++;
    }

    QApplication::restoreOverrideCursor();
    mypLayer->commitChanges();
//    mypLayer->endEditCommand();

}

QgsVectorLayer* AddCommand::addCategorySymbols(QgsVectorLayer *mypLayer)
{
    // get unique values
    QgsStringMap layer_style = QgsStringMap();
    QList<QVariant> unique_values;
    unique_values.append(QVariant("included"));//0
    unique_values.append(QVariant("excluded"));//1
    QList<QgsRendererCategoryV2> categories;
    QColor defaultColor;
    // define categories
    if(renderer == NULL)
    {
        foreach(QVariant unique_value, unique_values){
            // initialize the default symbol for this geometry type
            QgsSymbolV2 *defaultSymbol = QgsSymbolV2::defaultSymbol(mypLayer->geometryType());
            QgsSymbolLayerV2 *symbol_layer = QgsSimpleMarkerSymbolLayerV2::create();

            // configure a symbol layer
            if(unique_value.toString() == "included")
            { //exclude = 0 = included
                defaultColor = defaultSymbol->color();
                while(defaultColor.toRgb().red() > 175 && (defaultColor.toRgb().green() < 75 && defaultColor.toRgb().blue() < 75))
                {
                    defaultSymbol = QgsSymbolV2::defaultSymbol(mypLayer->geometryType());
                    defaultColor = defaultSymbol->color();
                }
                symbol_layer->setColor(defaultColor);
                symbol_layer->setOutlineColor(Qt::transparent);
            }
            else
            { //exclude = 1 = excluded
                symbol_layer->setColor(defaultColor);
                symbol_layer->setOutlineColor(Qt::red);
            }

            // replace default symbol layer with the configured one
            if(symbol_layer != NULL)
                defaultSymbol->changeSymbolLayer(0, symbol_layer);
          //  }
            // create renderer object
            QgsRendererCategoryV2 *category = new QgsRendererCategoryV2(unique_value, defaultSymbol, unique_value.toString());
            // entry for the list of category items
            categories.append(*category);
        }
        // create renderer object
        renderer = new QgsCategorizedSymbolRendererV2("exclude", categories);
    }
    // assign the created renderer to the layer
    if( renderer!= NULL)
        mypLayer->setRendererV2(renderer);

    return mypLayer;
}

//Expects no headers!!
void AddCommand::redo()//addLayer()
{
    //we cannot edit layers if the provider is "delimitedtext"
    //as a quick fix: resave as a shp file and load so we can modify it.
    QDir dir = QDir::temp();
    if(!dir.exists("MBGuiApp"))
        if(!dir.mkdir("MBGuiApp"))
            qDebug() << "Failed to make directory: " << dir.dirName() << "/MBGuiApp" << endl;

    QString uri;
    QgsVectorLayer * mypLayer;
    QString vectorFilename, layerName;
    foreach(QString myFileName,myFileNameList)
    {
        for(int i = 0; i<2; i++)
        {
            bool ok( false );
            QFileInfo myRasterFileInfo(myFileName);
            //layer name is filename without last extension or path
            layerName = myRasterFileInfo.completeBaseName();
            if(myRasterFileInfo.completeBaseName().toLatin1().lastIndexOf(".shp") == -1)
            { //input data text file x,y,z,h,v
                //temp file to save and read back from
                vectorFilename = QDir::temp().path().append("/MBGuiApp").append("/Temp_").append(layerName);

                if(!i)  //xy-layer//do this first
                {
                    //uri = "file:///" + myFileName + "?&useHeader=no&type=whitespace&xField=1&yField=2&zField=3crs=epsg:4326&wktField=point&index=yes";
                    uri = "file:///" + myFileName + "?&useHeader=no&type=whitespace&xField=1&yField=2&zField=3crs=epsg:4326&field=x:double(20)&field=y:double(20)&field=z:double(20)&index=yes";
                    vectorFilename.append(".shp");
                    renderer = NULL;
                }
                else  //yz-layer
                {
                    uri = "file:///" + myFileName + "?&useHeader=no&type=whitespace&xField=2&yField=3&zField=1crs=epsg:4326&field=x:double(20)&field=y:double(20)&field=z:double(20)&index=yes";
                    vectorFilename.append("_yz.shp");
                    layerName.append("_yz");
                }
                qDebug() << "vectorFilename " << vectorFilename << endl;

                //get layer but we cannot edit it if the provider is "delimitedtext"
                mypLayer    = new QgsVectorLayer(uri, layerName, "delimitedtext");

                QgsCoordinateReferenceSystem destCRS = QgsCoordinateReferenceSystem(4326, QgsCoordinateReferenceSystem::EpsgCrsId);;
                QgsCoordinateTransform* ct = new QgsCoordinateTransform( mypLayer->crs(), destCRS );
                QgsVectorFileWriter::WriterError error = QgsVectorFileWriter::writeAsVectorFormat(mypLayer, vectorFilename, "system", ct, "ESRI Shapefile");
                delete mypLayer;
                mypLayer = new QgsVectorLayer(vectorFilename, layerName, "ogr");

                //modify layer: add the exclude field to track selections
                addField(mypLayer);
                qDebug() << "Field Added"<<endl;
                mypLayer = addCategorySymbols(mypLayer);
                qDebug() << "Category Symbol Added"<<endl;
            }
            else
            { //input land mask file
                if(i) //don't add mask twice!
                    continue;
                uri = "file:///" + myFileName + "?&useHeader=no&type=whitespace&crs=epsg:4326&wktField=polygon&index=yes"; //shape
                mypLayer    = new QgsVectorLayer(uri, layerName, "delimitedtext");
            }

            if (mypLayer->isValid())
                qDebug() << "Layer is valid" << endl;
            else
            {
                qDebug() << "Layer is NOT valid" << endl;
                return;
            }
           // Add the Vector Layer to the Layer Registry and implicitly add to Layer Tree
           if(!i) //xy-layer
           {
                qDebug()<<"i:"<<i<<endl;
                QgsMapLayerRegistry::instance()->addMapLayer(mypLayer, FALSE);
                root->addLayer(mypLayer);

                // Add the Layer to the Layer Set
                 myLayerSet.append(QgsMapCanvasLayer(mypLayer));
                 // set the canvas to the extent of our layer
                 canvas_xy->setExtent(mypLayer->extent());

                 //create a stack to track selections in each layer for undo/redo actions
                QStack<QgsFeatureIds> *lstack = new QStack<QgsFeatureIds>();
                selectedMap->insert(mypLayer->id(),lstack);

                //keeps suffixes so we can append them back to our output files we'll write in order to run MB
                layerSuffixes->insert(mypLayer->id(), myRasterFileInfo.completeSuffix());
                qDebug() << "mypLayer->id():" << mypLayer->id() << endl;
                view->setCurrentLayer(mypLayer);
           }
           else //yz-layer
           {
               QgsMapLayerRegistry::instance()->addMapLayer(mypLayer, FALSE);
                // Explicitly add to layer tree.
                //step 2: append layer to the root_xy group node
               root_yz->addLayer(mypLayer);

                // Add the Layer to the Layer Set
                myLayerSet_yz.append(QgsMapCanvasLayer(mypLayer));
                // set the canvas to the extent of our layer
                canvas_yz->setExtent(mypLayer->extent());

                //create a stack to track selections in each layer for undo/redo actions
                QStack<QgsFeatureIds> *lstack = new QStack<QgsFeatureIds>();
                selectedMap_yz->insert(mypLayer->id(),lstack);
//                delete renderer;
                renderer = NULL;
                qDebug() << "mypLayer_yz->id():" << mypLayer->id() << endl;
                view_yz->setCurrentLayer(mypLayer);
           }
       }
    }
    // make sure at least one layer was successfully added
    if ( myLayerSet.isEmpty() )
      return;

    // Set the Map Canvas Layer Set
    canvas_xy->setLayerSet(myLayerSet);

    // make sure at least one layer was successfully added
    if ( myLayerSet_yz.isEmpty() )
      return;

    // Set the Map Canvas Layer Set
    canvas_yz->setLayerSet(myLayerSet_yz);

//    MBGuiApp::instance()->activateDeactivateLayerRelatedActions( MBGuiApp::instance()->activeLayer() );
    MBGuiApp::instance()->statusBar()->showMessage( canvas_xy->extent().toString( 2 ) );

}




//******************************************************************
// Select COMMAND
//******************************************************************
SelectedCommand::~SelectedCommand()
{
}
//SelectedCommand::SelectedCommand(QgsLayerTreeView *view,
//                                 QMap<QString,QStack<QgsFeatureIds>*> *selectedMap, QUndoCommand *parent) : QUndoCommand(parent)
//{
//    this->view = view;
//    this->selectedMap = selectedMap;

//    QgsVectorLayer* mypLayer = qobject_cast<QgsVectorLayer *>( this->view->currentLayer() );

//    setText(QObject::tr("Toggle2 Exclude %1 ").arg(mypLayer->name()));
//}
SelectedCommand::SelectedCommand(QgsLayerTreeView *view, QgsLayerTreeView *view_yz,
                                 QMap<QString,QStack<QgsFeatureIds>*> *selectedMap,QMap<QString,QStack<QgsFeatureIds>*> *selectedMap_yz, QUndoCommand *parent) : QUndoCommand(parent)
{
    this->view = view;
    this->view_yz = view_yz;
    this->selectedMap = selectedMap;
    this->selectedMap_yz = selectedMap_yz;

    QgsVectorLayer* mypLayer = qobject_cast<QgsVectorLayer *>( this->view->currentLayer() );

    setText(QObject::tr("Toggle2 Exclude %1 ").arg(mypLayer->name()));
}
void SelectedCommand::redo()
{
    QgsVectorLayer* mypLayer = qobject_cast<QgsVectorLayer *>( view->currentLayer() );
    QgsVectorLayer* mypLayer_yz = qobject_cast<QgsVectorLayer *>( view_yz->currentLayer() );

    //redo only: get the set of last selected feature ids
    QStack<QgsFeatureIds>* lstack;
    QStack<QgsFeatureIds>* lstack_yz;
//    if(mypLayer->selectedFeatureCount() == 0 && selectedMap->contains(mypLayer->id())){
//           lstack = selectedMap->value(mypLayer->id());
//           mypLayer->setSelectedFeatures(lstack->pop());
//       }


    if((mypLayer->selectedFeatureCount() == 0 && mypLayer_yz->selectedFeatureCount() == 0) && (selectedMap->contains(mypLayer->id()) && selectedMap_yz->contains(mypLayer_yz->id()))){
        lstack = selectedMap->value(mypLayer->id());
        lstack_yz = selectedMap_yz->value(mypLayer_yz->id());
        mypLayer->setSelectedFeatures(lstack->pop());
        mypLayer_yz->setSelectedFeatures(lstack_yz->pop());
    }
    else if(mypLayer->selectedFeatureCount() != 0 && mypLayer_yz->selectedFeatureCount() == 0)
    {
        mypLayer_yz->setSelectedFeatures(mypLayer->selectedFeaturesIds());
//        mypLayer_yz->setSelectedFeatures(mypLayer->selectedFeatures());
    }
    else if(mypLayer->selectedFeatureCount() == 0 && mypLayer_yz->selectedFeatureCount() != 0)
    {
        mypLayer->setSelectedFeatures(mypLayer_yz->selectedFeaturesIds());

    }
//    qDebug() << "FID: " << mypLayer->selectedFeatureCount() << endl;
//    qDebug() << "FID2: " << mypLayer_yz->selectedFeatureCount() << endl;

//    qDebug() << "mypLayer->id():" << mypLayer->id() << endl;
//    qDebug() << "mypLayer_yz->id():" << mypLayer_yz->id() << endl;


    //toggle the "exclude" field value
    toggleField(mypLayer);
    toggleField(mypLayer_yz);

    //push selected ids to stack for redo/undo
    lstack = selectedMap->value(mypLayer->id());
    lstack->push(mypLayer->selectedFeaturesIds());

    lstack_yz = selectedMap_yz->value(mypLayer_yz->id());
    lstack_yz->push(mypLayer_yz->selectedFeaturesIds());

    //block signal in order to deselect
    mypLayer->blockSignals(true);
    mypLayer_yz->blockSignals(true);

    mypLayer->deselect(mypLayer->selectedFeaturesIds());
    mypLayer_yz->deselect(mypLayer_yz->selectedFeaturesIds());

    mypLayer->blockSignals(false);
    mypLayer_yz->blockSignals(false);

}
void SelectedCommand::undo()
{
    QgsVectorLayer* mypLayer = qobject_cast<QgsVectorLayer *>( view->currentLayer() );
//    QgsVectorLayer* mypLayer_yz = qobject_cast<QgsVectorLayer *>( view_yz->currentLayer() );

    QStack<QgsFeatureIds>* lstack = selectedMap->value(mypLayer->id());
    mypLayer->setSelectedFeatures(lstack->pop());

//    QStack<QgsFeatureIds>* lstack_yz = selectedMap_yz->value(mypLayer_yz->id());
//    mypLayer_yz->setSelectedFeatures(lstack_yz->pop());

    toggleField(mypLayer);
//    toggleField(mypLayer_yz);

    lstack->push(mypLayer->selectedFeaturesIds());
//    lstack_yz->push(mypLayer_yz->selectedFeaturesIds());

    mypLayer->blockSignals(true);
//    mypLayer_yz->blockSignals(true);
    mypLayer->deselect(mypLayer->selectedFeaturesIds());
//    mypLayer_yz->deselect(mypLayer_yz->selectedFeaturesIds());
    mypLayer->blockSignals(false);
//    mypLayer_yz->blockSignals(false);
}
void SelectedCommand::toggleField(QgsVectorLayer* mypLayer)
{
    if ( !mypLayer->isEditable() )
      mypLayer->startEditing();

    QApplication::setOverrideCursor( Qt::WaitCursor );

    mypLayer->beginEditCommand( "Toggle2 Exclude Field" );

    //update existing field
    /** Key: field name, Value: field index*/
    QMap<QString, int> mFieldMap;
    const QgsFields& fields = mypLayer->fields();
    for ( int idx = 0; idx < fields.count(); ++idx )
    {
      QString fieldName = fields[idx].name();
      mFieldMap.insert( fieldName, idx );
    }

    /** Idx of changed attribute*/
    int mAttributeId;

    QMap<QString, int>::const_iterator fieldIt = mFieldMap.constFind( "exclude" );
    if ( fieldIt != mFieldMap.constEnd() )
      mAttributeId = fieldIt.value();

    //go through all the features and change the new attribute
    QgsFeature feature;

    int rownum = 1;

    QgsField field = mypLayer->fields().at( mAttributeId );

    QgsFeatureRequest req = QgsFeatureRequest().setFlags( QgsFeatureRequest::NoGeometry );

    QgsFeatureIterator fit = mypLayer->selectedFeaturesIterator(req);
    while ( fit.nextFeature( feature ) )
    {
        QVariant oldValue = feature.attribute("exclude").toString();
        QVariant value = (oldValue == "excluded")? "included" : "excluded"; //0 : 1;

        field.convertCompatible( value );
        mypLayer->changeAttributeValue( feature.id(), mAttributeId, value, feature.attributes().value( mAttributeId ) );

        rownum++;
    }

//    mypLayer->triggerRepaint();

    QApplication::restoreOverrideCursor();
    mypLayer->endEditCommand();
    mypLayer->commitChanges(); //necessary to avoid automatically using edit markers

}




//http://docs.qgis.org/testing/en/docs/pyqgis_developer_cookbook/vector.html
//void AddCommand::edit1()
//{
//    // 1. Retrieve information about attributes (the fields associated with a vector layer)
//    // "layer" is a QgsVectorLayer instance
//    // There is also fields() which is an alias to pendingFields()
//    foreach(QgsVectorLayer field, layer.pendingFields()){
//        qDebug() << "FieldName = " << field.name() << ", FieldTypeName = " << field.typeName() << endl;
//    }

//    //********************************
//    // 2. change the selection color
//    mapCanvas()->setSelectionColor( QColor("red") );

//    //********************************
//    // 3. Add features to the selected features list for a given layer
//    //********************************
//    // Get the active layer (must be a vector layer)
//    QgsVectorLayer layer = activeLayer();
//    // Get the first feature from the layer
//    QgsFeature feature = layer->getFeatures()->next();
//    if(feature.attribute("exclude") == false){
//        // Add this features to the selected list
//        layer->setSelectedFeatures([feature.id()]);
//    }

//    //********************************
//    // 4. clear the selection
//    //********************************
//    layer.setSelectedFeatures([]);

//    //********************************
//    // 5. Iterating over the features in a Vector Layer
//    //********************************
//    iter = layer.getFeatures();
//    foreach(feature, iter)
//    {
//        // retrieve every feature with its geometry and attributes
//        // fetch geometry
//        geom = feature.geometry();
//        qDebug() << "Feature ID " << feature.id() << " : " << endl;

//        // show some information about the feature
//        if(geom.type() == QGis.Point)
//        {
//            x = geom.asPoint();
//            qDebug() << "Point: " << str(x) << endl;
//        }
//        else if(geom.type() == QGis.Line)
//        {
//            x = geom.asPolyline();
//            qDebug() << "Line: " << len(x) << " points" << endl;
//        }
//        else if(geom.type() == QGis.Polygon)
//        {
//            x = geom.asPolygon();
//            numPts = 0;
//            foreach(ring, x){
//                numPts += len(ring);
//            }
//            qDebug() << "Polygon: " << len(x) << " rings with " << numPts << " points" << endl;
//        }
//        else
//            qDebug() << "Unknown" << endl;

//        // fetch attributes
//        attrs = feature.attributes();

//        // attrs is a list. It contains all the attribute values of this feature
//        qDebug() << "attrs = " << attrs << endl;
//    }

//    //********************************
//    // 6. Attributes can be referred to by their name
//    //********************************
//    qDebug() << "attributeAccessedByName = " << feature['name'] << endl;

//    //or faster by index
//    qDebug() << "attributeAccessedByIndex = " << feature[0] << endl;

//    //********************************
//    // 7. Iterating over selected features   - USE THIS FOR SETTING COLOR AND SYMBOL WHEN SELECTED
//    //********************************
//    // If you only need selected features
//    selection = layer.selectedFeatures();
//    qDebug() << "len(selection) = " << len(selection) << endl;
//    foreach(feature, selection){
//        // do whatever you need with the feature
//    }

//    // or with the Proccessing features() method
//    // By default, this will iterate over all the features in the layer, in case there is no selection,
//    // or over the selected features otherwise. Note that this behavior can be changed in the Processing options to ignore selections.
//    //import processing
//    features = processing.features(layer);
//    foreach(feature, features){
//        // do whatever you need with the feature
//    }

//    //********************************
//    // 8. Iterating over a subset of features
//    //********************************
//    request = QgsFeatureRequest();
//    request.setFilterRect(areaOfInterest);
//    foreach(feature, layer.getFeatures(request)){
//        // do whatever you need with the feature
//    }
//    //Limit the number of requested features
//    request = QgsFeatureRequest();
//    request.setLimit(2);
//    foreach(feature, layer.getFeatures(request)){
//        // loop through only 2 features
//    }

//    // The expression will filter the features where the field "location_name"
//    // contains the word "Lake" (case insensitive)
//    exp = QgsExpression('location_name ILIKE \'%Lake%\'');
//    request = QgsFeatureRequest(exp);

//    //The request can be used to define the data retrieved for each feature,
//    //so the iterator returns all features, but returns partial data for each of them.
//    // Only return selected fields
//    request.setSubsetOfAttributes([0,2]);
//    // More user friendly version
//    request.setSubsetOfAttributes(['name','id'],layer.pendingFields());
//    // Don't return geometry objects
//    request.setFlags(QgsFeatureRequest.NoGeometry);

//    //********************************
//    // 9. Modifying Vector Layers
//    //********************************
//    caps = layer.dataProvider().capabilities();
//    // Check if a particular capability is supported:
//    caps & QgsVectorDataProvider.DeleteFeatures;
//    // Print 2 if DeleteFeatures is supported

//    //Print layer's capabilities textual description in a comma sep list
//    caps_string = layer.dataProvider().capabilitiesString();
//    // Print:
//    // u'Add Features, Delete Features, Change Attribute Values,
//    // Add Attributes, Delete Attributes, Create Spatial Index,
//    // Fast Access to Features at ID, Change Geometries,
//    // Simplify Geometries with topological validation'

//    // Directly commit changes to the underlying data store (a file, database etc).
//    // If caching is enabled, a simple canvas refresh might not be sufficient
//    // to trigger a redraw and you must clear the cached image for the layer
//    if(mapCanvas().isCachingEnabled())
//        layer.setCacheImage(None);
//    else
//        mapCanvas().refresh();

//    // Add Features
//    if(caps & QgsVectorDataProvider.AddFeatures)
//    {
//        feat = QgsFeature(layer.pendingFields());
//        feat.setAttributes([0, 'hello']);
//        // Or set a single attribute by key or by index:
//        feat.setAttribute('name', 'hello');
//        feat.setAttribute(0, 'hello');
//        feat.setGeometry(QgsGeometry.fromPoint(QgsPoint(123, 456)));
//        (res, outFeats) = layer.dataProvider().addFeatures([feat]);
//    }

//    // Delete Features
//    if(caps & QgsVectorDataProvider.DeleteFeatures)
//        res = layer.dataProvider().deleteFeatures([5, 10]);

//    // Modify Features
//    fid = 100;   // ID of the feature we will modify

//    if(caps & QgsVectorDataProvider.ChangeAttributeValues)
//    {
//        attrs = { 0 : "hello", 1 : 123 };
//        layer.dataProvider().changeAttributeValues({ fid : attrs });
//    }
//    if(caps & QgsVectorDataProvider.ChangeGeometries)
//    {
//        geom = QgsGeometry.fromPoint(QgsPoint(111,222));
//        layer.dataProvider().changeGeometryValues({ fid : geom });
//    }

//    // Adding and Removing Fields
//    if(caps & QgsVectorDataProvider.AddAttributes)
//        res = layer.dataProvider().addAttributes([QgsField("mytext", QVariant.String), QgsField("myint", QVariant.Int)]);

//    if(caps & QgsVectorDataProvider.DeleteAttributes)
//        res = layer.dataProvider().deleteAttributes([0]);

//    // update to propogate changes
//    layer.updateFields();

//    //********************************
//    // 10. Modifying Vector Layers with an Editing Buffer
//    //********************************
//    // add two features (QgsFeature instances)
//    layer.addFeatures([feat1,feat2]);
//    // delete a feature with specified ID
//    layer.deleteFeature(fid);

//    // set new geometry (QgsGeometry instance) for a feature
//    layer.changeGeometry(fid, geometry);
//    // update an attribute with given field index (int) to given value (QVariant)
//    layer.changeAttributeValue(fid, fieldIndex, value);

//    // add new field
//    layer.addAttribute(QgsField("mytext", QVariant.String));
//    // remove a field
//    layer.deleteAttribute(fieldIndex);


//    // Undo/Redo Command wraps
//    layer.beginEditCommand("Feature triangulation");

//    // ... call layer's editing methods ...

//    if(problem_occurred)
//    {
//        // rollback changes
//        layer.destroyEditCommand();
//        return;
//    }

//    // ... more editing ...

//    layer.endEditCommand();

//    //You can also use the with edit(layer)-statement to wrap commit and rollback into a more semantic code block
//    with edit(layer)
//    {
//        f = layer.getFeatures().next();
//        f[0] = 5;
//        layer.updateFeature(f);
//    }

//    //********************************
//    // 11. Using Spatial Index
//    //********************************
//    // create spatial index
//    index = QgsSpatialIndex();

//    // add features to index
//    index.insertFeature(feat);

//    // bulk load all features at once
//    index = QgsSpatialIndex(layer.getFeatures());

//    // once spatial index is filled with some values, you can do some queries
//    // returns array of feature IDs of five nearest features
//    nearest = index.nearestNeighbor(QgsPoint(25.4, 12.7), 5);

//    // returns array of IDs of features which intersect the rectangle
//    intersect = index.intersects(QgsRectangle(22.5, 15.3, 23.1, 17.2));

//    //********************************
//    // 12. Writing Vector Layers
//    //********************************
//    // export vector layer option 1: from an instance of QgsVectorLayer
//    error = QgsVectorFileWriter.writeAsVectorFormat(layer, "my_shapes.shp", "CP1250", None, "ESRI Shapefile");

//    if(error == QgsVectorFileWriter.NoError)
//        qDebug() << "success!" << endl;

//    error = QgsVectorFileWriter.writeAsVectorFormat(layer, "my_json.json", "utf-8", None, "GeoJSON")
//    if(error == QgsVectorFileWriter.NoError)
//        qDebug() << "success again!" << endl;

//    // export vector layer option 2: directly from features
//    // define fields for feature attributes. A QgsFields object is needed
//    fields = QgsFields();
//    fields.append(QgsField("x", QVariant.Int));
//    fields.append(QgsField("y", QVariant.String));
//    fields.append(QgsField("z", QVariant.String));

//    /* create an instance of vector file writer, which will create the vector file.
//    Arguments:
//    1. path to new file (will fail if exists already)
//    2. encoding of the attributes
//    3. field map
//    4. geometry type - from WKBTYPE enum
//    5. layer's spatial reference (instance of
//       QgsCoordinateReferenceSystem) - optional
//    6. driver name for the output file */
////    writer = QgsVectorFileWriter("my_shapes.shp", "CP1250", fields, QGis.WKBPoint, None, "ESRI Shapefile");
//    writer = QgsVectorFileWriter("my_shapes.shp", "CP1250", fields, QGis.WKBPoint, None, "delimitedtext");

//    if(writer.hasError() != QgsVectorFileWriter.NoError)
//        qDebug() << "Error when creating shapefile: " << w.errorMessage() << endl;

//    // add a feature
//    fet = QgsFeature();
//    fet.setGeometry(QgsGeometry.fromPoint(QgsPoint(10,10)));
//    fet.setAttributes([1, "text"]);
//    writer.addFeature(fet);

//    // delete the writer to flush features to disk
//    del writer;

//    //********************************
//    // 13. Memory Provider
//    //********************************
//    // Memory provider is intended to be used mainly by plugin or 3rd party app developers.
//    // It does not store data on disk, allowing developers to use it as a fast backend for some temporary layers.
//    // The constructor also takes a URI defining the geometry type of the layer, one of:
//    //"Point", "LineString", "Polygon", "MultiPoint", "MultiLineString", or "MultiPolygon".

//    // create layer
//    vl = QgsVectorLayer("Point", "temporary_points", "memory");
//    pr = vl.dataProvider();

//    // add fields
//    pr.addAttributes([QgsField("name", QVariant.String), QgsField("age",  QVariant.Int), QgsField("size", QVariant.Double)]);
//    vl.updateFields(); // tell the vector layer to fetch changes from the provider

//    // add a feature
//    fet = QgsFeature();
//    fet.setGeometry(QgsGeometry.fromPoint(QgsPoint(10,10)));
//    fet.setAttributes(["Johny", 2, 0.3]);
//    pr.addFeatures([fet]);

//    // update layer's extent when new features have been added
//    // because change of extent in provider is not propagated to the layer
//    vl.updateExtents();

//    // Verify success
//    // show some stats
//    print "fields:", len(pr.fields());
//    print "features:", pr.featureCount();
//    e = layer.extent();
//    qDebug() << "extent:" << e.xMiniminum() << e.yMinimum() << e.xMaximum() << e.yMaximum() << endl;

//    // iterate over features
//    f = QgsFeature();
//    features = vl.getFeatures();
//    foreach(f, features)
//        qDebug() << "F:" << f.id() << f.attributes() << f.geometry().asPoint() << endl;



//    //********************************
//    // 14. Appearance (Symbology) of Vector Layers
//    //********************************
//    renderer = layer.rendererV2()

//    qDebug() << "Type:" << rendererV2.type() << endl;

//    qDebug() << "renderersList() = " << QgsRendererV2Registry.instance().renderersList() << endl;
//    /* Print:
//    [u'singleSymbol',
//    u'categorizedSymbol',
//    u'graduatedSymbol',
//    u'RuleRenderer',
//    u'pointDisplacement',
//    u'invertedPolygonRenderer',
//    u'heatmapRenderer']*/

//    qDebug() <<  rendererV2.dump() << endl;


//    //********************************
//    // 15. Single Symbol Renderer
//    //********************************
//    symbol = QgsMarkerSymbolV2.createSimple({'name': 'square', 'color': 'red'});
//    layer.rendererV2().setSymbol(symbol);

//    qDebug() << "SingleSymbolLayerProps = " << layer.rendererV2().symbol().symbolLayers()[0].properties() << endl;
//    /* Prints
//    {u'angle': u'0',
//    u'color': u'0,128,0,255',
//    u'horizontal_anchor_point': u'1',
//    u'name': u'circle',
//    u'offset': u'0,0',
//    u'offset_map_unit_scale': u'0,0',
//    u'offset_unit': u'MM',
//    u'outline_color': u'0,0,0,255',
//    u'outline_style': u'solid',
//    u'outline_width': u'0',
//    u'outline_width_map_unit_scale': u'0,0',
//    u'outline_width_unit': u'MM',
//    u'scale_method': u'area',
//    u'size': u'2',
//    u'size_map_unit_scale': u'0,0',
//    u'size_unit': u'MM',
//    u'vertical_anchor_point': u'1'}*/

//    // You can alter a single property...
//    layer.rendererV2().symbol().symbolLayer(0).setName("square");
//    // ... but not all properties are accessible from methods,
//    // you can also replace the symbol completely:
//    props = layer.rendererV2().symbol().symbolLayer(0).properties();
//    props["color"] = "yellow";
//    props["name"] = "square";
//    layer.rendererV2().setSymbol(QgsMarkerSymbolV2.createSimple(props));

//    //********************************
//    // 16. Categorized Symbol Renderer
//    //********************************
//    // to get list of categories
//    foreach(cat, rendererV2.categories())
//        qDebug() << cat.value().toString() << ": " << cat.label() << " :: " << str(cat.symbol()) << endl;




//    // To create exlusion symbol
//    // Get point symbol and overlay 2 rotated line symbols for an x or use just an x
//    // Get symbol list
//    symbolList = lyr.rendererV2().symbols()
//    symbol = symbolList[0]


//    //********************************
//    //
//    //********************************

//}

//void addCommand::edit2()
//{
//    layer =  QgsVectorLayer("Point?field=Rec_No:integer&field=Include:string(120)&field=Label:string(120)&field=X:double&field=Y:double&field=Z:double&field=Height:double&field=Project_Re:string(120)&field=NCA:string(120)&field=DayCrit:integer&field=EveCrit:integer&field=NightCrit:integer",
//                        item,"memory");
//    QgsMapLayerRegistry.instance().addMapLayer(layer);

//    // Receivers = as in the above example 'Receivers' is a list of results
//    foreach(int i, range(len(Receivers)))
//    {
//        // add a feature
//        feature = QgsFeature();

//        X = Receivers[i][3];
//        Y = Receivers[i][4];
//        feature.setGeometry( QgsGeometry.fromPoint(QgsPoint(X,Y)) );

//        // Details = as in the above example 'Details' is a list of results
//        if(Receivers[i][1] != 0)
//            Include = 'Yes';
//        else
//            Include = 'No';

//        values = [ QVariant(Receivers[i][0]), QVariant(Include), QVariant(Receivers[i][2]),
//                             QVariant(Receivers[i][3]), QVariant(Receivers[i][4]), QVariant(Receivers[i][5]), QVariant(Receivers[i][6]),
//                             QVariant(Details[0]), QVariant(Details[1]), QVariant(Details[2]), QVariant(Details[3]), QVariant(Details[4]) ];

//        feature.setAttributes(values);
//        layer.startEditing();
//        layer.addFeature(feature, True);
//        layer.commitChanges();
//    }
//}

//void AddCommand::edit3()
//{
//    // create layer
//    vl = QgsVectorLayer("Point", "temporary_points", "memory");
//    pr = vl.dataProvider();

//    // changes are only possible when editing the layer
//    vl.startEditing();
//    // add fields
//    pr.addAttributes([QgsField("name", QVariant.String),QgsField("age", QVariant.Int),QgsField("size", QVariant.Double)]);

//    // add a feature
//    fet = QgsFeature();
//    fet.setGeometry(QgsGeometry.fromPoint(QgsPoint(10,10)));
//    fet.setAttributes(["Johny", 2, 0.3]);
//    pr.addFeatures([fet]);

//    // commit to stop editing the layer
//    vl.commitChanges();

//    // update layer's extent when new features have been added
//    // because change of extent in provider is not propagated to the layer
//    vl.updateExtents();

//    // add layer to the legend
//    QgsMapLayerRegistry.instance().addMapLayer(vl);

//}



































// When Adding Groups
QString createCommandString(QString &name, const QgsLayerTreeGroup *initialPosition)
{
    return QObject::tr("%1 to (%2)").arg(name).arg(initialPosition->name());
}
// When Adding Layers
QString createCommandString(QStringList myFileNameList, const QgsLayerTreeGroup *initialPosition)
{
    return QObject::tr("%1 to (%2)").arg(myFileNameList.size() > 1 ? "Files" : "File").arg(initialPosition == QgsProject::instance()->layerTreeRoot() ? "Root" : initialPosition->name());
}

QString createCommandString(QStringList myFileNameList, const int &initialPosition)
{
    return QObject::tr("%1 at (%2)").arg(myFileNameList.size() > 1 ? "Files" : "File").arg(initialPosition);
}
QString createCommandString(int isGroup, const int &initialPosition)
{
    return QObject::tr("%1 at (%2)").arg(isGroup == 0 ? "Layer" : "Group").arg(initialPosition);
}
QString createCommandString(int isGroup, const QString &name)
{
    return QObject::tr("%1 as2 (%2)").arg(isGroup == 0 ? "Layer" : "Group").arg(name);
}
QString createCommandString(QgsLayerTreeNode::NodeType nodetype, const QgsLayerTreeView *view)
{
    return QObject::tr("%1 as (%2)").arg(nodetype == QgsLayerTreeNode::NodeType::NodeLayer ? "Layer" : "Group").arg(nodetype == QgsLayerTreeNode::NodeType::NodeLayer ? view->currentLayer()->name() : view->currentGroupNode()->name());
}



























