#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Helper::log(1, "Open Software");
    w.show();
    int statusCode;
    statusCode = a.exec();
    if (statusCode == 0)
        Helper::log(1, "Quit Software");
    else Helper::log(0, "Quit Software");
    return statusCode;
}
