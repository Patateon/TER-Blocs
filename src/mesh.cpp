#include "../headers/mesh.h"

Mesh::Mesh() {
}

Mesh::Mesh(QVector<QVector3D> vertices, std::vector<std::vector<unsigned int>> triangles) {
    m_vertices = vertices;
    m_triangles = triangles;
}

Mesh::~Mesh(){
    clear();
}

void Mesh::init() {
    //// Allocation des buffers de le GPU

    qDebug()<<"Allocation des buffers du maillage ...";

    // Vertex buffer
    vertexBuf.create();
    vertexBuf.bind();
    vertexBuf.allocate(m_vertices.data(), m_vertices.size());

    // Color buffer
    colorBuf.create();
    colorBuf.bind();
    colorBuf.allocate(m_colors.data(), m_colors.size());

    // Normal buffer
    normalBuf.create();
    normalBuf.bind();
    normalBuf.allocate(m_normals.data(), m_normals.size());

    // Triangle indices buffer
    triangleBuf.create();
    triangleBuf.bind();
    triangleBuf.allocate(m_triangles.data(), m_triangles.size() * sizeof(std::vector<int>));
}

void Mesh::draw(QOpenGLShaderProgram *program) {
    //// Rendu du maillage pour chaque buffers

    qDebug()<<"Rendu du maillage ...";
    qDebug()<<"Number Of Vertices:"<<m_vertices.size();
    qDebug()<<"Number Of Colors:"<<m_colors.size();
    qDebug()<<"Number Of Normals:"<<m_normals.size();
    qDebug()<<"Number Of Triangles:"<<m_triangles.size();

    vertexBuf.bind();
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
    glDrawArrays(GL_POINTS, 0, m_vertices.size());

    // glDrawElements(GL_TRIANGLES, m_triangles.size(), GL_UNSIGNED_INT, 0);

    // Désactiver les attributs
    program->disableAttributeArray(vertexLocation);
    program->disableAttributeArray(colorLocation);
    program->disableAttributeArray(normalLocation);
}

void Mesh::clear() {
    //// Suppression propre du maillage en vidant chaque buffers

    m_vertices.clear();
    m_colors.clear();
    m_normals.clear();
    m_triangles.clear();

    vertexBuf.destroy();
    colorBuf.destroy();
    normalBuf.destroy();
    triangleBuf.destroy();
}

void Mesh::loadOffFile(const std::string& filename, Nesh *mesh) {
    std::ifstream file(filename);

    std::string line;
    if (!file.is_open()) {
        qDebug() << "Failed to open .off file";
        return;
    }

    //Dans off sommets , indices triangles et couleur triangle
    int nbVertices = 0;
    int nbFaces = 0;
    QVector<QVector3D> vertices;
    QVector<QVector3D> colors;
    std::vector<std::vector<unsigned int>>  triangles;
    QVector<QVector3D> normals;
    bool isReadingVertices = false;
    int count_vertex = 0;
    int count_face = 0;

    //Lecture
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string keyword;
        if (!isReadingVertices)
            iss >> keyword;

        if (keyword == "OFF") {
            isReadingVertices = true;
        } else if (isReadingVertices && nbVertices == 0) {
            //Lecture du nombre de sommets et triangles
            iss >> nbVertices;
            iss >> nbFaces;
            vertices.reserve(nbVertices);
            normals.reserve(nbFaces);
            colors.reserve(nbFaces);
            triangles.reserve(nbFaces);
        } else if (isReadingVertices && count_vertex < nbVertices) {
            float x, y, z;
            iss >> x >> y >> z;
            vertices.push_back(QVector3D(x, y, z));
            count_vertex++;
        } else if (isReadingVertices && count_vertex == nbVertices && count_face < nbFaces) {
            int vertexCount;
            iss >> vertexCount;
            if (vertexCount == 3) {
                int v1, v2, v3;
                iss >> v1 >> v2 >> v3;
                std::vector<unsigned int> triangleIndex = {v1, v2, v3};
                triangles.push_back(triangleIndex);
                QVector3D v1v2 = vertices[v2] - vertices[v1];
                QVector3D v1v3 = vertices[v3] - vertices[v1];
                QVector3D normal = QVector3D::crossProduct(v1v2, v1v3).normalized();
                normals.push_back(normal);
                float r = 1.0, g = 1.0, b = 1.0;
                if (iss >> r >> g >> b) {
                    colors.push_back(QVector3D(r / 255.0f, g / 255.0f, b / 255.0f));
                } else {
                    colors.push_back(QVector3D(1.0f, 1.0f, 1.0f));
                }
                count_face++;
            }
        }
    }

    file.close();
    m_vertices=vertices;
    m_colors=colors;
    m_triangles=triangles;
    m_normals=normals;
    qDebug() << "File loaded successfully: " << QString::fromStdString(filename);

}

