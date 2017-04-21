#### TODO:
* Inventory improvements:
    * Shortcuts
        * From the full view set the shortcuts, but if it's not the Player's
        Inventory, the Item need to be added, if the shortcut already had an Item,
        it has to be replaced with the other.
    * Remove items
    * More inventory types (chest, bag etc.) (inventory_player_display, inventory_display)
* Lighting/Light source:
    * Portable type
    * More tests
* Level JSON cache
* Probability.items should be dynamic
* Timers for periodic executions
* Multiple panels
* Normalize the Player's speed
* If there will be more than one thread, the static buffer in `environment.c` will cause problems
* [BUILD] DIR_FIXTURE as env var

#### Possible optimizations:
* Sight point lookup
