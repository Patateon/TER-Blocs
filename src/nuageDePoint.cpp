#include "../headers/nuageDePoint.h"


Point_3 toPoint_3(const QVector3D& vec) {
    return Point_3(vec.x(), vec.y(), vec.z());
}

// Fonction pour effectuer la triangulation de Delaunay
void NuageDePoint::performDelaunayTriangulation(const QVector<QVector3D>& vertices,std::vector<std::vector<int>> triangles) {
    std::vector<Point_3> points;
    for(const QVector3D& vertex : vertices) {
        points.push_back(toPoint_3(vertex));
    }
    Delaunay dt;
    dt.insert(points.begin(), points.end());
    for (Delaunay::Finite_facets_iterator fit = dt.finite_facets_begin(); fit != dt.finite_facets_end(); ++fit) {

        Point_3 p0 = dt.triangle(*fit).vertex(0);
        Point_3 p1 = dt.triangle(*fit).vertex(1);
        Point_3 p2 = dt.triangle(*fit).vertex(2);
        // Vérifier si un des sommets du triangle est infini
        if (dt.is_infinite(*fit)) {
            continue;
        }
        else{
            int index_p0 = -1, index_p1 = -1, index_p2 = -1;
            for (int i = 0; i < vertices.size(); ++i) {
                if (vertices[i].x() == CGAL::to_double(p0.x()) &&
                    vertices[i].y() == CGAL::to_double(p0.y()) &&
                    vertices[i].z() == CGAL::to_double(p0.z())) {
                    index_p0 = i;
                }
                if (vertices[i].x() == CGAL::to_double(p1.x()) &&
                    vertices[i].y() == CGAL::to_double(p1.y()) &&
                    vertices[i].z() == CGAL::to_double(p1.z())) {
                    index_p1 = i;
                }
                if (vertices[i].x() == CGAL::to_double(p2.x()) &&
                    vertices[i].y() == CGAL::to_double(p2.y()) &&
                    vertices[i].z() == CGAL::to_double(p2.z())) {
                    index_p2 = i;
                }
            }


            std::vector<int> triangleIndices;
            triangleIndices.push_back(index_p0);
            triangleIndices.push_back(index_p1);
            triangleIndices.push_back(index_p2);
            triangles.push_back(triangleIndices);
        }
    }
}

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

NuageDePoint::NuageDePoint() {
    arrayBuf.create();
    colorBuf.create();
    normalBuf.create();
}

NuageDePoint::~NuageDePoint() {
    arrayBuf.destroy();
    colorBuf.destroy();
    normalBuf.destroy();
}

void NuageDePoint::addVertices(QVector3D vertice){
    vertices.append(vertice);
}

void NuageDePoint::addColors(QVector3D color){
    colors.append(color);
}

void NuageDePoint::addNormals(QVector3D normal){
    normals.append(normal);
}

QVector<QVector3D>& NuageDePoint::getVertices() {
    return vertices;
}

QVector<QVector3D>& NuageDePoint::getColors() {
    return colors;
}

QVector<QVector3D>& NuageDePoint::getNormals() {
    return normals;
}

std::vector<std::vector<int>>& NuageDePoint::getTriangles(){
    return triangles;
}

