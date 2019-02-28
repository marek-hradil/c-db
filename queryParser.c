#include <stdio.h>
#include <stdlib.h>
#include "consts.h"

extern void getRequestFromSelectQuery();

void getRequestFromQuery()
{
    char query[QUERY_MAX_LENGTH];
    printf("\n$:: ");
    scanf(" %99[^\n]", query);

    char *queryCopy = query;
    char *keyword;

    keyword = strtok_r(queryCopy, " ", &queryCopy);

    char select[6] = "SELECT";

    if (strcmp(keyword,select) != 0)
    {
        getRequestFromSelectQuery(queryCopy);
    }
    else
    {
        printf("Unknown command");
    }
}
