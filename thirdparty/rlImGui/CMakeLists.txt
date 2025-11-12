add_library(rlImGui rlImGui.cpp)
target_include_directories(rlImGui PUBLIC ./)
target_link_libraries(rlImGui PUBLIC imgui raylib)