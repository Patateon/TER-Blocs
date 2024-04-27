// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "../headers/mainwidget.h"

unsigned int MainWidget::SCREENWIDTH = 640;
unsigned int MainWidget::SCREENHEIGHT = 480;
bool MainWidget::mouseRotatePressed = false;
bool MainWidget::mouseMovePressed = false;
bool MainWidget::mouseZoomPressed = false;
int MainWidget::lastX = 0;
int MainWidget::lastY = 0;
int MainWidget::lastZoom = 0;
bool MainWidget::fullScreen = false;
bool MainWidget::mainCamera = true;

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    doneCurrent();
}

//! [0]
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    int x = e->position().x();
    int y = e->position().y();
    switch (e->button()) {
    case Qt::LeftButton:
        camera.beginRotate(x, y);
        mouseMovePressed = false;
        mouseRotatePressed = true;
        mouseZoomPressed = false;
        break;
    case Qt::RightButton:
        lastX = x;
        lastY = y;
        mouseMovePressed = true;
        mouseRotatePressed = false;
        mouseZoomPressed = false;
        break;
    case Qt::MiddleButton:
        if (!mouseZoomPressed) {
            lastZoom = y;
            mouseMovePressed = false;
            mouseRotatePressed = false;
            mouseZoomPressed = true;
        }
        break;
    default:
        QOpenGLWidget::mousePressEvent(e);
        break;
    }
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->position().x();
    int y = event->position().y();
    float cameraSpeed = 10.0f;

    if (mouseRotatePressed) {
        camera.rotate(x, y);
    }
    else if (mouseMovePressed) {
        camera.move((x - lastX) / static_cast<float>(SCREENWIDTH) * cameraSpeed, (lastY - y) / static_cast<float>(SCREENHEIGHT), 0.0);
        lastX = x;
        lastY = y;
    }
    else if (mouseZoomPressed) {
        camera.zoom(static_cast<float>(y - lastZoom) / SCREENHEIGHT);
        lastZoom = y;
    }
    update();
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // Clear mouse press positions
    mousePressPosition = QVector2D();

    // Reset mouse pressed flags
    mouseMovePressed = false;
    mouseRotatePressed = false;
    mouseZoomPressed = false;

    QOpenGLWidget::mouseReleaseEvent(e);
}

// void MainWidget::wheelEvent(QWheelEvent * event){

// }

void MainWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    default:
        QOpenGLWidget::keyPressEvent(event);
    }
}

void MainWidget::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_O:
        saveCurrentNuageDePoint();
        break;
    case Qt::Key_P:
        parseNuageDePoint();
        break;
    case Qt::Key_C:
        switchNuageDePoint();
        break;
    case Qt::Key_X:
        deleteCurrentNuageDePoint();
        break;
    case Qt::Key_I:
        clearCurrentNuageDePoint();
        break;
    case Qt::Key_U:
        analyseNuageDePoint();
        break;
    case Qt::Key_W:
        afficher_ndpComparaison=!afficher_ndpComparaison;
        update();
        break;
    case Qt::Key_S:
        mainCamera = !mainCamera;
        break;
    default:
        QOpenGLWidget::keyReleaseEvent(event);
        break;
    }
}
//! [0]

//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{

}
//! [1]

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.8, 0.8, 0.8, 1);

    initShaders();
    initTextures();

    // Ask user to select a PLY file
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open PLY File"), "../TER-Blocs/data", tr("PLY Files (*.ply)"));
    if(fileName.isNull()) {
        qDebug() << "No file selected. Exiting initialization.";
        return;
    }

    // Load the selected PLY file
    ndp = new NuageDePoint();
    allNuageDePoint.append(ndp);
    currentNuageDePoint=ndp;
    ply = new PlyFile();
    ply->loadPlyFile(fileName.toStdString(), currentNuageDePoint);
    //ndp->performDelaunayTriangulation(ndp->getVertices(),ndp->getTriangles());
    ndpComparaison= new NuageDePoint();
    ndpComparaison->clone(ndp);


    currentNuageDePoint->computeBarycentre();
    cameraTarget = currentNuageDePoint->getBarycentre();
    qDebug()<<"Barycentre du ndg courant : "<<cameraTarget;

    viewMatrix = QMatrix4x4();
    camera.initPos();
    camera.zoom(3);
    // updateCamera(cameraTarget);
    // float x,y,z;
    // camera.getPos(x,y,z);
    // qDebug()<<x<<" "<<y<<" "<<z;
    // camera.lookAt(target);
    // camera.getPos(x,y,z);
    // qDebug()<<x<<" "<<y<<" "<<z;
    // camera.apply();
    // camera.getPos(x,y,z);
    // qDebug()<<x<<" "<<y<<" "<<z;

    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}

