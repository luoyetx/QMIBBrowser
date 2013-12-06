#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/logo.ico"));
    MainWindow w;
    Helper::init();
    Helper::log(1, "Open Software");
    w.show();
    int statusCode;
    statusCode = a.exec();
    if (statusCode == 0)
        Helper::log(1, "Quit Software");
    else Helper::log(0, "Quit Software");
    Helper::finish();
    return statusCode;
}
