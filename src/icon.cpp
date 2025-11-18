#include "icon.h"
#define NANOSVG_IMPLEMENTATION
#define NANOSVGRAST_IMPLEMENTATION
#include "nanosvg.h"
#include "nanosvgrast.h"


namespace HORiz {

void IconManager::Load() {
    for (auto& name : ICONS_REGISTER) {
        auto filename = std::format("assets/icons/{}.svg", name);
        int w = 18, h = 18;
        int scale = 2;
        NSVGimage* svg = nsvgParseFromFile(filename.c_str(), "px", 96);
        NSVGrasterizer* rast = nsvgCreateRasterizer();
        w = svg->width * scale;
        h = svg->height * scale;
        auto rgba = static_cast<unsigned char*>(malloc(w * h * 4));
        for (NSVGshape* shape = svg->shapes; shape != nullptr; shape = shape->next) {
            shape->fill.type = NSVG_PAINT_COLOR; // 填充类型 = 颜色
            shape->fill.color = 0xFFFFFFFF; // 强制白色

            shape->stroke.type = NSVG_PAINT_COLOR;
            shape->stroke.color = 0xFFFFFFFF;
        }
        nsvgRasterize(rast, svg, 0, 0, (float)scale, rgba, w, h, w * 4);
        nsvgDelete(svg);

        Image img = {
            .data = rgba,
            .width = w,
            .height = h,
            .mipmaps = 1,
            .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
        };

        map[name] = raylib::Texture(img);

        free(rgba);
    }
}
}