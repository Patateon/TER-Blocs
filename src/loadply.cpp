#include "../headers/loadply.h"

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <iostream>

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
    int count_vertex=0;
    int nbVertices;
    int nbFaces;

    // Read the .ply file line by line
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string keyword;
        std::string keyword2;
        if(isReadingVertices==false)
            iss >> keyword;

        if (keyword == "end_header")
        {
            // Start reading vertices
            isReadingVertices = true;
        }
        else if (keyword == "element")
        {
            iss>>keyword2;
            if(keyword2=="vertex"){
                iss>>nbVertices;

            }
            else if(keyword2=="face"){
                iss>>nbFaces;

            }
        }
        else if (isReadingVertices&&count_vertex<nbVertices)
        {
            float x,y,z;
            int r,g,b;
            float nx,ny,nz;
            // Assuming the format is "x y z"
            //vertex.x=std::stod(keyword);
            iss >>x>>y >> z;
            iss >>r>> g >> b;
            iss >>nx>> ny >> nz;
            vertices.append(QVector3D(x,y,z));
            colors.append(QVector3D(r,g,b));
            normals.append(QVector3D(nx,ny,nz));
            count_vertex++;
        }
        else if (count_vertex>=nbVertices)
        {
            int faceVertices;
            // Assuming the format is "numVertices v1 v2 v3 ..."
            iss >> faceVertices;
            //faceVertices=std::stod(keyword);

            // Assuming triangular faces, read and store indices in a new PlyFace
            if (faceVertices == 3)
            {
                int i0,i1,i2;
                iss >> i0 >> i1 >> i2;
                faces.append(QVector3D(i0,i1,i2));
            }
        }

    }
    file.close();  // Close the file

    qDebug() << "Number of vertices : " << nbVertices;
    qDebug() << "Number of vertices read: " << vertices.size();
    qDebug() << "Number of faces : " << nbFaces;
    qDebug() << "Number of faces read: " << faces.size();
    qDebug() << "Vertices at indices 0, 10, and 50:";
    qDebug() << vertices[0];
    qDebug() << "File loaded successfully: " << QString::fromStdString(filename);

    arrayBuf.create();
    arrayBuf.bind();
    arrayBuf.allocate(vertices.data(), vertices.size() * sizeof(QVector3D));

    indexBuf.create();
    indexBuf.bind();
    indexBuf.allocate(faces.data(), faces.size() * sizeof(QVector3D));
}

void loadPLY::drawPlyGeometry(QOpenGLShaderProgram *program) {
    arrayBuf.bind();
    indexBuf.bind();

    // Activer et lier les données de sommet
    int vertexLocation = program->attributeLocation("vertex");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    // Activer et lier les données de couleur
    int colorLocation = program->attributeLocation("color");
    program->enableAttributeArray(colorLocation);
    program->setAttributeBuffer(colorLocation, GL_FLOAT, sizeof(QVector3D), 3, sizeof(QVector3D));

    // Activer et lier les données de normale
    int normalLocation = program->attributeLocation("normal");
    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer(normalLocation, GL_FLOAT, 2 * sizeof(QVector3D), 3, sizeof(QVector3D));

    qDebug() << "Drawing PLY geometry with " << faces.size() << " faces";

    // Dessiner les éléments
    glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0);

    // Désactiver les attributs
    program->disableAttributeArray(vertexLocation);
    program->disableAttributeArray(colorLocation);
    program->disableAttributeArray(normalLocation);
}

