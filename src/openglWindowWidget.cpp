// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "../headers/openglWindowWidget.h"


unsigned int OpenGLWindowWidget::SCREENWIDTH = 640;
unsigned int OpenGLWindowWidget::SCREENHEIGHT = 480;
bool OpenGLWindowWidget::mouseRotatePressed = false;
bool OpenGLWindowWidget::mouseMovePressed = false;
bool OpenGLWindowWidget::mouseZoomPressed = false;
int OpenGLWindowWidget::lastX = 0;
int OpenGLWindowWidget::lastY = 0;
int OpenGLWindowWidget::lastZoom = 0;
bool OpenGLWindowWidget::fullScreen = false;
bool OpenGLWindowWidget::mainCamera = true;


OpenGLWindowWidget::OpenGLWindowWidget(QWidget *parent)
    : QOpenGLWidget(parent){
    actionLabel = new QLabel("Parsing point cloud...", this);
    // Set the text color to blue
    actionLabel->setStyleSheet("QLabel { color: white; font: bold; font-size: 32px;}");
    // Create a drop shadow effect for the outline
    QGraphicsDropShadowEffect *outlineEffect = new QGraphicsDropShadowEffect();
    outlineEffect->setBlurRadius(5); // Set the thickness of the outline
    outlineEffect->setColor(Qt::black); // Set the color of the outline
    outlineEffect->setOffset(0, 0); // Offset to create the outline effect

    // Apply the drop shadow effect to the label
    actionLabel->setGraphicsEffect(outlineEffect);
    actionLabel->setAlignment(Qt::AlignCenter);
    actionLabel->setFixedSize(400, 55);
    actionLabel->hide(); // Initially hide the label
}

OpenGLWindowWidget::~OpenGLWindowWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    doneCurrent();
    delete actionLabel;
}

//! [0]
void OpenGLWindowWidget::mousePressEvent(QMouseEvent *e)
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

void OpenGLWindowWidget::mouseMoveEvent(QMouseEvent *event)
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

void OpenGLWindowWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // Clear mouse press positions
    mousePressPosition = QVector2D();

    // Reset mouse pressed flags
    mouseMovePressed = false;
    mouseRotatePressed = false;
    mouseZoomPressed = false;

    QOpenGLWidget::mouseReleaseEvent(e);
}

// void OpenGLWindowWidget::wheelEvent(QWheelEvent * event){

// }

void OpenGLWindowWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    default:
        QOpenGLWidget::keyPressEvent(event);
    }
}

void OpenGLWindowWidget::keyReleaseEvent(QKeyEvent *event)
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
        update();
        break;
    case Qt::Key_M:
        fileMesh = QFileDialog::getSaveFileName(this, tr("Open OFF File"), "../TER-Blocs/data", tr("OFF Files (*.off)"));
        currentNuageDePoint->buildMesh(fileMesh.toStdString());
        break;
    default:
        QOpenGLWidget::keyReleaseEvent(event);
        break;
    }
}
//! [0]

//! [1]
void OpenGLWindowWidget::timerEvent(QTimerEvent *)
{

}
//! [1]

void OpenGLWindowWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.8, 0.8, 0.8, 1);
    initShaders();
    initTextures();
    timer.start(12, this);



}

//! [3]
void OpenGLWindowWidget::initShaders()
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
void OpenGLWindowWidget::initTextures()
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
void OpenGLWindowWidget::resizeGL(int w, int h)
{
    camera.resize (w, h);
    projectionMatrix = camera.getProjectionMatrix();
}
//! [5]

void OpenGLWindowWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!currentNuageDePoint) {
        return;
    }
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

    // qDebug()<<"Camera Position :"<<cameraPosition;

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

void OpenGLWindowWidget::openPlyFileNuageDePoint()
{
    // Delete any existing allocated memory to prevent memory leaks
    //a faire
    qDebug()<< "Try to open...";
    NuageDePoint* previousNuageDePoint=ndp;
    NuageDePoint* previousNuageDePointComp=ndpComparaison;
    PlyFile* previousPly=ply;
    QVector<NuageDePoint *> previousAllNuageDePoint =allNuageDePoint;
    allNuageDePoint.clear();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open PLY File"), "../TER-Blocs/data", tr("PLY Files (*.ply)"));
    if(fileName.isNull()) {
        qDebug() << "No file selected. Exiting initialization.";

        // Restore previous allocated memory
        ndp = previousNuageDePoint;
        ndpComparaison = previousNuageDePointComp;
        ply = previousPly;
        allNuageDePoint = previousAllNuageDePoint;

        return;
    }

    ndp = new NuageDePoint();
    allNuageDePoint.append(ndp);
    currentNuageDePoint = ndp;
    ply = new PlyFile();
    ply->loadPlyFile(fileName.toStdString(), currentNuageDePoint);
    ndpComparaison = new NuageDePoint();
    ndpComparaison->clone(ndp);

    currentNuageDePoint->computeBarycentre();
    cameraTarget = currentNuageDePoint->getBarycentre();
    qDebug() << "Barycentre du ndg courant : " << cameraTarget;

    viewMatrix.setToIdentity();
    camera.initPos();
    camera.zoom(3);

    // Delete the previous allocated memory
    // delete previousNuageDePointComp;
    // delete previousPly;
    // for (NuageDePoint* pndp : previousAllNuageDePoint) {
    //     delete pndp;
    // }

    update(); // Trigger a repaint after opening the file
    qDebug() << "everything's fine in opening PLY";
}

