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

extern FILE * getHeaderFile(char * tableName, char * mode);
extern FILE * getDataFile(char * tableName, char * mode);
extern int removeDataFile(char * tableName);
extern int removeHeaderFile(char * tableName);
extern void log(char * msg);

int generateHeadFile(Table * table)
{
    FILE * newHeaderFile = getHeaderFile(table->info.name, "wb");
    if (newHeaderFile == NULL)
    {
        log("Could not create new header file");

        return -1;
    }

    fwrite(&table->info, sizeof(TableInfo), 1, newHeaderFile);
    fclose(newHeaderFile);

    log("Header file created successfully");

    return 0;
}

int generateDataFile(Table * table)
{
    FILE * newDataFile;
    newDataFile = getDataFile(table->info.name, "wb");

    if (newDataFile == NULL) {
        log("Could not create new data file");

        return -1;
    }

    fclose(newDataFile);

    log("Data file created successfully");

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

void readHeadTable(FILE * headerFile, Table * table)
{
    fread(&table->info, sizeof(TableInfo), 1, headerFile);
    int i = 0;
    while (fread(&table->columns[i], sizeof(Column), 1, headerFile) > 0)
    {
        i += 1;
    }
}

int addColumn(char * columnName, ColumnTypes columnType, Table * table)
{
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

    FILE * headerFile = getHeaderFile(table->info.name, "wb");
    fwrite(&table->info, sizeof(TableInfo), 1, headerFile);
    int i = 0;
    for (i = 0; i < table->info.columnCount; i++)
    {
        fwrite(&table->columns[i], sizeof(Column), 1, headerFile);
    }
    fclose(headerFile);

    return 0;
}

int initTable(char * tableName, Table * table)
{
    table->info.rowCount = 0;
    table->info.columnCount = 0;
    table->info.rowSize = 0;
    strcpy(table->info.name, tableName);

    int headerFileCreated = generateHeadFile(table);
    int dataFileCreated = generateDataFile(table);

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
        log("Table wasn't deleted, error occurred");
        return -1;
    }

    return 0;
}

int getRowCount(FILE * dataFile, int rowSize)
{
    fseek(dataFile, 0L, SEEK_END);
    int fileSize = ftell(dataFile);

    return fileSize / rowSize;
}
