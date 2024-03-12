TEMPLATE = app
TARGET = 3DBlocks

QT = core gui

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

# Add OpenGL module
QT += opengl
QT += openglwidgets
SOURCES += \
    src/geometryengine.cpp \
    src/loadply.cpp \
    src/main.cpp \
    src/mainwidget.cpp

HEADERS += \
    ../../loadply.h \
    headers/geometryengine.h \
    headers/loadply.h \
    headers/mainwidget.h \
    src/loadply.h

RESOURCES += \
    shaders/fshader.glsl \
    shaders/vshader.glsl
LIBS += -lopengl32
