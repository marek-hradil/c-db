#include "column.h"

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

enum ExpressionTypes {
    IS,
    IS_NOT,
};

typedef struct {
    char * columnName;
    ExpressionTypes expressionType;
    char * value;
} Expression;

#endif // EXPRESSION_H_
