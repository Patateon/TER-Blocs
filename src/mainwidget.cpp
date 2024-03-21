// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "../headers/mainwidget.h"

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    delete geometries;
    doneCurrent();
}

//! [0]
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->position());
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        moveCameraLeft();
        break;
    case Qt::Key_Right:
        moveCameraRight();
        break;
    case Qt::Key_Up:
        moveCameraUp();
        break;
    case Qt::Key_Down:
        moveCameraDown();
        break;
    case Qt::Key_S:
        rotateCameraDown();
        break;
    case Qt::Key_Z:
        rotateCameraUp();
        break;
    case Qt::Key_Q:
        rotateCameraLeft();
        break;
    case Qt::Key_D:
        rotateCameraRight();
        break;
    case Qt::Key_V:
        zoomIn();
        break;
    case Qt::Key_B:
        zoomOut();
        break;
    default:
        QOpenGLWidget::keyPressEvent(event);
    }
}

void MainWidget::keyReleaseEvent(QKeyEvent *event)
{
    QOpenGLWidget::keyReleaseEvent(event);
}

void MainWidget::moveCameraLeft()
{
    QVector3D translation(-0.1, 0.0, 0.0);
    projection.translate(translation);
    update();
}

void MainWidget::moveCameraRight()
{
    QVector3D translation(0.1, 0.0, 0.0);
    projection.translate(translation);
    update();
}

void MainWidget::moveCameraUp()
{
    QVector3D translation(0.0, 0.1, 0.0);
    projection.translate(translation);
    update();
}

void MainWidget::moveCameraDown()
{
    QVector3D translation(0.0, -0.1, 0.0);
    projection.translate(translation);
    update();
}
void MainWidget::rotateCameraLeft()
{
    QVector3D translation(0.0, 0.0, 0.0);
    rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), 5) * rotation;
    translation = rotation * translation;
    projection.translate(translation);
    update();
}

void MainWidget::rotateCameraRight()
{
    QVector3D translation(0.0, 0.0, 0.0);
    rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), -5) * rotation;
    translation = rotation * translation;
    projection.translate(translation);
    update();
}

void MainWidget::rotateCameraUp()
{
    // Déplacer la caméra vers le haut
    QVector3D translation(0.0, 0.0, 0.0);
    rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), -5) * rotation;
    translation = rotation * translation;
    projection.translate(translation);
    update();
}

void MainWidget::rotateCameraDown()
{
    QVector3D translation(0.0, 0.0, 0.0);
    rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 5) * rotation;
    translation = rotation * translation;
    projection.translate(translation);
    update();
}

void MainWidget::zoomIn()
{
    QVector3D translation(0.0, 0.0, 0.1);
    translation = rotation * translation;
    projection.translate(translation);
    update();
}

void MainWidget::zoomOut()
{
    QVector3D translation(0.0, 0.0,- 0.1);
    translation = rotation * translation;
    projection.translate(translation);
    update();
}
//! [0]

//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{
    // Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

        // Request an update
        update();
    }
}
//! [1]

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.8, 0.8, 0.8, 1);

    initShaders();
    initTextures();

    // Ask user to select a PLY file
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open PLY File"), "", tr("PLY Files (*.ply)"));
    if(fileName.isNull()) {
        qDebug() << "No file selected. Exiting initialization.";
        return;
    }

    // Load the selected PLY file
    mesh = new Mesh();
    ply = new PlyFile();
    ply->loadPlyFile(fileName.toStdString(), mesh);


    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}

//! [3]
void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}
//! [3]

//! [4]
void MainWidget::initTextures()
{
    // Load cube.png image
    texture = new QOpenGLTexture(QImage(":/cube.png").mirrored());

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);
}
//! [4]

//! [5]
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 1.0, zFar = 25.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}
//! [5]

void MainWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //! [2]
    // Enable depth buffer
    //glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    //glEnable(GL_CULL_FACE);
    //! [2]

    texture->bind();
    program.bind();

    //! [6]
    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -8.0);
    matrix.rotate(rotation);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);
    //! [6]

    // Use texture unit 0 which contains cube.png
    //program.setUniformValue("texture", 0);

    // Draw cube geometry
    //geometries->drawCubeGeometry(&program);
    mesh->drawGeometry(&program);
}
