#-------------------------------------------------
#
# Project created by QtCreator 2018-10-26T14:36:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = update-to-debian
TEMPLATE = app
INSTALLS += target

LIBS += -static-libstdc++


target.path = /opt/camera


SOURCES += main.cpp\
        updatewindow.cpp

HEADERS  += updatewindow.h

FORMS    += updatewindow.ui

DISTFILES += \
    update_backend.sh \
    update_real.sh \
    deployUpdate.sh

RESOURCES += \
    resources.qrc
