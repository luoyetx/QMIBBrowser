#include <QFile>
#include <QDebug>
#include <QTextStream>

#include "mibtree.h"

MIBTree::MIBTree(QTreeWidget *tree)
{
    //Initial Root
    root = new QTreeWidgetItem(tree);
    MIBNode *rootNode = newNode();
    rootNode->name = "mgmt";
    rootNode->oid = "1.3.6.1.2";

    root->setData(0, Qt::UserRole, QVariant::fromValue(rootNode));
    root->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    root->setText(0, "mgmt");
}

MIBTree::~MIBTree()
{
    //********destroy MIBNode data*********[IMPORTANT]
    destroyTree(root);
}

void MIBTree::destroyTree(QTreeWidgetItem *node)
{
    for (int i=0; i<node->childCount(); i++)
        destroyTree(node->child(i));
    delete node->data(0, Qt::UserRole).value<MIBNode*>();
}

/*Add node BY ParentName NodeName Position*/
void MIBTree::addNode(QString &parentName, QString &nodeName, QString &pos, MIBNode *node=NULL)
{
    //find parent node
    QTreeWidgetItem *parent;
    parent = findNodeItemByName(root, parentName);
    if (parent==NULL) {
        //Can't find parent Node
        //Do nothing
        return;
    }
    MIBNode *parentNode = parent->data(0, Qt::UserRole).value<MIBNode*>();
    if (node==NULL) {
        node = newNode();
        node->name = nodeName;
    }
    //set oid
    node->oid = parentNode->oid + "." + pos;
    QTreeWidgetItem *nodeItem = new QTreeWidgetItem(parent);
    parent->addChild(nodeItem);
    nodeItem->setData(0, Qt::UserRole, QVariant::fromValue(node));
    nodeItem->setText(0, node->name);
    qDebug() << parentName << "--->" << nodeName << "--->" << node->oid << "installed";
    qDebug() << nodeItem->childCount();
    //TODO
}

