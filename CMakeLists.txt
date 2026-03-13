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
add_library(car src/car/car_lib.c)
add_library(cli src/cli/cli_lib.c)
add_library(config src/config/config_lib.c)
add_library(file_manager src/file_manager/file_manager_lib.c)
add_library(queue src/queue/queue_lib.c)
add_library(statistics src/statistics_output/statistics_output_lib.c)
add_library(simulation src/sim/sim_lib.c)
 
# ===== Link Library Code =====

target_link_libraries(cli config)
target_link_libraries(file_manager config)
target_link_libraries(queue car)
target_link_libraries(statistics m SDL3::SDL3 SDL3_ttf::SDL3_ttf)
target_link_libraries(simulation m car cli config file_manager queue statistics)


# ===== Add Tests =====
add_executable(test_cli test/cli_test.c)
add_executable(test_config test/config_test.c)


# ===== Link Test Libraries =====


target_link_libraries(test_cli cli config)
target_link_libraries(test_config config)
 


# ===== Activate Unit Tests =====
enable_testing()

add_test(
    NAME CliUnitTest
    COMMAND test_cli
)

add_test(
    NAME ConfigUnitTest
    COMMAND test_cli
)


# ===== Add Target For Tests =====
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS test_cli
    DEPENDS test_config
    COMMENT "Running unit tests"
)

# ===== Add Main Executable =====
#add_executable(park-house src/main.c)
add_executable(vis src/statistics_test.c)

#target_link_libraries(park-house car cli config file_manager queue statistics simulation SDL3::SDL3 SDL3_ttf::SDL3_ttf)
target_link_libraries(vis m file_manager statistics SDL3::SDL3 SDL3_ttf::SDL3_ttf)

# only build main if tests successful
add_dependencies(vis run_tests)
 