#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include "file_paths.h"
#include "file_type.h"

int connectToDbSequence (db)
{
    printf("Connect to DB: \n");
    DIR *catalogue;
    catalogue = opendir(CATALOGUE_DIR);

    listDbsFromCatalogue(catalogue);

    connectToDb(db);

    close(catalogue);
}

char * getHeaderFilePath(char * tableName)
{
    char * tablePath;
    setFilePath(tableName, tablePath, HEADER);
    return tablePath;
}

char * getDataFilePath(char * tableName)
{
    char * tablePath;
    setFilePath(tableName, tablePath, DATA);
    return tablePath;
}

FILE * getHeaderFile (char * tableName, char mode[2])
{
    FILE * headerFile;
    char * tablePath = getHeaderFilePath(tableName);
    headerFile = fopen(tablePath, mode);

    return headerFile;
}

FILE * getDataFile (char * tableName, char mode[2])
{
    FILE * dataFile;
    char * tablePath = getDataFilePath(tableName);
    dataFile = fopen(tablePath, mode);

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
        strcpy(dbPath, CATALOGUE_DIR);
        strcat(dbPath, "/");
        strcat(dbPath, dbName);

        if ((db = opendir(dbPath)) != NULL) {

        }

        if (db == NULL)
        {
            char shouldTryAgain;
            printf("\nDatabase with this name not found. \n Try again? [y/n]: ");
            scanf(" %c", &shouldTryAgain);

            if (shouldTryAgain == 110) {
                return -1;
            } else {
                printf("\nChoose wisely: ");
            }
        }
    } while (db == NULL);
    printf("\nConnected.");

    return 0;
}

void setFilePath(char * tableName, char tablePath[200], int type)
{
    strcpy(tablePath, "./");
    strcat(tablePath, CATALOGUE_DIR);
    strcat(tablePath, "/");
    strcat(tablePath, tableName);
    if (type == HEADER) {
        strcat(tablePath, "_HEAD");
    }
    strcat(tablePath, ".dat");
}
