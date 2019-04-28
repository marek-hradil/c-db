#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "consts.h"
#include "column_types.h"
#include "table.h"
#include "query_types.h"
#include "compiled_query.h"
#include "compiled_column.h"

#define DOUBLE_QUOTE 34
#define SINGLE_QUOTE 39

extern int readHeadTable(FILE * headerFile, Table * table);
extern FILE * getHeaderFile(char * tableName, char mode[2]);
extern void removeFirstLastChar(char * temp);
extern void log(char * msg);

void getRequestFromInsertQuery(char queryParts[10][50], Table * table, CompiledQuery * compiledQuery)
{
    if (strcmp(queryParts[1], "into") != 0)
    {
        log("Target of query not found. Aborting... \n");
        return;
    }

    FILE * headerFile = getHeaderFile(queryParts[2], "rb");
    if (headerFile == NULL)
    {
        log("Unable to open header file.");
    }
    readHeadTable(headerFile, table);
    compiledQuery->type = INSERT;
    compiledQuery->target = queryParts[2];

    char temp[50];
    int i = 0;
    char * token;
    char * rest = queryParts[3];
    int hasColumnsReferenced = 0;

    if (strcmp(queryParts[3],"values") == 0)
    {
        for (i = 0; i < table->info.columnCount; i++)
        {
            compiledQuery->queryColumns[i]->name = table->columns[i]->name;
        }

        compiledQuery->columnCount = table->info.columnCount;
    }
    else if (strcmp(queryParts[4], "values") == 0)
    {
        hasColumnsReferenced = 1;
        strcpy(temp, queryParts[3]);
        removeFirstLastChar(temp);

        i = 0;
        rest = temp;

        while((token = strtok_r(rest, ",", &rest)))
        {
            strcpy(temp, token);
            if (temp[0] == SINGLE_QUOTE || temp[0] == DOUBLE_QUOTE)
            {
                removeFirstLastChar(temp);
            }
            compiledQuery->queryColumns[i]->name = temp;
            i += 1;
        }
        compiledQuery->columnCount = i;
    }
    else
    {
        log("VALUES keyword not found. Aborting...");
        return;
    }

    int valuesIndex = 4 + hasColumnsReferenced;
    // TODO: refactor query parts to better data structure
    strcpy(temp, queryParts[valuesIndex]);
    removeFirstLastChar(temp);
    strcpy(rest,temp);
    int j = 0;
    while((token = strtok_r(rest, ",", &rest)))
    {
        char * val = token;
        if (val[0] == SINGLE_QUOTE || val[0] == DOUBLE_QUOTE)
        {
            removeFirstLastChar(val);
        }
        compiledQuery->queryColumns[j]->value = val;
        j += 1;
    }
}

