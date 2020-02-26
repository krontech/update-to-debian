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


target.path = /opt/camera/camUpdate


SOURCES += main.cpp\
        updatewindow.cpp

HEADERS  += updatewindow.h

FORMS    += updatewindow.ui

DISTFILES += \
    camUpdate/update_backend.sh \
    camUpdate/update_real.sh \
    camUpdate/update.sh \
    deploy_update.sh \
    make_update.sh

RESOURCES += \
    resources.qrc
