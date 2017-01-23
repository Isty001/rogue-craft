#include <stdio.h>
#include <memory.h>
#include "storage.h"


StorageError buffer_new(Buffer *buff, char *file)
{
    buff->content = NULL;

    if (NULL == (buff->file = fopen(file, "wa+"))) {
        return SE_IO;
    }

    return SE_OK;
}

void buffer_append(Buffer *buffer, char *content)
{
    char *tmp = NULL;

    if (!buffer->content) {
        asprintf(&tmp, content);
    } else {
        asprintf(&tmp, "%s%s", buffer->content, content);
    }

    free(buffer->content);
    buffer->content = tmp;
}

StorageError buffer_persist(Buffer *buffer)
{
    size_t len = strlen(buffer->content);

    return 1 == fwrite(buffer->content, len, 1, buffer->file) ? SE_OK : SE_IO;
}

void buffer_free(Buffer *buffer)
{
    if (buffer->file) {
        fclose(buffer->file);
    }

    if (buffer->content) {
        free(buffer->content);
    }
}
