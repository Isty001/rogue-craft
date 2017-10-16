#ifndef ROGUECRAFT_OBJECT_H
#define ROGUECRAFT_OBJECT_H


#include <stdint.h>


union ObjectReference {
    const void *obj;
    const uint16_t id;
};


#endif

