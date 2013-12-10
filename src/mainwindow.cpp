#include <QDebug>
#include <QVariant>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "resultdetaildialog.h"
#include "advancedoptiondialog.h"
#include "setrequestdialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    requestInfo(new RequestInfo)//, request(new Request)
{
    ui->setupUi(this);
    initialWidgets();
    initialConnections();
    initialRequestInfo(requestInfo);
    updateWidgetValue(requestInfo);
    setWindowTitle("QMIB");
}

MainWindow::~MainWindow()
{
    delete mibTree;
    delete requestInfo;
    //remove table content[IMPORTANT]
    ui->MIBTableWidget->clearContents();
    ui->resultTableWidget->clearContents();
    delete ui;
    //delete request;
}

void MainWindow::initialRequestInfo(RequestInfo *requestInfo)
{
    requestInfo->ip = "127.0.0.1";
    requestInfo->port = "161";
    requestInfo->oid = "";
    requestInfo->community = "public";
    requestInfo->retry = 1;
    requestInfo->timeout = 100;
    requestInfo->version = version1;
}

void MainWindow::initialWidgets()
{
    ui->OidLineEdit->setReadOnly(true);
    initialMIBTreeWidget(ui->MIBTreeWidget);
    initialMIBTableWidget(ui->MIBTableWidget);
    initialResultTableWidget(ui->resultTableWidget);
    initialMenuToolBar(ui->mainToolBar);
    /*initialize OperationComboBox*/
    ui->operationComboBox->addItem("Get", QVariant(OperationGet));
    ui->operationComboBox->addItem("GetNext", QVariant(OperationGetNext));
    ui->operationComboBox->addItem("Set", QVariant(OperationSet));
    /*set splitter*/
    ui->splitter->setStretchFactor(1, 1);
}

void MainWindow::initialMIBTreeWidget(QTreeWidget *MIBTreeWidget)
{
    MIBTreeWidget->setColumnCount(1);
    MIBTreeWidget->setHeaderLabel("MIB");
    MIBTreeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);
    MIBTreeWidget->header()->setStretchLastSection(false);
    mibTree = new MIBTree(MIBTreeWidget);
}

void MainWindow::initialMIBTableWidget(QTableWidget *MIBTableWidget)
{
    qDebug() << ">>>>>>>>>>>Initialize MIBTableWidget<<<<<<<<<<<<<";
    /*Set Table Header*/
    QStringList verticalHeader;
    QStringList horizontalHeader;
    verticalHeader << "Name" << "Oid" << "Syntax" << "Access";
    verticalHeader << "Status" << "Index" << "Descr";
    horizontalHeader << "Value";
    MIBTableWidget->setColumnCount(horizontalHeader.length());
    MIBTableWidget->setRowCount(verticalHeader.length());
    MIBTableWidget->setVerticalHeaderLabels(verticalHeader);
    MIBTableWidget->setHorizontalHeaderLabels(horizontalHeader);
    /*Set Table Display Property*/
    MIBTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    MIBTableWidget->horizontalHeader()->setVisible(false);
    MIBTableWidget->horizontalHeader()->setStretchLastSection(true);
    MIBTableWidget->setRowHeight(6, 120);
    /*Add Items to Table*/
    QTableWidgetItem *itemName = new QTableWidgetItem("");
    QTableWidgetItem *itemOid = new QTableWidgetItem("");
    QTableWidgetItem *itemSyntax = new QTableWidgetItem("");
    QTableWidgetItem *itemAccess = new QTableWidgetItem("");
    QTableWidgetItem *itemStatus = new QTableWidgetItem("");
    QTableWidgetItem *itemIndex = new QTableWidgetItem("");
    QTableWidgetItem *itemDescr = new QTableWidgetItem("");
    //itemDescr->setWordWrap(true);
    //itemDescr->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    itemDescr->setTextAlignment(Qt::AlignLeft|Qt::AlignTop);
    MIBTableWidget->setItem(0, 0, itemName);
    MIBTableWidget->setItem(1, 0, itemOid);
    MIBTableWidget->setItem(2, 0, itemSyntax);
    MIBTableWidget->setItem(3, 0, itemAccess);
    MIBTableWidget->setItem(4, 0, itemStatus);
    MIBTableWidget->setItem(5, 0, itemIndex);
    MIBTableWidget->setItem(6, 0, itemDescr);
}

