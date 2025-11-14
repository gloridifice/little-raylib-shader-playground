#pragma once
#include "imgui.h"
#include "rlImGui.h"
#include "ImGuizmo.h"
#include "imgui_internal.h"
#include "raylib-cpp.hpp"
#include "raymath.h"
#include "gui_helper.h"
#include "baleine_type/primitive.h"
#include "baleine_type/memory.h"
#include "render/mod.h"
#include "raylib_types.h"
#include "render/post_processing.h"

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
    PostProcessingManager postProcessingManager;

    i32 sceneHeight = 400, sceneWidth = 300;
    RenderTexture2D sceneRenderTarget = LoadRenderTexture(
        sceneWidth,
        sceneHeight);

    raylib::Camera camera = {
        Vec3{0.0f, 2.0f, 4.0f},
        Vec3{0.0f, 0.f, 0.0f},
        Vec3{0.0f, 1.0f, 0.0f},
        45.0f,
        CAMERA_PERSPECTIVE
    };
    f32 cameraSpeed = 30.f;

    DirectionalLight directionalLight = {
        .direction = Vec3{-1.0f, -1.0f, 0.0f},
        .color = Vec3{1.0f, 0.0f, 1.0f},
        .intensity = 1.0f
    };

    f32 specularPower = 8.0f;
    f32 diffuseCoefficient = 0.6f;
    f32 specularCoefficient = 0.6f;
    Vec4 ambient = {0.1f, 0.1f, 0.1f, 1.f};

    Transform directionalLightTransform = {
        .translation = Vec3{0.0, 6.0, 0.0},
        .rotation = QuaternionFromAxisAngle(Vec3{0.0, -1.0, 0.0}, 0.f),
        .scale = Vector3One(),
    };
    raylib::Model car{"assets/models/old_car_new.glb"};

    void UpdateSceneCamera() {
        auto deltaTime = GetFrameTime();

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsKeyPressed(KEY_LEFT_ALT)) {
            HideCursor();
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) || IsKeyReleased(KEY_LEFT_ALT)) {
            ShowCursor();
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsKeyDown(KEY_L)) {
            UpdateCamera(&camera, CAMERA_FIRST_PERSON);

            auto dir = Vec3();
            if (IsKeyDown(KEY_W)) {
                dir.z -= 1;
            }
            if (IsKeyDown(KEY_S)) {
                dir.z += 1;
            }
            if (IsKeyDown(KEY_A)) {
                dir.x -= 1;
            }
            if (IsKeyDown(KEY_D)) {
                dir.x += 1;
            }

            dir = Vector3RotateByQuaternion(dir,
                                            QuaternionFromVector3ToVector3(
                                                Vec3{0.0f, 1.0f, 0.0f},
                                                camera.up));
            if (IsKeyDown(KEY_SPACE)) {
                dir.y += 1;
            }
            if (IsKeyDown(KEY_LEFT_SHIFT)) {
                dir.y -= 1;
            }

            camera.position = dir * (deltaTime * cameraSpeed) + camera.position;
        }
    }

    void ResizeSceneViewport(i32 width, i32 height) {
        UnloadRenderTexture(sceneRenderTarget);
        sceneRenderTarget = LoadRenderTexture(sceneWidth, sceneHeight);
        postProcessingManager.Resize(width, height);
    }

    void Update() {
        UpdateSceneCamera();

        directionalLight.direction = Vector3RotateByQuaternion(
            (Vec3){0.0f, 0.0f, -1.0f},
            directionalLightTransform.rotation);
        blinnPhongShader->UpdateViewWorldPos(camera.position);
        blinnPhongShader->UpdateDiffuseCoefficient(diffuseCoefficient);
        blinnPhongShader->UpdateSpecularCoefficient(specularCoefficient);
        blinnPhongShader->UpdateSpecularPower(specularPower);
        blinnPhongShader->UpdateDirectionalLight(directionalLight);
        blinnPhongShader->UpdateAmbient(ambient);

        auto& texture = sceneRenderTarget.texture;
        if (texture.height != sceneHeight || texture.width != sceneWidth) {
            ResizeSceneViewport(sceneWidth, sceneHeight);
        }
    }

    void RenderScene() {
        BeginTextureMode(sceneRenderTarget);
        {
            ClearBackground(RAYWHITE);

            // =========== 3D BEGIN =============

            camera.BeginMode();
            {
                DrawGrid(20.0f, 1.0f);
                blinnPhongShader->BeginMode();
                {
                    car.Draw(Vec3(), 1.f);
                }
                blinnPhongShader->EndMode();

                auto dirLightPos = directionalLightTransform.translation;
                DrawLine3D(dirLightPos,
                           directionalLight.direction.Normalize() * 5.0f +
                           dirLightPos,
                           BLUE);
                DrawSphere(dirLightPos, .7f, BLUE);
            }
            camera.EndMode();
            // #### 3D END #####

            DrawFPS(10, 10);
        }
        EndTextureMode();
    }

    void RenderGui() {
        postProcessingManager.Draw(sceneRenderTarget);
        BeginDrawing();
        {
            rlImGuiBegin(); // Imgui NEW FRAME ==========
            {
                ImGui::DockSpaceOverViewport();
                // Make a full window docking space

                ImWindow("Scene",
                         [&] {
                             auto regionSize =
                                 ImGui::GetContentRegionAvail();
                             auto windowPos = ImGui::GetWindowPos();
                             auto regionRelative =
                                 ImGui::GetWindowContentRegionMin();
                             sceneWidth = (i32)regionSize.x;
                             sceneHeight = (i32)regionSize.y;

                             auto sceneTexture = sceneRenderTarget.texture;

                             ImGui::Image(
                                 (ImTextureID)(uintptr_t)
                                 sceneTexture.id,
                                 regionSize,
                                 ImVec2(0, 1),
                                 ImVec2(1, 0)
                             );

                             ImGuizmo::SetOrthographic(false);
                             ImGuizmo::BeginFrame();
                             ImGuizmo::SetDrawlist(
                                 ImGui::GetCurrentWindow()->DrawList);

                             ImGuizmo::SetRect(
                                 regionRelative.x + windowPos.x,
                                 regionRelative.y + windowPos.y,
                                 regionSize.x,
                                 regionSize.y
                             );

                             ManipulateTransform(
                                 directionalLightTransform,
                                 camera,
                                 ImGuizmo::ROTATE,
                                 ImGuizmo::WORLD);
                         });


                ImWindow("Info",
                         [&] {
                             ImGui::SeparatorText("Camera");

                             ImGui::LabelText("Camera Position",
                                              "%.2f, %.2f, %.2f",
                                              camera.position.x,
                                              camera.position.y,
                                              camera.position.z);

                             ImGui::DragFloat("Camera Speed",
                                              &cameraSpeed,
                                              1.0f);

                             ImGui::SeparatorText("Light");

                             ImGui::LabelText("Light Direction",
                                              "%.2f, %.2f, %.2f",
                                              directionalLight.direction.x,
                                              directionalLight.direction.y,
                                              directionalLight.direction.z
                             );

                             if (ImGui::CollapsingHeader(
                                 "Light Color")) {
                                 ImGui::ColorPicker3("Light Color",
                                                     reinterpret_cast<float*>(&
                                                         directionalLight
                                                         .
                                                         color));
                             }

                             ImGui::SeparatorText("Blinn Phong");
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
                             if (ImGui::CollapsingHeader("Ambient")) {
                                 ImGui::ColorPicker4("Ambient",
                                                     reinterpret_cast<float*>(&
                                                         ambient));
                             }
                         });
            }
            rlImGuiEnd(); // Imgui END ~~~~~~~~~~~
        }
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
        postProcessingManager(PostProcessingManager(sceneWidth, sceneHeight)) {
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

        postProcessingManager.PushShader(
            LoadShader(0, "assets/shaders/post_processing_template.frag"));
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
    Unique<raylib::Window> window;
    Unique<EngineCore> core;

public:
    explicit Engine() = default;
    ~Engine() = default;

    void Run() {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        window = std::make_unique<raylib::Window>(DEFAULT_WINDOW_WIDTH,
                                                  DEFAULT_WINDOW_HEIGHT,
                                                  "Little Raylib Shader Playground");

        SetTargetFPS(120);

        core = std::make_unique<EngineCore>();
        core->Loop();

        raylib::Window::Close();
    }
};