#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "file_paths.h"
#include "file_type.h"

extern void log(char * msg);

char databasePath[100];

void setDb(char * name)
{
    strcpy(databasePath, name);
}

char * getDb()
{
    char * ptr = databasePath;
    return ptr;
}

void setFilePath(char * tableName, int type, char tablePath[100])
{
    strcpy(tablePath, databasePath);
    strcat(tablePath, "/");
    strcat(tablePath, tableName);
    if (type == HEADER) {
        strcat(tablePath, "_HEAD");
    }
    strcat(tablePath, ".dat");
}

int removeHeaderFile(char * tableName)
{
    char tablePath[100];
    setFilePath(tableName, HEADER, tablePath);
    int status = remove(tablePath);
    return status;
}

int removeDataFile(char * tableName)
{
    char tablePath[100];
    setFilePath(tableName, DATA, tablePath);
    int status = remove(tableName);
    return status;
}

int connectToDbSequence()
{
    printf("Connect to DB: \n");
    DIR * catalogue;
    catalogue = opendir(CATALOGUE_DIR);

    listDbsFromCatalogue(catalogue);

    connectToDb(catalogue);

    close(catalogue);
}

FILE * getHeaderFile (char * tableName, char * mode)
{
    char tablePath[100];
    setFilePath(tableName, HEADER, tablePath);
    FILE * headerFile = fopen(tablePath, mode);
    if (headerFile == NULL)
    {
        printf("Couldn't get header file at this path: %s\n", tablePath);
    }

    return headerFile;
}

FILE * getDataFile (char * tableName, char * mode)
{
    char tablePath[100];
    setFilePath(tableName, DATA, tablePath);
    FILE * dataFile = fopen(tablePath, mode);
    if (dataFile == NULL)
    {
        printf("Couldn't get data file at this path: %s\n", tablePath);
    }
    return dataFile;
}

int listDbsFromCatalogue (DIR *catalogue)
{
    struct dirent *db;
    if (catalogue!=NULL)
    {
        while(db = readdir(catalogue))
        {
            if (db->d_type == DT_DIR)
            {
                printf("\t %s \n",db->d_name);
            }
        }
        free(db);
        return 0;
    }
    else
    {
        printf("ERROR OPENIN DIRECTORY");
        return -1;
    }
}

int connectToDb(DIR * catalogue)
{
    char dbName[256];
    printf("\nChoose wisely: ");

    DIR * db;
    do {
        scanf("%256s", dbName);

        char dbPath[300];
        strcpy(dbPath, "./");
        strcat(dbPath, CATALOGUE_DIR);
        strcat(dbPath, "/");
        strcat(dbPath, dbName);

        if ((db = opendir(dbPath)) != NULL) {
            setDb(dbPath);
        }

        if (db == NULL)
        {
            char shouldTryAgain;
            printf("\nDatabase with this name not found. \n Try again? [y/n]: ");
            scanf(" %c", &shouldTryAgain);

            if (shouldTryAgain == 110) {
                return -1;
            } else {
                printf("\n$:: ");
            }
        }
    } while (db == NULL);
    free(db);
    printf("\nConnected.");

    return 0;
}
