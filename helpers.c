#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
