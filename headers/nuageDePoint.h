#ifndef NuageDePoint_H
#define NuageDePoint_H

#include "kdtree.h"
#include <vector>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_3.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_3 Point_3;
typedef CGAL::Delaunay_triangulation_3<K> Delaunay;

using namespace std;

#define DISTANCE_COULEURS 70.0 // entre 0 et 256
#define DISTANCE_XYZ 0.08 // entre 0 et 1
#define NBPOINTSMIN 1000
#define DISTANCE_COULEURS_DOMINANTE 0.15 // entre 0 et 1

enum class Descripteur {
    Graton,
    Plat,
    Slopers,
    Trou,
    Regle
};

class NuageDePoint
{
public:
    NuageDePoint();
    ~NuageDePoint();
    void clone(NuageDePoint* aCopier);

    void drawGeometry(QOpenGLShaderProgram *program);
    void performDelaunayTriangulation(const QVector<QVector3D>& vertices,std::vector<std::vector<int>> triangles);
    void addVertices(QVector3D vertice);
    void addColors(QVector3D color);
    void addNormals(QVector3D normal);
    QVector<QVector3D>& getVertices();
    QVector<QVector3D>& getColors();
    QVector<QVector3D>& getNormals();
    std::vector<std::vector<int>>& getTriangles();
    void bindAndAllocateBuffer();
    void buildKdtree();

    BasicANNkdTree getKdTree(){return kdtree;}

    QVector<NuageDePoint *> parseNuageDePoint();
    void analyseNuageDePoint();
    void clearNuageDePoint();

    void setDescripteur(Descripteur desc) { descripteur = desc; }

    Descripteur getDescripteur() const { return descripteur; }

    void computeBarycentre();
    QVector3D getBarycentre();

private:
    BasicANNkdTree kdtree;

    QVector<QVector3D> vertices;
    QVector<QVector3D> colors;
    QVector<QVector3D> normals;
    std::vector<std::vector<int>> triangles;
    QVector3D barycentre;


    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer colorBuf;
    QOpenGLBuffer normalBuf;
    QOpenGLBuffer triangleBuf;

    Descripteur descripteur;
};

#endif // NuageDePoint_H
