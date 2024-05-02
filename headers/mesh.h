#ifndef MESH_H
#define MESH_H

#include <vector>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
class Mesh
{
private:
    QVector<QVector3D> m_vertices;
    QVector<QVector3D> m_colors;//couleur de triangle dans un OFF
    QVector<QVector3D> m_normals;
    std::vector<std::vector<unsigned int>> m_triangles;

    QOpenGLBuffer vertexBuf;
    QOpenGLBuffer colorBuf;
    QOpenGLBuffer normalBuf;
    QOpenGLBuffer triangleBuf;

public:
    // Constructors and destructors
    Mesh();
    Mesh(QVector<QVector3D> vertices, std::vector<std::vector<unsigned int>> triangles);
    ~Mesh();

    // Buffers and rendering methods
    void init();
    void draw(QOpenGLShaderProgram *program);
    void clear();

    // Getters and Setters
    QVector<QVector3D> vertices() const { return m_vertices; }
    void vertices(QVector<QVector3D> vertices) {m_vertices = vertices; }

    QVector<QVector3D> colors() const { return m_colors; }
    void colors(QVector<QVector3D> colors) {m_colors = colors; }

    QVector<QVector3D> normals() const { return m_normals; }
    void normals(QVector<QVector3D> normals) {m_normals = normals; }

    std::vector<std::vector<unsigned int>> triangles() const { return m_triangles; }
    void triangles(std::vector<std::vector<unsigned int>> triangles) {m_triangles = triangles; }
    void loadOffFile(const std::string& filename, Mesh *mesh){}
    void addVertice(QVector3D vertice);
    void addColor(QVector3D color);
    void addNormal(QVector3D normal);
    void addTriangle(std::vector<unsigned int> triangle);


};

#endif // MESH_H
