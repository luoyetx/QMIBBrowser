#-------------------------------------------------
#
# Project created by QtCreator 2013-10-24T21:45:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QMIBBrowser
TEMPLATE = app

HEADERS += \
    src/mainwindow.h \
    src/snmpmanager.h \
    src/advancedoptiondialog.h \
    src/common.h \
    src/setrequestdialog.h \
    src/mibtree.h

SOURCES += \
    src/mainwindow.cpp \
    src/main.cpp \
    src/snmpmanager.cpp \
    src/advancedoptiondialog.cpp \
    src/setrequestdialog.cpp \
    src/mibtree.cpp \
    src/common.cpp

INCLUDEPATH += $$PWD/includes

LIBS += -L$$PWD/libs -lws2_32

CONFIG(debug, debug | release) {
    LIBS += -lsnmp_ppd
} else {
    DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT
    LIBS += -lsnmp_pp
}

FORMS += \
    ui/mainwindow.ui \
    ui/advancedoptiondialog.ui \
    ui/setrequestdialog.ui \

RESOURCES += \
    resource/resource.qrc

OTHER_FILES += \
    resource/logo.rc

RC_FILE += \
    resource/logo.rc
