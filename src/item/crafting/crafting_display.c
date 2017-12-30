#include "keymap.h"
#include "memory/memory.h"
#include "recipe.h"
#include "item/item.h"
#include "ui/menu/menu.h"
#include "ui/panel.h"


#define RECIPE_MENU_Y 1
#define RECIPE_MENU_X 3


typedef struct {
    ItemType type;
    CMenu *menu;
    WINDOW *recipe_win;
} CraftTabInfo;


static CraftTabInfo *new_info(ItemType type)
{
    CraftTabInfo *info = mem_alloc(sizeof(CraftTabInfo));
    info->type = type;

    return info;
}

static List *recipes_by_type(List *recipes, ItemType required_type)
{
    List *new = recipes->clone(recipes);

    return new->filter(new, (Predicate) function(bool, (Recipe *recipe) {
        return required_type == recipe->result->item.type;
    }));
}

static void assure_tab_pad_size(CTabList *tabs, List *known_recipes)
{
    CTab *curr = cursed_tablist_current(tabs);
    uint16_t height = max(known_recipes->count, getmaxy(curr->pad));

    cursed_tablist_resize_tabs(tabs, height, C_NO_OP);
}

static ITEM **items_from_recipes(List *recipes)
{
    int i = 0;
    ITEM **items = mem_alloc((recipes->count + 1) * sizeof(ITEM));

    items = recipes->fold_l(recipes, items, (Fold) function(void *, (ITEM **acc, Recipe *recipe) {
        acc[i] = new_item(recipe->result->name, NULL);
        set_item_userptr(acc[i], recipe);
        i++;

        return items;
    }));
    items[i] = NULL;

    return items;
}

static CMenu *create_recipe_menu(CTab *tab, ITEM **items)
{
    CMenu *menu = cursed_menu_new(items);
    MENU *parent_menu = cursed_menu_parent(menu);
    WINDOW *pad = tab->pad;

    set_menu_win(parent_menu, pad);
    set_menu_sub(parent_menu, derwin(pad, getmaxy(pad) - RECIPE_MENU_Y, ITEM_NAME_MAX, RECIPE_MENU_Y, RECIPE_MENU_X));
    set_menu_mark(parent_menu, "");
    post_menu(parent_menu);

    return menu;
}

static WINDOW *create_recipe_window(CTab *tab)
{
    WINDOW *pad = tab->pad;
    uint16_t width = getmaxx(pad) - ITEM_NAME_MAX - RECIPE_MENU_X;

    return derwin(pad, getmaxy(pad) - RECIPE_MENU_Y, width, RECIPE_MENU_Y, ITEM_NAME_MAX + RECIPE_MENU_X);
}

static void add_item_icons(CTab *tab, CMenu *menu, ITEM **items)
{
    uint16_t count = cursed_menu_count(menu);

    repeat(count,
        Recipe *recipe= item_userptr(items[i]);
        const Item *item = &recipe->result->item;

        styled(tab->pad, item->style,
            mvwprintw(tab->pad, RECIPE_MENU_Y + i, RECIPE_MENU_X - 2, "%lc", item->chr);
        )
    )
}

static void fill_tabs(CTabList *tab_list, List *known_recipes)
{
    uint16_t count = cursed_tablist_count(tab_list);
    CTab *tabs[count];
    cursed_tablist_to_array(tab_list, tabs);

    repeat(count,
        CTab *tab = tabs[i];
        CraftTabInfo *info = tab->userptr;
        List *recipes = recipes_by_type(known_recipes, info->type);

        if (0 == recipes->count) {
            info->menu = NULL;
        } else {
            ITEM **items = items_from_recipes(recipes);

            info->menu = create_recipe_menu(tab, items);
            info->recipe_win = create_recipe_window(tab);
            box(info->recipe_win, 0,0);
            add_item_icons(tab, info->menu, items);
        }
        recipes->free(recipes);
    )
}

static void display_current_recipe(CTabList *tabs)
{
    CTab *tab = cursed_tablist_current(tabs);
    CraftTabInfo *info  = tab->userptr;

    if (!info->menu) {
        return;
    }

    Recipe *recipe = item_userptr(current_item(cursed_menu_parent(info->menu)));
    WINDOW *recipe_win = info->recipe_win;

    werase(recipe_win);
    underline(recipe_win,
            mvwprintw(recipe_win, 0, 0, "Requirements:");
    )

    repeat(recipe->ingredient_count,
        const Ingredient *ingredient = &recipe->ingredients[i];

        mvwprintw(info->recipe_win, i + 1, 0, "%d x %s", ingredient->value, ingredient_to_str(ingredient));
    )

    cursed_tablist_display_current(tabs);
}

void craft_panel_display(InputEvent *event)
{
    Size size = size_new(getmaxy(WINDOW_MAIN) / 2, getmaxx(WINDOW_MAIN) / 2);
    WINDOW *win = ncurses_newwin_adjust(size, WINDOW_MAIN);

    const CTabDefinition defs[] = {
        {.title = "Tools", .userptr = new_info(ITEM_TOOL)},
        {.title = "Consumables", .userptr = new_info(ITEM_CONSUMABLE)},
        {.title = "Light sources", .userptr = new_info(ITEM_LIGHT_SOURCE)},
        {.title = "Blocks", .userptr = new_info(ITEM_MATERIAL)},
        {.title  = NULL}
    };
    List *known_recipes = event->player->skills.recipes;

    CTabList *tabs = cursed_tablist_new(win, C_TAB_TOP, defs);
    assure_tab_pad_size(tabs, known_recipes);

    fill_tabs(tabs, known_recipes);
    display_current_recipe(tabs);

    const PanelInfo info = {
        .type = PANEL_CRAFT,
        .tabs = tabs
    };
    panel_push_new(win, info);
}

void craft_panel_navigate(PanelInputEvent *event)
{
    CTabList *tabs = event->info->tabs;
    int input = event->input;

    if (KEY_WEST == input || KEY_EAST == input) {
        cursed_tablist_navigate(tabs, event->input);
    } else {
        CTab *current = cursed_tablist_current(tabs);
        CraftTabInfo *info = current->userptr;

        if (info->menu) {
            cursed_menu_navigate(info->menu, input);
        }
    }

    display_current_recipe(tabs);
}

static void free_info(CraftTabInfo *info)
{
    if (info->menu) {
        cursed_menu_free(info->menu);
    }
    mem_dealloc(info);
}

void craft_panel_close(PanelInputEvent *event)
{
    cursed_tablist_userptr_free(event->info->tabs, (CFree) free_info);
}
