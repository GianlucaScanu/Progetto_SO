#include <assert.h>
#include "bit_map.h"
#include <stdio.h>
#include <math.h>
#include "allocator.h"
#include "tree.h"


#include "string.h"



char * memory;
int bitmap_size;



int bitmap_structure_size(int levels){
    //Stores in bits_numbers the total number of tree nodes since we need a bit for every node
    int bits_number = total_nodes(levels);
    //Adding the size of the buffer of 2^(levels - 1) bits (first field of the bitmap struct)
    int size = bitmap_get_bytes(bits_number) * sizeof(uint8_t);
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

//Sets as taken (1) all the buddies (blocks) that are the ancestors of the free block requested represented by a bitmap bit
int set_ancestors(buddy_allocator* alloc, int bit){
    while (bit > 0){
        bit = parent(bit);
        bitmap_set_bit(alloc->bit_map, bit, 1);
    }
    return 0;
}

//Sets as taken (1) all the buddies (blocks) that are the descendants of the free block requested represented by a bitmap bit
int set_descendants(buddy_allocator* alloc, int bit){
    bitmap_set_bit(alloc->bit_map, bit, 1);
    if (left(bit) <= alloc->bit_map->num_bits){
        set_descendants(alloc, left(bit));
    }
    if (right(bit) <= alloc->bit_map->num_bits){
        set_descendants(alloc, right(bit));
    }
    return 0;
}

void * buddy_malloc(buddy_allocator* alloc, int size){

    void * block;

    //Checks that size greater than 0;
    //TODO: appartiene ai controlli

    //We want to know which level we want to ask a buddy (block)
    int level = required_level(alloc,size);

    //Gets index of the first node of level
    int first_node = first_of_level(level);

    //Finds and stores in free_node the first free buddy (block) able to contain size
    int free_node = -1;
    for (int i = 0; i < nodes_in_level(level); i++){
        if (!bitmap_bit(alloc->bit_map, i + first_node)){
            free_node = i + first_node;
            break;
        }
    }
    if (free_node == -1){
        //TODO:
        //Set a global variable to see the "NOT ENOUGH MEMORY" state
        //return of not initialized variable block
    }else{
        //Gets the chunk size of the required level
        int level_chunk_size = alloc->chunk_size << (alloc->levels - level - 1);

        //Sets as taken the buddy requested, his ancestors and his descendants
        //TODO: GESTISCI MEGLIO LA VARIABILE DEBUG -> SE 0 OK SE -1 ERRORE
        int debug_anc =  set_ancestors(alloc, free_node);
        int debug_des = set_descendants(alloc, free_node);
        

        //Gets the address in memory corresponding to the free block
        block = (void*)alloc->actual_memory + ((free_node - first_node)*level_chunk_size);
    }
    
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
    bitmap*  bit_map =(bitmap*) buffer;
    //the bits are stored in the same buffer right after the bitmap structure
    bitmap_init(bit_map, total_nodes(levels), ((uint8_t*)buffer) + bitmap_structure_size(levels));
    alloc->bit_map = bit_map;


    //TODO: se sono presenti errori restituire -1,-2, etc
    return 0;
}


void buddy_free(buddy_allocator* alloc, void* mem){
    //distance in bytes between memory start and mem
    int distance = (char*)mem - alloc->actual_memory;
    printf("len è : %d\n", distance);
    //
    int memory_offset = distance / alloc->chunk_size;
}

int main(){
    char buffer[1000000];
    char memory[1024*1024];
    //buddy struct allocated externally: we insert only the bitmap structures in the buffer
    buddy_allocator alloc;
    int err = buddy_init(buffer, 1000000, 10, &alloc, 2048, memory, 1024*1024);
    //Using int to pass the size we must respect the maximum value
    char* block = (char*)buddy_malloc(&alloc, 50*(&alloc)->chunk_size);
    char* block1 = (char*)buddy_malloc(&alloc, 50*(&alloc)->chunk_size);
    strcpy(block, "some string");
    printf("%s\n", &block[0]);
    buddy_free(&alloc,(void*)block1);
    return 0;
}