#include <assert.h>
#include "bit_map.h"
#include <stdio.h>
#include <math.h>
#include "allocator.h"
#include "tree.h"



char * memory;
int bitmap_size;



int bitmap_structure_size(int levels){
    //Stores in bits_numbers the total number of tree nodes since we need a bit for every node
    int bits_number = total_nodes(levels);
    //Adding the size of the buffer of 2^(levels - 1) bits (first field of the bitmap struct)
    int size = bitmap_getBytes(bits_number) * sizeof(uint8_t);
    //Adding the size of the struct remaining fields'
    size += 2 * sizeof(int);
}

//returns the level, from 0 to levels - 1,  whose blocks can contain "size"
int required_level(buddy_allocator* alloc, int size){
    int threshold = alloc->chunk_size;
    for (int i = alloc->levels; i >= 1 ; i--){
        if (size <= threshold){
            return i-1;
        } 
        threshold = threshold << 1;
    }
    //We try to give the maximum space possible
    return 0;
}

char * buddy_malloc(buddy_allocator* alloc, int size){

    char * block;

    //Checks that size greater than 0;
    //TODO: appartiene ai controlli

    //We want to know which level we want to ask a buddy (block)
    int level = required_level(alloc,size);

    //Gets index of the first node of level
    int first_node = first_of_level(level);

    //Asks the allocator for a block in that level
    //TODO: SE NON MODIFICA PASSA BLOCK COME INDIRIZZO NON VALORE


    printf("il livello per size 1 è : %d\n", level);
    printf("il primo di quel livello è: %d\n", first_node);

    return block;
}


//initialize a buddy allocator using "levels" levels for buddies
int buddy_init(char* buffer, int buffer_size, int levels, buddy_allocator* alloc, int min_chunk_size,
                char* memory, int memory_size){
                    
    //checks is buffer size is enough to store the bitmap
    assert(buffer_size > bitmap_structure_size(levels));

    //checks if given levels number in input is over the limit
    if (levels > MAX_LEVELS) levels = MAX_LEVELS;

    //checks if the memory size is enough for the given minimum chunk's size given
    //(1 << (levels - 1)) is the number of leaves of size min_chunk_size
    printf("memoria in chunks: %d\n", min_chunk_size * (1 << (levels - 1)));
    assert(memory_size >= min_chunk_size * (1 << (levels - 1)));

    //initializing buddy_allocator fields
    alloc->actual_memory = memory;
    alloc->memory_size = memory_size;
    alloc->levels = levels;
    alloc->chunk_size = min_chunk_size;
    bitmap  bit_map;
    bitmap_init(&bit_map, (1 << (levels - 1)), ((uint8_t*)buffer) + sizeof(*alloc));
    alloc->bit_map = &bit_map;



    //TODO: se sono presenti errori restituire -1,-2, etc
    return 0;
}

int main(){
    char buffer[1000000];
    char memory[1024*1024];
    //buddy struct allocated externally: we insert only the bitmap structures in the buffer
    buddy_allocator alloc;
    int err = buddy_init(buffer, 1000000, 10, &alloc, 2048, memory, 1024*1024);
    //Using int to pass the size we must respect the maximum value
    buddy_malloc(&alloc, 50*(&alloc)->chunk_size);
    return 0;
}