void MainWindow::initialResultTableWidget(QTableWidget *resultTableWidget)
{
    qDebug() << ">>>>>>>>>initialize resultTableWidget<<<<<<<<<<<<<";
    /*Set Table Header*/
    QStringList horizontalHeader;
    horizontalHeader << "Name/Oid" << "Type" << "Value";
    resultTableWidget->setColumnCount(horizontalHeader.length());
    resultTableWidget->setHorizontalHeaderLabels(horizontalHeader);
    /*Set Table Display Property*/
    resultTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    resultTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultTableWidget->verticalHeader()->setVisible(false);
    resultTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

void MainWindow::initialMenuToolBar(QToolBar *menuToolBar)
{
    //Add quick actions
    menuToolBar->addAction(ui->actionGet);
    menuToolBar->addAction(ui->actionGetNext);
    menuToolBar->addAction(ui->actionSet);
    menuToolBar->addAction(ui->actionClear);
}

/*Set up connections between widgets*/
void MainWindow::initialConnections()
{
    /*Initialize Actions*/
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), QApplication::instance(), SLOT(aboutQt()));
    connect(ui->actionAbout_this, SIGNAL(triggered()), this, SLOT(aboutThis()));
    /*Initialize Connections between Widgets*/
    connect(ui->IPLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onIPLineEditChanged(QString)));
    connect(ui->OidLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onOidLineEditChanged(QString)));
    connect(ui->advancedOptionPushButton, SIGNAL(clicked()), this, SLOT(onAdvancedOptionPushButtonClicked()));
    connect(ui->goPushButton, SIGNAL(clicked()), this, SLOT(onGoPushButtonClicked()));
    /*connect actions*/
    connect(ui->actionClear, SIGNAL(triggered()), this, SLOT(resetResultTableWidget()));
    connect(ui->actionLoad_MIB, SIGNAL(triggered()), this, SLOT(loadMIB()));
    connect(this, SIGNAL(statusBarUpdate(QString)), ui->statusBar, SLOT(showMessage(QString)));
    //TODO
    /*ResultTable*/
    connect(ui->resultTableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(onResultTableClicked(int,int)));
    connect(ui->resultTableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(resultTableDetail(int,int)));
    /*connect MIBTree*/
    connect(ui->MIBTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(onTreeItemClicked(QTreeWidgetItem*)));
    /*connect quick actions*/
    connect(ui->actionGet, SIGNAL(triggered()), this, SLOT(onActionGet()));
    connect(ui->actionGetNext, SIGNAL(triggered()), this, SLOT(onActionGetNext()));
    connect(ui->actionSet, SIGNAL(triggered()), this, SLOT(onActionSet()));
}

/*Update Widget Value with the given RequestInfo*/
void MainWindow::updateWidgetValue(RequestInfo *requestInfo)
{
    ui->IPLineEdit->setText(requestInfo->ip);
    ui->OidLineEdit->setText(requestInfo->oid);
}

/*Set up Request according to the Operation*/
Status MainWindow::setUpRequest(Operation operation, Request *request)
{
    qDebug() << "---------Setup Request " << operation << " ---------------";
    Status status;

    /*Update status Bar*/
    emit statusBarUpdate("Setup Request");

    /*Set Operation*/
    request->operation = operation;
    /*Set UdpAddress*/
    std::string ip;
    ip = requestInfo->ip.toStdString();
    UdpAddress udp(ip.c_str());
    bool ok;
    int port = requestInfo->port.toInt(&ok, 10);
    if (!ok) {
        /*error*/
        Helper::pop("Warning", "Wrong port");
        return Status_FAILED;
    }
    udp.set_port(port);
    request->address = udp;
    /*Set Version*/
    request->version = requestInfo->version;
    /*Set Community*/
    request->community = requestInfo->community.toStdString();
    /*Set Retry*/
    request->retry = requestInfo->retry;
    /*Set Timeout*/
    request->timeout = requestInfo->timeout;

    /*Pop Error MessegeBox in functions, Not handle here*/
    switch (operation) {
    case OperationGet:
        status = setUpRequestGet(request);
        break;
    case OperationGetNext:
        status = setUpRequestGetNext(request);
        break;
    case OperationSet:
        status = setUpRequestSet(request);
        break;
    default:
        status = Status_FAILED;
        break;
    }
    if (status == Status_SUCCESS)
        status = checkRequest(request);
    qDebug() << "##################Request Infomation####################";
    qDebug() << "Request address:       " << request->address.get_printable();
    qDebug() << "Request data(oid):     " << request->data.get_printable_oid();
    qDebug() << "Request data(value):   " << request->data.get_printable_value();
    qDebug() << "Request operation:     " << request->operation;
    qDebug() << "Request version:       " << request->version;
    qDebug() << "Request retry:         " << request->retry;
    qDebug() << "Request timeout:       " << request->timeout;
    qDebug() << "Request community:     " << request->community.c_str();
    qDebug() << "}";
    qDebug() << "########################################################";

    if (status == Status_SUCCESS)
        emit statusBarUpdate("Setup Request Successfully");
    return status;
}

