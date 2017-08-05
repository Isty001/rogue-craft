#include <mem_pool.h>
#include <list.h>
#include "util.h"
#include "color.h"
#include "player/player.h"


static MemPool *LIST_NODE_POOL;


#define color(nm)  \
    {.name = #nm, .value = COLOR_PAIR(nm)}

#define simple(nm) \
    {.name = #nm, .value = nm}

ConstLookup CONST_LOOKUP[] = {
    color(COLOR_NONE),
    color(COLOR_RED_F),
    color(COLOR_GREEN_F),
    color(COLOR_BLUE_F),
    color(COLOR_YELLOW_F),
    color(COLOR_GRAY_F),
    color(COLOR_BROWN_F),
    color(COLOR_CLARET_F),
    color(COLOR_DARK_GREEN_F),
    color(COLOR_DARK_GRAY_F),
    color(COLOR_GREEN_B),
    color(COLOR_DARK_GREEN_B),
    color(COLOR_BROWN_B),
    color(COLOR_YELLOW_B),
    color(COLOR_ORANGE_B),
    color(COLOR_RED_F_B),
    color(COLOR_WHITE_B),
    color(COLOR_GRAY_B),
    color(COLOR_DARK_GRAY_B),
    color(COLOR_DARK_BROWN_B),
    color(COLOR_DARK_BROWN_F),

    simple(A_BOLD),
    simple(A_UNDERLINE),

    /** Attributes */
    simple(HEALTH),
    simple(STAMINA),
    simple(HUNGER),
    simple(THIRST),

    /** Materials */
    simple(STONE),
    simple(DIRT),
    simple(SAND),
    simple(WATER),
    simple(WOOD),
    simple(SNOW),

    /** CellType */
    simple(SOLID),
    simple(CREATURE),
    simple(HOLLOW),
    simple(PLAYER),
    simple(LIQUID),
    {.name = "ITEM", .value = ITEM},

    /** LevelType */
    simple(CELLULAR),

    {NULL, -1}
};

uint64_t constant(const char *search)
{
    int i = 0;
    char *name;

    if (search) {
        while ((name = CONST_LOOKUP[i].name)) {
            if (0 == strcmp(name, search)) {
                return CONST_LOOKUP[i].value;
            }
            i++;
        }
    }
    fatal("Constant [%s] not found\n", search);
}

static void *alloc_node(size_t size)
{
    (void)size;
    profile_list_node(++);

    return pool_alloc(LIST_NODE_POOL);
}

static void release_node(void *node)
{
    profile_list_node(--);

    pool_release(LIST_NODE_POOL, node);
}

void list_node_pool_init(void)
{
    LIST_NODE_POOL = pool_init(list_node_size(), 100);

    list_set_allocators(alloc_node, release_node, NULL);
}

void list_node_pool_cleanup(void)
{
    pool_destroy(LIST_NODE_POOL);
}
