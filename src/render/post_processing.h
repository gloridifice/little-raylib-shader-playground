#pragma once

#include "raylib-cpp.hpp"
#include "baleine_type/primitive.h"
#include "baleine_type/vector.h"


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

    void Resize(baleine::i32 width, baleine::i32 height) {
        UnloadRenderTexture(innerTarget);
        innerTarget = LoadRenderTexture(width, height);
    }

    void PushShader(Shader shader) {
        shaders.push_back(shader);
    }

    void Draw(RenderTexture2D renderTarget) {
        baleine::i32 currentIndex = 0;
        for (auto s : shaders) {
            auto source = currentIndex % 2 == 1 ? innerTarget : renderTarget;
            auto target = currentIndex % 2 == 0 ? innerTarget : renderTarget;
            BeginTextureMode(target);
            {
                BeginShaderMode(s);
                {
                    DrawTextureRec(source.texture,
                                   (Rectangle){0, 0, (float)source.texture.width,
                                               (float)-source.texture.height},
                                   (Vector2){0, 0},
                                   WHITE);
                }
                EndShaderMode();
            }
            EndTextureMode();

            currentIndex += 1;
        }

        if (currentIndex % 2 == 1) {
            BeginTextureMode(renderTarget);
            {
                DrawTextureRec(innerTarget.texture,
                               (Rectangle){0, 0, (float)innerTarget.texture.width,
                                           (float)-innerTarget.texture.height},
                               (Vector2){0, 0},
                               WHITE);
            }
            EndTextureMode();
        }
    }
};