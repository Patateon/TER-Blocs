TEMPLATE = app
TARGET = 3DBlocks

QT = core gui

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

# Add OpenGL module
QT += opengl
QT += openglwidgets
SOURCES += \
    ann/src/ANN.cpp \
    ann/src/bd_fix_rad_search.cpp \
    ann/src/bd_pr_search.cpp \
    ann/src/bd_search.cpp \
    ann/src/bd_tree.cpp \
    ann/src/brute.cpp \
    ann/src/kd_dump.cpp \
    ann/src/kd_fix_rad_search.cpp \
    ann/src/kd_pr_search.cpp \
    ann/src/kd_search.cpp \
    ann/src/kd_split.cpp \
    ann/src/kd_tree.cpp \
    ann/src/kd_util.cpp \
    ann/src/perf.cpp \
    src/main.cpp \
    src/mainwidget.cpp \
    src/nuageDePoint.cpp \
    src/plyFile.cpp \
    src/Camera.cpp \
    src/Trackball.cpp \

HEADERS += \
    ann/include/ANN/ANN.h \
    ann/include/ANN/ANNperf.h \
    ann/include/ANN/ANNx.h \
    ann/src/bd_tree.h \
    ann/src/kd_fix_rad_search.h \
    ann/src/kd_pr_search.h \
    ann/src/kd_search.h \
    ann/src/kd_split.h \
    ann/src/kd_tree.h \
    ann/src/kd_util.h \
    ann/src/pr_queue.h \
    ann/src/pr_queue_k.h \
    headers/kdtree.h \
    headers/mainwidget.h \
    headers/nuageDePoint.h \
    headers/plyFile.h \
    headers/Camera.h \
    headers/Trackball.h \
    ANN/ANN.h \

RESOURCES += \
    shaders/fshader.glsl \
    shaders/vshader.glsl \
    data/meshcolor.ply \

INCLUDEPATH += /usr/include
INCLUDEPATH += $$PWD/ann/include
INCLUDEPATH += $$PWD/external/CGAL-5.6.1

win32: LIBS += -lopengl32
unix: LIBS += -lGL -lgmp -lmpfr
