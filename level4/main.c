#include <stdio.h>
#include <stdlib.h>

unsigned long _m = 0x0;

void n(void);
void p(char *format);

void main(void)
{
    n();
    return;
}


void n(void)
{
	char s[0x218];
    
    fgets(&s, 0x200, stdin);
    p((char *)&s);
    if (_m == 0x1025544) {
        system("/bin/cat /home/user/level5/.pass");
    }
    return;
}

void p(char *format)
{
    printf(format);
    return;
}
