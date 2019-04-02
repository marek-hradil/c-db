#include <stdio.h>
#include <stdlib.h>
#include "compiled_query.h"
#include "table.h"

int select(CompiledQuery * compiledQuery, Table * table, FILE * dataFile)
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
                        printf("| %s |", val);
                    }
                    else
                    {
                        printf("| NULL |");
                    }
                } break;

                case INT: {
                    int val;
                    fread(&val, sizeof(int), 1, dataFile);
                    printf("| %d |", val);
                } break;

                default:
                    printf("Unknown column type. Aborting.");
                    return -1;
            }
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
                    printf("Unknown column type. Aborting.");
                    return -1;
            }
        }
    }

    printf("\n");

    return 1;
}
