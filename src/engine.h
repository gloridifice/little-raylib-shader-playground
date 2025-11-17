#pragma once
#include "camera.h"
#include "imgui.h"
#include "rlImGui.h"
#include "ImGuizmo.h"
#include "imgui_internal.h"
#include "raylib-cpp.hpp"
#include "raymath.h"
#include "ui/gui_helper.h"
#include "icon.h"
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

static int igThemeV3(int hue07,
                     int alt07,
                     int nav07,
                     int lit01 = 0,
                     int compact01 = 0,
                     int border01 = 1,
                     int shape0123 = 1
) {
    bool rounded = shape0123 == 2;

    // V3 style from ImThemes
    ImGuiStyle& style = ImGui::GetStyle();

    const float _8 = compact01 ? 4 : 8;
    const float _4 = compact01 ? 2 : 4;
    const float _2 = compact01 ? 0.5 : 1;

    style.Alpha = 1.0f;
    style.DisabledAlpha = 0.3f;

    style.WindowPadding = ImVec2(4, _8);
    style.FramePadding = ImVec2(4, _4);
    style.ItemSpacing = ImVec2(_8, _2 + _2);
    style.ItemInnerSpacing = ImVec2(4, 4);
    style.IndentSpacing = 16;
    style.ScrollbarSize = compact01 ? 12 : 18;
    style.GrabMinSize = compact01 ? 16 : 20;

    style.WindowBorderSize = border01;
    style.ChildBorderSize = border01;
    style.PopupBorderSize = border01;
    style.FrameBorderSize = 0;

    style.WindowRounding = 4;
    style.ChildRounding = 6;
    style.FrameRounding = shape0123 == 0 ? 0 : shape0123 == 1 ? 4 : 12;
    style.PopupRounding = 4;
    style.ScrollbarRounding = rounded * 8 + 4;
    style.GrabRounding = style.FrameRounding;

    style.TabBorderSize = 0;
    style.TabBarBorderSize = 2;
    style.TabBarOverlineSize = 2;
    style.TabCloseButtonMinWidthSelected = -1;
    // -1:always visible, 0:visible when hovered, >0:visible when hovered if minimum width
    style.TabCloseButtonMinWidthUnselected = -1;
    style.TabRounding = rounded;

    style.CellPadding = ImVec2(8.0f, 4.0f);

    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Right;

    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.5f, 0.5f);
    style.SeparatorTextAlign.x = 1.00f;
    style.SeparatorTextBorderSize = 1;
    style.SeparatorTextPadding = ImVec2(0, 0);

    style.WindowMinSize = ImVec2(32.0f, 16.0f);
    style.ColumnsMinSpacing = 6.0f;

    // diamond sliders
    style.CircleTessellationMaxError = shape0123 == 3 ? 4.00f : 0.30f;

    auto lit = [&](ImVec4 hi) {
        float h, s, v;
        ImGui::ColorConvertRGBtoHSV(hi.x, hi.y, hi.z, h, s, v);
        ImVec4 lit = ImColor::HSV(h, s * 0.80, v * 1.00, hi.w).Value;
        return lit;
    };
    auto dim = [&](ImVec4 hi) {
        float h, s, v;
        ImGui::ColorConvertRGBtoHSV(hi.x, hi.y, hi.z, h, s, v);
        ImVec4 dim = ImColor::HSV(h, s, lit01 ? v * 0.65 : v * 0.65, hi.w).Value;
        if (hi.z > hi.x && hi.z > hi.y)
            return ImVec4(dim.x, dim.y, hi.z, dim.w);
        return dim;
    };

    const ImVec4 cyan = ImVec4(000 / 255.f, 192 / 255.f, 255 / 255.f, 1.00f);
    const ImVec4 red = ImVec4(230 / 255.f, 000 / 255.f, 000 / 255.f, 1.00f);
    const ImVec4 yellow = ImVec4(240 / 255.f, 210 / 255.f, 000 / 255.f, 1.00f);
    const ImVec4 orange = ImVec4(255 / 255.f, 144 / 255.f, 000 / 255.f, 1.00f);
    const ImVec4 lime = ImVec4(192 / 255.f, 255 / 255.f, 000 / 255.f, 1.00f);
    const ImVec4 aqua = ImVec4(000 / 255.f, 255 / 255.f, 192 / 255.f, 1.00f);
    const ImVec4 magenta = ImVec4(255 / 255.f, 000 / 255.f, 88 / 255.f, 1.00f);
    const ImVec4 purple = ImVec4(192 / 255.f, 000 / 255.f, 255 / 255.f, 1.00f);

    ImVec4 alt = cyan;
    /**/
    if (alt07 == 0 || alt07 == 'C')
        alt = cyan;
    else if (alt07 == 1 || alt07 == 'R')
        alt = red;
    else if (alt07 == 2 || alt07 == 'Y')
        alt = yellow;
    else if (alt07 == 3 || alt07 == 'O')
        alt = orange;
    else if (alt07 == 4 || alt07 == 'L')
        alt = lime;
    else if (alt07 == 5 || alt07 == 'A')
        alt = aqua;
    else if (alt07 == 6 || alt07 == 'M')
        alt = magenta;
    else if (alt07 == 7 || alt07 == 'P')
        alt = purple;
    if (lit01)
        alt = dim(alt);

    ImVec4 hi = cyan, lo = dim(cyan);
    /**/
    if (hue07 == 0 || hue07 == 'C')
        lo = dim(hi = cyan);
    else if (hue07 == 1 || hue07 == 'R')
        lo = dim(hi = red);
    else if (hue07 == 2 || hue07 == 'Y')
        lo = dim(hi = yellow);
    else if (hue07 == 3 || hue07 == 'O')
        lo = dim(hi = orange);
    else if (hue07 == 4 || hue07 == 'L')
        lo = dim(hi = lime);
    else if (hue07 == 5 || hue07 == 'A')
        lo = dim(hi = aqua);
    else if (hue07 == 6 || hue07 == 'M')
        lo = dim(hi = magenta);
    else if (hue07 == 7 || hue07 == 'P')
        lo = dim(hi = purple);
    //    if( lit01 ) { ImVec4 tmp = hi; hi = lo; lo = lit(tmp); }

    ImVec4 nav = orange;
    /**/
    if (nav07 == 0 || nav07 == 'C')
        nav = cyan;
    else if (nav07 == 1 || nav07 == 'R')
        nav = red;
    else if (nav07 == 2 || nav07 == 'Y')
        nav = yellow;
    else if (nav07 == 3 || nav07 == 'O')
        nav = orange;
    else if (nav07 == 4 || nav07 == 'L')
        nav = lime;
    else if (nav07 == 5 || nav07 == 'A')
        nav = aqua;
    else if (nav07 == 6 || nav07 == 'M')
        nav = magenta;
    else if (nav07 == 7 || nav07 == 'P')
        nav = purple;
    if (lit01)
        nav = dim(nav);

    const ImVec4
        link = ImVec4(0.26f, 0.59f, 0.98f, 1.00f),
        grey0 = ImVec4(0.04f, 0.05f, 0.07f, 1.00f),
        grey1 = ImVec4(0.08f, 0.09f, 0.11f, 1.00f),
        grey2 = ImVec4(0.10f, 0.11f, 0.13f, 1.00f),
        grey3 = ImVec4(0.12f, 0.13f, 0.15f, 1.00f),
        grey4 = ImVec4(0.16f, 0.17f, 0.19f, 1.00f),
        grey5 = ImVec4(0.18f, 0.19f, 0.21f, 1.00f);

    #define Luma(v,a) ImVec4((v)/100.f,(v)/100.f,(v)/100.f,(a)/100.f)

    style.Colors[ImGuiCol_Text] = Luma(100, 100);
    style.Colors[ImGuiCol_TextDisabled] = Luma(39, 100);
    style.Colors[ImGuiCol_WindowBg] = grey1;
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.09f, 0.10f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = grey1;
    style.Colors[ImGuiCol_Border] = grey4;
    style.Colors[ImGuiCol_BorderShadow] = grey1;
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.13f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = grey4;
    style.Colors[ImGuiCol_FrameBgActive] = grey4;
    style.Colors[ImGuiCol_TitleBg] = grey0;
    style.Colors[ImGuiCol_TitleBgActive] = grey0;
    style.Colors[ImGuiCol_TitleBgCollapsed] = grey1;
    style.Colors[ImGuiCol_MenuBarBg] = grey2;
    style.Colors[ImGuiCol_ScrollbarBg] = grey0;
    style.Colors[ImGuiCol_ScrollbarGrab] = grey3;
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = lo;
    style.Colors[ImGuiCol_ScrollbarGrabActive] = hi;
    style.Colors[ImGuiCol_CheckMark] = alt;
    style.Colors[ImGuiCol_SliderGrab] = lo;
    style.Colors[ImGuiCol_SliderGrabActive] = hi;
    style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.11f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = lo;
    style.Colors[ImGuiCol_ButtonActive] = grey5;
    style.Colors[ImGuiCol_Header] = grey3;
    style.Colors[ImGuiCol_HeaderHovered] = lo;
    style.Colors[ImGuiCol_HeaderActive] = hi;
    style.Colors[ImGuiCol_Separator] = ImVec4(0.13f, 0.15f, 0.19f, 1.00f);
    style.Colors[ImGuiCol_SeparatorHovered] = lo;
    style.Colors[ImGuiCol_SeparatorActive] = hi;
    style.Colors[ImGuiCol_ResizeGrip] = Luma(15, 100);
    style.Colors[ImGuiCol_ResizeGripHovered] = lo;
    style.Colors[ImGuiCol_ResizeGripActive] = hi;
    style.Colors[ImGuiCol_InputTextCursor] = Luma(100, 100);
    style.Colors[ImGuiCol_TabHovered] = grey3;
    style.Colors[ImGuiCol_Tab] = grey1;
    style.Colors[ImGuiCol_TabSelected] = grey3;
    style.Colors[ImGuiCol_TabSelectedOverline] = hi;
    style.Colors[ImGuiCol_TabDimmed] = grey1;
    style.Colors[ImGuiCol_TabDimmedSelected] = grey1;
    style.Colors[ImGuiCol_TabDimmedSelectedOverline] = lo;
    style.Colors[ImGuiCol_DockingPreview] = grey1;
    style.Colors[ImGuiCol_DockingEmptyBg] = Luma(20, 100);
    style.Colors[ImGuiCol_PlotLines] = grey5;
    style.Colors[ImGuiCol_PlotLinesHovered] = lo;
    style.Colors[ImGuiCol_PlotHistogram] = grey5;
    style.Colors[ImGuiCol_PlotHistogramHovered] = lo;
    style.Colors[ImGuiCol_TableHeaderBg] = grey0;
    style.Colors[ImGuiCol_TableBorderStrong] = grey0;
    style.Colors[ImGuiCol_TableBorderLight] = grey0;
    style.Colors[ImGuiCol_TableRowBg] = grey3;
    style.Colors[ImGuiCol_TableRowBgAlt] = grey2;
    style.Colors[ImGuiCol_TextLink] = link;
    style.Colors[ImGuiCol_TextSelectedBg] = Luma(39, 100);
    style.Colors[ImGuiCol_TreeLines] = Luma(39, 100);
    style.Colors[ImGuiCol_DragDropTarget] = nav;
    style.Colors[ImGuiCol_NavCursor] = nav;
    style.Colors[ImGuiCol_NavWindowingHighlight] = lo;
    style.Colors[ImGuiCol_NavWindowingDimBg] = Luma(0, 63);
    style.Colors[ImGuiCol_ModalWindowDimBg] = Luma(0, 63);

    if (lit01) {
        for (int i = 0; i < ImGuiCol_COUNT; i++) {
            float H, S, V;
            ImVec4& col = style.Colors[i];
            ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, H, S, V);
            if (S < 0.5)
                V = 1.0 - V, S *= 0.15;
            ImGui::ColorConvertHSVtoRGB(H, S, V, col.x, col.y, col.z);
        }
    }

    #undef Luma
    return 0;
}


