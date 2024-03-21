TEMPLATE = app
TARGET = 3DBlocks

QT = core gui

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

# Add OpenGL module
QT += opengl
QT += openglwidgets
SOURCES += \
    src/geometryengine.cpp \
    src/main.cpp \
    src/mainwidget.cpp \
    src/mesh.cpp \
    src/plyFile.cpp

HEADERS += \
    headers/geometryengine.h \
    headers/mainwidget.h \
    headers/mesh.h \
    headers/plyFile.h

RESOURCES += \
    shaders/fshader.glsl \
    shaders/vshader.glsl \
    data/meshcolor.ply \

INCLUDEPATH += /usr/include

win32: LIBS += -lopengl32
unix: LIBS += -lGL -lgmp -lmpfr
