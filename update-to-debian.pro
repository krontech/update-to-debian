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


## Tweaks for Debian builds.
exists( $${QT_SYSROOT}/etc/debian_version ) {
    target.path = /usr/bin

    DEBPACKAGE = chronos-gui
    DEBFULLNAME = $$system(git config user.name)
    DEBEMAIL = $$system(git config user.email)
    DEBFILES = $$files(debian/*, true)
    DEBCONFIG = $$find(DEBFILES, "\\.in$")
    DEBFILES -= $$DEBCONFIG
    QMAKE_SUBSTITUTES += $$DEBCONFIG

    system($$QMAKE_MKDIR -p $${OUT_PWD}/debian $${OUT_PWD}/debian/source)
    system($$QMAKE_COPY $$DEBFILES $${OUT_PWD}/debian)
} else {
    target.path = /opt/camera
}


SOURCES += main.cpp\
        updatewindow.cpp

HEADERS  += updatewindow.h

FORMS    += updatewindow.ui
