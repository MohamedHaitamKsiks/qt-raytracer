QT += core gui
QT += opengl
QT += openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ui/createentitypopup.cpp \
    ui/entityitem.cpp \
    ui/mainwindow.cpp \
    ui/raytracingglwidget.cpp \
    entity3d/ball3d.cpp \
    entity3d/entity3d.cpp \
    entity3d/entitymanager.cpp \
    main.cpp \
    random.cpp \
    renderer/drawcommand.cpp \
    renderer/raytracingrenderer.cpp

HEADERS += \
    ui/createentitypopup.h \
    ui/entityitem.h \
    ui/mainwindow.h \
    ui/raytracingglwidget.h \
    entity3d/ball3d.h \
    entity3d/entity3d.h \
    entity3d/entitymanager.h \
    renderer/drawcommand.h \
    renderer/material.h \
    renderer/ray.h \
    renderer/raytracingrenderer.h
    random.h

FORMS += \
    ui/createentitypopup.ui \
    ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc