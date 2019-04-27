#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "consts.h"
#include "column_types.h"
#include "table.h"
#include "query_types.h"
#include "compiled_query.h"
#include "compiled_column.h"

#define PARENTHESE_ENDING 41
#define COMMA 44

extern void removeFirstLastChar(char * temp);
extern void log(char * msg);

CompiledQuery * getRequestFromCreate(char queryParts[10][50])
{
    CompiledQuery * compiledQuery = malloc(sizeof(CompiledQuery));
    if (compiledQuery == NULL) {
        printf("Not allocated");
    }
    compiledQuery->type = CREATE;
    compiledQuery->target = queryParts[2];

    int i = 4;
    int columnsIndex = 0;
    char temp[50];
    while (strcmp(queryParts[i][0], PARENTHESE_ENDING) != 0)
    {
        printf("works");
        if (columnsIndex % 2 == 0)
        {
            CompiledColumn * column = malloc(sizeof(CompiledColumn));
            column->name = queryParts[i];
            compiledQuery->queryColumns[columnsIndex / 2] = column;
        }
        else
        {
            strcpy(temp, queryParts[i]);
            int valueLength = strlen(temp);
            if (temp[valueLength] == COMMA)
            {
                temp[valueLength] = '\0';
            }

            if (strcmp(temp, "int") != 0 && strcmp(temp, "varchar"))
            {
                log("Unknown type. Aborting");
                return;
            }

            compiledQuery->queryColumns[columnsIndex / 2]->value = temp;
        }
        i += 1;
    }

    compiledQuery->columnCount = columnsIndex / 2;

    return compiledQuery;
}