Status MIBTree::loadMIB(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Open file " << fileName << " Failed";
        return Status_FAILED;
    }
    QTextStream in(&file);
    QString line;
    while(!in.atEnd()) {
        line = in.readLine();
        if (line.length()==0) continue;
        if (line.indexOf("--")==0) continue;

        if (line.indexOf("--")>0)
            line = line.left(line.indexOf("--"));
        line = line.trimmed();
        /*******************************************************************
         *mib-2(nodeName) OBJECT IDENTIFIER ::= { mgmt(parentName) 1(pos) }
         *******************************************************************/
        if (line.indexOf("OBJECT IDENTIFIER ::=")>2)
            if (line.indexOf('{')>10 && line.indexOf('}')>10) {
                QString parentName, nodeName, pos;
                nodeName = line.left(line.indexOf("OBJECT IDENTIFIER ::="));
                int n1=line.indexOf('{')+1;
                int n2=line.indexOf('}')-n1;
                line = line.mid(n1,n2);
                line = line.trimmed();
                parentName = line.left(line.indexOf(' '));
                pos = line.mid(line.indexOf(' '));
                nodeName = nodeName.trimmed();
                parentName = parentName.trimmed();
                pos = pos.trimmed();
                //Add (ParentNonde, NodeName, POS) to MIBTree
                addNode(parentName, nodeName, pos);
                continue;
            }
        /**********************************************************************
         *ciscoCdpMIB(nodeName)   MODULE-IDENTITY
         *................................
         *................................
         *      ::= { ciscoMgmt(parentName) 23(pos) }
         **********************************************************************/
        if (line.indexOf("MODULE-IDENTITY")>2 && line.indexOf(',')==-1) {
            QString parentName, nodeName, pos;
            nodeName = line.left(line.indexOf("MODULE-IDENTITY"));
            while (!in.atEnd()) {
                line = in.readLine();
                if (line.length()==0 || line.indexOf("--")==0) continue;
                if (line.indexOf("--")>0)
                    line = line.left(line.indexOf("--"));
                line = line.trimmed();
                if (line.indexOf("::=")==0 && line.indexOf('{')>3 && line.indexOf('}')>3) {
                    int n1=line.indexOf('{')+1;
                    int n2=line.indexOf('}')-n1;
                    line = line.mid(n1,n2);
                    line = line.trimmed();
                    parentName = line.left(line.indexOf(' '));
                    pos = line.mid(line.indexOf(' '));
                    nodeName = nodeName.trimmed();
                    parentName = parentName.trimmed();
                    pos = pos.trimmed();
                    //Add (ParentNonde, NodeName, POS) to MIBTree
                    addNode(parentName, nodeName, pos);
                    break;
                }
            }
            continue;
        }
        /**********************************************************************
         *transmission(nodeName) OBJECT-IDENTITY ::= { mib-2(parentName) 10(pos) }
         **********************************************************************/
        if (line.indexOf("OBJECT-IDENTITY")>2 && line.indexOf(',')==-1) {
            QString parentName, nodeName, pos;
            nodeName = line.left(line.indexOf("OBJECT-IDENTITY"));
            int n1=line.indexOf('{')+1;
            int n2=line.indexOf('}')-n1;
            line = line.mid(n1,n2);
            line  = line.trimmed();
            parentName = line.left(line.indexOf(' '));
            pos = line.mid(line.indexOf(' '));
            nodeName = nodeName.trimmed();
            parentName = parentName.trimmed();
            pos = pos.trimmed();
            //Add (ParentNonde, NodeName, POS) to MIBTree
            addNode(parentName, nodeName, pos);
            continue;
        }

        if (line.indexOf("OBJECT-TYPE")>2 && line.indexOf(',')==-1) {
            QString parentName, nodeName, pos;
            MIBNode *node = newNode();
            nodeName = line.left(line.indexOf("OBJECT-TYPE"));
            nodeName = nodeName.trimmed();
            node->name = nodeName;
            while (!in.atEnd()) {
                line = in.readLine();
                if (line.length()==0 || line.indexOf("--")==0) continue;
                if (line.indexOf("--")>0)
                    line = line.left(line.indexOf("--"));
                line = line.trimmed();
                //SYNTAX
                if (line.indexOf("SYNTAX")==0) {
                    //SYNTAX INTEGER
                    if (line.indexOf("INTEGER")>0) {
                        node->syntax = "INTEGER";
                        //SYNTAX INTEGER { up(1),down(2),testing(3) }
                        //SYNTAX INTEGER ( up(1),down(2),testing(3) )
                        //if (line.indexOf('(')>10 && line.indexOf(')')>10)
                        //    continue;
                        /********************************************
                         *SYNTAX INTEGER {
                         *      forwarding(1),
                         *      not-forwarding(2)
                         *}
                         ********************************************/
                        if (line.indexOf('{')>10 && line.indexOf('}')==-1) {
                            while (!in.atEnd()) {
                                line = in.readLine();
                                if (line.indexOf('}')!=-1) break;
                            }
                            continue;
                        }
                    }
                    //SYNTAX else
                    else {
                        line = line.mid(6);
                        line = line.trimmed();
                        node->syntax = line;
                    }
                    continue;
                }
                /********************************************
                 *SYNTAX INTEGER
                 *{
                 *      forwarding(1),
                 *      not-forwarding(2)
                 *}
                 ********************************************/
                if (line.indexOf('{')==0 && node->syntax=="INTEGER") {
                    while (!in.atEnd()) {
                        line = in.readLine();
                        if (line.length()==0 || line.indexOf("--")==0) continue;
                        if (line.indexOf("--")>0)
                            line = line.left(line.indexOf("--"));
                        line = line.trimmed();
                        if (line.indexOf('}')!=-1) break;
                    }
                    continue;
                }
                //ACCESS
                if (line.indexOf("ACCESS")==0 || line.indexOf("MAX-ACCESS")==0) {
                    line = line.mid(line.indexOf(' '));
                    line = line.trimmed();
                    node->access = line;
                    continue;
                }
                //STATUS
                if (line.indexOf("STATUS")==0) {
                    line = line.mid(line.indexOf(' '));
                    line = line.trimmed();
                    node->status = line;
                    continue;
                }
                //INDEX
                if (line.indexOf("INDEX")==0) {
                    line = line.mid(line.indexOf(' '));
                    line = line.trimmed();
                    //INDEX { ipNetToMediaIfIndex, ipNetToMediaNetAddress }
                    node->index = line;
                    /*************************************************
                     *INDEX { ipNetToMediaIfIndex,
                     *        ipNetToMediaNetAddress
                     *}
                     **************************************************/
                    if (line.indexOf('}')==-1) {
                        while (!in.atEnd()) {
                            line = in.readLine();
                            if (line.length()==0 || line.indexOf("--")==0) continue;
                            if (line.indexOf("--")>0)
                                line = line.left(line.indexOf("--"));
                            line = line.trimmed();
                            node->index += ' ' + line;
                            if (line.indexOf('}')!=-1) break;
                        }
                    }
                    continue;
                }
                //DESCRIPTION
                /*****************************************************
                 *DESCRIPTION
                 *      "A list of interface entries.  The number of
                 *       entries is given by the value of ifNumber."
                 *****************************************************/
                if (line.indexOf("DESCRIPTION")==0) {
                    QString descr;
                    while (!in.atEnd()) {
                        line = in.readLine();
                        if (line.length()==0 || line.indexOf("--")==0) continue;
                        if (line.indexOf("--")>0)
                            line = line.left(line.indexOf("--"));
                        line = line.trimmed();
                        descr += ' ' + line;
                        if (line.indexOf('\"',2)>2) break;
                    }
                    node->description = descr.simplified();
                    continue;
                }
                //LAST
                if (line.indexOf("::=")==0 && line.indexOf('}')>3 && line.indexOf('{')>1) {
                    int n1=line.indexOf('{')+1;
                    int n2=line.indexOf('}')-n1;
                    line = line.mid(n1,n2);
                    line = line.trimmed();
                    parentName = line.left(line.indexOf(' '));
                    pos = line.mid(line.indexOf(' '));
                    parentName = parentName.trimmed();
                    pos = pos.trimmed();
                    //Add (ParentNonde, NodeName, POS) to MIBTree
                    addNode(parentName, nodeName, pos, node);
                    break;
                }
            }
        }
    }
    file.close();
    correctTree(root, false);
    return Status_SUCCESS;
}

