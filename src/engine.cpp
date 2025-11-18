#include "engine.h"

#include "imgui_internal.h"
#include "ui/gui_helper.h"

namespace HORiz {
void SetupMyImguiStyle() {
    ImGuiIO& io = ImGui::GetIO();
    FONT_NORMAL = io.Fonts->AddFontFromFileTTF("assets/fonts/MiSans-Normal.ttf", 18.0f);
    FONT_BOLD = io.Fonts->AddFontFromFileTTF("assets/fonts/MiSans-Bold.ttf", 18.0f);
    FONT_LIGHT = io.Fonts->AddFontFromFileTTF("assets/fonts/MiSans-Light.ttf", 18.0f);

    int hue = 0, alt = 0, nav = 0, shape = 0, borders = 1;
    bool light = false, compact = false;

    HORizImGui::igThemeV3(hue["CRYOLAMP"],
                          alt["CRYOLAMP"],
                          nav["CRYOLAMP"],
                          light,
                          compact,
                          borders,
                          shape);

    io.FontDefault = FONT_NORMAL;
}


void Viewport::Resize(i32 width, i32 height) {
    UnloadRenderTexture(renderTarget);
    this->width = width;
    this->height = height;
    renderTarget = LoadRenderTexture(width, height);

    for (auto& callback : onResize) {
        callback(width, height);
    }
}

void Viewport::TryResize(i32 width, i32 height) {
    if (this->width != width || this->height != height) {
        Resize(width, height);
    }
}

void EngineCore::ResizeSceneViewport(i32 width, i32 height) {
    sceneViewport.Resize(width, height);
    postProcessingManager.Resize(width, height);
}

void EngineCore::Update() {
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
}

void EngineCore::RenderScene() {
    BeginTextureMode(sceneViewport.GetRenderTarget());
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

void EngineCore::RenderGui() {
    postProcessingManager.Draw(sceneViewport.GetRenderTarget());
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
                         sceneViewport.TryResize((i32)regionSize.x, (i32)regionSize.y);

                         auto sceneTexture = sceneViewport.GetRenderTarget().texture;

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

EngineCore::~EngineCore() {
    rlImGuiShutdown();
}

EngineCore::EngineCore():
    sceneViewport(400, 300),
    postProcessingManager(sceneViewport.GetWidth(), sceneViewport.GetHeight()),
    camera(raylib::Transform{},
           45.0f,
           CAMERA_PERSPECTIVE) {

    sceneViewport.onResize.push_back([&](i32 width, i32 height) {
        postProcessingManager.Resize(width, height);
    });

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

void Engine::Run() {
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
}