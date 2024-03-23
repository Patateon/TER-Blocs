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

RESOURCES += \
    shaders/fshader.glsl \
    shaders/vshader.glsl \
    data/meshcolor.ply \

INCLUDEPATH += /usr/include

win32: LIBS += -lopengl32
unix: LIBS += -lGL -lgmp -lmpfr

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/external/release/ -lANN
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/external/debug/ -lANN
else:unix: LIBS += -L$$PWD/external/ -lANN

INCLUDEPATH += $$PWD/external/ann_1.1.2_MS_Win32_bin
DEPENDPATH += $$PWD/external/ann_1.1.2_MS_Win32_bin
