#ifndef MIBTREE_H
#define MIBTREE_H

#include <QTreeWidget>
#include "common.h"

typedef struct {
    QString name;
    QString syntax;
    QString access;
    QString status;
    QString index;
    QString description;
} MIBNode;
//Registe MIBNode to QMetaType
Q_DECLARE_METATYPE(MIBNode*)

class MIBTree {

public:
    MIBTree(QTreeWidget *tree);
    ~MIBTree();
    //load MIB file to tree
    Status loadMIB(QString fileName);
    MIBNode* getNodeByOid(QString oid);
    MIBNode* getNodeByName(QString name);

private:
    //Tree Root
    QTreeWidgetItem *root;
    void destroyTree(QTreeWidgetItem *node);
    //add node to MIBTree
    void addNode(QString &parentName, QString &nodeName, QString &pos, MIBNode *node);
    //init a new Node
    MIBNode* newNode();
    //find QTreeWidgetItem BY name
    QTreeWidgetItem* findNodeItemByName(QTreeWidgetItem *node, QString name);
};

#endif // MIBTREE_H
