#pragma once
#include "imgui.h"
#include "rlImGui.h"
#include "ImGuizmo.h"
#include "imgui_internal.h"
#include "raylib.h"
#include "raymath.h"
#include "gui_helper.h"
#include "baleine_type/primitive.h"
#include "baleine_type/memory.h"
#include "render/mod.h"
#include "raylib_helper.h"

using namespace baleine;

constexpr i32 DEFAULT_WINDOW_WIDTH = 1024;
constexpr i32 DEFAULT_WINDOW_HEIGHT = 576;

static Unique<BlinnPhongShaderContext> blinnPhongShader = std::make_unique<
    BlinnPhongShaderContext>("assets/shaders/blinn_phong.vert",
                             "assets/shaders/blinn_phong.frag");

static void SetupMyImguiStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.3f;
    style.FrameRounding = 2.3f;
    style.ScrollbarRounding = 0;
    style.WindowPadding = ImVec2(4.0, 4.0);

    style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
    style.Colors[ImGuiCol_TitleBgCollapsed] =
        ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(
        0.56f,
        0.56f,
        0.56f,
        1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(
        0.56f,
        0.56f,
        0.56f,
        0.91f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
    style.Colors[ImGuiCol_SliderGrabActive] =
        ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(
        1.00f,
        1.00f,
        1.00f,
        0.60f);
    style.Colors[ImGuiCol_ResizeGripActive] =
        ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] =
        ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(
        1.00f,
        0.60f,
        0.00f,
        1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
}

class EngineCore {
    i32 sceneHeight = 400, sceneWidth = 300;
    RenderTexture2D sceneRenderTarget = LoadRenderTexture(
        sceneWidth,
        sceneHeight);

    Camera camera = {
        (Vector3){0.0f, 20.0f, 48.0f},
        (Vector3){0.0f, 1.5f, 0.0f},
        (Vector3){0.0f, 1.0f, 0.0f},
        45.0f,
        CAMERA_PERSPECTIVE
    };

    DirectionalLight directionalLight = {
        .direction = (Vector3){-1.0f, -1.0f, 0.0f},
        .color = (Vector3){1.0f, 0.0f, 1.0f},
        .intensity = 1.0f
    };

    f32 specularPower = 8.0f;
    f32 diffuseCoefficient = 0.6f;
    f32 specularCoefficient = 0.6f;
    Vector4 ambient = {0.1f, 0.1f, 0.1f, 1.f};

    Transform directionalLightTransform = {
        .translation = Vector3{0.0, 22.0, 0.0},
        .rotation = QuaternionFromAxisAngle(Vector3{0.0, -1.0, 0.0}, 0.f),
        .scale = Vector3One(),
    };
    Model car;

    void Update() {
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            HideCursor();
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
            ShowCursor();
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            UpdateCamera(&camera, CAMERA_FIRST_PERSON);
            if (IsKeyDown(KEY_SPACE)) {
                camera.position.y += 10.0f * GetFrameTime();
            }
            if (IsKeyDown(KEY_LEFT_SHIFT)) {
                camera.position.y -= 10.0f * GetFrameTime();
            }
        }

        directionalLight.direction = Vector3RotateByQuaternion(
            (Vector3){0.0f, 0.0f, -1.0f},
            directionalLightTransform.rotation);
        blinnPhongShader->UpdateViewWorldPos(camera.position);
        blinnPhongShader->UpdateDiffuseCoefficient(diffuseCoefficient);
        blinnPhongShader->UpdateSpecularCoefficient(specularCoefficient);
        blinnPhongShader->UpdateSpecularPower(specularPower);
        blinnPhongShader->UpdateDirectionalLight(directionalLight);
        blinnPhongShader->UpdateAmbient(ambient);

        auto& texture = sceneRenderTarget.texture;
        if (texture.height != sceneHeight || texture.width != sceneWidth) {
            UnloadRenderTexture(sceneRenderTarget);
            sceneRenderTarget = LoadRenderTexture(sceneWidth, sceneHeight);
        }
    }

    void RenderScene() {
        BeginTextureMode(sceneRenderTarget);
        ClearBackground(RAYWHITE);

        // =========== 3D BEGIN =============
        BeginMode3D(camera);
        blinnPhongShader->BeginMode();
        DrawModel(car, Vector3Zero(), .1f, WHITE);
        blinnPhongShader->EndMode();

        DrawLine3D(directionalLightTransform.translation,
                   Vector3Add(
                       Vector3Scale(
                           Vector3Normalize(directionalLight.direction),
                           5.0f),
                       directionalLightTransform.translation),
                   BLUE);
        DrawSphere(directionalLightTransform.translation, .7f, BLUE);

        EndMode3D();
        // #### 3D END #####

        DrawFPS(10, 10);
        EndTextureMode();
    }

    void RenderGui() {
        BeginDrawing();
        rlImGuiBegin(); // Imgui NEW FRAME ==========

        ImGui::DockSpaceOverViewport(); // Make a full window docking space

        ImGuiWindowRegion("Scene",
                          [&] {
                              auto regionSize = ImGui::GetContentRegionAvail();
                              auto windowPos = ImGui::GetWindowPos();
                              auto regionRelative =
                                  ImGui::GetWindowContentRegionMin();
                              sceneWidth = (i32)regionSize.x;
                              sceneHeight = (i32)regionSize.y;

                              ImGui::Image(
                                  (ImTextureID)(uintptr_t)sceneRenderTarget.
                                  texture.id,
                                  regionSize,
                                  ImVec2(0, 1),
                                  ImVec2(1, 0)
                              );

                              ImGuizmo::SetOrthographic(false);
                              ImGuizmo::BeginFrame();
                              ImGuizmo::SetDrawlist(
                                  ImGui::GetCurrentWindow()->DrawList);

                              ImGuizmo::SetRect(regionRelative.x + windowPos.x,
                                                regionRelative.y + windowPos.y,
                                                regionSize.x,
                                                regionSize.y
                              );

                              ManipulateTransform(directionalLightTransform,
                                                  camera,
                                                  ImGuizmo::ROTATE,
                                                  ImGuizmo::WORLD);
                          });

        ImGuiWindowRegion("Info",
                          [&] {
                              ImGui::LabelText("Camera Position",
                                               "%.2f, %.2f, %.2f",
                                               camera.position.x,
                                               camera.position.y,
                                               camera.position.z);

                              ImGui::DragFloat3("Light Direction",
                                                reinterpret_cast<float*>(&
                                                    directionalLight.
                                                    direction));
                              ImGui::SliderFloat("Diffuse Coefficient",
                                                 &diffuseCoefficient,
                                                 0.0,
                                                 1.0);
                              ImGui::SliderFloat("Specular Coefficient",
                                                 &specularCoefficient,
                                                 0.0,
                                                 1.0);

                              ImGui::DragFloat("Specular Power",
                                               &specularPower,
                                               0.5,
                                               1.0,
                                               20.0);
                              if (ImGui::CollapsingHeader("Light Color")) {
                                  ImGui::ColorPicker3("Light Color",
                                      reinterpret_cast<float*>(&directionalLight
                                          .
                                          color));
                              }
                              if (ImGui::CollapsingHeader("Ambient")) {
                                  ImGui::ColorPicker4("Ambient",
                                      reinterpret_cast<float*>(&ambient));
                              }
                          });

        rlImGuiEnd(); // Imgui END ~~~~~~~~~~~

        EndDrawing();
    }

    void Render() {
        RenderScene();
        RenderGui();
    }

public:
    /// Shutdown
    ~EngineCore() {
        rlImGuiShutdown();
    }

    /// Startup
    explicit EngineCore():
        car(LoadModel("assets/models/old_car_new.glb")) {
        rlImGuiSetup(true);

        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        SetupMyImguiStyle();

        blinnPhongShader->LoadAndInit();

        car.materials[0].shader = blinnPhongShader->raylibShader;
        car.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture(
            "assets/models/old_car_d.png");

        GenTextureMipmaps(&car.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture);
        SetTextureFilter(car.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture,
                         TEXTURE_FILTER_TRILINEAR);
    }

    void Loop() {
        while (!WindowShouldClose()) {
            Update();
            Render();
        }
    }
};

class Engine {
private:
    Unique<EngineCore> core;

public:
    explicit Engine() = default;
    ~Engine() = default;

    void Run() {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(DEFAULT_WINDOW_WIDTH,
                   DEFAULT_WINDOW_HEIGHT,
                   "Little Raylib Shader Playground");

        SetTargetFPS(120);

        core = std::make_unique<EngineCore>();
        core->Loop();

        CloseWindow();
    }
};