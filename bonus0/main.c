#include <stdio.h>
#include <string.h>
#include <unistd.h>
<<<<<<< HEAD
#include <stdlib.h>

void p(char *dest, char *s)
{
	char buffer[4096];
    
    puts(s);
    read(0, buffer, 4096);
    *strchr(buffer, '\n') = '\0';
    strncpy(dest, buffer, 20);
=======

void p(char *dest, char *c)
{
	char s[4096];
	puts(c);
	read(0, s, 4096);
	char *sub = strchr(s, '\n');
	*sub = '\0';
	strncpy(dest, s, 20);
>>>>>>> d38cc17 (bonus0 done)
}

void pp(char *dest)
{
<<<<<<< HEAD
	char s1[20];
	char s2[20];
    
    p(s1, "-"); // -> shellcode
    p(s2, "-"); // -> change eip sauter sur s1
    strcpy(dest, s1);

	int len = strlen(dest);
	dest[len] = ' ';

    strcat(dest, s2);
    return;
}


int main(void)
{
    char dest [42];
    
    pp(dest);
    puts(dest);
    return 0;
=======
	char s[28];
	char s2[28];
	p(s, "-");
	p(s2, "-");

	strcpy(dest, s);
	int len = strlen(dest);
	strcat(dest, s2);
	dest[len] = ' ';
}

int main(void)
{
	char s[22];

	pp(s);
	puts(s);
>>>>>>> d38cc17 (bonus0 done)
}
