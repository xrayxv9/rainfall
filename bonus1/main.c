
#include <stdlib.h>
#include <string.h>
int main(int ac, char **av)
{
	int ato = atoi(av[1]);
	if (ato >= 10)
		return 1;
	char buffer[20];
	memcpy(buffer, av[2], ato * 4);
	if (ato == 0x574f4c46)
		system("/bin/sh");
	return 1;
}
