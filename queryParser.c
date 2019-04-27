#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "consts.h"
#include "compiled_query.h"

extern int getRequestFromSelectQuery();
extern int getRequestFromInsertQuery();

int getRequestFromQuery()
{
    char query[QUERY_MAX_LENGTH];
    printf("\n$:: ");
    scanf(" %299[^\n]", query);

    char * token;
    char * rest = query;
    char queryParts[10][50];
    int i = 0;

    while ((token = strtok_r(rest, " ", &rest)))
    {
        int j = 0;
        int length = strlen(token);
        char temp[length];

        strcpy(temp, token);

        for (j = 0; j < length; j++)
        {
            temp[j] = tolower(temp[j]);
        }

        temp[length] = '\0';
        strcpy(queryParts[i], temp);
        i++;
    }

    if (i > 19)
    {
        printf("Query parts overflow \n");
    }

    CompiledQuery * compiledQuery;
    if (strcmp(queryParts[0], "select") == 0)
    {
        compiledQuery = getRequestFromSelectQuery(queryParts);
    }
    else if (strcmp(queryParts[0], "insert") == 0)
    {
        compiledQuery = getRequestFromInsertQuery(queryParts);
    }
    else if (strcmp(queryParts[0], "update") == 0)
    {
        compiledQuery = getRequestFromUpdateQuery(queryParts);
    }
    else if (strcmp(queryParts[0], "delete") == 0)
    {
        compiledQuery = getRequestFromDeleteQuery(queryParts);
    }
    else if (strcmp(queryParts[0], "create") == 0 && strcmp(queryParts[1], "table") == 0)
    {
        compiledQuery = getRequestFromCreate(queryParts);
        printf("%s %s", compiledQuery->queryColumns[0]->name, compiledQuery->queryColumns[0]->value);
    }

    free(compiledQuery);
    return 1;
}
