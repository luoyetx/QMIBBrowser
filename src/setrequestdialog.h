#ifndef SETREQUESTDIALOG_H
#define SETREQUESTDIALOG_H

#include <QDialog>
#include "common.h"

#define TYPE        int
#define INTEGER     0
#define STRING      1

namespace Ui {
class SetRequestDialog;
}

class SetRequestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetRequestDialog(Vb *data, QString nodeType, QWidget *parent = 0);
    ~SetRequestDialog();

    /*false if OkPushButton Clicked, otherwise true, default is true*/
    bool cancel;
    /*Return data*/
    Vb *getData();

private:
    Ui::SetRequestDialog *ui;
    Vb *data;

signals:
    //void dataSeted(Vb *data);

private slots:
    void onOkPushButtonClicked();
    //void onCancelPushButonClicked();

};

#endif // SETREQUESTDIALOG_H
