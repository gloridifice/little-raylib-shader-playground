#pragma once

#include "raylib.h"
#include "raylib-cpp.hpp"
#include "raylib_types.h"
#include "baleine_type/primitive.h"

class MyCamera {
public:
    raylib::Camera camera{};
    raylib::Transform transform{};

    explicit MyCamera(raylib::Transform transform, baleine::f32 fovy, CameraProjection projection):
        transform(transform) {
        camera.fovy = fovy;
        camera.projection = projection;
        UpdateState();
    }

    void UpdateState() {
        camera.SetPosition(transform.translation);
        camera.SetTarget(transform.Forward() + transform.translation);
        camera.SetUp(transform.Up());
    }
};


class CameraController {
public:
    baleine::f32 speed = 20.0;

    void Update(MyCamera& camera) const {
        baleine::f32 deltaTime = GetFrameTime();

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsKeyPressed(KEY_LEFT_ALT)) {
            HideCursor();
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) || IsKeyReleased(KEY_LEFT_ALT)) {
            ShowCursor();
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsKeyDown(KEY_L)) {
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

            dir = dir.RotateByQuaternion(camera.transform.rotation);

            if (IsKeyDown(KEY_SPACE)) {
                dir.y += 1;
            }
            if (IsKeyDown(KEY_LEFT_SHIFT)) {
                dir.y -= 1;
            }

            camera.transform.translation = dir * (deltaTime * speed) + camera.transform.translation;
        }
        camera.UpdateState();
    }
};