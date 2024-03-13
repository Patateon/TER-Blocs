#ifndef LOADPLY_H
#define LOADPLY_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>



class loadPLY {
public:
    loadPLY();
    ~loadPLY();

    void loadPlyFile(const std::string& filename);
    void drawPlyGeometry(QOpenGLShaderProgram *program);

private:
    QVector<QVector3D> vertices;
    QVector<unsigned int> faces;
    QVector<QVector3D> colors;
    QVector<QVector3D> normals;

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    QOpenGLBuffer colorBuf;
    QOpenGLBuffer normalBuf;

};

#endif // LOADPLY_H
