#include "../headers/mesh.h"

float euclidean_distance(const float p1, const float p2) {
    float dr = p1 - p2;
    return sqrt( dr*dr );
}

float euclidean_distance(const QVector3D& p1, const QVector3D& p2) {
    float diffR = p1.x() - p2.x();
    float diffG = p1.y() - p2.y();
    float diffB = p1.z() - p2.z();
    return sqrt(diffR * diffR + diffG * diffG + diffB * diffB);
}


struct QVector3DComparer {
    bool operator()(const QVector3D& c1, const QVector3D& c2) const {
        // Compare les coordonnées x, y, et z
        if (c1.x() != c2.x()) return c1.x() < c2.x();
        if (c1.y() != c2.y()) return c1.y() < c2.y();
        return c1.z() < c2.z();
    }
};

// Fonction pour trouver la couleur dominante
QVector3D getDominantColor(const QVector<QVector3D>& couleurs) {
    // Définir une carte pour compter les occurrences de chaque couleur
    std::map<QVector3D, int, QVector3DComparer> compteurs;

    // Itérer sur toutes les couleurs et les regrouper
    for (const QVector3D& couleur : couleurs) {
        // Vérifier si une couleur similaire est déjà présente dans la carte
        bool couleurTrouvee = false;
        for (auto& it : compteurs) {
            if (euclidean_distance(couleur, it.first) < DISTANCE_COULEURS_DOMINANTE) { // Choisir une distance de seuil appropriée
                it.second++;
                couleurTrouvee = true;
                break;
            }
        }
        // Si la couleur n'a pas été trouvée, l'ajouter à la carte
        if (!couleurTrouvee) {
            compteurs[couleur] = 1;
        }
    }

    // Trouver la couleur avec le compteur le plus élevé
    QVector3D couleurDominante;
    int maxCompteur = 0;
    for (const auto& it : compteurs) {
        if (it.second > maxCompteur) {
            maxCompteur = it.second;
            couleurDominante = it.first;
        }
    }

    return couleurDominante;
}

Mesh::Mesh() {
    arrayBuf.create();
    colorBuf.create();
    normalBuf.create();
}
Mesh::~Mesh() {
    arrayBuf.destroy();
    colorBuf.destroy();
    normalBuf.destroy();
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
            float distancePoint=euclidean_distance(sMeshVertices[0], v[i]);
            /*
            float distanceRouge=euclidean_distance(sMeshColors[0].x()*256 , c[i].x()*256);
            float distanceVert=euclidean_distance(sMeshColors[0].y()*256 , c[i].y()*256);
            float distanceBleu=euclidean_distance(sMeshColors[0].z()*256 , c[i].z()*256);
            */
            float distanceCouleur=euclidean_distance(sMeshColors[0], c[i]);
            //if(distancePoint < DISTANCE_XYZ && distanceRouge + distanceVert + distanceBleu < DISTANCE_COULEURS ) {
            if(distancePoint < DISTANCE_XYZ && distanceCouleur < DISTANCE_COULEURS/256.0 ) {
                sousMesh->addVertices( v[i] ); v.removeAt(i);
                sousMesh->addColors( c[i] ); c.removeAt(i);
                sousMesh->addNormals( n[i] ); n.removeAt(i);
                i--;
            }
        }
        if( sousMesh->getVertices().size() > NBPOINTSMIN) {
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

void Mesh::clearMesh() {
    qDebug() << "Nombre de vertices avant nettoyage " << vertices.size();
    QVector3D dominantColor = getDominantColor(colors);
    qDebug() << "Couleur dominante " << dominantColor.x()  << dominantColor.y () << dominantColor.z();
    for(int i=0; i<vertices.size();){
        float distance = euclidean_distance(dominantColor, colors[i]);
        if(distance < DISTANCE_COULEURS_DOMINANTE){
            vertices.removeAt(i);
            colors.removeAt(i);
            normals.removeAt(i);
        } else {
            i++;
        }
    }
    qDebug() << "Nombre de vertices apres nettoyage " << vertices.size();
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
