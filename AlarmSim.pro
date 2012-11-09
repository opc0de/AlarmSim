#-------------------------------------------------
#
# Project created by QtCreator 2012-10-19T09:19:21
#
#-------------------------------------------------

QT       += core gui network

TARGET = AlarmSim
TEMPLATE = app

LIBS += "C:/OpenSSL-Win32/lib/MinGW/libeay32.a"

INCLUDEPATH += "C:/OpenSSL-Win32/include/"


SOURCES += main.cpp\
        mainwindow.cpp \
    splashscreen.cpp

HEADERS  += mainwindow.h \
    splashscreen.h

FORMS    += mainwindow.ui \
    splashscreen.ui

RESOURCES += \
    resources.qrc

RC_FILE += \
    iconz.rc
