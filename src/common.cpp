#include <QMessageBox>

#include "common.h"

void Helper::log(int level, QString message)
{

}

void Helper::pop(QString title, QString message)
{
    if (title == "Error") {
        QMessageBox::warning(0, title, message);
        return;
    }
    if (title == "Warning") {
        QMessageBox::warning(0, title, message);
        return;
    }
    if (title == "info") {
        QMessageBox::about(0, title, message);
        return;
    }
}
