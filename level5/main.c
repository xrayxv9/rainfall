#include <stdio.h>
#include <stdlib.h>

void o()
{
    system("/bin/sh");
	exit(1);
}

void n()
{
	char str[536];

	fgets(str, 512, stdin);
	printf(str);
	exit(1);
}

int main(void)
{
	n();
	return 0;
}
