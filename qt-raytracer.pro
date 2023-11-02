QT += core gui
QT += opengl
QT += openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    entity3d/ball3d.cpp \
    entity3d/entity3d.cpp \
    main.cpp \
    mainwindow.cpp \
    random.cpp \
    raytracingglwidget.cpp \
    renderer/drawcommand.cpp \
    renderer/raytracingrenderer.cpp

HEADERS += \
    entity3d/ball3d.h \
    entity3d/entity3d.h \
    mainwindow.h \
    renderer/drawcommand.h \
    renderer/material.h \
    random.h \
    renderer/ray.h \
    renderer/raytracingrenderer.h \
    raytracingglwidget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
