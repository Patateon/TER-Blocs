#ifndef NuageDePoint_H
#define NuageDePoint_H

#include "kdtree.h"
#include <vector>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

using namespace std;

#define DISTANCE_COULEURS 70.0 // entre 0 et 256
#define DISTANCE_XYZ 0.08 // entre 0 et 1
#define NBPOINTSMIN 1000
#define DISTANCE_COULEURS_DOMINANTE 0.15 // entre 0 et 1

class NuageDePoint
{
public:
    NuageDePoint();
    ~NuageDePoint();
    void clone(NuageDePoint* aCopier);

    void drawGeometry(QOpenGLShaderProgram *program);

    void addVertices(QVector3D vertice);
    void addColors(QVector3D color);
    void addNormals(QVector3D normal);
    QVector<QVector3D>& getVertices();
    QVector<QVector3D>& getColors();
    QVector<QVector3D>& getNormals();
    void bindAndAllocateBuffer();
    void buildKdtree();

    BasicANNkdTree getKdTree(){return kdtree;}

    QVector<NuageDePoint *> parseNuageDePoint();
    void clearNuageDePoint();

private:
    BasicANNkdTree kdtree;

    QVector<QVector3D> vertices;
    QVector<QVector3D> colors;
    QVector<QVector3D> normals;

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer colorBuf;
    QOpenGLBuffer normalBuf;
};

#endif // NuageDePoint_H
