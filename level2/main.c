#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void	p(void);

int	main(void)
{
	p();
	return (0);
}

void	p(void)
{
	char buffer[64];
	void *retaddr;

	fflush(stdout);
	gets(buffer);
	if (((unsigned long int)retaddr & 0xb0000000) == 0xb0000000)
	{
		printf("(%p)\n", retaddr);
		_exit(1);
	}
	puts(buffer);
	strdup(buffer);
	return ;
}
