#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void p(char *dest, char *s)
{
	char buffer[4096];
    
    puts(s);
    read(0, buffer, 4096);
    *strchr(buffer, '\n') = '\0';
    strncpy(dest, buffer, 20);
}

void pp(char *dest)
{
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
}
