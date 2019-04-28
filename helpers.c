#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "column_types.h"

void removeFirstLastChar(char * temp)
{
    int i = 0;
    while(temp[i+2]!='\0')
    {
        temp[i] = temp[i+1];
        i += 1;
    }
    temp[i]='\0';
}

int getTypeFromString(char * type)
{
    if (strcmp(type, "int") == 0)
    {
        return INT;
    }
    else if (strcmp(type, "varchar") == 0)
    {
        return VARCHAR;
    }
}
