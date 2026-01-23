#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

static u_int32_t _m = 0;

void v(void)
{
	char format[0x218];

    fgets(&format, 0x200, stdin);
    printf(&format);
    if (_m == 0x40) {
        fwrite("Wait what?!\n", 1, 0xc, stdout);
        system("/bin/sh");
    }
    return;
}

int main(void)
{
    v();
    return 0;
}
