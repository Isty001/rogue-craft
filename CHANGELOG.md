### 0.2 (master):

###### NEW:
* [x] Basic menu
* [#] Materials can be collected
* [ ] Placing collected materials on the map as new cells

###### FIXED:
* [x] cell_free with the Player's cell prototype failed, because it wasn't marked with `in_registry`


---

### 0.1:

###### NEW:
* [x] Tools will wear out after using them
* [x] Items can be dropped from the inventory
* [x] Items can be placed on shortcut from the inventory
* [x] i686 tar build
###### IMPROVEMENT:
* [x] Visible point lookup was very slow. Instead of checking all points inside the current camera, just iterate the visible points,
and reduce the lookup time significantly.
* [x] Logging
* [x] Replace gettimeofday with clock_gettime

###### FIXED:
* [x] Clear out some really nasty stack smashing bugs
* [x] When the inventory opened, the selected item is not marked
* [x] Items are added to the first empty slot
* [x] Tar build did not copy the env variables
* [x] Bunch of Valgrind errors
