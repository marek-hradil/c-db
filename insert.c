#include <stdio.h>
#include <stdlib.h>
#include "compiled_query.h"
#include "table.h"

int insert(CompiledQuery * compiledQuery, Table * table, FILE * dataFile)
{
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
                || strcmp(table->columns[j]->name, compiledQuery->queryColumns[i]->name)
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
            switch(table->columns[j]->type) {
                case VARCHAR: {
                    char val[VARCHAR_DEFAULT_LENGTH];
                    memset(val, '\0', sizeof(val));
                    fwrite(val, sizeof(char[70]), 1, dataFile);
                } break;

                case INT: {
                    int val = 0;
                    fwrite(&val, sizeof(int), 1, dataFile);
                } break;

                default: {
                    printf("Insert aborted, unknown type.");
                    return -1;
                }
            }
        }
    }

    table->info.rowCount += 1;
    return 1;
}
