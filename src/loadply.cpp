#include "../headers/loadply.h"

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

// Constructeur et destructeur déjà déclarés dans le fichier d'en-tête
// loadPly::loadPly() {
//     // Initialisation du constructeur
// }

// loadPly::~loadPly() {
//     // Nettoyage du destructeur
// }

void loadPLY::loadPlyFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "vertex") {
            PlyVertex vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        } else if (token == "face") {
            PlyFace face;
            int numVertices;
            iss >> numVertices >> face.vertexIndex1 >> face.vertexIndex2 >> face.vertexIndex3;
            // Gérer les faces avec plus de 3 vertices si nécessaire
            faces.push_back(face);
        }
    }
    // Créer et lier le tampon de sommets
    arrayBuf.create();
    arrayBuf.bind();
    arrayBuf.allocate(vertices.data(), vertices.size() * sizeof(PlyVertex));

    // Créer et lier le tampon d'indices
    indexBuf.create();
    indexBuf.bind();
    indexBuf.allocate(faces.data(), faces.size() * sizeof(PlyFace));

    // Vous devez ensuite transférer les données des vertices et des faces
    // aux tampons OpenGL appropriés (arrayBuf et indexBuf).
}

void loadPLY::drawPlyGeometry(QOpenGLShaderProgram *program) {
    // Assurez-vous que les tampons sont liés
    arrayBuf.bind();
    indexBuf.bind();

    // Configurer les attributs pour les sommets
    int vertexLocation = program->attributeLocation("vertex");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(PlyVertex));

    // Dessiner les faces
    glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0);

    // Désactiver les attributs
    program->disableAttributeArray(vertexLocation);
}
