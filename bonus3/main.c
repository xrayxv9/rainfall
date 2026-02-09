#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int ac, char *av[]) {
	char s[136];

	if (ac  != 2)
		return (1);

	FILE *fd = fopen("/home/user/end/.pass", "r");
	memset(s, '\0', 132);
	fread(s, 1, 66, fd);

	s[65] = '\0';
	s[atoi(av[1])] = '\0';
	fread(s + 66, 41, 1, fd);
	fclose(fd);
	if (!strcmp(av[1], s))
		execl("/bin/sh", "sh", NULL);
	else{
		puts(s + 66);
	}
	return 1;
}
