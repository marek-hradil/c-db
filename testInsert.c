#include <stdio.h>
#include <stdlib.h>
#include "compiled_query.h"
#include "table.h"
#include "column_types.h"
#include "column.h"
#include "compiled_column.h"

extern int insert(CompiledQuery *compiledQueryMock, Table * tableMock, FILE * dataFileMock);
extern int generateHeadFile(Table * table);
extern void setDb();

int testInsert()
{
    setDb("./dbs/faceb00k");
    initTable("test_insert");
    FILE * testFile = getDataFile("test_insert", "ab");

    // Mock data
    CompiledQuery * compiledQueryMock = malloc(sizeof(CompiledQuery));

    compiledQueryMock->type = INSERT;
    compiledQueryMock->columnCount = 2;

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

    addColumn("first", VARCHAR, "test_insert");
    addColumn("second", INT, "test_insert");
    addColumn("third", VARCHAR, "test_insert");

    FILE * headerFile = getHeaderFile("test_insert", "rb");
    Table * table = readHeadTable(headerFile);
    fclose(headerFile);

    printf("Mocking done. \n");

    // Run command
    printf("Running insert command. \n");
    int i = 0;
    for (i = 0; i < 3; i++)
    {
        insert(compiledQueryMock, table, testFile);
    }
    printf("Command ran. \n");

    fclose(testFile);

    // Read back values
    testFile = fopen("./dbs/faceb00k/test_insert.dat", "rb");

    for (i = 0; i < 3; i++)
    {
        char val1[70];
        fread(val1, sizeof(char[70]), 1, testFile);

        int val2;
        fread(&val2, sizeof(int), 1, testFile);

        char val3[70];
        fread(val3, sizeof(char[70]), 1, testFile);

        // Print
        printf("first: %s, second: %d, third: %s \n", val1, val2, val3);
    }

    fclose(testFile);
    free(table);
    free(compiledQueryMock);

    return 0;
}
