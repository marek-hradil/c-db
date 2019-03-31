#include <stdio.h>
#include <stdlib.h>
#include "compiled_query.h"
#include "table.h"
#include "column_types.h"
#include "column.h"

extern int connectToDbSequence();
extern void getRequestFromQuery();
extern int initTable();
extern int insert();

int main()
{
    testInsert();
    return 0;
}

void testInsert()
{
    printf("Unit test on inserting into data file.");

    FILE * testFile;
    testFile = fopen("./test/test_insert.dat", "wb");

    // Mock data
    CompiledQuery * compiledQueryMock;
    compiledQueryMock = malloc(sizeof(CompiledQuery));

    compiledQueryMock.type = INSERT;

    strcpy(compiledQueryMock.queryColumns[0]->name, "first");
    compiledQueryMock.queryColumns[0]->type = INT;

    strcpy(compiledQueryMock.queryColumns[1]->name, "second");
    compiledQueryMock.queryColumns[1]->type = VARCHAR;

    strcpy(compiledQueryMock.queryValues[0], "ABCDEF");
    strcpy(compiledQueryMock.queryValues[1], "1234");

    Table table;
    strcpy(table.columns[0]->name, "first");
    table.columns[0]->type = INT;

    strcpy(table.columns[1]->name, "second");
    table.columns[1]->type = VARCHAR;

    insert(compiledQueryMock, table, testFile);
}
