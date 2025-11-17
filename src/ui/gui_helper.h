#pragma once

#include "../icon.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "raylib.h"
#include "../raylib_types.h"
#include "baleine_type/primitive.h"
#include "baleine_type/functional.h"

bool ManipulateMatrix(Matrix& matrix,
                      Camera& camera,
                      ImGuizmo::OPERATION operation,
                      ImGuizmo::MODE mode);

bool ManipulateFloat16(baleine::f32 matrix[16],
                       Camera& camera,
                       ImGuizmo::OPERATION operation,
                       ImGuizmo::MODE mode);

bool ManipulateTransform(Transform& transform,
                         Camera& camera,
                         ImGuizmo::OPERATION operation,
                         ImGuizmo::MODE mode);

inline void ImWindow(const char* name, baleine::Fn<void()> function) {
    ImGui::Begin(name);
    function();
    ImGui::End();
}

namespace ImGui {
bool DragFloat3_ColorBorder(
    const char* label,
    float v[3],
    float speed = 0.1f,
    float inputWidth = 80.0f);

inline void ShowVec3(const Vec3& vec, const char* label) {
    LabelText(label,
              "%.2f, %.2f, %.2f",
              vec.x,
              vec.y,
              vec.z);
}

inline void ShowTransform(const raylib::Transform& transform, const char* label) {
    auto translation = transform.translation;
    auto rotationEuler = transform.rotation.ToEuler();
    auto scale = transform.scale;

    Text(label);
    ShowVec3(translation, "Translation");
    ShowVec3(rotationEuler, "Rotation");
    ShowVec3(scale, "Scale");
}

inline void ModifyTransform(raylib::Transform& transform) {
    auto rotationEuler = static_cast<Vec3>(transform.rotation.ToEuler()) * RAD2DEG;

    DragFloat3_ColorBorder("Translation", (float*)&transform.translation);
    DragFloat3_ColorBorder("Rotation", (float*)&rotationEuler, 1.f);
    DragFloat3_ColorBorder("Scale", (float*)&transform.scale);

    transform.rotation = Quat::FromEuler(rotationEuler * DEG2RAD);
}
}

namespace HORizImGui {

void ShowIcon(const char* name, Vec2 size);
void IconText(const char* icon_name, const char* text, ...);

void SpacingN(i32 n);
}