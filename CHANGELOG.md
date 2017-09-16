### 0.1.0 (master):

###### NEW:
* [x] Tools will wear out after using them
* [x] Items can be dropped from the inventory
* [x] Items can be placed on shortcut from the inventory
* [x] i386 tar build

###### IMPROVEMENT:
* [x] Visible point lookup was very slow. Instead of checking all points inside the current camera, just iterate the visible points,
and reduce the lookup time significantly.
* [x] Logging
* [x] Replace gettimeofday with clock_gettime

###### FIXED:
* [x] Clear out some really nasty stack smashing bugs
* [x] When the inventory opened, the selected item is not marked
* [ ] Items are added to the first empty slot
* [x] Tar build did not copy the env variables
* [x] Bunch of Valgrind errors