static ImFont* FONT_NORMAL;
static ImFont* FONT_BOLD;
static ImFont* FONT_LIGHT;

static void SetupMyImguiStyle() {
    ImGuiIO& io = ImGui::GetIO();
    FONT_NORMAL = io.Fonts->AddFontFromFileTTF("assets/fonts/MiSans-Normal.ttf", 18.0f);
    FONT_BOLD = io.Fonts->AddFontFromFileTTF("assets/fonts/MiSans-Bold.ttf", 18.0f);
    FONT_LIGHT = io.Fonts->AddFontFromFileTTF("assets/fonts/MiSans-Light.ttf", 18.0f);

    int hue = 0, alt = 0, nav = 0, shape = 0, borders = 1;
    bool light = false, compact = false;

    igThemeV3(hue["CRYOLAMP"],
              alt["CRYOLAMP"],
              nav["CRYOLAMP"],
              light,
              compact,
              borders,
              shape);

    io.FontDefault = FONT_NORMAL;
}

class EngineCore {
    PostProcessingManager postProcessingManager;

    i32 sceneHeight = 400, sceneWidth = 300;
    RenderTexture2D sceneRenderTarget = LoadRenderTexture(
        sceneWidth,
        sceneHeight);

    MyCamera camera = MyCamera{raylib::Transform{},
                               45.0f, CAMERA_PERSPECTIVE};
    CameraController controller;

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

