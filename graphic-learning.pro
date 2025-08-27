#-------------------------------------------------
#
# Project created by QtCreator 2023-10-13T13:45:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graphic-learning
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

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        painter.cpp \
        BasicRaysTab.cpp \
        DiffuseTab.cpp \
        SpecularTab.cpp \
        ShadowTab.cpp \
        ReflectionTab.cpp \
        CameraTab.cpp \
        LinesTab.cpp \
        TriangleTab.cpp

HEADERS += \
        mainwindow.h \
        painter.h \
        BasicRaysTab.h \
        DiffuseTab.h \
        sphere.h \
        vector.h \
        light.h \
        config/*.h \
        SpecularTab.h \
        ShadowTab.h \
        ReflectionTab.h \
        matrix.h \
        CameraTab.h \
        LinesTab.h \
        TriangleTab.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
