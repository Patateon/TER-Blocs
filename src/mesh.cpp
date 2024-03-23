#include "../headers/mesh.h"

float euclidean_distance(const float p1, const float p2) {
    float dr = p1 - p2;
    return sqrt( dr*dr );
}

Mesh::Mesh() {
    arrayBuf.create();
    colorBuf.create();
    normalBuf.create();
}

Mesh::~Mesh(){
    vertices.clear();
    colors.clear();
    normals.clear();
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

QVector<Mesh *> Mesh::parseMesh() {
    QVector<Mesh *> allMesh;
    QVector<QVector3D> v = vertices;
    QVector<QVector3D> c = colors;
    QVector<QVector3D> n = normals;
    while( !v.isEmpty() ) {
        Mesh* sousMesh = new Mesh();
        sousMesh->addVertices( v[0] ); v.removeAt(0);
        sousMesh->addColors( c[0] ); c.removeAt(0);
        sousMesh->addNormals( n[0] ); n.removeAt(0);
        QVector<QVector3D> sMeshColors=sousMesh->getColors();
        QVector<QVector3D> sMeshVertices=sousMesh->getVertices();
        for(int i = 0; i < c.size(); i++ ) {
            float distancePointX=euclidean_distance(sMeshVertices[0].x() , v[i].x());
            float distancePointY=euclidean_distance(sMeshVertices[0].y() , v[i].y());
            float distancePointZ=euclidean_distance(sMeshVertices[0].z() , v[i].z());
            float distanceRouge=euclidean_distance(sMeshColors[0].x()*255 , c[i].x()*255);
            float distanceVert=euclidean_distance(sMeshColors[0].y()*255 , c[i].y()*255);
            float distanceBleu=euclidean_distance(sMeshColors[0].z()*255 , c[i].z()*255);
            if(distancePointX + distancePointY + distancePointZ < DISTANCE_XYZ && distanceRouge + distanceVert + distanceBleu < DISTANCE_COULEURS ) {
                sousMesh->addVertices( v[i] ); v.removeAt(i);
                sousMesh->addColors( c[i] ); c.removeAt(i);
                sousMesh->addNormals( n[i] ); n.removeAt(i);
                i--;
            }
        }
        if( sousMesh->getVertices().size() > 300) {
            allMesh.append(sousMesh);
            qDebug() << "Sous mesh n: " << allMesh.size() << "de taille : " << allMesh[allMesh.size()-1]->getVertices().size();
        }
        else{
            delete sousMesh;
        }
    }
    qDebug() << "Nombre de sous mesh par couleur " << allMesh.size();
    qDebug() << "Nombre de vertices du premier sous mesh " << allMesh[0]->getVertices().size();
    return allMesh;
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
