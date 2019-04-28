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

void getRequestFromCreate(char queryParts[10][50], CompiledQuery * compiledQuery)
{
    compiledQuery->type = CREATE;
    compiledQuery->target = queryParts[2];

    int i = 4;
    int columnsIndex = 0;
    while (queryParts[i][0] != PARENTHESE_ENDING)
    {
        if (columnsIndex % 2 == 0)
        {
            compiledQuery->queryColumns[columnsIndex / 2]->name = queryParts[i];
        }
        else
        {
            char * temp = queryParts[i];
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
}

