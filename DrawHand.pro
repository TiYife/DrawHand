#-------------------------------------------------
#
# Project created by QtCreator 2019-09-21T10:47:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DrawHand
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    rendermesh.cpp \
    panel.cpp \
    mainwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    panel.cpp \
    rendermesh.cpp \
    main.cpp \
    mainwindow.cpp \
    panel.cpp \
    rendermesh.cpp \
    fileutil.cpp \
    mesh.cpp \
    meshbuilders.cpp \
    render.cpp

HEADERS += \
    rendermesh.h \
    panel.h \
    mainwindow.h \
    fileutil.h \
    fileutil.h \
    mainwindow.h \
    panel.h \
    rendermesh.h \
    fileutil.h \
    mainwindow.h \
    panel.h \
    rendermesh.h \
    mesh.h \
    meshbuilders.h \
    transform.h \
    commontype.h \
    render.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    textures.qrc \
    shaders.qrc \
    objs.qrc

INCLUDEPATH += D:\DevelopTools\eigen3

DISTFILES +=
