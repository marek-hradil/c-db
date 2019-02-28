#include <stdio.h>
#include <stdlib.h>

extern int connectToDbSequence();
extern void getRequestFromQuery();
extern int initTable();

int main()
{
    printf("Starting... \n");
    getRequestFromQuery();

    return 0;
}
