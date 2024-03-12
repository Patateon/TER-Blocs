#include "../headers/loadply.h"

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>


loadPLY::loadPLY() {
}

void loadPLY::loadPlyFile(const std::string& filename) {

        std::ifstream file(filename);

        std::string line;
        // Check if the file is open
        if (!file.is_open())
        {
            qDebug() << "Failed to open .ply file";
            return;
        }

        // Flag to indicate whether vertex or face data is being read
        bool isReadingVertices = false;
        bool isReadingFaces = false;

        // Read the .ply file line by line
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string keyword;
            if(isReadingVertices==false)
                iss >> keyword;

            if (keyword == "end_header")
            {
                // Start reading vertices
                isReadingVertices = true;
                isReadingFaces = false;
            }
            else if (keyword == "element" && line.find("face") != std::string::npos)
            {
                // Start reading faces

                PlyFace face;
                isReadingVertices = false;
                isReadingFaces = true;

                iss >> face.vertexIndex1 >> face.vertexIndex2 >> face.vertexIndex3;
                faces.push_back(face);
            }
            else if (isReadingVertices)
            {
                PlyVertex vertex;

                // Assuming the format is "x y z"
                iss >>vertex.x>> vertex.y >> vertex.z;
                vertices.push_back(vertex);
            }
            else if (isReadingFaces)
            {
                int faceVertices;
                // Assuming the format is "numVertices v1 v2 v3 ..."
                iss >> faceVertices;

                // Assuming triangular faces, read and store indices in a new PlyFace
                if (faceVertices == 3)
                {
                    PlyFace face;
                    iss >> face.vertexIndex1 >> face.vertexIndex2 >> face.vertexIndex3;
                    faces.push_back(face);
                }
            }

        }
    file.close();  // Close the file


    qDebug() << "Number of vertices read: " << vertices.size();
    qDebug() << "Number of faces read: " << faces.size();
    qDebug() << "Vertices at indices 0, 10, and 50:";
    qDebug() << "Index 0:" << vertices[0].x << vertices[0].y << vertices[0].z;
    qDebug() << "Index 10:" << vertices[10].x << vertices[10].y << vertices[10].z;
    qDebug() << "Index 50:" << vertices[50].x << vertices[50].y << vertices[50].z;

    qDebug() << "File loaded successfully: " << QString::fromStdString(filename);

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

    qDebug() << "Drawing PLY geometry with " << faces.size() << " faces";

    glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0);

    program->disableAttributeArray(vertexLocation);
}
