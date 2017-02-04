#ifndef ROGUECRAFT_MATERIAL_H
#define ROGUECRAFT_MATERIAL_H


typedef enum  {
    VOID,
    STONE,
    DIRT
} MaterialType;


typedef struct {
    uint16_t strength;
} Material;


#endif
