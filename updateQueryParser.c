#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "consts.h"
#include "column_types.h"
#include "table.h"
#include "query_types.h"
#include "compiled_query.h"
#include "compiled_column.h"

extern int readHeadTable(FILE * headerFile);
extern FILE * getHeaderFile(char * tableName, char mode[2]);
extern void removeFirstLastChar(char * temp);
extern void log(char * msg);

CompiledQuery * getRequestFromUpdateQuery(char queryParts[10][50])
{
    FILE * headerFile = getHeaderFile(queryParts[1], "rb");
    if (headerFile == NULL)
    {
        log("Table not found");
        return;
    }

    CompiledQuery * compiledQuery = malloc(sizeof(CompiledQuery));
    int i = 0;
    for (i = 0; i < 3; i++) {
        compiledQuery->queryColumns[i] = malloc(sizeof(CompiledColumn));
        if (compiledQuery->queryColumns[i] == NULL)
        {
            log("Query column allocation failed.");
        }
    }
    Table * table = readHeadTable(headerFile);
    fclose(headerFile);

    compiledQuery->type = UPDATE;
    compiledQuery->target = queryParts[1];

    if (strcmp(queryParts[2], "set") != 0)
    {
        log("SET keyword not found");
        return;
    }

    char temp[50];
    i = 0;
    char * token;
    char * rest = queryParts[3];
    strcpy(temp, queryParts[3]);
    strcpy(rest,temp);

    int j = 0;
    while((token = strtok_r(rest, ",=", &rest)))
    {
        strcpy(temp, token);
        if (temp[0] == 34 || temp[0] == 39)
        {
            removeFirstLastChar(temp);
        }

        if (j % 2 == 0)
        {
            strcpy(compiledQuery->queryColumns[j/2]->name,temp);
        }
        else
        {
            strcpy(compiledQuery->queryColumns[j/2]->value,temp);
        }

        j += 1;
    }

    return compiledQuery;
}

