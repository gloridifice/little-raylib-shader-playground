cmake_minimum_required(VERSION 3.10)

add_library(NanoSVG INTERFACE)
target_include_directories(NanoSVG INTERFACE src/)
set_target_properties(NanoSVG PROPERTIES LINKER_LANGUAGE C)
