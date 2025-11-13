#pragma once

#include "raylib.h"
#include "raymath.h"

inline Transform IdentityTransform()
{
    return {
        .translation = Vector3Zero(),
        .rotation = QuaternionIdentity(),
        .scale = Vector3One()
    };
}
