#include "post_processing.h"


namespace HORiz {
void PostProcessingManager::Resize(baleine::i32 width, baleine::i32 height) {
    UnloadRenderTexture(innerTarget);
    innerTarget = LoadRenderTexture(width, height);
}

void PostProcessingManager::Draw(RenderTexture2D renderTarget) const {
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
}