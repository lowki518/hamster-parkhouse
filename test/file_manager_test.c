#include "../include/file_manager_lib.h"
#include "../include/data_types.h"
#include "../include/car_lib.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

#define TEST_OUTPUT_PATH "/workspaces/hamster-parkhouse/test/tmp/"

/*
@brief helper: creates the test output directory
*/
void setup_test_dir() {
    if (mkdir(TEST_OUTPUT_PATH, 0777) != 0 && errno != EEXIST) {
        printf("ERROR: Could not create test directory: %s\n", TEST_OUTPUT_PATH);
        exit(1);
    }
}

/*
@brief helper: removes all Simulation_X.txt files and the test directory itself
*/
void teardown_test_dir() {
    DIR *dir = opendir(TEST_OUTPUT_PATH);
    if (!dir) return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        int nr;
        if (sscanf(entry->d_name, "Simulation_%d.txt", &nr) == 1) {
            char path[128];
            sprintf(path, "%s%s", TEST_OUTPUT_PATH, entry->d_name);
            remove(path);
        }
    }
    closedir(dir);
    rmdir(TEST_OUTPUT_PATH);
}

/*
@brief helper: removes all Simulation_X.txt files from the test directory
*/
void cleanup_all_test_files() {
    DIR *dir = opendir(TEST_OUTPUT_PATH);
    if (!dir) return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        int nr;
        if (sscanf(entry->d_name, "Simulation_%d.txt", &nr) == 1) {
            char path[128];
            sprintf(path, "%s%s", TEST_OUTPUT_PATH, entry->d_name);
            remove(path);
        }
    }
    closedir(dir);
}

/*
@brief helper: checks whether a simulation file exists
*/
int file_exists(int file_number) {
    char filename[128];
    sprintf(filename, "%sSimulation_%d.txt", TEST_OUTPUT_PATH, file_number);
    FILE *f = fopen(filename, "r");
    if (f) {
        fclose(f);
        return 1;
    }
    return 0;
}

/*
@brief tests get_new_file_number with an empty directory
*/
void test_get_new_file_number_empty_dir() {
    cleanup_all_test_files();

    int nr = get_new_file_number(TEST_OUTPUT_PATH);
    assert(nr == 1); // no files present -> first number is 1
}

/*
@brief tests get_new_file_number with existing simulation files
*/
void test_get_new_file_number_with_files() {
    cleanup_all_test_files();

    FILE *f1 = fopen(TEST_OUTPUT_PATH "Simulation_1.txt", "w");
    fclose(f1);
    FILE *f3 = fopen(TEST_OUTPUT_PATH "Simulation_3.txt", "w");
    fclose(f3);

    int nr = get_new_file_number(TEST_OUTPUT_PATH);
    assert(nr == 4); // highest existing is 3 -> next is 4

    cleanup_all_test_files();
}

/*
@brief tests get_new_file_number with an invalid directory
*/
void test_get_new_file_number_invalid_dir() {
    int nr = get_new_file_number("/tmp/this_does_not_exist_12345/");
    assert(nr == -1); // invalid directory -> error
}

/*
@brief tests that create_new_file_with_head_data creates a file
*/
void test_create_new_file_creates_file() {
    cleanup_all_test_files();

    create_new_file_with_head_data(TEST_OUTPUT_PATH, 1, 100, 30, 50, 75.0f, 5, 42);

    assert(file_exists(1));

    cleanup_all_test_files();
}

/*
@brief tests that create_new_file_with_head_data writes the correct header content
*/
void test_create_new_file_head_content() {
    cleanup_all_test_files();

    create_new_file_with_head_data(TEST_OUTPUT_PATH, 2, 200, 50, 100, 50.0f, 3, 99);

    char filepath[128];
    sprintf(filepath, "%sSimulation_2.txt", TEST_OUTPUT_PATH);
    FILE *f = fopen(filepath, "r");
    assert(f != NULL);

    char buffer[512];
    int found_sim_nr = 0;
    int found_header = 0;

    while (fgets(buffer, sizeof(buffer), f)) {
        if (strstr(buffer, "Simulation Nr. 2")) found_sim_nr = 1;
        if (strstr(buffer, "Simulated Steps"))  found_header = 1;
    }

    fclose(f);

    assert(found_sim_nr); // simulation number must appear in file
    assert(found_header); // column headers must appear in file

    cleanup_all_test_files();
}

