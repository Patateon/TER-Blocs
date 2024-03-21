#ifndef MESH_H
#define MESH_H

#include "kdtree.h"
#include "Vec3.h"


#include <vector>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

using namespace std;

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
