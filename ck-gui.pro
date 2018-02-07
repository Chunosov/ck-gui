#-------------------------------------------------
#
# Project created by QtCreator 2018-01-11T17:07:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ck-gui
TEMPLATE = app
DESTDIR = $$_PRO_FILE_PWD_/bin

ORION = $$_PRO_FILE_PWD_/orion/
include($$ORION"orion.pri")

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RESOURCES += \
    resources.qrc

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/ck.cpp \
    src/appconfig.cpp \
    src/appevents.cpp \
    src/ckjson.cpp \
    src/utils.cpp \
    src/jsonformat.cpp \
    src/texteditorwindow.cpp \
    src/searchwindowbase.cpp \
    src/searchenvswindow.cpp \
    src/searchprogramswindow.cpp \
    src/searchpackageswindow.cpp

HEADERS += \
    src/mainwindow.h \
    src/ck.h \
    src/appconfig.h \
    src/appevents.h \
    src/ckjson.h \
    src/utils.h \
    src/jsonformat.h \
    src/texteditorwindow.h \
    src/searchwindowbase.h \
    src/searchenvswindow.h \
    src/searchprogramswindow.h \
    src/searchpackageswindow.h
