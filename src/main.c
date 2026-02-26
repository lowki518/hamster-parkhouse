#include <stdio.h>
#include <string.h>

void handle_user_input(char * input);



int main() {
    char input[256];
    printf("Hello to our program");

    while (1) {
        printf("> "); // start of user input line
        scanf("%255[^\n]%*c", input);
        handle_user_input(input);
        // Also break condition for while loop in handle_user_input() needed
    }
    return 0;
}