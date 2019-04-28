#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "consts.h"
#include "column_types.h"
#include "table.h"
#include "query_types.h"
#include "compiled_query.h"

extern int readHeadTable(FILE * headerFile, Table * table);
extern FILE * getHeaderFile(char * tableName, char mode[2]);
extern void log(char * msg);

void getRequestFromSelectQuery(char queryParts[10][50], Table * table, CompiledQuery * compiledQuery)
{
    compiledQuery->type = SELECT;
    if (strcmp(queryParts[2], "from") != 0)
    {
        log("FROM keyword not found.\n");
        return;
    }

    compiledQuery->target = queryParts[3];

    FILE * headerFile = getHeaderFile(queryParts[3], "rb");
    if (headerFile == NULL)
    {
        log("Table not found.");
        return;
    }

    readHeadTable(headerFile, table);

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
                strcpy(compiledQuery->queryColumns[i]->name, temp);
                isPaired = 1;
                i++;
            }
        }

        if (isPaired == 0) {
            printf("Column %s not found. Aborting...", temp);
            return;
        }
    }

    if (i != 0) {
        compiledQuery->columnCount = i;
    } else {
        printf("No column found. Aborting...");
        return;
    }
}

