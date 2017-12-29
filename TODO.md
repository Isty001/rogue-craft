#### TODO:
* [INVENTOY]
    * More inventory types (chest, bag etc.) (inventory_player_display, inventory_display)
* [LEVEL] Lighting/Light source:
    * Portable type
    * More tests
    * More types
    * JSON cache
* [MISC]
    * Probability.items should be dynamic
* [UI]
    * Multiple panels
    * Better menu system
    * Self update
* [PLAYER]
    * XP, skills, learning/level system
    * Quests
    * Replace ray-casting with something more efficient
    * Collectable achievements
* [ITEM]
    * Crafting system
    * Money/Trade system
* [CRAFTING]
    * During an open craft panel, if a recipe becomes fulfillable, in the future, recalculations must be done for the UI
* [AI]
    * (non-)hostile NPCs
* [BE AWARE OF]
    * If there will be more than one thread, the static buffer in `environment.c` will cause problems
* [BUILD]
    * DEB package
* [GENERAL]
    * Save and reload the game state


#### Possible optimizations:
* It's a waste to always recreate the Quadtree when the player moves
