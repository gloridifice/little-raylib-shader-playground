#version 330

struct DirectionalLight {
    vec3 color;
    vec3 direction;
    float intensity;
};

in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;
in vec4 fragHCPos;
in vec4 fragWorldPos;

out vec4 finalColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform DirectionalLight directionalLight;
uniform vec3 viewWorldPos;


void main() {
    vec4 color = vec4(vec3(0.0), 1.0);
    vec4 albedo = texture(texture0, fragTexCoord);
    vec3 ndcPos = fragHCPos.xyz / fragHCPos.w;

    vec3 light = normalize(-directionalLight.direction);
    vec3 eye = normalize(viewWorldPos - fragWorldPos.xyz);
    vec3 halfway = normalize(light + eye);
    float specular = pow(max(dot(halfway, normalize(fragNormal)), 0.0), 8.0);

    color = vec4(directionalLight.color * specular * 0.5 + 0.5 * albedo.xyz, 1.0);

//    finalColor = color * colDiffuse * fragColor;
    finalColor = color;
}