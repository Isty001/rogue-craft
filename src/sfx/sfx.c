#include <vlc/vlc.h>
#include <limits.h>
#include <glob.h>
#include "storage/storage.h"
#include "util/environment.h"
#include "sfx.h"


static libvlc_instance_t *VLC_ENGINE;
static libvlc_media_player_t *GLOBAL_PLAYER;


void sfx_init(void)
{
    VLC_ENGINE = libvlc_new(0, NULL);
    GLOBAL_PLAYER = libvlc_media_player_new(VLC_ENGINE);
}

static void base_path_for(char *path, char *type, char *name)
{
    sprintf(path, "%s/sfx/%s/%s", getenv(ENV_DIR_RESOURCES), type, name);
}

static void full_path_for(char *path, char *type, char *name)
{
    sprintf(path, "%s/sfx/%s/%s.wav", getenv(ENV_DIR_RESOURCES), type, name);
}

static void play_file(char *path)
{
    if (!file_exists(path)) {
        return;
    }

    libvlc_media_t *media = libvlc_media_new_path(VLC_ENGINE, path);

    libvlc_media_player_set_media(GLOBAL_PLAYER, media);
    libvlc_media_player_play(GLOBAL_PLAYER);
    libvlc_media_release(media);
}

void sfx_play(char *type, char *name)
{
    if (!name) return;

    char path[PATH_MAX];
    full_path_for(path, type, name);

    play_file(path);
}

void sfx_play_rand(char *type, char *name)
{
    if (!name) return;

    char path[PATH_MAX];
    base_path_for(path, type, name);
    sprintf(path, "%s_*", path);

    glob_t list;
    if (0 != glob(path, GLOB_PERIOD, NULL, &list)) {
        return;
    }
    char *selected = list.gl_pathv[rand_in(0, list.gl_pathc)];

    play_file(selected);
}

void sfx_cleanup(void)
{
    libvlc_release(VLC_ENGINE);
    libvlc_media_player_release(GLOBAL_PLAYER);
}