void OpenGLWindowWidget::saveCurrentNuageDePoint(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save PLY File"), "", tr("PLY Files (*.ply)"));
    if(fileName.isNull()) {
        qDebug() << "No file selected. Exiting initialization.";
        return;
    }
    ply->writePlyFile(fileName.toStdString(),currentNuageDePoint);
}

void OpenGLWindowWidget::deleteCurrentNuageDePoint(){
    delete allNuageDePoint[nextNuageDePointIndice - 1];
    allNuageDePoint.removeAt(nextNuageDePointIndice - 1);
    if (!allNuageDePoint.isEmpty()) {
        currentNuageDePoint = allNuageDePoint[nextNuageDePointIndice - 1];
        currentNuageDePoint->bindAndAllocateBuffer();
        update();
    } else {
        currentNuageDePoint = nullptr;
    }
    update();
}

void OpenGLWindowWidget::clearCurrentNuageDePoint() {
    currentNuageDePoint->clearNuageDePoint();
    currentNuageDePoint->bindAndAllocateBuffer();
    update();
}

void OpenGLWindowWidget::switchNuageDePoint(){
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

void OpenGLWindowWidget::parseNuageDePoint(){
    QVector<NuageDePoint *> NuageDePointSupplementaire = currentNuageDePoint->parseNDP();
    //QVector<NuageDePoint *> NuageDePointSupplementaire = currentNuageDePoint->parseNuageDePoint();
    for(NuageDePoint* ndp : NuageDePointSupplementaire) {
        ndp->computeBarycentre();
        allNuageDePoint.append( ndp );
    }
}

void OpenGLWindowWidget::analyseNuageDePoint(){
    currentNuageDePoint->analyseNuageDePoint();

}

void OpenGLWindowWidget::handleOpenNDPButtonClicked()
{
    openPlyFileNuageDePoint();
}

void OpenGLWindowWidget::handleSaveNDPButtonClicked()
{
    saveCurrentNuageDePoint();
}
void OpenGLWindowWidget::handleClearCurrentNuageDePoint(){
    // Calculate the position of the label to center it
    actionLabel->setText("Reduce en cours ..");
    int labelX = (parentWidget()->width() - actionLabel->width()) / 2;
    int labelY = (parentWidget()->height() - actionLabel->height()) / 2;
    actionLabel->move(labelX, labelY);
    actionLabel->show();

    QCoreApplication::processEvents();

    clearCurrentNuageDePoint();
    actionLabel->hide();
}
void OpenGLWindowWidget::handleDeleteCurrentNuageDePoint(){
   deleteCurrentNuageDePoint();
}
void OpenGLWindowWidget::handleSwitchNuageDePoint(){
    switchNuageDePoint();
}
void OpenGLWindowWidget::handleParseNuageDePoint(){
    // Calculate the position of the label to center it
    actionLabel->setText("Parsing en cours .. ");
    int labelX = (parentWidget()->width() - actionLabel->width()) / 2;
    int labelY = (parentWidget()->height() - actionLabel->height()) / 2;
    actionLabel->move(labelX, labelY);
    actionLabel->show();

    QCoreApplication::processEvents();

    parseNuageDePoint();

     actionLabel->hide();
}
void OpenGLWindowWidget::handleAnalyseNuageDePoint(){
    // Calculate the position of the label to center it
    actionLabel->setText("Analyse en cours ..");
    int labelX = (parentWidget()->width() - actionLabel->width()) / 2;
    int labelY = (parentWidget()->height() - actionLabel->height()) / 2;
    actionLabel->move(labelX, labelY);
    actionLabel->show();

    QCoreApplication::processEvents();

    analyseNuageDePoint();
    actionLabel->hide();
}
void OpenGLWindowWidget::handleCompareNuageDePoint(){
    afficher_ndpComparaison=!afficher_ndpComparaison;
    emit compareButtonColorChanged(afficher_ndpComparaison);
    update();
}
