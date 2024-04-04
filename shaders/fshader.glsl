#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

varying vec3 vColor;
varying vec3 vNormal;
varying vec3 vPosition;

void main()
{
    vec3 lightPos= vec3(0.0,0.0,-25.0);
    vec3 ambientColor=vec3(1.0,1.0,1.0); // Couleur ambiante

    // Calcul du vecteur de lumière
    vec3 lightDir = normalize(lightPos - vPosition);

    // Calcul de l'intensité de la lumière diffuse
    float diffuse = max(dot(vNormal, lightDir), 0.0);

    // Calcul de l'intensité de la lumière ambiante
    float ambient = 0.1; // Réglage de l'intensité ambiante

    // Couleur diffuse du matériau
    vec3 diffuseColor = vColor;

    // Couleur de la lumière
    vec3 lightColor = vec3(1.0); // Lumière blanche

    // Calcul final de la couleur en utilisant le modèle de Phong
    vec3 finalColor = (ambient * ambientColor + diffuse * lightColor * diffuseColor);

    // Assigner la couleur finale au fragment
    gl_FragColor = vec4(finalColor, 1.0);
}
