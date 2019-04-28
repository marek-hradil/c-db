#include <stdio.h>
#include <stdlib.h>
#include "compiled_query.h"
#include "table.h"

int update(CompiledQuery * compiledQuery, Table * table, FILE * dataFile, int filePointer)
{
    rewind(dataFile);
    fseek(dataFile, filePointer, SEEK_SET);

    int i = 0;
    for (i = 0; i < table->info.columnCount; i++)
    {
        int j = 0;
        int isPaired = 0;
        for (j = 0; j < compiledQuery->columnCount; j++)
        {
            if (
                isPaired != 0
                || !compiledQuery->queryColumns[i]->name
                || strcmp(table->columns[i]->name, compiledQuery->queryColumns[j]->name)
            ) {
                continue;
            }

            switch(table->columns[j]->type) {
                case VARCHAR: {
                    // Default VARCHAR 70
                    // TODO: Support various length of columns
                    char val[VARCHAR_DEFAULT_LENGTH];
                    strcpy(val, compiledQuery->queryColumns[i]->value);
                    fwrite(val, sizeof(char[VARCHAR_DEFAULT_LENGTH]), 1, dataFile);
                } break;

                case INT: {
                    char val[INT_DEFAULT_LENGTH];
                    strcpy(val, compiledQuery->queryColumns[i]->value);
                    int intVal = atoi(val);
                    fwrite(&intVal, sizeof(int), 1, dataFile);
                } break;

                default: {
                    printf("Insert aborted, unknown type.");
                    return -1;
                }
            }
            isPaired = 1;
        }

        if (isPaired == 0) {
            switch(table->columns[i]->type)
            {
                case VARCHAR: {
                    fseek(dataFile, sizeof(char[VARCHAR_DEFAULT_LENGTH]), SEEK_SET);
                } break;

                case INT: {
                    fseek(dataFile, sizeof(int), SEEK_SET);
                } break;

                default:
                    printf("Unknown column type. Aborting.");
                    return -1;
            }
        }
    }
}

int executeUpdate(CompiledQuery * compiledQuery, Table * table)
{
    int i = 0;
    int pointer = 0;
    int status = 0;

    FILE * dataFile = getDataFile(compiledQuery->target, "rb");

    for (i = 0; i < compiledQuery->columnCount; i++)
    {
        status = update(compiledQuery, table, dataFile, pointer);
        pointer += table->info.rowSize;
    }

    fclose(dataFile);

    return status;
}
