#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

static u_int32_t _m = 0;

void v(void)
{
	char format[512];

    fgets(format, 512, stdin);
    printf(format);
    if (_m == 64) {
        fwrite("Wait what?!\n", 1, 12, stdout);
        system("/bin/sh");
    }
    return;
}

int main(void)
{
    v();
    return 0;
}