//! [3]
void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vshader.glsl"))
       close();

    // Compile geometry shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/gshader.glsl"))
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
    camera.resize (w, h);
    projectionMatrix = camera.getProjectionMatrix();
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
    glEnable(GL_CULL_FACE);
    //! [2]

    texture->bind();
    program.bind();

    // Get camera position for view matrix computation and for lightnings computations
    float x,y,z;
    camera.getPos(x,y,z);
    QVector3D cameraPosition;
    cameraPosition = QVector3D(x, y, z);

    //! [6]
    // Calculate model view transformation
    QMatrix4x4 modelMatrix;
    // Compute view matrix
    if (mainCamera){
        // modelMatrix.translate(-cameraTarget);
        viewMatrix = camera.lookAt(cameraTarget, cameraUp);
    }else{
        // modelMatrix.translate(-cameraTarget2);
        viewMatrix = camera.lookAt(cameraTarget2, cameraUp);
    }

    qDebug()<<"Camera Position :"<<cameraPosition;

    // Set modelview-projection matrix
    program.setUniformValue("model", modelMatrix);
    program.setUniformValue("view", viewMatrix);
    program.setUniformValue("projection", projectionMatrix);
    program.setUniformValue("cameraPosition", cameraPosition);
    //! [6]

    // Use texture unit 0 which contains cube.png
    //program.setUniformValue("texture", 0);

    // Draw nuage de point
    // Enable point size for rendering points
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Set the desired point size
    float pointSize = 5.0f; // Set your desired point size here
    glPointSize(pointSize);
    currentNuageDePoint->drawGeometry(&program);
    if(afficher_ndpComparaison){ndpComparaison->drawGeometry(&program);}
}

void MainWidget::saveCurrentNuageDePoint(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save PLY File"), "", tr("PLY Files (*.ply)"));
    if(fileName.isNull()) {
        qDebug() << "No file selected. Exiting initialization.";
        return;
    }
    ply->writePlyFile(fileName.toStdString(),currentNuageDePoint);
}

void MainWidget::deleteCurrentNuageDePoint(){
    delete allNuageDePoint[nextNuageDePointIndice - 1];
    allNuageDePoint.removeAt(nextNuageDePointIndice - 1);
    if (!allNuageDePoint.isEmpty()) {
        currentNuageDePoint = allNuageDePoint[nextNuageDePointIndice - 1];
        currentNuageDePoint->bindAndAllocateBuffer();
        update();
    } else {
        currentNuageDePoint = nullptr;
    }
}

void MainWidget::clearCurrentNuageDePoint() {
    currentNuageDePoint->clearNuageDePoint();
    currentNuageDePoint->bindAndAllocateBuffer();
    update();
}

void MainWidget::switchNuageDePoint(){
    if( nextNuageDePointIndice >= allNuageDePoint.size() ) { nextNuageDePointIndice = 0 ; }
    currentNuageDePoint = allNuageDePoint[nextNuageDePointIndice];
    currentNuageDePoint->bindAndAllocateBuffer();
    nextNuageDePointIndice++;
    /*
    qDebug() << currentNuageDePoint->getVertices().size() ;
    qDebug() << currentNuageDePoint->getColors().size() ;
    qDebug() << currentNuageDePoint->getNormals().size() ;
    */
    cameraTarget2 = currentNuageDePoint->getBarycentre();
    update();
}

void MainWidget::parseNuageDePoint(){
    QVector<NuageDePoint *> NuageDePointSupplementaire = currentNuageDePoint->parseNDP();
    //QVector<NuageDePoint *> NuageDePointSupplementaire = currentNuageDePoint->parseNuageDePoint();
    for(NuageDePoint* ndp : NuageDePointSupplementaire) {
        ndp->computeBarycentre();
        allNuageDePoint.append( ndp );
    }
}

void MainWidget::analyseNuageDePoint(){
    currentNuageDePoint->analyseNuageDePoint();

}
