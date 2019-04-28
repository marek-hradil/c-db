#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "consts.h"
#include "query_types.h"
#include "compiled_query.h"
#include "compiled_column.h"
#include "table.h"

extern FILE * getHeaderFile(char * tableName, char mode[2]);
extern void log(char * msg);

void getRequestFromDeleteQuery(char queryParts[10][50], Table * table, CompiledQuery * compiledQuery)
{
    if (strcmp(queryParts[1], "from") != 0) {
        log("FROM keyword not found");
    }

    FILE * headerFile = getHeaderFile(queryParts[2], "rb");
    if (headerFile == NULL)
    {
        log("Table not found");
        return;
    }
    readHeadTable(headerFile, table);

    fclose(headerFile);

    compiledQuery->target = queryParts[2];
    compiledQuery->type = DELETE;
}
