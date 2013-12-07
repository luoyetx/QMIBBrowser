#ifndef RESULTDETAILDIALOG_H
#define RESULTDETAILDIALOG_H

#include <QDialog>

namespace Ui {
class ResultDetailDialog;
}

class ResultDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultDetailDialog(QString name, QString type, QString value, QWidget *parent = 0);
    ~ResultDetailDialog();

private:
    Ui::ResultDetailDialog *ui;
};

#endif // RESULTDETAILDIALOG_H
