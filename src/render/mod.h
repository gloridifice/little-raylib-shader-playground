#pragma once

#include "light.h"
#include "baleine_type/string.h"
#include "raylib.h"
#include "../raylib_types.h"
#include "../ecs/mod.h"
#include "baleine_type/primitive.h"

#define DEFINE_SHADER_UPDATE_FUNC(NAME, LOC_NAME, TYPE, UNIFORM_TYPE) \
void Update##NAME(const TYPE& value) const { \
SetShaderValue(raylibShader, LOC_NAME, &value, UNIFORM_TYPE); \
}

using namespace baleine;

namespace HORiz {

#pragma region Classes

class ShaderContext {
protected:
    String vsFilename;
    String fsFilename;

public:
    // BEGIN Lazy Load ======
    Shader raylibShader{};

private:
    // Locations ----
    i32 viewWorldPosLoc = 0,
        directionalLightColorLoc = 0,
        directionalLightDirectionLoc = 0,
        directionalLightIntensityLoc = 0;

    // END Lazy Load =====

public:
    virtual ~ShaderContext() = default;
    explicit ShaderContext(String&& vsFilename, String&& fsFilename);

    virtual void LoadAndInit();

    void UpdateDirectionalLight(const DirectionalLight& light) const;
    DEFINE_SHADER_UPDATE_FUNC(ViewWorldPos,
                              viewWorldPosLoc,
                              Vec3,
                              SHADER_UNIFORM_VEC3);

    void BeginMode() const {
        BeginShaderMode(raylibShader);
    }

    void EndMode() {
        EndShaderMode();
    }
};

class BlinnPhongShaderContext: public ShaderContext {
private:
    // BEGIN Lazy Load
    i32 specularPowerLoc = 0;
    i32 diffuseCoefficientLoc = 0;
    i32 specularCoefficientLoc = 0;
    i32 ambientLoc = 0;

    // END Lazy Load
public:
    BlinnPhongShaderContext(String&& vsFilename, String&& fsFilename) :
        ShaderContext(std::move(vsFilename), std::move(fsFilename)) {}

    virtual void LoadAndInit() override;

    DEFINE_SHADER_UPDATE_FUNC(Ambient, ambientLoc, Vec4, SHADER_UNIFORM_VEC4);

    DEFINE_SHADER_UPDATE_FUNC(SpecularPower,
                              specularPowerLoc,
                              f32,
                              SHADER_UNIFORM_FLOAT);
    DEFINE_SHADER_UPDATE_FUNC(DiffuseCoefficient,
                              diffuseCoefficientLoc,
                              f32,
                              SHADER_UNIFORM_FLOAT);
    DEFINE_SHADER_UPDATE_FUNC(SpecularCoefficient,
                              specularCoefficientLoc,
                              f32,
                              SHADER_UNIFORM_FLOAT);
};

#pragma endregion

#pragma region Systems

void SysDrawModels(World& world);

#pragma endregion

}