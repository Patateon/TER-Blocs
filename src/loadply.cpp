#include "../headers/loadply.h"

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>



void loadPLY::loadPlyFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "vertex") {
            PlyVertex vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        } else if (token == "face") {
            PlyFace face;
            int numVertices;
            iss >> numVertices >> face.vertexIndex1 >> face.vertexIndex2 >> face.vertexIndex3;
            faces.push_back(face);
        }
    }
    arrayBuf.create();
    arrayBuf.bind();
    arrayBuf.allocate(vertices.data(), vertices.size() * sizeof(PlyVertex));

    indexBuf.create();
    indexBuf.bind();
    indexBuf.allocate(faces.data(), faces.size() * sizeof(PlyFace));

}

void loadPLY::drawPlyGeometry(QOpenGLShaderProgram *program) {
    arrayBuf.bind();
    indexBuf.bind();

    int vertexLocation = program->attributeLocation("vertex");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(PlyVertex));

    glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0);

    program->disableAttributeArray(vertexLocation);
}
