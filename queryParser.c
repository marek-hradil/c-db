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

    if (strcmp(keyword, "SELECT")) {
        getRequestFromSelectQuery();
    }
}
