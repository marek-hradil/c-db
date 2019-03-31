#ifndef COMPILED_QUERY_H_
#define COMPILED_QUERY_H_

#include "query_types.h"
#include "consts.h"
#include "column.h"

typedef struct {
    Column * queryColumns[TABLE_MAX_COLUMNS_LENGTH];
    QueryTypes type;
    char * queryValues[TABLE_MAX_COLUMNS_LENGTH];
    char * target;
} CompiledQuery;

#endif // COMPILED_QUERY_H_
