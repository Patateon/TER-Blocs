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
