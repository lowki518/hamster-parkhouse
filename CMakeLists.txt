cmake_minimum_required(VERSION 3.10)

project(hamster_parkhouse C)

# C-Standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include-folders

#internal
include_directories(include)

# --------------------------------------------------
# Library
# --------------------------------------------------

add_library(playlist
    src/playlist.c
)

# --------------------------------------------------
# Unit-Test-Executable
# --------------------------------------------------

add_executable(test_playlist
    test/test_playlist.c
)

target_link_libraries(test_playlist
    playlist
)

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