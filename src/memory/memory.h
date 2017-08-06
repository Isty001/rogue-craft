#ifndef ROGUE_CRAFT_MEMORY_H
#define ROGUE_CRAFT_MEMORY_H


void *mem_alloc(int size);

void *mem_calloc(int nmemb, int size);

void *mem_realloc(void *old, unsigned size);

void mem_dealloc(void *ptr);

void list_node_pool_init(void);

void list_node_pool_cleanup(void);


#endif
