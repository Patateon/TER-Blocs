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
    headers/loadply.h \
    headers/geometryengine.h \
    headers/mainwidget.h \

RESOURCES += \
    shaders/fshader.glsl \
    shaders/vshader.glsl \
    data/sphere.ply \
    data/meshcolor.ply \

win32: LIBS += -lopengl32
unix: LIBS += -lGL
