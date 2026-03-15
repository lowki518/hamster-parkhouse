#include "../include/file_manager_lib.h"
#include "../include/data_types.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*
@brief tests the new_file_number function
*/
void test_get_file_number() {

    assert(get_new_file_number("./data_test_1") == 3);
    assert(get_new_file_number("./data_test_2") == 1);
}

int main() {
    
}