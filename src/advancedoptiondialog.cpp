#include <QDebug>
#include <QVariant>
#include <QMessageBox>

#include "advancedoptiondialog.h"
#include "ui_advancedoptiondialog.h"

AdvancedOptionDialog::AdvancedOptionDialog(RequestInfo *requestInfo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdvancedOptionDialog), requestInfo(new RequestInfo)
{
    ui->setupUi(this);
    /*initialize versionComboBox*/
    ui->versionComboBox->addItem("1", QVariant(VERSION1));
    ui->versionComboBox->addItem("3", QVariant(VERSION3));
    /*initialize connections between widgets*/
    connect(ui->cancelPushButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
    //connect(ui->okPushButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(this, SIGNAL(requestInfoChanged(RequestInfo*)), parent, SLOT(updateRequestInfo(RequestInfo*)));

    initialRequestInfo(requestInfo);
}

AdvancedOptionDialog::~AdvancedOptionDialog()
{
    delete ui;
    delete requestInfo;
}

/*Initialize RequestInfo and Set Widget Value*/
void AdvancedOptionDialog::initialRequestInfo(RequestInfo *requestInfo)
{
    this->requestInfo->ip = requestInfo->ip;
    this->requestInfo->port = requestInfo->port;
    this->requestInfo->oid = requestInfo->oid;
    this->requestInfo->community = requestInfo->community;
    this->requestInfo->retry = requestInfo->retry;
    this->requestInfo->timeout = requestInfo->timeout;
    this->requestInfo->version = requestInfo->version;
    /*Set Widget value*/
    ui->IPLineEdit->setText(requestInfo->ip);
    ui->portLineEdit->setText(requestInfo->port);
    ui->communityLineEdit->setText(requestInfo->community);
    QString retry;
    retry.setNum(requestInfo->retry, 10);
    ui->RetryLineEdit->setText(retry);
    switch (requestInfo->version) {
    case version1:
        ui->versionComboBox->setCurrentIndex(0);
        break;
    case version3:
        ui->versionComboBox->setCurrentIndex(1);
        break;
    default:
        ui->versionComboBox->setCurrentIndex(0);
        break;
    }
}

/*Change MainWindow RequestInfo when goPushBtton clicked and Close the Dialog*/
void AdvancedOptionDialog::onOkPushButtonClicked()
{
    bool ok;
    /*Check IP here*/
    IpAddress ip;
    ip = ui->IPLineEdit->text().toStdString().c_str();
    ok = ip.valid();
    if (!ok) {
        /*error*/
        QMessageBox::warning(this, "Warning", "Wrong IP");
        return;
    }
    requestInfo->ip = ui->IPLineEdit->text();
    /*Check port*/
    QString port = ui->portLineEdit->text();
    port.toInt(&ok, 10);
    if (!ok) {
        /*error*/
        QMessageBox::warning(this, "Warning", "Wrong Port");
        return;
    }
    requestInfo->port = port;
    requestInfo->community = ui->communityLineEdit->text();
    int retry;
    retry = ui->RetryLineEdit->text().toInt(&ok, 10);
    if (!ok) {
        /*error*/
        QMessageBox::warning(this, "Warning", "Wrong Retry");
        return;
    }
    requestInfo->retry = retry;
    int index = ui->versionComboBox->currentIndex();
    VERSION ver = ui->versionComboBox->itemData(index).toInt(&ok);
    if (!ok) {
        /*error*/
        QMessageBox::warning(this, "Warning", "Wrong Version");
    }
    snmp_version version;
    switch (ver) {
    case VERSION1:
        version = version1;
        break;
    case VERSION3:
        version = version3;
        break;
    default:
        version = version1;
        break;
    }
    requestInfo->version = version;
    /*No Error Emit SIGNAL and Closed*/
    emit(requestInfoChanged(requestInfo));
    this->close();
}
