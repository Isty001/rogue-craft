
[![Build Status](https://travis-ci.org/rogue-craft/rogue-craft.svg?branch=master)](https://travis-ci.org/rogue-craft/rogue-craft) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/c2494381a8164514adb5061f7b1796e8)](https://www.codacy.com/app/Isty001/rogue-craft?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=rogue-craft/rogue-craft&amp;utm_campaign=Badge_Grade)  [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)


## Rogue Craft

This is the main repository of Rogue Craft a 2D terminal sandbox/RPG/survival game,
based on ncurses. It's developed and tested on Linux, but should work on other
UNIX-like systems too. Please check the [Wiki](https://github.com/rogue-craft/rogue-craft/wiki) (in progress)
for the guide and documentation. You can also follow the development [here](rogue-craft.github.io).

![screenshot](https://raw.githubusercontent.com/rogue-craft/doc-resources/master/README/screenshot_1.png)

### Install

For running `ncursesw5` is required (`panelw5` too but usually it comes with ncurses), and `vlc`. Your terminal must support 256 colors,
for example try setting your `TERM` to `xterm-256color` to achieve this. The terminal also needs mouse interaction support.

#### From release tar

Choose a release from [here](https://github.com/rogue-craft/rogue-craft/releases) then

```
tar -xf rogue-craft-*.tar.gz
cd rogue-craft
sh ./install.sh
```

#### From source

To compile you'll need `gcc`, `libncursesw5-dev` and `libvlc-dev` too.

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
Help with the [TODO](https://github.com/rogue-craft/rogue-craft/blob/master/TODO.md), or [Issues](https://github.com/rogue-craft/rogue-craft/blob/master/ISSUES.md) highly appreciated.
Check the Wiki for help, and of course you can also contact [me](https://github.com/Isty001).
