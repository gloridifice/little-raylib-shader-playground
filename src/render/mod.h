#pragma once

#include "baleine_type/string.h"
#include "raylib.h"
#include "baleine_type/primitive.h"

#define DEFINE_SHADER_UPDATE_FUNC(NAME, LOC_NAME, TYPE, UNIFORM_TYPE) \
void Update##NAME(const TYPE& value) const { \
SetShaderValue(raylibShader, LOC_NAME, &value, UNIFORM_TYPE); \
}

using namespace baleine;

struct DirectionalLight {
    Vector3 direction;
    Vector3 color;
    f32 intensity;
};

class UniversalShaderContext {
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
    virtual ~UniversalShaderContext() = default;
    explicit UniversalShaderContext(String&& vsFilename, String&& fsFilename);

    virtual void LoadAndInit();

    void UpdateDirectionalLight(const DirectionalLight& light) const;
    DEFINE_SHADER_UPDATE_FUNC(ViewWorldPos,
                              viewWorldPosLoc,
                              Vector3,
                              SHADER_UNIFORM_VEC3);

    void BeginMode() const {
        BeginShaderMode(raylibShader);
    }

    void EndMode() {
        EndShaderMode();
    }
};

class BlinnPhongShaderContext: public UniversalShaderContext {
private:
    // BEGIN Lazy Load
    i32 specularPowerLoc = 0;
    i32 diffuseCoefficientLoc = 0;
    i32 specularCoefficientLoc = 0;
    i32 ambientLoc = 0;

    // END Lazy Load
public:
    BlinnPhongShaderContext(String&& vsFilename, String&& fsFilename) :
        UniversalShaderContext(std::move(vsFilename), std::move(fsFilename)) {}

    virtual void LoadAndInit() override {
        UniversalShaderContext::LoadAndInit();

        specularPowerLoc = GetShaderLocation(raylibShader, "specularPower");
        diffuseCoefficientLoc = GetShaderLocation(raylibShader,
                                                  "diffuseCoefficient");
        specularCoefficientLoc = GetShaderLocation(raylibShader,
                                                  "specularCoefficient");
        ambientLoc = GetShaderLocation(raylibShader, "ambient");
    }

    DEFINE_SHADER_UPDATE_FUNC(Ambient, ambientLoc, Vector4, SHADER_UNIFORM_VEC4);

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