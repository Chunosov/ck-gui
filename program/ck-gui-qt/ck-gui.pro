#-------------------------------------------------
#
# Project created by QtCreator 2018-01-11T17:07:01
#
#-------------------------------------------------

QT += core gui widgets

TARGET = ck-gui
TEMPLATE = app
DESTDIR = $$_PRO_FILE_PWD_/bin

QSCINTILLA_PATH=$$(QSCINTILLA_PATH)
!exists($$QSCINTILLA_PATH/Qsci/qsciscintilla.h) {
  error(Path to QScintilla library is not set or incorrect. \
    Download library from https://pypi.org/project/QScintilla \
    build it  and provide path to it via environment variable QSCINTILLA_PATH)
}
INCLUDEPATH += $$QSCINTILLA_PATH
#TODO: win32:LIBS += "$$QSCINTILLA_PATH/?.lib"
unix:LIBS += "-L$$QSCINTILLA_PATH" -lqscintilla2_qt5

ORION_INCLUDE = $$(ORION_QT_PATH)/orion.pri
!exists($$ORION_INCLUDE) {
  error(Path to orion-qt library is not set or incorrect. \
    Download library from https://github.com/orion-project/orion-qt \
    and provide path to it via environment variable ORION_QT_PATH)
}
include($$ORION_INCLUDE)

CONFIG += c++14

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
    src/appevents.cpp \
    src/ckjson.cpp \
    src/texteditorwindow.cpp \
    src/searchwindowbase.cpp \
    src/searchenvswindow.cpp \
    src/searchprogramswindow.cpp \
    src/searchpackageswindow.cpp \
    src/searchreposwindow.cpp

HEADERS += \
    src/mainwindow.h \
    src/ck.h \
    src/appevents.h \
    src/ckjson.h \
    src/texteditorwindow.h \
    src/searchwindowbase.h \
    src/searchenvswindow.h \
    src/searchprogramswindow.h \
    src/searchpackageswindow.h \
    src/searchreposwindow.h
