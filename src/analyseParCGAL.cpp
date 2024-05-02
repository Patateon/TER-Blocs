#include "../headers/nuageDePoint.h"

// CGAL Include
#include <CGAL/linear_least_squares_fitting_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/poisson_surface_reconstruction.h>


// Types
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point3D;
typedef Kernel::Vector_3 Vector3D;
typedef std::pair<Point3D, Vector3D> Pwn;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;

float dotProduct(const QVector3D& v1, const QVector3D& v2) {
    return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}


QVector3D generate_unique_color(const std::vector<QVector3D>& couleurs) {
    // Générateur de nombres aléatoires
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    // Générer une couleur unique RVB
    QVector3D color;
    bool unique = false;
    while (!unique) {
        // Générer des valeurs RVB aléatoires
        float r = dist(gen);
        float g = dist(gen);
        float b = dist(gen);

        // Vérifier si la couleur est unique
        unique = true;
        for (const auto& c : couleurs ) {
            float dr = c.x() - r;
            float dg = c.y() - g;
            float db = c.z() - b;
            // Calculer la distance euclidienne entre la nouvelle couleur et les couleurs existantes
            float distance = std::sqrt(dr * dr + dg * dg + db * db);
            // Si la distance est inférieure à une certaine valeur, la couleur n'est pas unique
            if (distance < 0.15f) { // Changer cette valeur selon vos besoins
                unique = false;
                break;
            }
        }

        // Si la couleur est unique, l'ajouter à la liste et sortir de la boucle
        if (unique) {
            color.setX(r);
            color.setY(g);
            color.setZ(b);
            break;
        }
    }

    return color;
}


//The value returned is a fitting quality between 0 and 1, where 0 means that the variance is the same along any
//plane going through the best fitting line, and 1 means that the variance is null
//orthogonally to the best fitting plane (hence the fit is perfect).
double calculate_linear_least_squares_fitting(const std::vector<Point_3>& cgal_points, Plane_3& plane, Kernel::Point_3& centroid) {
    centroid = CGAL::centroid(cgal_points.begin(), cgal_points.end());
    return CGAL::linear_least_squares_fitting_3(cgal_points.begin(), cgal_points.end(), plane, centroid, CGAL::Dimension_tag<0>(), Kernel(), CGAL::Default_diagonalize_traits<double, 3>());
}


