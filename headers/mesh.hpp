/* #ifndef MESH_H
#define MESH_H

#include <string.h>
#include <vector>

// Opengl QT
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include <glm/glm.hpp>

class Mesh
{
private:
    // Vertices buffers
    QOpenGLBuffer m_posVbo;
    QOpenGLBuffer m_normalVbo;
    QOpenGLBuffer m_texCoordVbo;
    QOpenGLBuffer m_ebo;

    // Vertices attributs arrays
	std::vector<glm::vec3> m_vertexPositions;
	std::vector<glm::vec3> m_vertexNormals;
	std::vector<glm::vec2> m_vertexTexCoords;
    std::vector<unsigned short> m_triangleIndices;

public:
    // Constructor
    Mesh(){}
    Mesh(
        std::vector<glm::vec3> vertices,
        std::vector<unsigned short> indices
    );
    // Destructor
    ~Mesh();

    //// Getter par référence des attributs de notre mesh
    // Renvoie le pointeur et la valeur (Donc pas besoin de Setter)
    std::vector<glm::vec3> & vertexPosition() {return m_vertexPositions;}
    std::vector<glm::vec3> & vertexNormals() {return m_vertexNormals;}
    std::vector<glm::vec2> & vertexTexCoords() {return m_vertexTexCoords;}
    std::vector<unsigned short> & triangleIndices() {return m_triangleIndices;}

    // Recompute normals and texture coordinates from vertex
    void recomputeNormals();
    void recomputeTextureCoordinates();

    //// Fill buffers, renders with thoses buffers, clear them all

    // Init before de mainloop
    void init();

    // Render each loop
    void render();

    // Clear after the mainloop
    void clear();
};

#endif
 */