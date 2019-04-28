#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "consts.h"
#include "column_types.h"
#include "table.h"
#include "query_types.h"
#include "compiled_query.h"
#include "compiled_column.h"

extern int readHeadTable(FILE * headerFile, Table * table);
extern FILE * getHeaderFile(char * tableName, char mode[2]);
extern void removeFirstLastChar(char * temp);
extern void log(char * msg);

void getRequestFromUpdateQuery(char queryParts[10][50], Table * table, CompiledQuery * compiledQuery)
{
    FILE * headerFile = getHeaderFile(queryParts[1], "rb");
    if (headerFile == NULL)
    {
        log("Table not found");
        return;
    }
    readHeadTable(headerFile, table);

    compiledQuery->type = UPDATE;
    compiledQuery->target = queryParts[1];

    if (strcmp(queryParts[2], "set") != 0)
    {
        log("SET keyword not found");
        return;
    }

    int i = 0;
    char * token;
    char * rest = queryParts[3];

    int j = 0;
    while((token = strtok_r(rest, ",=", &rest)))
    {
        char * temp = token;
        if (temp[0] == 34 || temp[0] == 39)
        {
            removeFirstLastChar(temp);
        }

        if (j % 2 == 0)
        {
            compiledQuery->queryColumns[j/2]->name = temp;
        }
        else
        {
            compiledQuery->queryColumns[j/2]->value = temp;
        }

        j += 1;
    }

    printf("out");
}

