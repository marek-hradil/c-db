#include "column_types.h"

#define COLUMN_MAX_NAME_LENGTH 256

#ifndef COLUMN_H_
#define COLUMN_H_

typedef struct column {
    char name[COLUMN_MAX_NAME_LENGTH];
    ColumnTypes type;
} Column;

#endif // COLUMN_H_
