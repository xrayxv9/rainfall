#include <stdio.h>
#include <stdlib.h>

unsigned long _m = 0x0;

void n(void);
void p(char *format);

int main(void)
{
    n();
    return 0;
}


void n(void)
{
	char s[512];
    
    fgets(s, 512, stdin);
    p(s);
    if (_m == 16930116) {
        system("/bin/cat /home/user/level5/.pass");
    }
    return;
}

void p(char *format)
{
    printf(format);
    return;
}
