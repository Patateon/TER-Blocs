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

// Met à jour les centroïdes en calculant la moyenne des points du cluster
QVector3D updateCentroids(const QVector<QVector3D>& cluster) {
    float sumX = 0, sumY = 0, sumZ = 0;
    for (const QVector3D& point : cluster) {
        sumX += point.x();
        sumY += point.y();
        sumZ += point.z();
    }
    int n = cluster.size();
    return QVector3D(sumX / n, sumY / n, sumZ / n);
}

QVector3D updateCentroidColors(const QVector<QVector3D>& clusterColors) {
    float sumX = 0, sumY = 0, sumZ = 0;
    for (const QVector3D& color : clusterColors) {
        sumX += color.x();
        sumY += color.y();
        sumZ += color.z();
    }
    int n = clusterColors.size();
    return QVector3D(sumX / n, sumY / n, sumZ / n);
}

// Initialisation des centroïdes
QVector<QVector3D> initializeCentroidsKMeansPlusPlus(const QVector<QVector3D>& points,const QVector<QVector3D>& color, int k,QVector<QVector3D>& centroidColors) {
    QVector<QVector3D> centroids;
    // Premier centroide
    int firstIndex = rand() % points.size();
    centroids.push_back(points[firstIndex]);
    centroidColors.push_back(color[firstIndex]);
    qDebug()<<points.size();
    //Sélectionner les k-1 centroïdes restants, le plus loin d'un autre centroide
    while (centroids.size() < k) {
        QVector<float> distancesSquared(points.size(), std::numeric_limits<float>::max());
        QVector<float> distancesSquaredColor(points.size(), std::numeric_limits<float>::max());
        for (int i = 0; i < points.size(); ++i) {
            for (int j=0;j<centroids.size();j++) {
                float distanceSquared = euclidean_distance(points[i], centroids[j]);
                float distanceSquaredColor= euclidean_distance(color[i],centroidColors[j]);
                distancesSquared[i] = std::min(distancesSquared[i], distanceSquared);
                distancesSquaredColor[i]=std::min(distancesSquaredColor[i], distanceSquaredColor);
            }
        }
        float dmax=std::numeric_limits<float>::min();
        float dmaxColor=std::numeric_limits<float>::min();
        int indmax=-1;
        for(int i=0;i<distancesSquared.size();i++){
            if(distancesSquared[i]>dmax){//&&distancesSquaredColor[i]>dmaxColor) {
                dmax=distancesSquared[i];
                dmaxColor=distancesSquaredColor[i];
                indmax=i;
            }
        }
        centroids.push_back(points[indmax]);
        centroidColors.push_back(color[indmax]);
    }

    return centroids;
}



