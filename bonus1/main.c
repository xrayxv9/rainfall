
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int	main(int ac, char **av)
{
	int32_t		ato;
	char	buffer[20];

	ato = atoi(av[1]);
	if (ato >= 10)
		return (1);
	memcpy(buffer, av[2], ato * 4);
	if (ato == 0x574f4c46)
		system("/bin/sh");
	return (1);
}
