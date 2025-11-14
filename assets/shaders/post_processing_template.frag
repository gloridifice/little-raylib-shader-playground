#version 330

in vec2 fragTexCoord;

out vec4 fragColor;

uniform sampler2D texture0; 
uniform vec4 colDiffuse;

void main() {
    vec3 color = texture(texture0, fragTexCoord).xyz;
    color = vec3(1.0) - color;

    fragColor = vec4(color, 1.0);
}