#include "gui_helper.h"
#include "raymath.h"
#include "rcamera.h"

using namespace baleine;

bool ManipulateMatrix(Matrix& matrix,
                      Camera& camera,
                      ImGuizmo::OPERATION operation,
                      ImGuizmo::MODE mode) {
    float m[16];
    for (int i = 0; i < 16; i++)
        m[i] = reinterpret_cast<float*>(&matrix)[i];

    bool changed = ManipulateFloat16(m, camera, operation, mode);

    if (changed) {
        matrix = MatrixTranspose(*reinterpret_cast<Matrix*>(m));
    }

    return changed;
}


bool ManipulateFloat16(f32 matrix[16],
                       Camera& camera,
                       ImGuizmo::OPERATION operation,
                       ImGuizmo::MODE mode) {
    Matrix view = MatrixTranspose(GetCameraMatrix(camera));
    Matrix proj = MatrixTranspose(GetCameraProjectionMatrix(
        &camera,
        static_cast<float>(GetScreenWidth()) / static_cast<float>(
            GetScreenHeight())));
    return ImGuizmo::Manipulate(reinterpret_cast<float*>(&view),
                                reinterpret_cast<float*>(&proj),
                                operation,
                                mode,
                                matrix);
}

bool ManipulateTransform(Transform& transform,
                         Camera& camera,
                         ImGuizmo::OPERATION operation,
                         ImGuizmo::MODE mode) {
    f32 matrix[16];
    auto euler = QuaternionToEuler(transform.rotation);
    euler = Vector3Scale(euler, RAD2DEG);

    ImGuizmo::RecomposeMatrixFromComponents((f32*)&transform.translation,
                                            (f32*)&euler,
                                            (f32*)&transform.scale,
                                            matrix);

    if (ManipulateFloat16(matrix, camera, operation, mode)) {
        ImGuizmo::DecomposeMatrixToComponents(matrix,
                                              (f32*)&transform.translation,
                                              (f32*)&euler,
                                              (f32*)&transform.scale);
        euler = Vector3Scale(euler, DEG2RAD);
        transform.rotation = QuaternionFromEuler(euler.x, euler.y, euler.z);

        return true;
    }

    return false;
}