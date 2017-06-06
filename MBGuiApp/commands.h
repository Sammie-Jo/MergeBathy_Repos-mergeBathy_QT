#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>

//QGis Includes
#include <qgsmapcanvas.h>
#include <qgslayertreeview.h>
#include <qgslayertreenode.h>
#include <qgsmaplayer.h>
#ifdef Qt5
    #include <qgsrenderer.h>
#else
    #include <qgsrendererv2.h>
#endif

//********************************
// RenameCommand
//********************************
class RenameCommand : public QUndoCommand
{
public:
    explicit RenameCommand(QgsLayerTreeNode::NodeType nodetype,QgsLayerTreeView * view, QUndoCommand *parent = 0);
    ~RenameCommand();

    void undo() override;
    void redo() override;

private:
    QString newName;
    QString oldName;
    bool ok;
    QgsLayerTreeView * view;
};
//********************************
// DeleteCommand
//********************************
class DeleteCommand : public QUndoCommand
{
public:
    explicit DeleteCommand(QgsLayerTreeGroup *root,QgsLayerTreeGroup *root_yz,
                           QgsLayerTreeView * view,QgsLayerTreeView * view_yz, QUndoCommand *parent = 0);

    void undo() override;
    void redo() override;

private:
    QgsMapLayer *layer;
    QgsMapLayer *layer_yz;
    QgsLayerTreeGroup *root;
    QgsLayerTreeGroup *root_yz;
    QgsLayerTreeView * view;
    QgsLayerTreeView * view_yz;
    QStringList myFileNameList;
    #ifdef Qt5
        QList<QgsMapLayer*> myLayerSet;
        QList<QgsMapLayer*> myLayerSet_yz;
    #else
        QList<QgsMapCanvasLayer> myLayerSet;
        QList<QgsMapCanvasLayer> myLayerSet_yz;
    #endif
};
QString createCommandString(QString &name, const QgsLayerTreeGroup *initialPosition);
QString createCommandString(QStringList myFileNameList, const QgsLayerTreeGroup *initialPosition);
QString createCommandString(QStringList myFileNameList, const int &initialPosition);
QString createCommandString(int isGroup, const int &initialPosition);
QString createCommandString(int isGroup, const QString &name);
QString createCommandString(QgsLayerTreeNode::NodeType nodetype, const QgsLayerTreeView *view);

//********************************
// AddGroupCommand
//********************************
class AddGroupCommand : public QObject, public QUndoCommand
{
public:
    AddGroupCommand(QgsLayerTreeGroup * root,QgsLayerTreeGroup * root_yz ,
                    QgsLayerTreeView * view,QgsLayerTreeView * view_yz, QUndoCommand *parent = 0);
    ~AddGroupCommand();

    void undo() override;
    void redo() override;

private:

    QgsLayerTreeView * view;
    QgsLayerTreeView * view_yz;
    QgsLayerTreeGroup * initialPosition;
    QgsLayerTreeGroup * initialPosition_yz;
//    QgsLayerTreeNode * node;
    QgsLayerTreeGroup * newGroup;
    QgsLayerTreeGroup * newGroup_yz;
    QgsLayerTreeGroup * newGroupParent;
    QgsLayerTreeGroup * newGroupParent_yz;
    QgsLayerTreeGroup * root;
    QgsLayerTreeGroup * root_yz;

};
//********************************
// AddCommand
//********************************
class AddCommand : public QObject, public QUndoCommand
{
public:
    #ifdef Qt5
        AddCommand(QgsLayerTreeGroup * root,QgsLayerTreeGroup * root_yz ,
                   QgsMapCanvas * canvas_xy, QgsMapCanvas * canvas_yz,
                   QList<QgsMapLayer> myLayerSet, QList<QgsMapLayer> myLayerSet_yz,
                   QgsLayerTreeView * view,QgsLayerTreeView * view_yz,
                   QMap<QString,QStack<QgsFeatureIds>*> *selectedMap,QMap<QString,QStack<QgsFeatureIds>*> *selectedMap_yz,QMap<QString,QString> *layerSuffixes,QUndoCommand *parent = 0);
    #else
        AddCommand(QgsLayerTreeGroup * root,QgsLayerTreeGroup * root_yz ,
                   QgsMapCanvas * canvas_xy, QgsMapCanvas * canvas_yz,
                   QList<QgsMapCanvasLayer> myLayerSet,QList<QgsMapCanvasLayer> myLayerSet_yz,
                   QgsLayerTreeView * view,QgsLayerTreeView * view_yz,
                   QMap<QString,QStack<QgsFeatureIds>*> *selectedMap,QMap<QString,QStack<QgsFeatureIds>*> *selectedMap_yz,QMap<QString,QString> *layerSuffixes,QUndoCommand *parent = 0);
    #endif

    ~AddCommand();

    void undo() override;
    void redo() override;
//void edit1();
//void edit2();
//void edit3();
void addField(QgsVectorLayer* mypLayer);
QgsVectorLayer* addCategorySymbols(QgsVectorLayer* mypLayer);

private:
    QStringList myFileNameList;
    #ifdef Qt5
        QList<QgsMapLayer> *myLayerSet;
        QList<QgsMapLayer> *myLayerSet_yz;
    #else
        QList<QgsMapCanvasLayer> myLayerSet;
        QList<QgsMapCanvasLayer> myLayerSet_yz;
    #endif
    QgsLayerTreeView * view;
    QgsLayerTreeView * view_yz;
    QgsLayerTreeGroup * initialPosition;
    QgsLayerTreeGroup * initialPosition_yz;
    QgsMapCanvas * canvas_xy;
    QgsMapCanvas * canvas_yz;
    QgsLayerTreeGroup * root;
    QgsLayerTreeGroup * root_yz;
#ifdef Qt5
    QgsFeatureRenderer *renderer;
#else
    QgsFeatureRendererV2 *renderer;
#endif
    QMap<QString,QStack<QgsFeatureIds>*> *selectedMap;
    QMap<QString,QStack<QgsFeatureIds>*> *selectedMap_yz;
    QMap<QString,QString> *layerSuffixes;

};
//********************************
// SelectedCommand
//********************************
class SelectedCommand : public QObject, public QUndoCommand
{
public:
//    SelectedCommand(QgsLayerTreeView * view, QMap<QString,QStack<QgsFeatureIds>*> *selectedMap,QUndoCommand *parent = 0);

    SelectedCommand(QgsLayerTreeView * view, QgsLayerTreeView *view_yz,
                    QMap<QString,QStack<QgsFeatureIds>*> *selectedMap,QMap<QString,QStack<QgsFeatureIds>*> *selectedMap_yz,QUndoCommand *parent = 0);
    ~SelectedCommand();

    void undo() override;
    void redo() override;

    void toggleField(QgsVectorLayer* mypLayer);

private:
    QgsLayerTreeView * view;
    QgsLayerTreeView * view_yz;
    QMap<QString,QStack<QgsFeatureIds>*> *selectedMap;
    QMap<QString,QStack<QgsFeatureIds>*> *selectedMap_yz;


};

#endif // COMMANDS_H
