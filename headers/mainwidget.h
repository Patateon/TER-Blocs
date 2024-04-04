// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "plyFile.h"
#include "Camera.h"


#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QVector>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class GeometryEngine;

class MainWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    using QOpenGLWidget::QOpenGLWidget;
    ~MainWidget();

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *e) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;


    void saveCurrentNuageDePoint();
    void deleteCurrentNuageDePoint();
    void clearCurrentNuageDePoint();
    void switchNuageDePoint();
    void parseNuageDePoint();
    void analyseNuageDePoint();

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShaders();
    void initTextures();

private:
    QBasicTimer timer;
    QOpenGLShaderProgram program;
    GeometryEngine *geometries = nullptr;

    PlyFile *ply =nullptr;
    QVector<NuageDePoint *> allNuageDePoint;
    int nextNuageDePointIndice =1;
    NuageDePoint *currentNuageDePoint;
    NuageDePoint *ndp;

    NuageDePoint *ndpComparaison;
    bool afficher_ndpComparaison=false;

    QOpenGLTexture *texture = nullptr;

    Camera camera;
    static unsigned int SCREENWIDTH ;
    static unsigned int SCREENHEIGHT ;
    static bool mouseRotatePressed ;
    static bool mouseMovePressed ;
    static bool mouseZoomPressed ;
    static int lastX, lastY, lastZoom;
    static bool fullScreen ;


    QVector2D mousePressPosition;

};

#endif // MAINWIDGET_H
