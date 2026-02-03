#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void n()
{
	system("/bin/cat /home/user/level7/.pass");
	return ;
}

void m()
{
	puts("nop\n");
	return ;
}

int main(int ac, char **av)
{
	char *s = malloc(64);
	void (*s2)(void) = malloc(4);

	strcpy(s, av[1]);
	s2 = m;
	s2();
}
