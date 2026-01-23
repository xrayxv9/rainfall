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
	int32_t	unaff_retaddr;
	int32_t	var_68h;
	char	*src;
	int32_t	var_10h;

	fflush(stdout);
	gets(&src);
	if ((unaff_retaddr & 0xb0000000) == 0xb0000000)
	{
		printf("(%p)\n", unaff_retaddr);
		_exit(1);
	}
	puts(&src);
	strdup(&src);
	return ;
}