void MIBTree::correctTree(QTreeWidgetItem *node, bool flag)
/*flag=true if parent node has index value*/
{
    if (flag==false && node->childCount()==0) {
        node->data(0,Qt::UserRole).value<MIBNode*>()->oidplus = ".0";
        return;
    }
    for (int i=0; i<node->childCount(); i++)
        if (node->data(0, Qt::UserRole).value<MIBNode*>()->index=="")
            correctTree(node->child(i), false);
        else correctTree(node->child(i), true);
}

MIBNode* MIBTree::newNode()
{
    MIBNode *node = new MIBNode;
    node->name = "";
    node->syntax = "";
    node->access = "";
    node->status = "";
    node->index = "";
    node->description = "";
    node->oidplus = "";
    return node;
}

MIBNode* MIBTree::getNodeByName(QString &name)
{
    QTreeWidgetItem *rv;
    rv = findNodeItemByName(root, name);
    if (rv!=NULL) return rv->data(0, Qt::UserRole).value<MIBNode*>();
    else return NULL;
}

MIBNode* MIBTree::getNodeByOid(QString &oid)
{
    QTreeWidgetItem *rv;
    rv = findNodeItemByOid(root, oid);
    if (rv!=NULL) return rv->data(0, Qt::UserRole).value<MIBNode*>();
    else return NULL;
}

QTreeWidgetItem* MIBTree::findNodeItemByName(QTreeWidgetItem *node, QString &name)
/*DFS*/
{
    if (node->data(0, Qt::UserRole).value<MIBNode*>()->name == name)
        //find node
        return node;
    else {
        //search in children
        QTreeWidgetItem *temp;
        for (int i=0; i<node->childCount(); i++) {
            temp = findNodeItemByName(node->child(i), name);
            if (temp != NULL)
                return temp;
        }
    }
    return NULL;
}

QTreeWidgetItem* MIBTree::findNodeItemByOid(QTreeWidgetItem *node, QString &oid)
/*DFS*/
{
    if (node->data(0, Qt::UserRole).value<MIBNode*>()->oid == oid) {
        //find node
        return node;
    }
    else {
        QTreeWidgetItem *temp;
        for (int i=0; i<node->childCount(); i++) {
            temp = findNodeItemByOid(node->child(i), oid);
            if (temp != NULL)
                return temp;
        }
    }
    return NULL;
}
