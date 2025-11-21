#include "mod.h"

namespace HORiz {
ShaderContext::ShaderContext(String&& vsFilename,
                             String&& fsFilename) :
    vsFilename(std::move(vsFilename)),
    fsFilename(std::move(fsFilename)) {}

void ShaderContext::LoadAndInit() {
    raylibShader = LoadShader(vsFilename.c_str(), fsFilename.c_str());
    viewWorldPosLoc = GetShaderLocation(raylibShader, "viewWorldPos");
    directionalLightColorLoc = GetShaderLocation(
        raylibShader,
        "directionalLight.color");
    directionalLightDirectionLoc = GetShaderLocation(
        raylibShader,
        "directionalLight.direction");
    directionalLightIntensityLoc = GetShaderLocation(
        raylibShader,
        "directionalLight.intensity");
}

void ShaderContext::UpdateDirectionalLight(
    const DirectionalLight& light) const {
    SetShaderValue(raylibShader,
                   directionalLightIntensityLoc,
                   &light.intensity,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(raylibShader,
                   directionalLightColorLoc,
                   &light.color,
                   SHADER_UNIFORM_VEC3);
    SetShaderValue(raylibShader,
                   directionalLightDirectionLoc,
                   &light.direction,
                   SHADER_UNIFORM_VEC3);
}

void BlinnPhongShaderContext::LoadAndInit() {
    ShaderContext::LoadAndInit();

    specularPowerLoc = GetShaderLocation(raylibShader, "specularPower");
    diffuseCoefficientLoc = GetShaderLocation(raylibShader,
                                              "diffuseCoefficient");
    specularCoefficientLoc = GetShaderLocation(raylibShader,
                                               "specularCoefficient");
    ambientLoc = GetShaderLocation(raylibShader, "ambient");
}

void SysDrawModels(World& world) {
    auto models = world.view<raylib::Model, raylib::Transform>();
    models.each([](const raylib::Model& model, const raylib::Transform& transform) {
        auto [axis, angle] = transform.rotation.ToAxisAngle();
        model.Draw(transform.translation, axis, angle, transform.scale);
    });
}
}