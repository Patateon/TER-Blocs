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
    std::vector<PlyVertex> vertices;
    std::vector<PlyFace> faces;
    std::vector<PlyColor> colors;
    std::vector<PlyNormal> normals;

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;

};

#endif // LOADPLY_H
