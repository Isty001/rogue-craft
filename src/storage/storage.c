#include <stdio.h>
#include <sys/stat.h>
#include <ftw.h>
#include <errno.h>
#include "../util.h"
#include "storage.h"


#define ROOT_DIR_NAME ".rogue-craft"


static char ROOT_DIR[100];


void storage_rm_rf(char *path)
{
    nftw(path, (__nftw_func_t) remove, 0, 0);
}

static StorageError create_dir(char *dir)
{
    if (0 != mkdir(dir, 0600) && errno != EEXIST) {
        return SE_CREATE_DIR;
    }

    return SE_OK;
}

void storage_init(char *base_dir)
{
    sprintf(ROOT_DIR, "%s/"ROOT_DIR_NAME, base_dir);

    if (SE_OK != create_dir(ROOT_DIR)) {
        fatal("Unable to create [%s] directory with permission [0600]", ROOT_DIR);
    }
}

StorageError storage_save(char *name, Player *player)
{
    StorageError err;
    char path[100 + strlen(name)];

    sprintf(path, "%s/%s", ROOT_DIR, name);

    if (SE_OK != (err = create_dir(path))) {
        return err;
    }

    return err;
}