/*Set up GetRequest*/
Status MainWindow::setUpRequestGet(Request *request)
{
    request->data.clear();
    std::string oid;
    oid = requestInfo->oid.toStdString();
    //oid should be set
    if (oid == "") {
        emit statusBarUpdate("Setup Request Failed");
        Helper::pop("Warning", "Oid should be set");
        return Status_FAILED;
    }
    request->data.set_oid(Oid(oid.c_str()));
    return Status_SUCCESS;
}

/*Set up GetNextRequest*/
Status MainWindow::setUpRequestGetNext(Request *request)
{
    request->data.clear();
    std::string oid;
    oid = requestInfo->oid.toStdString();
    //oid should be set
    if (oid == "") {
        emit statusBarUpdate("Setup Request Failed");
        Helper::pop("Warning", "Oid should be set");
        return Status_FAILED;
    }
    request->data.set_oid(Oid(oid.c_str()));
    return Status_SUCCESS;
}

/*Set up SetRequest*/
Status MainWindow::setUpRequestSet(Request *request)
{
    request->data.clear();
    std::string oid;
    oid = requestInfo->oid.toStdString();
    //oid should be set
    if (oid == "") {
        emit statusBarUpdate("Setup Request Failed");
        Helper::pop("Warning", "Oid should be set");
        return Status_FAILED;
    }
    request->data.set_oid(Oid(oid.c_str()));
    MIBNode *node = mibTree->getNodeByOid(requestInfo->oid);
    if (node == NULL) {
        emit statusBarUpdate("Setup Request Failed");
        Helper::pop("Warning", "Wrong Oid");
        return Status_FAILED;
    }
    /*Pop Set Dialog*/
    SetRequestDialog *dialog = new SetRequestDialog(&(request->data), node->syntax, this);
    dialog->exec();
    if (dialog->cancel) {
        /*Cancel PushButton Clicked*/
        emit statusBarUpdate("Setup Request Failed");
        delete dialog;
        return Status_FAILED;
    }
    else {
        /*Ok PushButton Clicked*/
        request->data = *(dialog->getData());
        delete dialog;
        qDebug() << "#############RequestSet data############################";
        qDebug() << "oid:   " << request->data.get_printable_oid();
        qDebug() << "data:  " << request->data.get_printable_value();
        qDebug() << "########################################################";
        return Status_SUCCESS;
    }
}

/*Check the Request Valid or Not*/
Status MainWindow::checkRequest(Request *request)
{
    /*Check address*/
    if (!request->address.valid()) {
        /*error*/
        emit statusBarUpdate("Setup Request Failed");
        Helper::pop("Warning", "Wrong IP Address");
        return Status_FAILED;
    }
    /*Check oid*/
    Oid oid;
    request->data.get_oid(oid);
    if (oid=="") {
        emit statusBarUpdate("Setup Request Failed");
        Helper::pop("Warning", "Oid should be set");
        return Status_FAILED;
    }
    /*Check data*/
    switch (request->operation) {
    case OperationGet:
    case OperationGetNext:
        /*directly set null*/
        request->data.set_null();
        break;
    case OperationSet:
        /*value shoudn't be null(including OID)*/
        if (!request->data.valid()) {
            emit statusBarUpdate("Setup Request Failed");
            Helper::pop("Warning", "Value Should be set");
            return Status_FAILED;
        }
        break;
    default:
        return Status_FAILED;
        break;
    }
    /*Check version*/
    /*Check retry*/
    /*Check timeout*/
    /*Check community*/
    return Status_SUCCESS;
}

