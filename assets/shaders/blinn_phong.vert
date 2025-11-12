#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec4 vertexColor;
in vec3 vertexNormal;

out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 fragNormal;
out vec4 fragHCPos;
out vec4 fragWorldPos;

uniform mat4 mvp;
uniform mat4 matModel;

void main() {
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;
    fragNormal = vertexNormal;
    vec4 ret = mvp * vec4(vertexPosition, 1.0);
    fragHCPos = ret;
    fragWorldPos = matModel * vec4(vertexPosition, 1.0);

    gl_Position = ret;
}