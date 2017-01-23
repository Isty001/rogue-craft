#ifndef ROGUECRAFT_SERIALIZER_H
#define ROGUECRAFT_SERIALIZER_H


#include <stdlib.h>
#include <limits.h>


typedef enum {
    SE_OK,
    SE_IO,
} StorageError;

typedef struct {
    char *content;
    FILE *file;
} Buffer;


StorageError buffer_new(Buffer *buff, char *file);

void buffer_append(Buffer *buffer, char *content);

StorageError buffer_persist(Buffer *buffer);

void buffer_free(Buffer *buffer);


#endif
