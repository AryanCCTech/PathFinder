 #version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in float highlightFlag; // New input for highlight flag

uniform mat4 modelView;
uniform mat4 projection;

out vec3 fragNormal;
out vec3 fragPosition;
out float fragHighlightFlag; // Pass flag to fragment shader

void main() {
    fragPosition = vec3(modelView * vec4(position, 1.0));
    fragNormal = mat3(transpose(inverse(modelView))) * normal;
    fragHighlightFlag = highlightFlag; // Pass to fragment shader
    gl_Position = projection * vec4(fragPosition, 1.0);
}