void NuageDePoint::analyseNuageDePoint(){
    Pwn_vector points;
    Plane_3 plane;
    Kernel::Point_3 centroid;
    std::vector<Point_3> cgal_points;
    for(int i=0;i<vertices.size();i++) {
        cgal_points.push_back(Kernel::Point_3(vertices[i].x(),vertices[i].y(), vertices[i].z()));
    }
    double qualite =calculate_linear_least_squares_fitting(cgal_points, plane, centroid);
    // Le plan est défini par l'équation ax + by + cz + d = 0
    double a = plane.a();
    double b = plane.b();
    double c = plane.c();
    double d = plane.d();
    qDebug() << "qualité du plan : " << qualite;
    qDebug() <<  a << "x + " << b << "y + " << c << " z + " << d;
    // Calcul de l'angle d'inclinaison entre la normale du plan et un vecteur vertical prédéfini
    double vertical_angle = acos(abs(plane.orthogonal_vector() * Kernel::Vector_3(0, 0, 1))); // Produit scalaire entre les vecteurs
    // Convertir l'angle en degrés
    double vertical_angle_degrees = vertical_angle *180/M_PI;

    int positive_distance_count = 0;
    int negative_distance_count = 0;

    // Compter les distances positives et négatives
    for(int i = 0; i < vertices.size(); i++) {
        double distance = (a * vertices[i].x() + b * vertices[i].y() + c * vertices[i].z() + d) / std::sqrt(a*a + b*b + c*c);
        if(distance > 0) {
            positive_distance_count++;
        } else if(distance < 0) {
            negative_distance_count++;
        }
    }

    // Identifier les faces profondes et étroites en fonction des compteurs
    if(positive_distance_count > negative_distance_count) {
        // La majorité des points ont des distances positives, indiquant des faces profondes
        qDebug() << "Face profonde";
    } else {
        // La distribution des distances est plus équilibrée, indiquant des faces étroites
        qDebug() << "Face étroite";
    }


    qDebug() << "inclinaison" << vertical_angle_degrees;

    /////projection
    /*
    QVector<QVector3D> vertices_b;
    for(int i= 0; i <vertices.size(); i++) {
        double distance =(a * cgal_points[i].x() + b * cgal_points[i].y() + c * cgal_points[i].z() + d)/std::sqrt(a*a+b*b+c*c);
        Kernel::Point_3 projected_point = cgal_points[i] -distance*plane.orthogonal_vector();
        vertices_b.push_back(QVector3D(projected_point.x(),projected_point.y(),projected_point.z()));
    }

    qDebug() << "taille vertice proj : " << vertices_b.size();
    vertices=vertices_b;
*/
    ////fin projection
    for(int i=0;i<vertices.size();i++) {
        points.push_back(std::make_pair(Kernel::Point_3(vertices[i].x(),vertices[i].y(), vertices[i].z()), Kernel::Vector_3(normals[i].x(), normals[i].y(), normals[i].z())));
    }
    // Instantiate shape detection engine.
    Efficient_ransac ransac;
    // Provide input data.
    ransac.set_input(points);
    // Register planar shapes via template method.
    ransac.add_shape_factory<Plane>();
    // Register spherical shapes via template method
    //   ransac.add_shape_factory<Sphere>();
    /*ransac.add_shape_factory<Cone>();
    ransac.add_shape_factory<Cylinder>();*/
    //   ransac.add_shape_factory<Torus>();
    // Detect registered shapes with default parameters.
    // Set parameters for shape detection.
    Efficient_ransac::Parameters parameters;
    // Set probability to miss the largest primitive at each iteration.
    parameters.probability = 0.05;
    // Detect shapes with at least 200 points.
    parameters.min_points = 25000;
    // Set maximum Euclidean distance between a point and a shape.
    parameters.epsilon = 0.003;
    // Set maximum Euclidean distance between points to be clustered.
    parameters.cluster_epsilon = 2;
    // Set maximum normal deviation.
    // 0.8 < dot(surface_normal, point_normal);
    parameters.normal_threshold = 0.8;
    // Detect shapes.
    ransac.detect(parameters);
    // Print number of detected shapes.
    qDebug() << ransac.shapes().end() - ransac.shapes().begin()
             << " shapes detected." ;

    // Check if shapes have been detected
    if (ransac.shapes().empty()) {
        qDebug() << "No shapes detected.";
        return; // Exit the function if no shapes are detected
    }

    // Récupérer les formes détectées
    const auto& detected_shapes = ransac.shapes();

    // Créer une couleur unique pour chaque forme
    std::vector<QVector3D> colorsShape;
    for (uint i = 0; i < detected_shapes.size(); i++) {
        // Générer une couleur unique
        colorsShape.push_back(generate_unique_color(colorsShape));
    }
    int couleur= 0;
    Efficient_ransac::Shape_range::iterator it = ransac.shapes().begin();
    vertices.clear();normals.clear();colors.clear();

    std::vector<QVector3D> moyNormalShape;

    /////regarde si sloper plat ou ni l'un ni l autre
    int verticeSloper=0;
    int verticePlat=0;
    int verticeNiPlatNiSloper=0;


    CGAL::Vector_3<Kernel> normalPlanCGAL = plane.orthogonal_vector();
    QVector3D planNormal(normalPlanCGAL.x(), normalPlanCGAL.y(), normalPlanCGAL.z());

    while (it != ransac.shapes().end()) {
        // Récupérer la forme détectée
        boost::shared_ptr<Efficient_ransac::Shape> shape = *it;

        // Utiliser Shape_base::info() pour imprimer les paramètres de la forme détectée
        qDebug() << (*it)->info();

        QVector3D sumNormalShape;

        // Itérer à travers les indices de points assignés à chaque forme détectée
        std::vector<std::size_t>::const_iterator index_it = (*it)->indices_of_assigned_points().begin();
        while (index_it != (*it)->indices_of_assigned_points().end()) {
            // Récupérer le point avec normale à partir de votre propre nuage de points
            const Point_with_normal& p = points[*index_it]; // Supposant que 'points' est votre nuage de points avec normales
            // Dessinez ou utilisez les points avec leur couleur attribuée (par exemple, pour la visualisation)
            // Ajouter le point avec sa normale à vos vecteurs de vertices et de normals
            QVector3D vertex(p.first.x(), p.first.y(), p.first.z());
            QVector3D normal(p.second.x(), p.second.y(), p.second.z());
            sumNormalShape+=normal;
            vertices.push_back(vertex);
            normals.push_back(normal);
            colors.push_back(colorsShape[couleur]);

            index_it++;
        }
        sumNormalShape/=(*it)->indices_of_assigned_points().size();

        if( (*it)->indices_of_assigned_points().size()>vertices.size()/4.0 && dotProduct(sumNormalShape,planNormal) > 0.5){
            //qDebug() << "Surement une prise plate";
            verticePlat+=(*it)->indices_of_assigned_points().size();
        }
        else if ((*it)->indices_of_assigned_points().size()>vertices.size()/4.0 ){
            //qDebug() << "Surement un sloper";
            verticeSloper+=(*it)->indices_of_assigned_points().size();
        }
        else {
            //qDebug() << "Ni une prise plate, ni un sloper"   ;
            verticeNiPlatNiSloper+=(*it)->indices_of_assigned_points().size();
        }
        moyNormalShape.push_back(sumNormalShape);
        it++;couleur++;
    }

    float moyDotProductNormals=0;
    for(uint i=0 ; i< moyNormalShape.size() ; i++){
        for(uint j =i+1; j< moyNormalShape.size() ;j++){
            float dotP=dotProduct(moyNormalShape[i] , moyNormalShape[j]);
            moyDotProductNormals+=dotP;
        }
    }
    moyDotProductNormals/=(moyNormalShape.size()*(moyNormalShape.size() + 1))/2;

    qDebug() << " Moyenne des produits scalaires entre les normales (1normal = 1shape)  : " << moyDotProductNormals;
    // Vérifiez si la forme détectée est un torus
    //premier forme = torus avec bcp de point = trou
    Efficient_ransac::Shape_range shapes = ransac.shapes();
    Efficient_ransac::Shape_range::iterator it_torus = shapes.begin();
    if (Torus* torus = dynamic_cast<Torus*>(it_torus->get()) )  {
        qDebug() << "Surement une prise a trou";
    }
    else if(moyDotProductNormals <0.25){
        //beaucoup d angle
        qDebug() << "Surement une regle";
    }
    else if(verticePlat > verticeSloper && verticePlat > verticeNiPlatNiSloper){
        qDebug() << "Surement une prise plate";
        //shape plan  bcp de point avec normal dans le sens de la normal du mur
    } else if (verticeSloper > verticeNiPlatNiSloper){
        qDebug() << "Surement un sloper";
        //shape plan  bcp de point avec normal incliner par rapport a la normal au mur
    } else {
        qDebug() << "Ni une prise plate, ni un sloper"   ;
        //pas de shape avec bcp de point
    }
}