/*
@brief tests that create_new_file_with_head_data handles an invalid path gracefully
*/
void test_create_new_file_invalid_path() {
    // should not crash, just print an error internally
    create_new_file_with_head_data("/tmp/this_does_not_exist_12345/", 1, 100, 30, 50, 75.0f, 5, 42);
    // no assert needed - test passes as long as there is no crash / segfault
}

/*
@brief tests that open_file_r returns a valid file pointer for an existing file
*/
void test_open_file_r_existing() {
    cleanup_all_test_files();

    create_new_file_with_head_data(TEST_OUTPUT_PATH, 5, 100, 30, 50, 75.0f, 5, 42);

    FILE *f = open_file_r(TEST_OUTPUT_PATH, 5);
    assert(f != NULL);
    fclose(f);

    cleanup_all_test_files();
}

/*
@brief tests that open_file_r returns NULL for a non-existing file
*/
void test_open_file_r_nonexistent() {
    cleanup_all_test_files();

    FILE *f = open_file_r(TEST_OUTPUT_PATH, 9999);
    assert(f == NULL);
}

/*
@brief tests that open_file_a returns a valid file pointer for an existing file
*/
void test_open_file_a_existing() {
    cleanup_all_test_files();

    create_new_file_with_head_data(TEST_OUTPUT_PATH, 6, 100, 30, 50, 75.0f, 5, 42);

    FILE *f = open_file_a(TEST_OUTPUT_PATH, 6);
    assert(f != NULL);
    fclose(f);

    cleanup_all_test_files();
}

/*
@brief tests that open_file_a returns NULL for a non-existing file (invalid path)
*/
void test_open_file_a_nonexistent() {
    // "a" mode creates the file if it doesn't exist -> use invalid path instead
    FILE *f = open_file_a("/tmp/this_does_not_exist_12345/", 9999);
    assert(f == NULL);
}

/*
@brief tests that append_data_per_timestep adds a line to the file
*/
void test_append_data_per_timestep() {
    cleanup_all_test_files();

    create_new_file_with_head_data(TEST_OUTPUT_PATH, 7, 100, 30, 50, 75.0f, 5, 42);

    char filepath[128];
    sprintf(filepath, "%sSimulation_7.txt", TEST_OUTPUT_PATH);

    // count lines before appending
    FILE *f = fopen(filepath, "r");
    assert(f != NULL);
    int lines_before = 0;
    char buf[256];
    while (fgets(buf, sizeof(buf), f)) lines_before++;
    fclose(f);

    FILE *file = open_file_a(TEST_OUTPUT_PATH, 7);

    append_data_per_timestep(file, 10, 15, 12.5f, 3, 2, 20, 0);

    fclose(file);

    // count lines after appending
    f = fopen(filepath, "r");
    assert(f != NULL);
    int lines_after = 0;
    while (fgets(buf, sizeof(buf), f)) lines_after++;
    fclose(f);

    assert(lines_after > lines_before); // at least one line was added

    cleanup_all_test_files();
}

/*
@brief runs all file_manager_lib tests
*/
int main() {
    setup_test_dir();              // create fresh test directory

    test_get_new_file_number_empty_dir();
    test_get_new_file_number_with_files();
    test_get_new_file_number_invalid_dir();
    test_create_new_file_creates_file();
    test_create_new_file_head_content();
    test_create_new_file_invalid_path();
    test_open_file_r_existing();
    test_open_file_r_nonexistent();
    test_open_file_a_existing();
    test_open_file_a_nonexistent();
    test_append_data_per_timestep();

    teardown_test_dir();           // delete test directory and all contents
    printf("All file_manager_lib tests passed!\n");

    return 0;
}