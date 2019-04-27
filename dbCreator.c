#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "column.h"
#include "column_types.h"
#include "table.h"
#include "table_info.h"
#include "file_paths.h"
#include "file_type.h"
#include "compiled_query.h"

extern FILE * getHeaderFile(char * tableName, char mode[2]);
extern FILE * getDataFile(char * tableName, char mode[2]);
extern int removeDataFile(char * tableName);
extern int removeHeaderFile(char * tableName);
extern void log(char * msg);

int generateHeadFile(Table * table)
{
    FILE * newHeaderFile = getHeaderFile(table->info.name, "wb");
    if (newHeaderFile == NULL)
    {
        printf("Could not create new header file");

        return -1;
    }

    fwrite(&table->info, sizeof(TableInfo), 1, newHeaderFile);
    fclose(newHeaderFile);

    printf("Header file created successfully");

    return 0;
}

int generateDataFile(Table * table)
{
    FILE * newDataFile;
    newDataFile = getDataFile(table->info.name, "wb");

    if (newDataFile == NULL) {
        printf("Could not create new data file");

        return -1;
    }

    fclose(newDataFile);

    printf("Data file created successfully");

    return 0;
}

Column * buildNewColumn(char * name, ColumnTypes type)
{
    Column * newColumn = malloc(sizeof(Column));
    if (newColumn == NULL) {
        log("Column alloc failed.");
    }
    strcpy(newColumn->name, name);
    newColumn->type = type;

    return newColumn;
}

Table * readHeadTable (FILE * headerFile)
{
    Table * newTable = malloc(sizeof(Table));
    if (newTable == NULL) {
        log("Table alloc failed");
    }
    fread(&newTable->info, sizeof(TableInfo), 1, headerFile);
    int i = 0;
    while (fread(&newTable->columns[i], sizeof(Column), 1, headerFile) > 0)
    {
        i += 1;
    }

    return newTable;
}

int addColumn(char * columnName, ColumnTypes columnType, char * tableName)
{
    FILE * headerFile = getHeaderFile(tableName, "rb");
    if (headerFile == NULL)
    {
        printf("Couldn't get header file. Aborting.");
        return -1;
    }
    Table * table = readHeadTable(headerFile);
    table->columns[table->info.columnCount] = buildNewColumn(columnName, columnType);
    table->info.columnCount += 1;

    if (columnType == VARCHAR)
    {
        table->info.rowSize += sizeof(char[VARCHAR_DEFAULT_LENGTH]);
    }
    else if (columnType == INT)
    {
        table->info.rowSize += sizeof(int);
    }

    fclose(headerFile);
    headerFile = getHeaderFile(tableName, "wb");
    fwrite(&table->info, sizeof(TableInfo), 1, headerFile);
    int i = 0;
    for (i = 0; i < table->info.columnCount; i++)
    {
        fwrite(&table->columns[i], sizeof(Column), 1, headerFile);
    }
    fclose(headerFile);
    free(table);

    return 0;
}

int deleteColumn(char * columnName, char * tableName)
{
    FILE * headerFile = getHeaderFile(tableName, "rb");
    if (headerFile == NULL)
    {
        printf("Couldn't get header file. Aborting.");
        return -1;
    }

    Table * table = readHeadTable(headerFile);
    Column newColumns[table->info.columnCount];
    int i = 0;
    for (i = 0; i < table->info.columnCount; i++)
    {
        if (strcmp(&table->columns[i]->name, columnName) == 0)
        {

        }
    }

    // TODO: Remove from data file!
}

int initTable(char * tableName)
{
    Table * table = malloc(sizeof(Table));
    table->info.rowCount = 0;
    table->info.columnCount = 0;
    table->info.rowSize = 0;
    strcpy(table->info.name, tableName);

    int headerFileCreated = generateHeadFile(table);
    int dataFileCreated = generateDataFile(table);
    free(table);

    if (headerFileCreated == 0 && dataFileCreated == 0)
    {
        return 0;
    }
    return -1;
}

int deleteTable(char * tableName)
{
    int headerFileRemoved = removeHeaderFile(tableName);
    int dataFileRemoved = removeDataFile(tableName);
    if (headerFileRemoved != 0 || dataFileRemoved != 0)
    {
        printf("Table wasn't deleted, error occurred");
        return -1;
    }

    return 0;
}

int getRowCount(char * tableName, int rowSize)
{
    FILE * dataFile = getDataFile(tableName, "rb");
    if (dataFile == NULL)
    {
        printf("Couldn't get data file. Aborting. \n");
        return -1;
    }

    fseek(dataFile, 0L, SEEK_END);
    int fileSize = ftell(dataFile);
    fclose(dataFile);

    return fileSize / rowSize;
}
