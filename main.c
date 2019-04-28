#include <stdio.h>
#include <stdlib.h>

extern int testInsert();
extern int testSelect();
extern void getRequestFromQuery();

int main()
{
    connectToDbSequence();
    while (1)
    {
        getRequestFromQuery();
    }

    return 0;
}