QVector<NuageDePoint*> NuageDePoint::parseNDP() {
    buildKdtree();
    QVector<NuageDePoint*> allNDP;
    float seuilDistance=DISTANCE_XYZ;
    float seuilCouleur=DISTANCE_COULEURS;
    // Nombre de clusters (k)
    int k = 20; // Vous pouvez modifier ce nombre selon vos besoins
    QVector<QVector3D> centroidColors;
    QVector<QVector3D> centroids=initializeCentroidsKMeansPlusPlus(vertices,colors,k,centroidColors);

    // Initialisation des clusters
    QVector<QVector<QVector3D>> clusters(k);
    QVector<QVector<QVector3D>> colors_(k);
    QVector<QVector<QVector3D>> normals_(k);

    // Algorithme K-means
    bool converged = false;
    qDebug()<<"Entree boucle";
    while (!converged) {
        std::vector<int> mark(getVertices().size(), 0);
        std::vector<int> classe(getVertices().size(), -1);
        std::vector<float> dist(getVertices().size(),FLT_MAX);
        int nbVoisin=6000;
        // On récupère les voisins de chaque centroide et on stock leur distance minimale a un centroide
        for (int i = 0; i < k; ++i) {
            ANNidxArray id_nearest_neighbors = new ANNidx[ nbVoisin ];
            ANNdistArray square_distances_to_neighbors = new ANNdist[ nbVoisin ];
            kdtree.knearest(centroids[i],nbVoisin,id_nearest_neighbors,square_distances_to_neighbors);
            //On récupère les distances et on attribue les classes
            for(int j=0;j<nbVoisin;j++){
                int ind=id_nearest_neighbors[j];
                float distance=square_distances_to_neighbors[j];
                if(distance<dist[ind]&&distance<seuilDistance){
                    float distColor=euclidean_distance(centroidColors[i],colors[ind]);
                    if(distColor<seuilCouleur/256.0){
                        dist[ind]=distance;
                        classe[ind]=i;
                    }

                }
            }
            delete [] id_nearest_neighbors;
            delete [] square_distances_to_neighbors;
        }
        // Assigner les classes
        for(int i=0;i<classe.size();i++){
            if(classe[i]!=-1){
                clusters[classe[i]].push_back(vertices[i]);
                colors_[classe[i]].push_back(colors[i]);
                normals_[classe[i]].push_back(normals[i]);
                mark[i]=1;
            }
        }
        //Maj centroide
        QVector<QVector3D> oldCentroids=centroids;
        centroids.clear();
        centroidColors.clear();
        for (int i = 0; i < k; ++i) {
            if (!clusters[i].empty()) {
                centroids.push_back(updateCentroids(clusters[i]));
                centroidColors.push_back(updateCentroidColors(colors_[i]));
            }
        }
        //Ajout de centroide

        bool add=false;
        int seuilNbNonAssignes=vertices.size()/4;
        int nbNonAssignes=0;
        std::vector<int> indiceNonAssignes;
        for(int i=0;i<mark.size();i++){
            if(mark[i]==0) {
                nbNonAssignes++;
                indiceNonAssignes.push_back((i));
            }
        }
        if(nbNonAssignes>seuilNbNonAssignes){
            add=true;
        }
        //qDebug()<<"nn assignes"<<nbNonAssignes<<"k "<<k;
        QVector<QVector3D> sommetNonAssignes;
        if(add){
            for(int i=0;i<indiceNonAssignes.size();i++){
                sommetNonAssignes.push_back(vertices[indiceNonAssignes[i]]);
            }
        }
        //Verification convergence sur nombre de changement de classe si on ajoute pas de centroide
        int seuilConvergence=15;
        int nbChangementCentroide=0;
        qDebug()<<k;

        for(int i=0;i<centroids.size();i++){
            if(oldCentroids[i]!=centroids[i]){
                nbChangementCentroide++;
            }
        }
        qDebug()<<nbChangementCentroide<<"cgm";
        if(nbChangementCentroide<seuilConvergence){
            converged=true;
        }
        else{
            if(add){
                k++;
                clusters.resize(k);
                colors_.resize(k);
                normals_.resize(k);
                int randomIndex = rand() % sommetNonAssignes.size();
                centroids.push_back(vertices[indiceNonAssignes[randomIndex]]);
                oldCentroids.push_back(vertices[indiceNonAssignes[randomIndex]]);
                centroidColors.push_back(colors[indiceNonAssignes[randomIndex]]);
                centroidColors.resize(k);
                centroids.resize((k));
                oldCentroids.resize(k);
            }
            for(int i=0;i<clusters.size();i++){
                clusters[i].clear();
                colors_[i].clear();
                normals_[i].clear();
            }
        }

        /*
        else{
            for(int i=0;i<clusters.size();i++){
                clusters[i].clear();
                colors_[i].clear();
                normals_[i].clear();
            }
        }
*/

    }
    // Créer les sous-meshes à partir des clusters
    for (int i = 0; i < k; ++i) {
        qDebug()<<i<<clusters.size();
        qDebug()<<clusters[i].size();
        // Vérifier si le cluster n'est pas vide
        if (!clusters[i].isEmpty()) {
            NuageDePoint* sousNDP = new NuageDePoint();
            // Utilisation de <= pour inclure le dernier point du cluster
            for (int j = 0; j < clusters[i].size(); ++j) {
                sousNDP->addVertices(clusters[i][j]);
                sousNDP->addColors(colors_[i][j]);
                sousNDP->addNormals(normals_[i][j]);
            }
            qDebug() << "Taille sous mesh" << sousNDP->getVertices().size();
            if(sousNDP->getVertices().size()<NBPOINTSMAX && sousNDP->getVertices().size()>NBPOINTSMIN){
                allNDP.push_back(sousNDP);
            }

            qDebug() << "Nombre de sous mesh par couleur " << allNDP.size();
        }
    }


    return allNDP;
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
