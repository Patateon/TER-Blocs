#ifndef MESH_H
#define MESH_H

#include "kdtree.h"
#include <vector>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

using namespace std;

#define DISTANCE_COULEURS 120.0
#define DISTANCE_XY 0.14

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void drawGeometry(QOpenGLShaderProgram *program);

    void addVertices(QVector3D vertice);
    void addColors(QVector3D color);
    void addNormals(QVector3D normal);
    QVector<QVector3D>& getVertices();
    QVector<QVector3D>& getColors();
    QVector<QVector3D>& getNormals();
    void bindAndAllocateBuffer();
    void buildKdtree();

    QVector<Mesh *> parseMesh();

private:
    BasicANNkdTree kdtree;

    QVector<QVector3D> vertices;
    QVector<QVector3D> colors;
    QVector<QVector3D> normals;

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer colorBuf;
    QOpenGLBuffer normalBuf;
};

#endif // MESH_H
