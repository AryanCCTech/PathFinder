#version 330 core
in vec3 fragNormal;
in vec3 fragPosition;
in float fragHighlightFlag; // Receive flag

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec4 lineColor;  // Add line color uniform

out vec4 color;

void main() {
    if (lineColor.a > 0.0) {  // Check if we're drawing lines
        color = lineColor;
    } else {
        // Original shading calculations
        vec3 norm = normalize(fragNormal);
        vec3 lightDir = normalize(lightPos - fragPosition);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 viewDir = normalize(viewPos - fragPosition);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

        vec3 ambient = 0.1 * vec3(1.0, 0.5, 0.31);
        vec3 diffuse = diff * vec3(1.0, 0.5, 0.31);
        vec3 specular = spec * vec3(1.0, 1.0, 1.0);

        color = vec4(ambient + diffuse + specular, 1.0);
    }
}
