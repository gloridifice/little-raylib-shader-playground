#include "gui_helper.h"
#include "raymath.h"
#include "rcamera.h"

bool ManipulateMatrix(Matrix& matrix, Camera& camera, ImGuizmo::OPERATION operation, ImGuizmo::MODE mode) {
    float m[16];
    for (int i = 0; i < 16; i++)
        m[i] = reinterpret_cast<float*>(&matrix)[i];

    Matrix view = MatrixTranspose(GetCameraMatrix(camera));
    Matrix proj = MatrixTranspose(GetCameraProjectionMatrix(
        &camera,
        static_cast<float>(GetScreenWidth()) / static_cast<float>(GetScreenHeight())));
    bool changed = ImGuizmo::Manipulate(reinterpret_cast<float*>(&view),
                                        reinterpret_cast<float*>(&proj),
                                        operation,
                                        mode,
                                        m);

    if (changed) {
        matrix = MatrixTranspose(*reinterpret_cast<Matrix*>(m));
    }

    return changed;
}
