#pragma once
#include <format>
#include <unordered_map>

#include "Texture.hpp"
#include "baleine_type/string.h"
#include "baleine_type/vector.h"
#include "../utils/singleton_macro.h"

using namespace baleine;

namespace HORiz {
static Vec<String> ICONS_REGISTER = {
    "axes", "video-camera"
};

class IconManager {
    DECLARE_MEYERS_SINGLETON(IconManager);

public:
    std::unordered_map<String, raylib::Texture> map{};

    void Load();

    void Unload() {
        for (auto& [key, texture] : map) {
            texture.Unload();
        }
        map.clear();
    }
};
}