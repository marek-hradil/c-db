#define TABLE_INFO_MAX_NAME_LENGTH 100

#ifndef TABLE_INFO_INCLUDED_
#define TABLE_INFO_INCLUDED_

typedef struct {
    char name[TABLE_INFO_MAX_NAME_LENGTH];
    int rowCount;
    int columnCount;
    int rowSize;
} TableInfo;

#endif // TABLE_INFO_INCLUDED_
