#ifndef LOADPLY_H
#define LOADPLY_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

struct PlyVertex {
    float x, y, z;
};

struct PlyFace {
    int vertexIndex1, vertexIndex2, vertexIndex3;
};

struct PlyNormal {
    float nx, ny, nz;
};

struct PlyColor {
    int r,g, b;
};

class loadPLY {
public:
    loadPLY();
    ~loadPLY();

    void loadPlyFile(const std::string& filename);
    void drawPlyGeometry(QOpenGLShaderProgram *program);

private:
    QVector<QVector3D> vertices;
    QVector<QVector3D> faces;
    QVector<QVector3D> colors;
    QVector<QVector3D> normals;

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;

};

#endif // LOADPLY_H
