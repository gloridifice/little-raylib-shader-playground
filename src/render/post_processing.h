#pragma once

#include "raylib-cpp.hpp"
#include "baleine_type/primitive.h"
#include "baleine_type/vector.h"

using namespace baleine;

namespace HORiz {

class PostProcessingManager {
private:
    Vec<Shader> shaders;
    RenderTexture2D innerTarget;

public:
    ~PostProcessingManager() {
        UnloadRenderTexture(innerTarget);
        for (const auto s : shaders) {
            UnloadShader(s);
        }
    }

    explicit PostProcessingManager(baleine::i32 width, baleine::i32 height) {
        innerTarget = LoadRenderTexture(width, height);
    }

    void Resize(baleine::i32 width, baleine::i32 height);

    void PushShader(Shader shader) {
        shaders.push_back(shader);
    }

    void Draw(RenderTexture2D renderTarget) const;
};
}