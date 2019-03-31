#ifndef COMPILED_QUERY_H_
#define COMPILED_QUERY_H_

#include "query_types.h"
#include "consts.h"
#include "compiled_column.h"

typedef struct {
    CompiledColumn * queryColumns[TABLE_MAX_COLUMNS_LENGTH];
    int columnCount;
    QueryTypes type;
    char * target;
} CompiledQuery;

#endif // COMPILED_QUERY_H_
