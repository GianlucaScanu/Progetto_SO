#pragma once

#define MAX_LEVELS 16

typedef struct{
    char* actual_memory;
    bitmap * bit_map;
    int memory_size;
    int levels;
    int chunk_size;
}buddy_allocator;


int buddy_init(char* buffer, int buffer_size, int levels, buddy_allocator* alloc,

         int min_chunk_size, char* memory, int memory_size);


void * buddy_malloc(buddy_allocator* alloc, int size);


void buddy_free(buddy_allocator* alloc, void* mem);