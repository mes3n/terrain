#version 460 core
out vec4 FragColor;

in vec3 fragNormal;
in vec3 fragPos;

uniform vec3 cameraPos;
uniform vec3 lightOrigin = vec3(50.0, 150.0, 50.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);


const vec3 objectColor = vec3(0.2, 0.2, 0.2);

// ambient color
const float ambientStrength = 0.2;

vec3 ambient = ambientStrength * lightColor;

// diffused color
vec3 normal = normalize(fragNormal);
vec3 lightDirection = normalize(lightOrigin - fragPos);

float diffuseFactor = max(dot(normal, lightDirection), 0.0);
vec3 diffuse = diffuseFactor * lightColor;


// specular color
const float specularStrenght = 1.0;
const float materialShininess = 256;

vec3 viewDirection = normalize(cameraPos - fragPos);
vec3 reflectionDirection = reflect(-lightDirection, normal);

float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), materialShininess);
vec3 specular = specularStrenght * spec * lightColor;

void main () 
{
    FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
    // FragColor = vec4(fragNormal, 1.0);
}
