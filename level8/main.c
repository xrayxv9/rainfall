#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct s_user {
    char name[32]; // Name
    int  is_auth; // Auth flag
};

int main(void) {
    struct s_user *auth = NULL;
    char *service = NULL;
    char input[128];

    while (true) {
        printf("%p, %p \n", (void*)auth, (void*)service);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            return 0;
        }

        if (strncmp(input, "auth ", 5) == 0) {
            auth = malloc(4); 
            
            char *argv = input + 5;
            if (strlen(argv) < 31) {
                strcpy(auth->name, argv);
            }
        }

        else if (strncmp(input, "reset", 5) == 0) {
            free(auth);
        }

        else if (strncmp(input, "service", 7) == 0) {
            char *argv = input + 8;
            service = strdup(argv);
        }

        else if (strncmp(input, "login", 5) == 0) {
            if (auth->is_auth != 0) {
                system("/bin/sh");
            } else {
                printf("Password:\n");
            }
        }
    }
    return 0;
}
