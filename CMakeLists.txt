cmake_minimum_required(VERSION 3.31)

set(CMAKE_FIND_DEBUG_MODE TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-implicit-const-int-float-conversion")

project(my_raylib)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

set(RAYLIB_VERSION 5.5)

FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
        FIND_PACKAGE_ARGS
)

FetchContent_MakeAvailable(raylib)

set(BUILD_TEST OFF)
add_subdirectory(thirdparty)
set(BUILD_TEST ON)

add_executable(${PROJECT_NAME})
add_subdirectory(src)

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# target link here!
target_link_libraries(${PROJECT_NAME}
        raylib
        BaleineType
        ImGui
        rlImGui
        ImGuizmo
        raylib_cpp
        NanoSVG
        EnTT
        RTTR::Core_STL
)

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif ()
