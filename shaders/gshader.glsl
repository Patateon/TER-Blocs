#version 330 core // Utilise le profil de base GLSL version 3.30

layout (points) in;
layout (triangle_strip, max_vertices = 32) out; // Nombre maximum de sommets pour le disque

uniform vec3 cameraPosition; // Position de la caméra

// Variables d'entrée depuis le shader de sommets
in vec3 vColor[];
in vec3 vNormal[];
in vec3 vPosition[];

// Variables de sortie vers le shader de fragments
out vec3 gColor;
out vec3 gNormal;
out vec3 gPosition;

// Définir le nombre de segments pour le disque
const int NUM_SEGMENTS = 3;
const float DISK_RADIUS = 0.01; // Rayon du disque

void main() {
    vec3 center = vPosition[0]; // Centre du disque
    vec3 normal = normalize(vNormal[0]); // Normalisée pour garantir une longueur de 1

    // Calculer la distance entre la caméra et le point
    float distanceToCamera = length(cameraPosition - center);

    if (distanceToCamera < 8) {

        // Calculer les vecteurs tangents au plan du disque
        vec3 tangent1 = normalize(cross(normal, vec3(0.0, 0.0, 1.0)));
        vec3 tangent2 = normalize(cross(normal, tangent1));

        for (int i = 0; i <= NUM_SEGMENTS; ++i) {
            // Calculer l'angle pour chaque segment
            float angle = radians(float(i) / float(NUM_SEGMENTS) * 360.0);

            // Calculer la position autour du centre en utilisant les coordonnées polaires
            // et en tenant compte de la normale
            vec3 offset = tangent1 * cos(angle) + tangent2 * sin(angle);

            // Calculer la nouvelle position en ajoutant l'offset au centre
            vec3 newPosition = center + offset * DISK_RADIUS;

            // Transmettre la couleur et la normale inchangées
            gColor = vColor[0];
            gNormal = vNormal[0];
            gPosition = newPosition;

            // Assigner la nouvelle position à gl_Position
            gl_Position = gl_in[0].gl_Position + vec4(offset, 0.0) * DISK_RADIUS;

            // Émettre le vertex modifié
            EmitVertex();
        }

        // Terminer la primitive avec les vertices supplémentaires
        EndPrimitive();
    }
    else{
        // Si la distance à la caméra est supérieure à 0.05, afficher uniquement le point sans le disque
        /*gColor = vColor[0];
        gNormal = vNormal[0];
        gPosition = center;
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();
        EndPrimitive();*/
        // Calculer les vecteurs tangents au plan du disque
        vec3 tangent1 = normalize(cross(normal, vec3(0.0, 0.0, 1.0)));
        vec3 tangent2 = normalize(cross(normal, tangent1));

        for (int i = 0; i <= 3; ++i) {
            // Calculer l'angle pour chaque segment
            float angle = radians(float(i) / 3.0 * 360.0);

            // Calculer la position autour du centre en utilisant les coordonnées polaires
            // et en tenant compte de la normale
            vec3 offset = tangent1 * cos(angle) + tangent2 * sin(angle);

            // Calculer la nouvelle position en ajoutant l'offset au centre
            vec3 newPosition = center + offset * DISK_RADIUS;

            // Transmettre la couleur et la normale inchangées
            gColor = vColor[0];
            gNormal = vNormal[0];
            gPosition = newPosition;
            if(i%3==0){
                gl_Position = gl_in[0].gl_Position;

                // Émettre le vertex modifié
                EmitVertex();
            }

            // Assigner la nouvelle position à gl_Position
            gl_Position = gl_in[0].gl_Position + vec4(offset, 0.0) * DISK_RADIUS;

            // Émettre le vertex modifié
            EmitVertex();

        }

        // Terminer la primitive avec les vertices supplémentaires
        EndPrimitive();
    }
}
