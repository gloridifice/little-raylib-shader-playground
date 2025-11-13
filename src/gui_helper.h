#pragma once

#include "imgui.h"
#include "ImGuizmo.h"
#include "raylib.h"
#include "baleine_type/primitive.h"
#include "baleine_type/functional.h"
#include "baleine_type/string.h"

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

inline void ImGuiWindowRegion(const char* name, baleine::Fn<void()> function) {
    ImGui::Begin(name);
    function();
    ImGui::End();
}