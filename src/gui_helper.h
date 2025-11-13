#pragma once

#include "imgui.h"
#include "ImGuizmo.h"
#include "raylib.h"

bool ManipulateMatrix(Matrix& matrix, Camera& camera, ImGuizmo::OPERATION operation, ImGuizmo::MODE mode);