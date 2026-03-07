cmake_minimum_required(VERSION 3.10)

project(hamster_parkhouse C)

# C-Standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Configuration
set(SDL_X11 OFF CACHE BOOL "" FORCE)

# Include-folders

add_subdirectory(external)


#internal
include_directories(../include)

# --------------------------------------------------
# Library
# --------------------------------------------------

add_library(car src/car.c)
add_library(cli ../src/cli.c)
add_library(config ../src/config.c)
add_library(file_manager ../src/file_manager.c)
add_library(statistics_output ../src/statistics_output.c)


# --------------------------------------------------
# Unit-Test-Executable
# --------------------------------------------------

add_executable(test_vis ../test/visualization_test.c)
add_executable(test_cli ../test/cli_test.c)
add_executable(test_sim ../test/simulation_test.c)

target_link_libraries(test_vis playlist)

# --------------------------------------------------
# Activate CTest
# --------------------------------------------------

enable_testing()

add_test(
    NAME PlaylistUnitTests
    COMMAND test_playlist
)

# --------------------------------------------------
# Ensure testing before each build
# --------------------------------------------------

# Custom Target to execute tests
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS test_playlist
    COMMENT "Running unit tests"
)

# --------------------------------------------------
#  Main
# --------------------------------------------------

# Executable
add_executable(playlist_app
    main.c
    src/playlist.c
)

target_link_libraries(playlist_app
    playlist
)

# only build main if tests successful
add_dependencies(playlist_app run_tests)