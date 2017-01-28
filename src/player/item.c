#include <memory.h>
#include "item.h"


static ItemError consume_non_persistent(Consumable *item, Attribute *attribute)
{
    uint16_t new = attribute->current + item->value;

    if (new > attribute->limit) {
        attribute->current = attribute->limit;
        item->value = new - attribute->limit;

        return IE_REPEAT;
    }
    attribute->current = new;

    return IE_CONSUMED;
}

ItemError item_consume(Item *parent, Player *player)
{
    if (CONSUMABLE != parent->type) {
        return IE_INVALID_ARGUMENT;
    }

    Consumable *item = &parent->consumable;
    Attribute *attribute = player->attr.type_map[item->type];

    if (item->permanent) {
        attribute->limit += item->value;

        return IE_CONSUMED;
    }

    return consume_non_persistent(item, attribute);
}

Item *item_clone(ItemPrototype *prototype)
{
    Item *item = alloc(sizeof(Item));
    memcpy(item, &prototype->data, sizeof(Item));

    if (prototype->randomize) {
        prototype->randomize(item);
    }

    return item;
}

void item_free(Item *item)
{
    free(item);
}
