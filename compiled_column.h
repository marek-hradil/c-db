#include "column_types.h"
#include "column.h"

#ifndef COMPILED_COLUMN_H_
#define COMPILED_COLUMN_H_

typedef struct {
    char * name;
    ColumnTypes type;
    char * value;
} CompiledColumn;

#endif // COMPILED_COLUMN_H_
