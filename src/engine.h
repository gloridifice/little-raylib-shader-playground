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
        (Vector3){0.0f, 14.0f, 14.0f},
        (Vector3){0.0f, 0.5f, 0.0f},
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

    Matrix modelMatrix = MatrixTranslate(0, 20.0, 0);

    Vector3 rayPos = Vector3(0., 25., 0.);
    Model car;

    void Update() {
        UpdateCamera(&camera, CAMERA_ORBITAL);
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

        DrawLine3D(rayPos,
                   Vector3Add(
                       Vector3Scale(
                           Vector3Normalize(directionalLight.direction),
                           5.0f),
                       rayPos),
                   BLUE);
        DrawSphere(rayPos, .7f, BLUE);

        EndMode3D();
        // #### 3D END #####

        DrawFPS(10, 10);
        EndTextureMode();
    }

    void RenderGui() {
        BeginDrawing();
        rlImGuiBegin(); // Imgui NEW FRAME ==========

        ImGui::DockSpaceOverViewport(); // Make a full window docking space

        ImGui::Begin("Scene");

        auto regionSize = ImGui::GetContentRegionAvail();
        sceneWidth = (i32)regionSize.x;
        sceneHeight = (i32)regionSize.y;

        ImGui::Image((ImTextureID)(uintptr_t)sceneRenderTarget.texture.id,
                     regionSize,
                     ImVec2(0, 1),
                     ImVec2(1, 0)
        );

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::BeginFrame();
        ImGuizmo::SetDrawlist(ImGui::GetCurrentWindow()->DrawList);
        ImGuizmo::SetRect(0,
                          0,
                          (float)GetScreenWidth(),
                          (float)GetScreenHeight());

        ManipulateMatrix(modelMatrix,
                         camera,
                         ImGuizmo::TRANSLATE,
                         ImGuizmo::WORLD);

        ImGui::End();

        ImGui::Begin("Info");
        ImGui::LabelText(ImGuizmo::IsUsing() ? "Using" : "Not Using", "");

        ImGui::DragFloat3("Light Direction",
                          reinterpret_cast<float*>(&directionalLight.
                              direction));
        ImGui::SliderFloat("Diffuse Coefficient",
                           &diffuseCoefficient,
                           0.0,
                           1.0);
        ImGui::SliderFloat("Specular Coefficient",
                           &specularCoefficient,
                           0.0,
                           1.0);

        ImGui::DragFloat("Specular Power", &specularPower, 0.5, 1.0, 20.0);
        if (ImGui::CollapsingHeader("Light Color")) {
            ImGui::ColorPicker3("Light Color",
                                reinterpret_cast<float*>(&directionalLight.
                                    color));
        }
        if (ImGui::CollapsingHeader("Ambient")) {
            ImGui::ColorPicker4("Ambient",
                                reinterpret_cast<float*>(&ambient));
        }
        ImGui::End();

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