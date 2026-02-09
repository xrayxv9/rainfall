#include <stdio.h>
#include <stdlib.h>

void o()
{
    system("/bin/sh");
	exit(1);
}

void n()
{
	char format[512];

	fgets(format, 512, stdin);
	printf(format);
	exit(1);
}

int main(void)
{
	n();
	return 0;
}
