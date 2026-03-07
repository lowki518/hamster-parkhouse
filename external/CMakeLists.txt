add_subdirectory(SDL)
add_subdirectory(SDL_ttf)

add_library(external INTERFACE)
target_link_libraries(external INTERFACE SDL3::SDL3)
target_link_libraries(external INTERFACE SDL3_ttf::SDL3_ttf)