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

using namespace ImGui;
void HORizImGui::ShowIcon(const char* name, const Vec2 size) {
    auto& texture = IconManager::Instance().map[name];
    auto s = ImVec2(size.x, size.y);
    ImGui::Image(texture.id, s);
}

void HORizImGui::IconText(const char* icon_name, const char* text, ...) {
    ShowIcon(icon_name, Vec2(18.f, 18.f));
    SameLine();

    va_list args;
    va_start(args, text);
    TextV(text, args);
    va_end(args);
}

void HORizImGui::SpacingN(i32 n) {
    for (int i = 0; i < n; ++i) {
        Spacing();
    }
}

bool ImGui::DragFloat3_ColorBorder(
    const char* label,
    float v[3],
    float speed,
    float inputWidth) {
    bool changed = false;

    PushID(label);

    BeginGroup();

    static const ImU32 LOCAL_COLORS[3] = {
        IM_COL32(200, 100, 100, 255),   // X - soft red
        IM_COL32(100, 200, 100, 255),   // Y - soft green
        IM_COL32(100, 100, 200, 255)    // Z - soft blue
    };

    for (int i = 0; i < 3; i++) {
        PushID(i);

        // 每个 XYZ 日内联显示
        SetNextItemWidth(inputWidth);

        // 正常输入 + 拖拽
        if (DragFloat("##drag", &v[i], speed, 0.0f, 0.0f, "%.3f"))
            changed = true;

        // 获取矩形来画左边彩色边框
        ImVec2 min = GetItemRectMin();
        ImVec2 max = GetItemRectMax();

        ImDrawList* draw = GetWindowDrawList();
        draw->AddLine(
            ImVec2(min.x, min.y),
            ImVec2(min.x, max.y),
            LOCAL_COLORS[i],
            3.0f // 左边框粗细
        );

        SameLine();
        PopID();
    }

    SameLine();
    TextUnformatted(label);

    EndGroup();
    PopID();

    return changed;
}