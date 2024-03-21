#include "../headers/loadply.h"

#include <fstream>
#include <string>
#include <sstream>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <iostream>

loadPLY::loadPLY() {
    arrayBuf.create();
    colorBuf.create();
    normalBuf.create();
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
        }
        else if (isReadingVertices&&count_vertex<nbVertices)
        {
            float x,y,z;
            float r,g,b;
            float nx,ny,nz;
            // Assuming the format is "x y z"
            //vertex.x=std::stod(keyword);
            iss >>x>>y >> z;
            iss >>r>> g >> b;
            iss >>nx>> ny >> nz;
            vertices.append(QVector3D(x,y,z));
            colors.append(QVector3D(r/255.0f,g/255.0f,b/255.0f));
            normals.append(QVector3D(nx,ny,nz));
            count_vertex++;
        }
    }
    file.close();  // Close the file

    qDebug() << "Number of vertices : " << nbVertices;
    qDebug() << "Number of vertices read: " << vertices.size();
    qDebug() << "Vertices at indices 0, 10, and 50:";
    qDebug() << vertices[1];
    qDebug() << colors[1];
    qDebug() << normals[1];
    qDebug() << "File loaded successfully: " << QString::fromStdString(filename);

    QVector3D minBound = vertices[0];
    QVector3D maxBound = vertices[0];

    for (const QVector3D& vertex : vertices) {
        minBound.setX(qMin(minBound.x(), vertex.x()));
        minBound.setY(qMin(minBound.y(), vertex.y()));
        minBound.setZ(qMin(minBound.z(), vertex.z()));

        maxBound.setX(qMax(maxBound.x(), vertex.x()));
        maxBound.setY(qMax(maxBound.y(), vertex.y()));
        maxBound.setZ(qMax(maxBound.z(), vertex.z()));
    }

    // Calculate the scaling factors for each axis
    QVector3D boundingBoxSize = maxBound - minBound;
    float scale = 2.0 / std::max({boundingBoxSize.x(), boundingBoxSize.y(), boundingBoxSize.z()});

    // Calculate the translation vector to center the model
    QVector3D center = 0.5 * (minBound + maxBound);

    // Scale and translate the vertices
    for (QVector3D& vertex : vertices) {
        vertex = scale * (vertex - center);
    }


    arrayBuf.bind();
    arrayBuf.allocate(vertices.data(), vertices.size() * sizeof(QVector3D));
    colorBuf.bind();
    colorBuf.allocate(colors.data(), colors.size() * sizeof(QVector3D));
    normalBuf.bind();
    normalBuf.allocate(normals.data(), normals.size() * sizeof(QVector3D));

}

void loadPLY::drawPlyGeometry(QOpenGLShaderProgram *program) {

    arrayBuf.bind();
    int vertexLocation = program->attributeLocation("vertex");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    colorBuf.bind();
    int colorLocation = program->attributeLocation("color");
    program->enableAttributeArray(colorLocation);
    program->setAttributeBuffer(colorLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    normalBuf.bind();
    int normalLocation = program->attributeLocation("normal");
    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer(normalLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    glDrawArrays(GL_POINTS, 0, vertices.size());

    // Désactiver les attributs
    program->disableAttributeArray(vertexLocation);
    program->disableAttributeArray(colorLocation);
    program->disableAttributeArray(normalLocation);
}




