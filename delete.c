#include <stdio.h>
#include <stdlib.h>
#include "table.h"
#include "compiled_query.h"

extern FILE * getDataFile(char * tableName, char mode[2]);

int delete(CompiledQuery * compiledQuery, Table * table, FILE * dataFile, int filePointer)
{
    rewind(dataFile);
    fseek(dataFile, filePointer, SEEK_CUR);

    unsigned char buffer[table->info.rowSize];
    int tableSize = table->info.rowSize * table->info.rowCount;
    while (ftell(dataFile) <= tableSize - (table->info.rowSize * 2)) {
        printf("%d = %d \n", ftell(dataFile), tableSize);
        fseek(dataFile, filePointer + table->info.rowSize, SEEK_SET);
        printf("%d \n", ftell(dataFile));
        fread(buffer, 1, sizeof buffer, dataFile);
        printf("%d \n", ftell(dataFile));
        fseek(dataFile, filePointer -(table->info.rowSize * 2), SEEK_SET);
        printf("%d \n", ftell(dataFile));
        fwrite(buffer, 1, sizeof buffer, dataFile);
        printf("%d \n", ftell(dataFile));
        fseek(dataFile, filePointer + table->info.rowSize, SEEK_SET);
        printf("%d \n", ftell(dataFile));
    }

    ftruncate(fileno(dataFile), tableSize - table->info.rowSize);
    table->info.rowCount -= 1;
}

int executeDelete(CompiledQuery * compiledQuery, Table * table)
{
    int i = 0;
    int pointer = 0;
    int status = 0;

    FILE * dataFile = getDataFile(compiledQuery->target, "rb");

    for (i = 0; i < compiledQuery->columnCount; i++)
    {
        status = delete(compiledQuery, table, dataFile, pointer);
        pointer += table->info.rowSize;
    }

    fclose(dataFile);

    return status;
}
