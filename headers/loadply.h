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

class loadPLY {
public:
    loadPLY();
    ~loadPLY();

    void loadPlyFile(const std::string& filename);
    void drawPlyGeometry(QOpenGLShaderProgram *program);

private:
    std::vector<PlyVertex> vertices;
    std::vector<PlyFace> faces;

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;

};

#endif // LOADPLY_H