void MainWindow::handleRequest(Operation operation, Request *request)
{
    Status status;
    status = setUpRequest(operation, request);
    if (status == Status_SUCCESS) {
        /*Setup Request Correctly and Prepare to Send Request*/
        emit statusBarUpdate("Sending Request");
        status = SnmpManager::handleOperation(request);
        if (status == Status_SUCCESS) {
            /*Handle Request Successfully*/
            emit statusBarUpdate("Request Success");
            if (request->operation != OperationSet) {
                /*Update ResultTableWidget*/
                qDebug() << ">>>>>>>>>>>>>Insert Items to ResultTableWidget<<<<<<<<<<<";
                int row = ui->resultTableWidget->rowCount();
                ui->resultTableWidget->insertRow(row);
                //TODO, here just for debug
                /*Set Value Readable*/
                QTableWidgetItem *itemOid = new QTableWidgetItem("");
                QTableWidgetItem *itemValue = new QTableWidgetItem("");
                QTableWidgetItem *itemType = new QTableWidgetItem("");
                MIBNode *rv;
                QString oid = request->data.get_printable_oid();
                QString pos;

                pos = oid.mid(oid.lastIndexOf('.'));
                oid = oid.left(oid.lastIndexOf('.'));
                qDebug() << "######################Oid separated################";
                qDebug() << oid << " " << pos;
                qDebug() << "###################################################";

                rv = mibTree->getNodeByOid(oid);
                itemValue->setText(request->data.get_printable_value());
                itemType->setText(rv->syntax);
                itemOid->setText(rv->name+pos);
                itemValue->setToolTip(itemValue->text());
                ui->resultTableWidget->setItem(row, 0, itemOid);
                ui->resultTableWidget->setItem(row, 1, itemType);
                ui->resultTableWidget->setItem(row, 2, itemValue);
            }
            /*flesh Oid (Needed By OperationGetNext), but I'll flesh each time*/
            ui->OidLineEdit->setText(request->data.get_oid().get_printable());
        }
        else {
            /*Handle Reuqest Error*/
            emit statusBarUpdate("Request Fail");
            qDebug() << "Handle Requeset Error";
        }
    }
    else {
        /*Setup Request Error*/
        qDebug() << "Setup Request Error";
    }
}

/*Public Slots*/

/*Update RequestInfo with a given RequestInfo
 *This is a public slot for AdvancedOptionDialog
*/
void MainWindow::updateRequestInfo(RequestInfo *requestInfo)
{
    this->requestInfo->ip = requestInfo->ip;
    this->requestInfo->port = requestInfo->port;
    this->requestInfo->oid = requestInfo->oid;
    this->requestInfo->retry = requestInfo->retry;
    this->requestInfo->timeout = requestInfo->timeout;
    this->requestInfo->community = requestInfo->community;
    this->requestInfo->version = requestInfo->version;
    updateWidgetValue(this->requestInfo);
    qDebug() << "################RequestInfo Updated#####################";
    qDebug() << "requestInfo ip:        " << this->requestInfo->ip;
    qDebug() << "requestInfo port:      " << this->requestInfo->port;
    qDebug() << "requestInfo oid:       " << this->requestInfo->oid;
    qDebug() << "requestInfo community: " << this->requestInfo->community;
    qDebug() << "requestInfo retry:     " << this->requestInfo->retry;
    qDebug() << "requestInfo timeout:   " << this->requestInfo->timeout;
    qDebug() << "requestInfo version:   " << this->requestInfo->version;
    qDebug() << "########################################################";
}

/*Private Slots*/

void MainWindow::onIPLineEditChanged(const QString &ip)
{
    qDebug() << "------------IP Address is changed to " << ip << " ------------";
    requestInfo->ip = ip;
}

