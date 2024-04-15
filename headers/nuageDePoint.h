#ifndef NuageDePoint_H
#define NuageDePoint_H

#include <random>

#include "kdtree.h"
#include <vector>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_3.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_3 Point_3;
typedef CGAL::Delaunay_triangulation_3<K> Delaunay;

#include <CGAL/property_map.h>
#include <CGAL/IO/read_points.h>
#include <CGAL/Point_with_normal_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Shape_detection/Efficient_RANSAC.h>

// Type declarations.
typedef CGAL::Exact_predicates_inexact_constructions_kernel  Kernel;
typedef std::pair<Kernel::Point_3, Kernel::Vector_3>         Point_with_normal;
typedef std::vector<Point_with_normal>                       Pwn_vector;
typedef CGAL::First_of_pair_property_map<Point_with_normal>  Point_map;
typedef CGAL::Second_of_pair_property_map<Point_with_normal> Normal_map;
typedef CGAL::Shape_detection::Efficient_RANSAC_traits
    <Kernel, Pwn_vector, Point_map, Normal_map>             Traits;
typedef CGAL::Shape_detection::Efficient_RANSAC<Traits> Efficient_ransac;
typedef CGAL::Shape_detection::Plane<Traits>            Plane;
typedef CGAL::Shape_detection::Cone<Traits>             Cone;
typedef CGAL::Shape_detection::Cylinder<Traits>         Cylinder;
typedef CGAL::Shape_detection::Sphere<Traits>           Sphere;
typedef CGAL::Shape_detection::Torus<Traits>            Torus;


using namespace std;

#define DISTANCE_COULEURS 50.0 // entre 0 et 256
#define DISTANCE_XYZ 0.06 // entre 0 et 1
#define NBPOINTSMIN 1000
#define NBPOINTSMAX 5000
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
    QVector<NuageDePoint*> parseNDP();
    void analyseNuageDePoint();
    void clearNuageDePoint();

    void setDescripteur(Descripteur desc) { descripteur = desc; }

    Descripteur getDescripteur() const { return descripteur; }

private:
    BasicANNkdTree kdtree;

    QVector<QVector3D> vertices;
    QVector<QVector3D> colors;
    QVector<QVector3D> normals;
    std::vector<std::vector<int>> triangles;


    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer colorBuf;
    QOpenGLBuffer normalBuf;
    QOpenGLBuffer triangleBuf;

    Descripteur descripteur;
};

#endif // NuageDePoint_H
