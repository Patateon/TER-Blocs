TEMPLATE = app
TARGET = 3DBlocks

QT = core gui

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

# Add OpenGL module
QT += opengl
QT += openglwidgets
SOURCES += \
    src/main.cpp \
    src/mainwidget.cpp \
    src/mesh.cpp \
    src/plyFile.cpp \


HEADERS += \
    headers/kdtree.h \
    headers/mainwidget.h \
    headers/mesh.h \
    headers/plyFile.h \
    headers/Vec3.h\
    ANN/ANN.h


RESOURCES += \
    shaders/fshader.glsl \
    shaders/vshader.glsl \
    data/meshcolor.ply \

INCLUDEPATH += /usr/include
INCLUDEPATH += $$PWD/ann/include

win32: LIBS += -lopengl32
unix: LIBS += -lGL -lgmp -lmpfr




