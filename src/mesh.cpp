#include "../headers/mesh.h"

Mesh::Mesh() {
    arrayBuf.create();
    colorBuf.create();
    normalBuf.create();
}

void Mesh::addVertices(QVector3D vertice){
    vertices.append(vertice);
}

void Mesh::addColors(QVector3D color){
    colors.append(color);
}

void Mesh::addNormals(QVector3D normal){
    normals.append(normal);
}

QVector<QVector3D>& Mesh::getVertices() {
    return vertices;
}

QVector<QVector3D>& Mesh::getColors() {
    return colors;
}

QVector<QVector3D>& Mesh::getNormals() {
    return normals;
}
void Mesh::buildKdtree(){
    /*
    vector<Vec3> verticesVectorVec3;
    for(const QVector3D& vertex : vertices){
        Vec3 v(vertex.x(),vertex.y(),vertex.z());
        verticesVectorVec3.push_back(v);
    }
    kdtree.build(verticesVectorVec3);*/
}

void Mesh::bindAndAllocateBuffer(){
    arrayBuf.bind();
    arrayBuf.allocate(vertices.data(), vertices.size() * sizeof(QVector3D));
    colorBuf.bind();
    colorBuf.allocate(colors.data(), colors.size() * sizeof(QVector3D));
    normalBuf.bind();
    normalBuf.allocate(normals.data(), normals.size() * sizeof(QVector3D));
}


void Mesh::drawGeometry(QOpenGLShaderProgram *program) {

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
