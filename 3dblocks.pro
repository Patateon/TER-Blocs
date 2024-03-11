TEMPLATE = app
TARGET = name_of_the_app

QT = core gui

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

# Add OpenGL module
QT += opengl
QT += openglwidgets
SOURCES += \
    geometryengine.cpp \
    main.cpp \
    mainwidget.cpp

HEADERS += \
    geometryengine.h \
    mainwidget.h

RESOURCES += \
    fshader.glsl \
    vshader.glsl
