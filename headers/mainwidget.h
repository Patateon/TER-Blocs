// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "geometryengine.h"
#include "plyFile.h"
#include "mesh.h"


#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
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
    void timerEvent(QTimerEvent *e) override;
    void moveCameraUp();
    void moveCameraDown();
    void moveCameraRight();
    void moveCameraLeft();
    void keyReleaseEvent(QKeyEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void rotateCameraLeft();
    void rotateCameraRight();
    void rotateCameraUp();
    void rotateCameraDown();
    void zoomIn();
    void zoomOut();

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
    Mesh * mesh =nullptr;

    QOpenGLTexture *texture = nullptr;

    QMatrix4x4 projection;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed = 0;
    QQuaternion rotation;
};

#endif // MAINWIDGET_H
