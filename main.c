#include <stdio.h>
#include <stdlib.h>

extern int testInsert();
extern int testSelect();
extern void getRequestFromQuery();

int main()
{
    testInsert();
    //testSelect();
    connectToDbSequence();
    getRequestFromQuery();

    return 0;
}

