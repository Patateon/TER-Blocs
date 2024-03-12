#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
varying vec3 vColor;
varying vec3 vNormal;
uniform sampler2D texture;



//! [0]
void main()
{
    // Set fragment color from texture
    //gl_FragColor = texture2D(texture, v_texcoord);
     gl_FragColor = vec4(vColor, 1.0);
}
//! [0]

