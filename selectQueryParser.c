#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "consts.h"
#include "column_types.h"
#include "table.h"
#include "query_types.h"
#include "compiled_query.h"

extern int readHeadTable(FILE * headerFile);
extern FILE * getHeaderFile(char * tableName, char mode[2]);

CompiledQuery * getRequestFromSelectQuery(char queryParts[10][50])
{
    CompiledQuery * compiledQuery = malloc(sizeof(CompiledQuery));
    compiledQuery->type = SELECT;
    if (strcmp(queryParts[2], "from") != 0)
    {
        printf("Target of query not found. Aborting... \n");
        return;
    }

    compiledQuery->target = queryParts[3];

    FILE * headerFile = getHeaderFile(queryParts[3], "rb");
    if (headerFile == NULL)
    {
        printf("Table not found");
        return;
    }

    Table * table = readHeadTable(headerFile);

    char * token;
    char * rest = queryParts[1];
    char temp[50];
    int i = 0;

    while ((token = strtok_r(rest, ",", &rest)))
    {
        strcpy(temp, token);
        int j = 0;
        int isPaired = 0;
        for (j = 0; j < table->info.columnCount; j++)
        {
            if (strcmp(temp, table->columns[j]->name) == 0)
            {
                compiledQuery->queryColumns[i]->name = temp;
                isPaired = 1;
                i++;
            }
        }

        if (isPaired == 0) {
            printf("Column %s not found. Aborting...", temp);
            return;
        }
    }

    fclose(headerFile);
    free(table);

    if (i != 0) {
        compiledQuery->columnCount = i;
    } else {
        printf("No column found. Aborting...");
        return;
    }

    return compiledQuery;
}

