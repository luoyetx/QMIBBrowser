#ifndef ADVANCEDOPTIONDIALOG_H
#define ADVANCEDOPTIONDIALOG_H

#include <QDialog>

#include "common.h"
#include "snmpmanager.h"

#define VERSION     int
#define VERSION1    1
#define VERSION3    3

namespace Ui {
class AdvancedOptionDialog;
}

class AdvancedOptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdvancedOptionDialog(RequestInfo *requestInfo, QWidget *parent = 0);
    ~AdvancedOptionDialog();

private:
    Ui::AdvancedOptionDialog *ui;
    RequestInfo *requestInfo;

    /*Initialize RequestInfo and Set Widget Value*/
    void initialRequestInfo(RequestInfo *requestInfo);

signals:
    void requestInfoChanged(RequestInfo *requestInfo);

private slots:
    /*Change MainWindow RequestInfo when goPushBtton clicked and Close the Dialog*/
    void onOkPushButtonClicked();

};

#endif // ADVANCEDOPTIONDIALOG_H
