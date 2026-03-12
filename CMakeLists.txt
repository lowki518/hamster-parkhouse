cmake_minimum_required(VERSION 3.10)

project(hamster_parkhouse C)

# C-Standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Configuration
set(SDL_X11 OFF CACHE BOOL "" FORCE)

# Include-folders

add_subdirectory(external/SDL)
add_subdirectory(external/SDL_ttf)


#internal
include_directories(include)

# --------------------------------------------------
# Library
# --------------------------------------------------

#add_library(car src/car/car_lib.c)
#add_library(cli src/cli/cli_lib.c)
#add_library(config src/config/config_lib.c)
#add_library(file_manager src/file_manager/file_manager_lib.c)
#add_library(queue src/queue/queue_lib.c)
add_library(statistics_output src/statistics_gui/statistics_output_lib.c)
#add_library(simulation_loop src/simulation_loop.c)

target_link_libraries(statistics_output SDL3::SDL3 SDL3_ttf::SDL3_ttf m)

# --------------------------------------------------
# Unit-Test-Executable
# --------------------------------------------------

#add_executable(test_vis test/visualization_test.c)
#add_executable(test_cli test/cli_test.c)
#add_executable(test_sim test/simulation_test.c)

# --------------------------------------------------
# Libraries Link For Tests
# --------------------------------------------------

#target_link_libraries(test_vis SDL3::SDL3 SDL3_ttf::SDL3_ttf statistics_output file_manager)
#target_link_libraries(test_cli cli config file_manager)
#target_link_libraries(test_sim car queue file_manager)

# --------------------------------------------------
# Activate CTest
# --------------------------------------------------

#enable_testing()

#add_test(
#    NAME VisualisationUnitTest
#    COMMAND test_u_vis
#)

#add_test(
#    NAME CliUnitTest
#    COMMAND test_u_cli
#)

#add_test(
#    NAME SimulationUnitTest
#    COMMAND test_u_sim
#)

# --------------------------------------------------
# Ensure testing before each build
# --------------------------------------------------

# Custom Target to execute tests
#add_custom_target(run_tests
#    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
#    DEPENDS test_u_vis
#    DEPENDS test_u_cli
#    DEPENDS test_u_sim
#    COMMENT "Running unit tests"
#)

# --------------------------------------------------
#  Main
# --------------------------------------------------

add_executable(statistics src/test.c)

target_link_libraries(statistics statistics_output SDL3::SDL3 SDL3_ttf::SDL3_ttf m)

# Executable
#add_executable(tot_simulation
#    src/main.c
#    src/car/car_lib.c
#    src/cli/cli_lib.c
#    src/config/config_lib.c
#    src/file_manager/file_manager_lib.c
#    src/queue/queue_lib.c
#    src/statistics_gui/statistics_output_lib.c
#    src/simulation_loop.c
#)

#target_link_libraries(tot_simulation car cli config file_manager queue statistics_output simulation_loop SDL3::SDL3 SDL3_ttf::SDL3_ttf)


# only build main if tests successful
#add_dependencies(tot_simulation run_tests)