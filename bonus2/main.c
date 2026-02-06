#include <stdlib.h>
#include <string.h>

#define FINNISH 1
#define NETHERLAND 2
#define DEFAULT 3

int m = DEFAULT;

void greetuser()
{
	char s2[16] = {0};
	char s[88];

	switch (m){
		case DEFAULT:
			memcpy(s2, "Hello ", 19);
	}

}

int main(int ac, char **av)
{
	char s[160] = {0};
	if (ac != 3)
		return 1;
	strncpy(s + 80, av[1], 40);
	strncpy(s + 40, av[1], 40);
	char *lang = getenv("LANG");

	if (!memcmp(lang, "fi", 2))
		m = FINNISH;
	else if (!memcmp(lang, "nl", 2))
		m = NETHERLAND;
	else
		m = DEFAULT;	


}
