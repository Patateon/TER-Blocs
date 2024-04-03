#include "../headers/plyFile.h"

#include <fstream>
#include <string>
#include <sstream>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <iostream>

PlyFile::PlyFile(){};

void PlyFile::loadPlyFile(const std::string& filename, NuageDePoint *ndp) {

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
            ndp->addVertices(QVector3D(x,y,z));
            ndp->addColors(QVector3D(r/256.0f,g/256.0f,b/256.0f));
            ndp->addNormals(QVector3D(nx,ny,nz));
            count_vertex++;
        }
    }
    file.close();  // Close the file
    QVector<QVector3D> vertices=ndp->getVertices();
    QVector<QVector3D> colors=ndp->getColors();
    QVector<QVector3D> normals=ndp->getNormals();
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

    ndp->bindAndAllocateBuffer();
  }

void PlyFile::writePlyFile(const std::string& filename, NuageDePoint* ndp) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        qDebug() << "Failed to open .ply file for writing";
        return;
    }
    QVector<QVector3D> vertices=ndp->getVertices();
    QVector<QVector3D> colors=ndp->getColors();
    QVector<QVector3D> normals=ndp->getNormals();
    // Write PLY header
    file << "ply\n";
    file << "format ascii 1.0\n";
    file << "element vertex " << vertices.size() << "\n";
    file << "property float x\n";
    file << "property float y\n";
    file << "property float z\n";
    file << "property uchar red\n";
    file << "property uchar green\n";
    file << "property uchar blue\n";
    file << "property float nx\n";
    file << "property float ny\n";
    file << "property float nz\n";
    file << "end_header\n";

    // Write vertex data
    for (int i = 0; i < vertices.size(); ++i) {
        const QVector3D& vertex = vertices[i];
        const QVector3D& color = colors[i];
        const QVector3D& normal = normals[i];
        file << vertex.x() << " " << vertex.y() << " " << vertex.z() << " ";
        file << static_cast<int>(color.x()*256) << " " << static_cast<int>(color.y()*256) << " " << static_cast<int>(color.z()*256) << " ";
        file << normal.x() << " " << normal.y() << " " << normal.z() << "\n";
    }

    file.close();
    qDebug() << "PLY file written successfully: " << QString::fromStdString(filename);
}


