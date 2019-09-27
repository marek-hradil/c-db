#include <stdio.h>
#include <stdlib.h>
#include "compiled_query.h"
#include "table.h"
#include "column_types.h"
#include "column.h"
#include "compiled_column.h"

extern int insert(CompiledQuery * compiledQueryMock, Table * tableMock, FILE * dataFileMock);
extern int generateHeadFile(Table * table);
extern void setDb();
extern int initTable(char * tableName, Table * table);
extern void log(char * msg);

int testInsert()
{
    Table * table = malloc(sizeof(Table));
    if (table == NULL) {
        log("Not allocated table");
    }
    setDb("./dbs/faceb00k");
    initTable("test_insert", table);

    // Mock data
    CompiledQuery * compiledQueryMock = malloc(sizeof(CompiledQuery));
    if (compiledQueryMock == NULL)
    {
        log("Not allocated compiled query mock");
    }

    compiledQueryMock->type = INSERT;
    compiledQueryMock->columnCount = 2;
    compiledQueryMock->target = "test_insert";

    CompiledColumn column1;
    column1.name = "first";
    column1.value = "ABCDEF";
    compiledQueryMock->queryColumns[0] = &column1;

    CompiledColumn column2;
    column2.name = "second";
    column2.value = "123";
    compiledQueryMock->queryColumns[1] = &column2;

    CompiledColumn column3;
    compiledQueryMock->queryColumns[2] = &column3;

    addColumn("first", VARCHAR, table);
    addColumn("second", INT, table);
    addColumn("third", VARCHAR, table);

    FILE * headerFile = getHeaderFile("test_insert", "rb");
    readHeadTable(headerFile, table);
    fclose(headerFile);

    log("Mocking done. \n");

    // Run command
    log("Running insert command. \n");
    int i = 0;
    for (i = 0; i < 3; i++)
    {
        executeInsert(compiledQueryMock, table);
    }
    log("Command ran. \n");

    free(table);
    free(compiledQueryMock);

    return 0;
}
