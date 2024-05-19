// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef OpenGLWindowWidget_H
#define OpenGLWindowWidget_H

#include "plyFile.h"
#include "camera.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QVector>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QLabel>
#include <QCoreApplication>
#include <QGraphicsDropShadowEffect>



class OpenGLWindowWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    OpenGLWindowWidget(QWidget *parent);
    ~OpenGLWindowWidget();
    void keyReleaseEvent(QKeyEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void compareButtonColorChanged(bool green);

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    // void wheelEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *e) override;

    void openPlyFileNuageDePoint();
    void saveCurrentNuageDePoint();
    void deleteCurrentNuageDePoint();
    void clearCurrentNuageDePoint();
    void switchNuageDePoint();
    void parseNuageDePoint();
    void analyseNuageDePoint();

    void updateCamera(QVector3D target);

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShaders();
    void initTextures();

private:
    QLabel *actionLabel;

    QBasicTimer timer;
    QOpenGLShaderProgram program;

    PlyFile *ply =nullptr;
    QVector<NuageDePoint *> allNuageDePoint;
    int nextNuageDePointIndice =1;
    NuageDePoint *currentNuageDePoint;
    NuageDePoint *ndp;

    NuageDePoint *ndpComparaison;
    bool afficher_ndpComparaison=false;

    QOpenGLTexture *texture = nullptr;

    //QString fileMesh;

    Camera camera;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projectionMatrix;
    QVector3D cameraTarget;
    QVector3D cameraTarget2;
    QVector3D cameraUp = QVector3D(0.0, 1.0, 0.0);
    static unsigned int SCREENWIDTH ;
    static unsigned int SCREENHEIGHT ;
    static bool mouseRotatePressed ;
    static bool mouseMovePressed ;
    static bool mouseZoomPressed ;
    static int lastX, lastY, lastZoom;
    static bool fullScreen ;
    static bool mainCamera ;


    QVector2D mousePressPosition;

public slots:
    void handleOpenNDPButtonClicked();
    void handleSaveNDPButtonClicked();
    void handleClearCurrentNuageDePoint();
    void handleDeleteCurrentNuageDePoint();
    void handleSwitchNuageDePoint();
    void handleParseNuageDePoint();
    void handleAnalyseNuageDePoint();
    void handleCompareNuageDePoint();
    void handleSaveMeshPoisson();
    void handleSwitchCamera();


};

#endif // OpenGLWindowWidget_H
