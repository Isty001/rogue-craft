#ifndef ROGUECRAFT_MATERIAL_H
#define ROGUECRAFT_MATERIAL_H


typedef enum  {
    VOID,
    STONE,
    DIRT
} MaterialType;


typedef const struct {
    uint16_t strength;
} Material;


#endif
