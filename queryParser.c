#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "consts.h"
#include "compiled_query.h"
#include "table.h"

extern int getRequestFromSelectQuery();
extern int getRequestFromInsertQuery();
extern void log(char * msg);

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
        log("Query parts overflow \n");
    }

    CompiledQuery * compiledQuery = malloc(sizeof(CompiledQuery));
    for (i = 0; i < 10; i++)
    {
        compiledQuery->queryColumns[i] = malloc(sizeof(CompiledColumn));
        if (compiledQuery->queryColumns[i] == NULL)
        {
            log("Compiled column not allocated");
        }
    }
    if (compiledQuery == NULL)
    {
        log("Compiled query alloc failed");
    }
    Table * table = malloc(sizeof(Table));
    if (table == NULL) {
        log("Table alloc failed");
    }

    if (strcmp(queryParts[0], "select") == 0)
    {
        getRequestFromSelectQuery(queryParts, table, compiledQuery);
        executeSelect(compiledQuery, table);
    }
    else if (strcmp(queryParts[0], "insert") == 0)
    {
        getRequestFromInsertQuery(queryParts, table, compiledQuery);
        executeInsert(compiledQuery, table);
    }
    else if (strcmp(queryParts[0], "update") == 0)
    {
        getRequestFromUpdateQuery(queryParts, table, compiledQuery);
        executeUpdate(compiledQuery, table);
    }
    else if (strcmp(queryParts[0], "delete") == 0)
    {
        getRequestFromDeleteQuery(queryParts, table, compiledQuery);
        executeDelete(compiledQuery, table);
    }
    else if (strcmp(queryParts[0], "create") == 0 && strcmp(queryParts[1], "table") == 0)
    {
        getRequestFromCreate(queryParts, compiledQuery);
        create(compiledQuery, table);
    }

    free(compiledQuery);
    free(table);
    return 1;
}
