#ifndef ROGUECRAFT_STORAGE_H
#define ROGUECRAFT_STORAGE_H


#include "../player/player.h"


typedef enum {
    SE_OK,
    SE_CREATE_DIR
} StorageError;


void storage_init(char *base_dir);

void storage_rm_rf(char *path);

StorageError storage_save(char *name, Player *player);


#endif
