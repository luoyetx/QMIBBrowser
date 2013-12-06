#include <QMessageBox>
#include <QDateTime>

#include "common.h"

static FILE *logfile;
static const char *Helper_file = "log/log.txt";
static const char *Helper_error = "[ERROR] ";
static const char *Helper_info = "[INFO ] ";

void Helper::init()
{
    logfile = fopen(Helper_file, "a");
    if (logfile == NULL) logfile = stdout;
}

void Helper::finish()
{
    fclose(logfile);
}

void Helper::log(int level, QString message)
{
    if (level == 0)
        fprintf(logfile, Helper_error);
    else fprintf(logfile, Helper_info);
    QDateTime time = QDateTime::currentDateTime();
    fprintf(logfile, time.toString("[yyyy-MM-dd hh:mm:ss] ").toStdString().c_str());
    fprintf(logfile, message.toStdString().c_str());
    fprintf(logfile, "\n");
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
