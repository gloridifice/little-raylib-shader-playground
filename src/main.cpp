#include <iostream>

#include "imgui.h"
#include "raylib.h"
#include "raymath.h"
#include "baleine_type/primitive.h"
#include "rlImGui.h"


#define GLSL_VERSION 330

using namespace baleine;

struct DirectionalLight {
    Vector3 direction;
    Vector3 color;
    f32 intensity;
};

void UpdateDirectionalLight(Shader shader, DirectionalLight& light) {
    SetShaderValue(shader, GetShaderLocation(shader, "directionalLight.intensity"), &light.intensity,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, GetShaderLocation(shader, "directionalLight.color"), &light.color,
                   SHADER_UNIFORM_VEC3);
    SetShaderValue(shader, GetShaderLocation(shader, "directionalLight.direction"), &light.direction,
                   SHADER_UNIFORM_VEC3);
}

void UpdateViewPos(Shader shader, Vector3& view_world_pos) {
    SetShaderValue(shader, GetShaderLocation(shader, "viewWorldPos"), &view_world_pos,
                   SHADER_UNIFORM_VEC3);
}

int main() {
    InitWindow(800, 450, "Fooo");

    // Setup Imgui
    rlImGuiSetup(true);

    Shader shader = LoadShader(TextFormat("assets/shaders/blinn_phong.vert"),
                               TextFormat("assets/shaders/blinn_phong.frag"));

    Camera camera = {0};
    camera.position = (Vector3){0.0f, 14.0f, 14.0f};
    camera.target = (Vector3){0.0f, 0.5f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    DirectionalLight directional_light{};
    directional_light.direction = (Vector3){-1.0f, -1.0f, 0.0f};
    directional_light.color = (Vector3){1.0f, 0.0f, 1.0f};
    directional_light.intensity = 1.0f;

    Model car = LoadModel("assets/models/old_car_new.glb");
    car.materials[0].shader = shader;
    car.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture("assets/models/old_car_d.png");

    UpdateDirectionalLight(shader, directional_light);

    GenTextureMipmaps(&car.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture);
    SetTextureFilter(car.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture, TEXTURE_FILTER_TRILINEAR);

    int value = 0;

    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_ORBITAL);
        UpdateViewPos(shader, camera.position);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera); // 3D BEGIN =============
        BeginShaderMode(shader);
        DrawModel(car, Vector3Zero(), .1f, WHITE);
        EndShaderMode();

        auto ray_pos = (Vector3){0.0, 25.0, 0.0};
        DrawLine3D(ray_pos,
                   Vector3Add(Vector3Scale(Vector3Normalize(directional_light.direction), 5.0f), ray_pos),
                   BLUE);
        DrawSphere(ray_pos, .7f, BLUE);

        EndMode3D(); // 3D END ~~~~~~~~~~~~

        DrawFPS(10, 10);

        rlImGuiBegin(); // Imgui BEGIN ==========
        ImGui::Text("123");
        ImGui::DragFloat3("Light Direction",
                          reinterpret_cast<float*>(&directional_light.direction));
        ImGui::ColorPicker3("Light Color",
                            reinterpret_cast<float*>(&directional_light.color));
        UpdateDirectionalLight(shader, directional_light);
        rlImGuiEnd(); // Imgui END ~~~~~~~~~~~

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}
