#include "resultdetaildialog.h"
#include "ui_resultdetaildialog.h"

ResultDetailDialog::ResultDetailDialog(QString name, QString type, QString value, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultDetailDialog)
{
    ui->setupUi(this);
    ui->nameLineEdit->setText(name);
    ui->typeLineEdit->setText(type);
    ui->valueTextEdit->setText(value);
    ui->valueTextEdit->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    ui->nameLineEdit->setReadOnly(true);
    ui->typeLineEdit->setReadOnly(true);
    ui->valueTextEdit->setReadOnly(true);
    connect(ui->closePushButton, SIGNAL(clicked()), this, SLOT(close()));
    setWindowTitle("Result Detail");
}

ResultDetailDialog::~ResultDetailDialog()
{
    delete ui;
}
