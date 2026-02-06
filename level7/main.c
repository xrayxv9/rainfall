#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

struct Node {
	int id;
	char *buffer;
};

char c[68];

int main(int ac, char **argv)
{
	struct Node *node1;
	struct Node *node2;
    FILE *stream;
    

	node1 = (struct Node *)malloc(sizeof(struct Node));
	node1->id = 1;
	node1->buffer = (char *)malloc(8);

	node2 = (struct Node *)malloc(sizeof(struct Node));
	node2->id = 2;
	node2->buffer = (char *)malloc(8);

	strcpy(node1->buffer, argv[1]);
	strcpy(node2->buffer, argv[2]);

    stream = fopen("/home/user/level8/.pass", "r");
    fgets(c, 0x44, stream);
    puts("~~");
    return 0;
}


void m(void)
{
    time_t t;

    t = time(0);
    printf("%s - %d\n", c, (int)t);
    return;
}

