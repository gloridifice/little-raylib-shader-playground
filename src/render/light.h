#pragma once

#include "../raylib_types.h"
#include "baleine_type/primitive.h"

namespace HORiz {
struct DirectionalLight {
    Vec3 direction;
    Vec3 color;
    baleine::f32 intensity{};
};
}