#pragma once

#include "raylib-cpp.hpp"

using Vec2 = raylib::Vector2;
using Vec3 = raylib::Vector3;
using Vec4 = raylib::Vector4;
using Quat = raylib::Quaternion;

namespace raylib {
struct Transform {
    Vec3 translation;
    Quat rotation;
    Vec3 scale;

    Transform():
        translation(Vec3::Zero()),
        rotation(Quat::Identity()),
        scale(Vec3::One()) {}

    [[nodiscard]] Vec3 Forward() const {
        const auto ret = Vec3{0.0f, 0.0f, -1.0f};
        return ret.RotateByQuaternion(rotation);
    }

    [[nodiscard]] Vec3 Right() const {
        const auto ret = Vec3{1.0f, 0.0f, 0.0f};
        return ret.RotateByQuaternion(rotation);
    }

    [[nodiscard]] Vec3 Up() const {
        const auto ret = Vec3{0.0f, 1.0f, 0.0f};
        return ret.RotateByQuaternion(rotation);
    }
};
}

using Trans = raylib::Transform;