#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;

attribute vec3 vertex;
attribute vec3 color;
attribute vec3 normal;
varying vec3 vColor;
varying vec3 vNormal;
varying vec3 vPosition;

void main() {
    gl_Position = mvp_matrix * vec4(vertex, 1.0);
    vPosition= vec3(gl_Position);
    vColor = color;
    vNormal = normal;
}
//! [0]
