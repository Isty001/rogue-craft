
[![Build Status](https://travis-ci.org/rogue-craft/rogue-craft.svg?branch=master)](https://travis-ci.org/rogue-craft/rogue-craft) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/c2494381a8164514adb5061f7b1796e8)](https://www.codacy.com/app/Isty001/rogue-craft?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=rogue-craft/rogue-craft&amp;utm_campaign=Badge_Grade)  [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)


## Rogue Craft

Rogue Craft is a 2D roguelike (kinda sandbox/RPG/survival) game, based on ncurses, in an early development stage.
Please read the [Wiki](https://github.com/rogue-craft/rogue-craft/wiki) (in progress) for the guide and documentation.
You can also follow the development [here](https://rogue-craft.github.io).

**Click [here](https://www.youtube.com/watch?v=rqqhYEehAro) to watch a video of it.**

[![screenshot](https://raw.githubusercontent.com/rogue-craft/doc-resources/master/README/banner.png)](https://www.youtube.com/watch?v=rqqhYEehAro)

---

### Install

For running `ncursesw5` is required (`panelw5` too but usually it comes with ncurses), and `vlc`. Your terminal must support 256 colors,
(for example you could set `$TERM` to `xterm-256color`) and needs mouse interaction support too.

Currently tested only on Linux.

#### From release tar

Choose a release from [here](https://github.com/rogue-craft/rogue-craft/releases) then

```
tar -xf rogue-craft-*.tar.gz
cd rogue-craft
sh ./install.sh
```

#### From source

To compile you'll need `gcc`, on Debian based systems you'll need`libncursesw5-dev`  (and maybe `libncurses5-dev` too) and `libvlc-dev`, they might be called differently on your OS, or even comes with the base package, and no separate dev package need to be installed.

```
git clone --recursive https://github.com/rogue-craft/rogue-craft
cd rogue-craft
make
make install
```

You can remove the build objects with `make clean`, or run the tests with `make run-test`

To play, simply run `rogue-craft` in your terminal.
Please note that it's still development version.

### Contribute

If the project got your attention, feel free to contribute with your ideas, and code. :-)
Help with the [TODO](https://github.com/rogue-craft/rogue-craft/blob/master/TODO.md), or the [ISSUES](https://github.com/rogue-craft/rogue-craft/blob/master/ISSUES.md) is highly appreciated.
Please open an issue/PR if you found a bug, or have an idea, or contact [me](https://github.com/Isty001).

