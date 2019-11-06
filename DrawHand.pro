#-------------------------------------------------
#
# Project created by QtCreator 2019-09-21T10:47:00
#
#-------------------------------------------------

QT       += core gui
QT += opengl

CONFIG += resource_big

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
    fileutil.cpp \
    main.cpp \
    mainwindow.cpp \
    mesh.cpp \
    meshbuilders.cpp \
    panel.cpp \
    rendermesh.cpp \
    window.cpp

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
    commontype.h \
    fileutil.h \
    mainwindow.h \
    mesh.h \
    meshbuilders.h \
    panel.h \
    rendermesh.h \
    transform.h \
    window.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    textures.qrc \
    shaders.qrc \
    objs.qrc \
    joints.qrc

INCLUDEPATH += D:/DevelopTools/eigen3 \
    D:/DevelopTools/opencv/build/include

#LIBS += -LD:/DevelopTools/opencv/build/x64/vc14/lib    \
#    -lopencv_core2413d.dll    \
#    -lopencv_highgui2413d \
#    -lopencv_imgproc2413d \
#    -lopencv_features2d2413d \
#    -lopencv_calib3d2413d \
#LIBS += D:/DevelopTools/opencv/build/x64/vc14/lib

CONFIG(debug, debug|release): {
LIBS += -LD:/DevelopTools/opencv/build/x64/vc14/lib \
-lopencv_core2413d \
-lopencv_imgproc2413d \
-lopencv_highgui2413d \
-lopencv_ml2413d \
-lopencv_video2413d \
-lopencv_features2d2413d \
-lopencv_calib3d2413d \
-lopencv_objdetect2413d \
-lopencv_contrib2413d \
-lopencv_legacy2413d \
-lopencv_flann2413d
} else:CONFIG(release, debug|release): {
LIBS += -LD:/DevelopTools/opencv/build/x64/vc14/lib \
-lopencv_core2413 \
-lopencv_imgproc2413 \
-lopencv_highgui2413 \
-lopencv_ml2413 \
-lopencv_video2413 \
-lopencv_features2d2413 \
-lopencv_calib3d2413 \
-lopencv_objdetect2413 \
-lopencv_contrib2413 \
-lopencv_legacy2413 \
-lopencv_flann2413
}

DISTFILES +=
