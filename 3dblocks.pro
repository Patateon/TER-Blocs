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
    src/analyseParCGAL.cpp \
    src/camera.cpp \
    src/main.cpp \
    src/mainwidget.cpp \
    src/mesh.cpp \
    src/nuageDePoint.cpp \
    src/openglWindowWidget.cpp \
    src/plyFile.cpp \
    src/trackball.cpp

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
    headers/camera.h \
    headers/kdtree.h \
    headers/mesh.h \
    headers/nuageDePoint.h \
    headers/openglWindowWidget.h \
    headers/plyFile.h \
    headers/trackball.h \
    headers/mainwidget.h

RESOURCES += \
    shaders/fshader.glsl \
    shaders/vshader.glsl \
    shaders/gshader.glsl \
    data/meshcolor.ply \

# Nécessaire pour la reconstruction par poisson
DEFINES += CGAL_EIGEN3_ENABLED

INCLUDEPATH += /usr/include

# Ajout de ANN (KD Tree)
INCLUDEPATH += $$PWD/ann/include
# Ajout de CGAL (Traitement 3D)
INCLUDEPATH += $$PWD/external/CGAL-5.6.1
# Ajout de Eigen (pour poisson)
INCLUDEPATH += $$PWD/external/eigen-3.4.0

win32: LIBS += -lopengl32
unix: LIBS += -lGL -lgmp -lmpfr

DISTFILES += \
    shaders/gshader.glsl
