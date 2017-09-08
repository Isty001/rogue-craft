### 0.1.0 (master):

###### NEW:
* [x] Tools will wear out after using them
* [ ] Items can be dropped from the inventory
* [x] Items can be placed on shortcut from the inventory
* [x] i386 tar build

###### FIXED:
* [x] Clear out some really nasty stack smashing bugs
* [x] When the inventory opened, the selected item is not marked
* [x] Tar build did not copy the env variables
* [x] Bunch of Valgrind errors

###### INTERNAL:
* [x] Logging
* [x] Replace gettimeofday with clock_gettime
