<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct s_user {
    char name[32]; // Name
    int  auth; // Auth flag
};

int main(void) {
    struct s_user *_auth = NULL;
    char *_service = NULL;
    char input[128];

    while (true) {
        printf("%p, %p \n", (void*)_auth, (void*)_service);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            return 0;
        }

        if (strncmp(input, "auth ", 5) == 0) {
            _auth = malloc(4); 
            
            char *argv = input + 5;
            if (strlen(argv) < 31) {
                strcpy(_auth->name, argv);
            }
        }

        else if (strncmp(input, "reset", 5) == 0) {
            free(_auth);
        }

        else if (strncmp(input, "service", 7) == 0) {
            char *argv = input + 8;
            _service = strdup(argv);
        }

        else if (strncmp(input, "login", 5) == 0) {
            if (_auth->auth != 0) {
                system("/bin/sh");
            } else {
                printf("Password:\n");
            }
        }
    }
    return 0;
=======
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define true 1

int main(void)
{
	char *auth = NULL;
	char *service = NULL;
	char buffer[128];

	while (true)
	{
		printf("%p %p\n", auth, service);
		
		char *val = fgets(buffer, 0x80, stdin);
		if (!strcmp(val, "auth "))
		{
			auth = malloc(4);
			*auth = '\0';
		}
		else if (!strcmp(val, "reset"))
		{
			if (auth)
				free(auth);
		}
		else if (!strcmp(val, "service"))
			service = strdup(val + 3);
		else if (!strcmp(val, "login"))
		{

		}
	}
>>>>>>> 36b6ceb (level8 .c not done)
}
