#include <stdio.h>
#include <stdlib.h>

#include "consts.h"
#include "column_types.h"
#include "table.h"
#include "query_types.h"
#include "compiled_query.h"

CompiledQuery getRequestFromSelectQuery(char* query)
{
    CompiledQuery compiledQuery;
    compiledQuery.type = SELECT;

    char str[QUERY_MAX_LENGTH];
    strncpy(str, query, sizeof str - 1);
    str[QUERY_MAX_LENGTH - 1] = "\0";

    char * pch;
    pch = strtok(str,", ");

    int index = 0;

    while (pch != NULL)
    {
        strcpy(compiledQuery.queryColumns[index], pch);
        printf("%s", compiledQuery.queryColumns[index]->name);
        index += 1;
        pch = strtok(NULL, ", ");
    }

    return compiledQuery;
}

