#pragma once
#include "camera.h"
#include "imgui.h"
#include "rlImGui.h"
#include "raylib-cpp.hpp"
#include "raymath.h"
#include "baleine_type/primitive.h"
#include "baleine_type/memory.h"
#include "render/mod.h"
#include "raylib_types.h"
#include "render/post_processing.h"
#include "entt.hpp"
#include "../../../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/ApplicationServices.framework/Versions/A/Frameworks/HIServices.framework/Versions/A/Headers/AXRoleConstants.h"
#include "baleine_type/functional.h"
#include "utils/singleton_macro.h"

using namespace baleine;

namespace HORiz {
constexpr i32 DEFAULT_WINDOW_WIDTH = 1024;
constexpr i32 DEFAULT_WINDOW_HEIGHT = 576;

static Unique<BlinnPhongShaderContext> blinnPhongShader = std::make_unique<
    BlinnPhongShaderContext>("assets/shaders/blinn_phong.vert",
                             "assets/shaders/blinn_phong.frag");

static ImFont* FONT_NORMAL;
static ImFont* FONT_BOLD;
static ImFont* FONT_LIGHT;

static void SetupMyImguiStyle();

class Viewport {
private:
    i32 width, height;
    RenderTexture2D renderTarget;

public:
    Vec<Fn<void(i32, i32)>> onResize {};
    explicit Viewport(const i32 width, const i32 height):
        width(width),
        height(height),
        renderTarget(LoadRenderTexture(width, height)) {}

    ~Viewport() {
        UnloadRenderTexture(renderTarget);
    }

    void Resize(i32 width, i32 height);
    void TryResize(i32 width, i32 height);

    RenderTexture2D& GetRenderTarget() {
        return renderTarget;
    }

    i32 GetWidth() const {
        return width;
    }

    i32 GetHeight() const {
        return height;
    }
};

class EngineCore {
    PostProcessingManager postProcessingManager;
    Viewport sceneViewport;

    MyCamera camera;
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

    entt::registry world;

    void ResizeSceneViewport(i32 width, i32 height);

    void Update();

    void RenderScene();

    void RenderGui();

public:
    /// Startup
    explicit EngineCore();

    /// Loop
    void Loop() {
        while (!WindowShouldClose()) {
            Update();
            RenderScene();
            RenderGui();
        }
    }

    /// Shutdown
    ~EngineCore();
};

class Engine {
    DECLARE_MEYERS_SINGLETON(Engine);

private:
    Unique<raylib::Window> window = nullptr;
    Unique<EngineCore> core = nullptr;

public:
    static EngineCore& CoreInstance() {
        return *Instance().core;
    }

    raylib::Window GetWindow() {
        return *window;
    }

    void Run();
};
}