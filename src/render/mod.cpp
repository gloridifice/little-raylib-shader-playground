#include "mod.h"

UniversalShaderContext::UniversalShaderContext(String&& vsFilename,
                                               String&& fsFilename) :
    vsFilename(std::move(vsFilename)),
    fsFilename(std::move(fsFilename)) {}

void UniversalShaderContext::LoadAndInit() {
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

void UniversalShaderContext::UpdateDirectionalLight(
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