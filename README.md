[![Build Status](https://travis-ci.org/rogue-craft/rogue-craft.svg?branch=master)](https://travis-ci.org/rogue-craft/rogue-craft) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/c2494381a8164514adb5061f7b1796e8)](https://www.codacy.com/app/Isty001/rogue-craft?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=rogue-craft/rogue-craft&amp;utm_campaign=Badge_Grade)  [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)


## Rogue Craft

This is the main repository of Rogue Craft a 2D terminal sandbox/RPG/survival game,
based on ncurses. It's developed and tested on Linux, but should work on other
UNIX-like systems too. Please check the [Wiki](https://github.com/rogue-craft/rogue-craft/wiki) (in progress)
for the guide and documentation. You can also follow the development [here](rogue-craft.github.io).

### Install

For running `ncursesw5` is required, and your terminal must support 256 colors,
for example you could put this line in your .bashrc/.zshrc or whatever you use, `export TERM=xterm-256color`.
The terminal also needs mouse interaction support.

#### From release tar

Grab a release tar from [here](https://github.com/rogue-craft/rogue-craft/releases) then

```
tar -xf rogue-craft-*.tar.gz
cd rogue-craft
./install.sh
```

#### From source

To compile you need `gcc`, `ncursesw5-dev` library installed.

```
git clone --recursive https://github.com/rogue-craft/rogue-craft
cd rogue-craft
make
make install
```

You can remove the build objects with `make clean`, or run the tests with `make run-test`

To play, simply execute `rogue-craft` in your terminal.
Please note that it's still a very early development stage.

### Contribute

If the project got your attention, feel free to contribute with your ideas, and code. :-)
Help with the [TODO](https://github.com/rogue-craft/rogue-craft/blob/master/TODO.md), [Plans](#Plans) or [Issues](https://github.com/rogue-craft/rogue-craft/blob/master/ISSUES.md) are also highly appreciated.
Check the Wiki for help, and of course you can also contact [me](https://github.com/Isty001).

### <a name="Plans"></a>Plans

This is just a rough list of planned features.

* Lighting
    * Portable light sources that can be carried by the player
    * Lightings will have a limited lifetime
    * Generate random light sources on the level
* Inventory
    * New types of inventory (chest, bag, etc.)
    * Replacing shortcuts
* Level
    * Destroyed cells can be picked up
    * They may also drop items
    * Cells can be reused for building
    * More level types
* UI
    * Basic menu structure (new/saved game, options etc.)
* Crafting
    * Recipes can be collected, learned and executed
* AI
* Quests
* Save and reload the game
* Sounds