QVector<NuageDePoint *> NuageDePoint::parseNuageDePoint() {
    QVector<NuageDePoint *> allNuageDePoint;
    QVector<QVector3D> v = vertices;
    QVector<QVector3D> c = colors;
    QVector<QVector3D> n = normals;
    while( !v.isEmpty() ) {
        NuageDePoint* sousNuageDePoint = new NuageDePoint();
        sousNuageDePoint->addVertices( v[0] ); v.removeAt(0);
        sousNuageDePoint->addColors( c[0] ); c.removeAt(0);
        sousNuageDePoint->addNormals( n[0] ); n.removeAt(0);
        QVector<QVector3D> sNuageDePointColors=sousNuageDePoint->getColors();
        QVector<QVector3D> sNuageDePointVertices=sousNuageDePoint->getVertices();
        for(int i = 0; i < c.size(); i++ ) {
            float distancePoint=euclidean_distance(sNuageDePointVertices[0], v[i]);
            /*
            float distanceRouge=euclidean_distance(sNuageDePointColors[0].x()*256 , c[i].x()*256);
            float distanceVert=euclidean_distance(sNuageDePointColors[0].y()*256 , c[i].y()*256);
            float distanceBleu=euclidean_distance(sNuageDePointColors[0].z()*256 , c[i].z()*256);
            */
            float distanceCouleur=euclidean_distance(sNuageDePointColors[0], c[i]);
            //if(distancePoint < DISTANCE_XYZ && distanceRouge + distanceVert + distanceBleu < DISTANCE_COULEURS ) {
            if(distancePoint < DISTANCE_XYZ && distanceCouleur < DISTANCE_COULEURS/256.0 ) {
                sousNuageDePoint->addVertices( v[i] ); v.removeAt(i);
                sousNuageDePoint->addColors( c[i] ); c.removeAt(i);
                sousNuageDePoint->addNormals( n[i] ); n.removeAt(i);
                i--;
            }
        }
        if( sousNuageDePoint->getVertices().size() > NBPOINTSMIN) {
            allNuageDePoint.append(sousNuageDePoint);
            qDebug() << "Sous NuageDePoint n: " << allNuageDePoint.size() << "de taille : " << allNuageDePoint[allNuageDePoint.size()-1]->getVertices().size();
        }
        else{
            delete sousNuageDePoint;
        }
    }
    qDebug() << "Nombre de sous NuageDePoint par couleur " << allNuageDePoint.size();
    qDebug() << "Nombre de vertices du premier sous NuageDePoint " << allNuageDePoint[0]->getVertices().size();
    return allNuageDePoint;
}