    void ResizeSceneViewport(i32 width, i32 height) {
        UnloadRenderTexture(sceneRenderTarget);
        sceneRenderTarget = LoadRenderTexture(sceneWidth, sceneHeight);
        postProcessingManager.Resize(width, height);
    }

    void Update() {
        controller.Update(camera);

        directionalLight.direction = Vector3RotateByQuaternion(
            (Vec3){0.0f, 0.0f, -1.0f},
            directionalLightTransform.rotation);
        blinnPhongShader->UpdateViewWorldPos(camera.transform.translation);
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
            ClearBackground(raylib::Color(Vec4(.14f, .14f, .13f, 1.f)));

            // =========== 3D BEGIN =============

            camera.camera.BeginMode();
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
            camera.camera.EndMode();
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
                                 camera.camera,
                                 ImGuizmo::ROTATE,
                                 ImGuizmo::WORLD);
                         });

                ImWindow("Info",
                         [&] {
                             ImGui::PushFont(FONT_BOLD);
                             ImGui::SeparatorText("Camera");
                             HORizImGui::SpacingN(3);
                             ImGui::PopFont();

                             HORizImGui::IconText("axes", "Camera Transform");

                             HORizImGui::SpacingN(2);

                             ImGui::ModifyTransform(camera.transform);

                             HORizImGui::SpacingN(3);

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
        IconManager::Instance().Load();
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

        // postProcessingManager.PushShader(
        //     LoadShader(0, "assets/shaders/post_processing_template.frag"));
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
        SetExitKey(KEY_NULL);

        core = std::make_unique<EngineCore>();
        core->Loop();

        raylib::Window::Close();
    }
};