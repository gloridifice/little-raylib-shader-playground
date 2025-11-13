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
uniform float specularPower;
uniform float diffuseCoefficient;
uniform float specularCoefficient;
uniform vec4 ambient;

vec3 blinnPhong(vec3 normal, vec3 light, vec3 view, float specularPow,
    vec3 albedo, vec3 ambient
) {
    vec3 halfway = normalize(light + view);
    float diffuse = max(dot(normal, light), 0.0);
    float specular = pow(max(dot(halfway, normal), 0.0), specularPow);

    return albedo * (ambient + (specular * specularCoefficient + diffuse * diffuseCoefficient) * directionalLight.intensity * directionalLight.color);
}

void main() {
    vec3 albedo = texture(texture0, fragTexCoord).xyz;

    vec3 normal = normalize(fragNormal);
    vec3 light = normalize(-directionalLight.direction);
    vec3 view = normalize(viewWorldPos - fragWorldPos.xyz);
    vec3 halfway = normalize(light + view);

    finalColor = vec4(blinnPhong(normal, light, view, specularPower, albedo, ambient.xyz), 1.0);
}