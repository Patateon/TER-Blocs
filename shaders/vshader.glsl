#version 330 core // Use GLSL version 3.30 core profile

// Input attributes
in vec3 vertex;
in vec3 color;
in vec3 normal;

// Output varying variables to geometry shader
out vec3 vColor;
out vec3 vNormal;
out vec3 vPosition;

// Uniform matrix
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main() {
    // Pass vertex attributes to geometry shader
    vColor = color;
    vNormal = normal;
    vPosition = vertex;

    // Compute MVP matrix
    mat4 mvp_matrix = projection * view * model;

    // Transform vertex position
    gl_Position = mvp_matrix * vec4(vertex, 1.0);
}
