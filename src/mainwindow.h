#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTableWidget>
#include <QToolBar>
#include <QLabel>

#include "common.h"
#include "snmpmanager.h"
#include "mibtree.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    RequestInfo *requestInfo;
    MIBTree *mibTree;
    //Request *request;

    /*initial Class*/
    void initialRequestInfo(RequestInfo *requestInfo);
    void initialWidgets();
    void initialMIBTreeWidget(QTreeWidget *MIBTreeWidget);
    void initialMIBTableWidget(QTableWidget *MIBTableWidget);
    void initialResultTableWidget(QTableWidget *resultTableWidget);
    void initialMenuToolBar(QToolBar *menuToolBar);
    void initialConnections(void);

    /*Update Widget Value with the given RequestInfo*/
    void updateWidgetValue(RequestInfo *requestInfo);

    /*Set up Request according to the Operation*/
    Status setUpRequest(Operation operation, Request *request);
    Status setUpRequestGet(Request *request);
    Status setUpRequestGetNext(Request *request);
    Status setUpRequestSet(Request *request);

    /*Handle Request*/
    void handleRequest(Operation operation, Request *request);

    /*Check the Request Valid or Not*/
    Status checkRequest(Request *request);

signals:
    void statusBarUpdate(QString messages);

public slots:
    /*connection with AdvancedOptionDialog*/
    void updateRequestInfo(RequestInfo *requestInfo);

private slots:
    void onIPLineEditChanged(const QString &ip);
    void onOidLineEditChanged(const QString &oid);
    void onAdvancedOptionPushButtonClicked();
    void onGoPushButtonClicked();
    /*Reset ResultTableWidget*/
    void resetResultTableWidget();
    /*Open Dialog to load MIB txt*/
    void loadMIB();
    /**/
    void onTreeItemClicked(QTreeWidgetItem *node);
    void onResultTableClicked(int row, int column);
    void resultTableDetail(int row, int column);
    /*Actions*/
    void onActionGet();
    void onActionGetNext();
    void onActionSet();
    /*About*/
    void aboutThis();
};

#endif // MAINWINDOW_H
