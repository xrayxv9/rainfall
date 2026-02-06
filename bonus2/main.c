#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Langs
{
	FINNISH = 1,
	NETHERLAND = 2,
	DEFAULT = 3
}		Langs;

Langs m = DEFAULT;

void	greetuser(char *username)
{
	char	s1[30];

	switch (m)
	{
		case DEFAULT:
			strcpy(s1, "Hello ");
			break ;
		case FINNISH:
			strcpy(s1, "Hyvää päivää ");
			break ;
		case NETHERLAND:
			strcpy(s1, "Goedemiddag! ");
			break ;
	}
	strcat(s1, username);
	puts(s1);
}

int	main(int ac, char **av)
{
	char main_buffer[80];
	char	*lang;

	if (ac != 3)
		return (1);
	memset(main_buffer, 0, 76);
	strncpy(main_buffer, av[1], 40);
	strncpy(main_buffer + 40, av[2], 32);
	lang = getenv("LANG");
		if (lang != NULL) {

		if (!memcmp(lang, "fi", 2))
			m = FINNISH;
		else if (!memcmp(lang, "nl", 2))
			m = NETHERLAND;
		else
			m = DEFAULT;
	}
	char temp_copy[76];
    memcpy(temp_copy, main_buffer, 76);
	greetuser(temp_copy);
	return (0);
}