float dotProduct(const QVector3D& v1, const QVector3D& v2) {
    return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

void NuageDePoint::analyseNuageDePoint(){
    QVector3D moyNormals;
    QVector3D moyPositions;
    float moyDotProductNormals=0;
    // int nbDotProductSupA075=0;
    // int nbDotProductSupA05Inf075=0;
    // int nbDotProductSupA025Inf05=0;
    // int nbDotProductSupA00Inf025=0;
    // int nbDotProductInfA00SupAMinus025=0;
    // int nbDotProductInfAMinus025SupAMinus05=0;
    int nbDotProductSupA05=0;
    int nbDotProductInfA05SupAMinus05=0;
    int nbDotProductInfAMinus05=0;
    int pasTrou=0; int trou=0;
    vector<int> vecPeutEtreTrou;
    for(int i = 0; i < normals.size() ; i++){
        moyNormals+=normals[i];
        moyPositions+= vertices[i];
        int peutEtreTrou=0;
        for(int j = i; j < normals.size() ; j++){
            float dotP=dotProduct(normals[i] , normals[j]);
            // dotP >= 0.75 ? nbDotProductSupA075++ :
            //     dotP >=0.5 ? nbDotProductSupA05Inf075++ :
            //         dotP >= 0.25 ? nbDotProductSupA025Inf05++ :
            //             dotP >= 0.0 ? nbDotProductSupA00Inf025++ :
            //                 dotP >= -0.25 ? nbDotProductInfA00SupAMinus025++ :
            //                     dotP >= -0.5 ? nbDotProductInfAMinus025SupAMinus05++ : nbDotProductInfAMinus05++;

            dotP >= 0.5 ? nbDotProductSupA05++ : dotP >= -0.5 ? nbDotProductInfA05SupAMinus05++ : nbDotProductInfAMinus05++;
            moyDotProductNormals+=dotP;

            if(dotP < -0.5){
                QVector3D vec=vertices[j]-vertices[i];
                vec.normalize();
                float dotPjvec=dotProduct(vec,normals[i]);
                dotPjvec < 0.5 ? pasTrou++ : peutEtreTrou++ ;
            }
        }
        vecPeutEtreTrou.push_back(peutEtreTrou);
    }
    for(int i = 0 ; i < vecPeutEtreTrou.size() ; i++){
        if(vecPeutEtreTrou[i]>0.1*vertices.size()){trou+=vecPeutEtreTrou[i];}
        else{pasTrou+=vecPeutEtreTrou[i];}
    }
    moyPositions/=vertices.size();
    moyNormals/=normals.size();
    moyDotProductNormals/=(normals.size()*(normals.size() + 1))/2;
    qDebug() << " Moyenne des normales : " << moyNormals;
    qDebug() << " Moyenne des Position : " << moyPositions;
    qDebug() << " Moyenne des produits scalaires entre les normales : " << moyDotProductNormals;
    // qDebug() << " Nombre de DP >= 0.75 : " << nbDotProductSupA075;
    // qDebug() << " Nombre de DP >= 0.5 : & < 0.75 : " << nbDotProductSupA05Inf075;
    // qDebug() << " Nombre de DP >= 0.25 &  < 0.5 : " << nbDotProductSupA025Inf05;
    // qDebug() << " Nombre de DP >= 0.0 & < 0.25 : " << nbDotProductSupA00Inf025;
    // qDebug() << " Nombre de DP < 0.0 &  >= -0.25 : " << nbDotProductInfA00SupAMinus025;
    // qDebug() << " Nombre de DP < -0.25 &  >= -0.5 : " << nbDotProductInfAMinus025SupAMinus05;
    // qDebug() << " Nombre de DP >= -0.5 : & < 0.5 : " << nbDotProductSupAMinus05Inf05;

    qDebug() << " Nombre de DP >= 0.5 : " << nbDotProductSupA05;
    qDebug() << " Nombre de DP >= -0.5 & < 0.5 : " << nbDotProductInfA05SupAMinus05;
    qDebug() << " Nombre de DP < -0.5 : " << nbDotProductInfAMinus05;
    qDebug() << " Trou? : " << trou << "Contour externe : " << pasTrou;

}

void NuageDePoint::clearNuageDePoint() {
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

void NuageDePoint::clone( NuageDePoint* aCopier) {
    vertices = aCopier->vertices;
    colors = aCopier->colors;
    normals = aCopier->normals;

    (this)->bindAndAllocateBuffer();
}

void NuageDePoint::buildKdtree(){

    vector<QVector3D> verticesVectorVec3;
    for(const QVector3D& vertex : vertices){
        verticesVectorVec3.push_back(vertex);
    }
    kdtree.build(verticesVectorVec3);
}

void NuageDePoint::bindAndAllocateBuffer(){
    arrayBuf.bind();
    arrayBuf.allocate(vertices.data(), vertices.size() * sizeof(QVector3D));
    colorBuf.bind();
    colorBuf.allocate(colors.data(), colors.size() * sizeof(QVector3D));
    normalBuf.bind();
    normalBuf.allocate(normals.data(), normals.size() * sizeof(QVector3D));
}

void NuageDePoint::drawGeometry(QOpenGLShaderProgram *program) {

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

    triangleBuf.bind();
    triangleBuf.allocate(triangles.data(), triangles.size() * sizeof(std::vector<int>));

    glDrawArrays(GL_POINTS, 0, vertices.size());

    // Désactiver les attributs
    program->disableAttributeArray(vertexLocation);
    program->disableAttributeArray(colorLocation);
    program->disableAttributeArray(normalLocation);
}

void NuageDePoint::computeBarycentre() {
    QVector3D tmp = QVector3D();
    int vertices_size = this->vertices.size();
    for(int i = 0; i < vertices_size; i++){
        tmp += this->vertices[i];
    }
    tmp *= (1.f / (float) vertices_size);
    barycentre = tmp;
}

QVector3D NuageDePoint::getBarycentre() {
    return barycentre;
}
