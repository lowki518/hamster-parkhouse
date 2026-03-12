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
 
# ===== Main Executable =====
add_executable(test src/test.c)
 
# ===== Link Libraries =====
target_link_libraries(test PRIVATE m SDL3::SDL3 SDL3_ttf::SDL3_ttf)
 
target_include_directories(test PRIVATE include)
 
# ===== Build Configuration =====
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
 
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Linking: SDL3::SDL3 SDL3_ttf::SDL3_ttf")