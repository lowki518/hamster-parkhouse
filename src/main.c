#include <stdio.h>
#include <string.h>
#include "cli/cli.h"



int main() {
    char input[100];
    printf("Hello to our program");

    while (1) {
        printf("> "); // start of user input line
        if (!scanf("%99[^\n]%*c", input)) {
            printf("Error");
            break;
        }
        //handle_user_input(input);
        // Also break condition for while loop in handle_user_input() needed
    }
    return 0;
}