void MainWindow::onOidLineEditChanged(const QString &oid)
{
    qDebug() << "-----------Oid is changed to " << oid << " ----------------";
    requestInfo->oid = oid;
}

void MainWindow::onAdvancedOptionPushButtonClicked()
{
    qDebug() << ">>>>>>>>>>Open advancedOptionDialog<<<<<<<<<<<";
    AdvancedOptionDialog *dialog = new AdvancedOptionDialog(requestInfo, this);
    dialog->exec();
    delete dialog;
}

void MainWindow::onGoPushButtonClicked()
{
    qDebug() << ">>>>>>>>Prepare to Send Request<<<<<<<<<<";
    int index = ui->operationComboBox->currentIndex();
    bool ok;
    Operation operation = ui->operationComboBox->itemData(index).toInt(&ok);
    if (!ok) {
        Helper::pop("Waring", "Wrong Request Method");
        return;
    }
    Request *request = new Request;
    handleRequest(operation, request);
    delete request;
}

/*Reset ResultTableWidget*/
void MainWindow::resetResultTableWidget()
{
    ui->resultTableWidget->clearContents();
    ui->resultTableWidget->setRowCount(0);
}

/*Open Dialog to load MIB txt*/
void MainWindow::loadMIB()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("File (*.txt)"));
    qDebug() << ">>>>>>>>>>>Prepare to load file: " << fileName << "<<<<<<<<<";
    if (fileName == "")
        return;
    if (mibTree->loadMIB(fileName)==Status_SUCCESS) {
        //SUCCESS LOAD
    }
    else {
        //FAILED
        Helper::log(0, "Load MIB file Failed");
        Helper::pop("Error", "Load MIB file Failed");
    }
}

void MainWindow::onTreeItemClicked(QTreeWidgetItem *node)
{
    MIBNode *rv = node->data(0, Qt::UserRole).value<MIBNode*>();
    ui->OidLineEdit->setText(rv->oid+rv->oidplus);
    QTableWidget *tb = ui->MIBTableWidget;
    tb->item(0, 0)->setText(rv->name);
    tb->item(1, 0)->setText(rv->oid);
    tb->item(2, 0)->setText(rv->syntax);
    tb->item(3, 0)->setText(rv->access);
    tb->item(4, 0)->setText(rv->status);
    tb->item(5, 0)->setText(rv->index);
    tb->item(6, 0)->setText(rv->description);
    return;
}

void MainWindow::onResultTableClicked(int row, int column)
{
    QTableWidgetItem *rv = ui->resultTableWidget->item(row, 0);
    QString oid = rv->text();
    MIBNode *node;
    node = mibTree->getNodeByName(oid.left(oid.indexOf('.')));
    if (node != NULL) {
        ui->OidLineEdit->setText(node->oid+oid.mid(oid.indexOf('.')));
    }
}

void MainWindow::resultTableDetail(int row, int column)
{
    QString name, type, value;
    name = ui->resultTableWidget->item(row, 0)->text();
    type = ui->resultTableWidget->item(row, 1)->text();
    value = ui->resultTableWidget->item(row, 2)->text();
    ResultDetailDialog *dialog = new ResultDetailDialog(name, type, value, this);
    dialog->exec();
    delete dialog;
}

void MainWindow::onActionGet()
{
    qDebug() << ">>>>>>>>Prepare to Send Request<<<<<<<<<<";
    Request *request = new Request;
    handleRequest(OperationGet, request);
    delete request;
}

void MainWindow::onActionGetNext()
{
    qDebug() << ">>>>>>>>Prepare to Send Request<<<<<<<<<<";
    Request *request = new Request;
    handleRequest(OperationGetNext, request);
    delete request;
}

void MainWindow::onActionSet()
{
    qDebug() << ">>>>>>>>Prepare to Send Request<<<<<<<<<<";
    Request *request = new Request;
    handleRequest(OperationSet, request);
    delete request;
}

void MainWindow::aboutThis()
{
    QMessageBox::about(this, tr("About Application"),
                       tr("The Application is based on <a href=\"http://www.agentpp.com/\"><b>snmp++</b></a>"
                          "and <a href=\"http://qt-project.org/\"><b>Qt</b></a>.<br>"
                          "Thanks to those open source projects making our life easier."));
}
