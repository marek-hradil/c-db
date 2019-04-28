#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compiled_query.h"
#include "column_types.h"
#include "table.h"

extern int getTypeFromString(char * type);
extern int initTable(char * tableName, Table * table);
extern int addColumn(char * columnName, ColumnTypes columnType, Table * table);

int create(CompiledQuery * compiledQuery, Table * table)
{
    initTable(compiledQuery->target, table);
    int i = 0;

    for (i = 0; i < compiledQuery->columnCount; i++)
    {
        int type = getTypeFromString(compiledQuery->queryColumns[i]->value);
        addColumn(compiledQuery->queryColumns[i]->name, type, table);
    }

    return 1;
}