/*
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

    ////////////////////tentative pour enlver du mur
    std::vector<Point_3> prise_face_points;
    QVector<QVector3D> vertices_b;
    QVector<QVector3D> colors_b;
    QVector<QVector3D> normals_b;
    for(int i= 0; i <vertices.size(); i++) {
       double distance =(a * cgal_points[i].x() + b * cgal_points[i].y() + c * cgal_points[i].z() + d)/std::sqrt(a*a+b*b+c*c);
        (if distance >= 0){
            vertices_b.push_back(vertices[i]);
            colors_b.push_back(colors[i]);
            normals_b.push_back(normals[i]);
    }

    qDebug() << vertices_b.size();
    vertices=vertices_b;colors=colors_b;normals=normals_b;




*/

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

void NuageDePoint::buildMesh() {
    qDebug()<<"Starting building mesh ...";

    qDebug()<<"Pair building ...";
    std::vector<Pwn> points;
    for(int i=0;i<vertices.size();i++) {
        points.push_back(std::make_pair(Point3D(vertices[i].x(),vertices[i].y(), vertices[i].z()), Vector3D(normals[i].x(), normals[i].y(), normals[i].z())));
    }
    qDebug()<<"Pair built !";

    Polyhedron output_mesh;
    qDebug()<<"Average spacing computing ...";
    double average_spacing = CGAL::compute_average_spacing<CGAL::Sequential_tag>
        (points, 6, CGAL::parameters::point_map(CGAL::First_of_pair_property_map<Pwn>()));
    qDebug()<<"Average spacing computed !";
    qDebug()<<"Surface reconstruction ...";
    if (CGAL::poisson_surface_reconstruction_delaunay
        (points.begin(), points.end(),
         CGAL::First_of_pair_property_map<Pwn>(),
         CGAL::Second_of_pair_property_map<Pwn>(),
         output_mesh, average_spacing))
    {
        qDebug()<<"Surface reconstruction done !";
        qDebug()<<"Mesh writing in off ...";
        std::ofstream out("data/test.off");
        out << output_mesh;
        qDebug()<<"Mesh written !";
    }
    else
        qDebug()<<"Failed to build mesh !";
        return;
    qDebug()<<"Mesh built !";
    return;
}
