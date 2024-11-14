#version 330 core
in vec3 fragNormal;
in vec3 fragPosition;
in float fragHighlightFlag; // Receive flag

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 riverPathColor; // Color for river path

out vec4 color;

void main() {
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPosition);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 ambient = 0.1 * vec3(1.0, 1.0, 1.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
    vec3 specular = spec * vec3(1.0, 1.0, 1.0);

    vec3 baseColor = vec3(1.0, 0.5, 0.31); // Default color
    vec3 highlightedColor = riverPathColor; // River path color

    // Use highlightedColor when flag is set
    color = vec4(fragHighlightFlag * highlightedColor + (1.0 - fragHighlightFlag) * baseColor, 1.0);
}
