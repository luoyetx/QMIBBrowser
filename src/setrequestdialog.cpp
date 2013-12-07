#include "setrequestdialog.h"
#include "ui_setrequestdialog.h"

SetRequestDialog::SetRequestDialog(Vb *data, QString nodeType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetRequestDialog), data(new Vb), cancel(true)
{
    ui->setupUi(this);
    *(this->data) = *data;
    /*initialize widget value*/
    QString oid;
    oid = data->get_printable_oid();
    ui->oidLineEdit->setText(oid);
    //ui->oidLineEdit->setReadOnly(true);
    if (nodeType != "")
        ui->nodeTypeLineEdit->setText(nodeType);
    else ui->nodeTypeLineEdit->setText("[Warning]This Oid can't be set");
    ui->nodeTypeLineEdit->setReadOnly(true);
    ui->typeComboBox->addItem("INTEGER", QVariant(INTEGER));
    ui->typeComboBox->addItem("OCTET STRING", QVariant(STRING));
    /*connections between widgets*/
    connect(ui->okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
    //connect(ui->cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButonClicked()));
    //connect(ui->okPushButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->cancelPushButton, SIGNAL(clicked()), this, SLOT(close()));
    setWindowTitle("Set");
}

SetRequestDialog::~SetRequestDialog()
{
    delete ui;
    delete data;
}

Vb *SetRequestDialog::getData()
{
    return data;
}

void SetRequestDialog::onOkPushButtonClicked()
{
    cancel = false;
    /*Set data value*/
    int index = ui->typeComboBox->currentIndex();
    bool ok;
    TYPE type = ui->typeComboBox->itemData(index).toInt(&ok);
    QString value = ui->valueLineEdit->text();
    switch (type) {
    case INTEGER:
        int rv;
        rv = value.toInt(&ok, 10);
        if (!ok) {
            /*Wrong Integer Number*/
            Helper::pop("Warning", "Wrong Integer Number");
            return;
        }
        data->set_value(rv);
        break;
    case STRING:
        data->set_value(value.toStdString().c_str());
        break;
    default:
        return;
        break;
    }
    /*No Error Close Dialog*/
    this->close();
}
