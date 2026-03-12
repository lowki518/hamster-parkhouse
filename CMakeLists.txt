cmake_minimum_required(VERSION 3.10)
 
project(hamster_parkhouse C)
 
# C-Standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
 
# ===== SDL3 & SDL_ttf Submodules =====
add_subdirectory(external/SDL)
add_subdirectory(external/SDL_ttf)
 
# ===== Include Directories =====
include_directories(include)

# ===== Add Library Code =====
add_library(statistics_output src/statistics_gui/statistics_output_lib.c)
 
# ===== Main Executable =====
add_executable(test src/statistics_test.c)
 
# ===== Link Libraries =====
target_link_libraries(statistics_output SDL3::SDL3 SDL3_ttf::SDL3_ttf)
target_link_libraries(test m SDL3::SDL3 SDL3_ttf::SDL3_ttf statistics_output)
 
target_include_directories(test PRIVATE include)
 