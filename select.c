#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiled_query.h"
#include "table.h"

extern void log(char * msg);
extern FILE * getDataFile(char * tableName, char * mode);

int select(CompiledQuery * compiledQuery, Table * table, FILE * dataFile, int filePointer)
{
    rewind(dataFile);
    fseek(dataFile, filePointer, SEEK_SET);
    printf("{\n");

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

            isPaired = 1;
            switch(table->columns[i]->type)
            {
                case VARCHAR: {
                    char val[VARCHAR_DEFAULT_LENGTH];
                    fread(val, sizeof(char[VARCHAR_DEFAULT_LENGTH]), 1, dataFile);
                    if (val[0] != '\0')
                    {
                        printf("\t\"%s\": \"%s\"", table->columns[i]->name, val);
                    }
                    else
                    {
                        printf("\t\"%s\": null", table->columns[i]->name);
                    }
                } break;

                case INT: {
                    int val;
                    fread(&val, sizeof(int), 1, dataFile);
                    printf("\t\"%s\": %d", table->columns[i]->name, val);
                } break;

                default:
                    log("Unknown column type. Aborting.");
                    return -1;
            }

            if (compiledQuery->columnCount - 1 != j && isPaired)
            {
                printf(",");
            }
            printf("\n");
        }

        if (isPaired == 0)
        {
            switch(table->columns[i]->type)
            {
                case VARCHAR: {
                    fseek(dataFile, sizeof(char[VARCHAR_DEFAULT_LENGTH]), SEEK_SET);
                } break;

                case INT: {
                    fseek(dataFile, sizeof(int), SEEK_SET);
                } break;

                default:
                    log("Unknown column type. Aborting.");
                    return -1;
            }
        }
    }

    printf("}");

    return 1;
}

int executeSelect(CompiledQuery * compiledQuery, Table * table)
{
    int i = 0;
    int pointer = 0;
    int status = 0;

    FILE * dataFile = getDataFile(compiledQuery->target, "rb");
    if (dataFile == NULL)
    {
        log("Couldn't open data file");
        return;
    }

    table->info.rowCount = getRowCount(dataFile, table->info.rowSize);

    printf("[");
    for (i = 0; i < table->info.rowCount; i++)
    {
        status = select(compiledQuery, table, dataFile, pointer);
        if (table->info.rowCount -1 != i)
        {
            printf(",");
        }
        pointer += table->info.rowSize;
    }
    printf("]\n");

    fclose(dataFile);

    return status;
}
