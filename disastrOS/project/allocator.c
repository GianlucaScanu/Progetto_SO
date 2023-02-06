#include <assert.h>
#include "bit_map.h"
#include <stdio.h>
#include <math.h>
#include "allocator.h"
#include "tree.h"





/* returns the size of the structure used to store the bitmap */
int bitmap_structure_size(int levels){

    //stores in bits_numbers the total number of tree nodes since we need a bit for every node
    int bits_number = total_nodes(levels);

    //adding the size of the buffer of 2^(levels - 1) bits (first field of the bitmap struct)
    int size = bitmap_get_bytes(bits_number) * sizeof(uint8_t);

    //adding the size of the struct remaining fields
    size += 2 * sizeof(int);
}


/* returns the level, from 0 to levels - 1,  whose blocks can contain "size" */
int required_level(buddy_allocator* alloc, int size){

    int threshold = alloc->chunk_size;

    for (int i = alloc->levels; i >= 1 ; i--){

        if (size <= threshold){
            return i-1;
        } 

        threshold = threshold << 1;
    }

    //we try to give the maximum space possible
    return 0;
}


/* sets to taken (1) all the buddies (blocks) that are
 the ancestors of the free block requested, represented by a bitmap's bit */
int set_ancestors(buddy_allocator* alloc, int bit){

    while (bit > 0){
        bit = parent(bit);
        bitmap_set_bit(alloc->bit_map, bit, 1);
    }

    return 0;
}


/* returns 0 if the block's ancestors are not used, 1 otherwise */
int search_ancestors(buddy_allocator* alloc, int bit, int * parent_node){

    while (bit > 0){

        if (bitmap_bit(alloc->bit_map, parent(bit))){

            //bit's parent is 1 and bit's buddy is 1
            if (bitmap_bit(alloc->bit_map, buddy(bit))) return 0;

            //bit's parent is 1 and bit's buddy is 0
            else{
              *parent_node = parent(bit);
              return 1;  
            } 

        }else{
            bit = parent(bit);
        }
    }

    return 0;
}

/* returns a pointer to the start of the first available smallest block that 
   can contain "size". If there is no room to contain "size" it returns NULL */
void * buddy_malloc(buddy_allocator* alloc, int size){

    void* block;

    int level;

    int first_node;

    int free_node;

    int parent_node;

    block = NULL;

    if (size > alloc->memory_size) return block;

    //we want to know which level we want to ask a buddy (block)
    level = required_level(alloc,size);

    //gets the index of the first node of level
    first_node = first_of_level(level);

    //finds and stores in free_node the first free buddy (block) able to contain size
    free_node = -1;
    for (int i = 0; i < nodes_in_level(level); i++){
        if (!bitmap_bit(alloc->bit_map, i + first_node)){
            //il bit considerato è 0     
            if(!search_ancestors(alloc, i + first_node, &parent_node)){
                free_node = i + first_node;
                break;
            }
            //if the loop does not break it is not free, so i jumps to the next possible free block 
            i = i  + (1 << (get_level(parent_node) - level));
        }
    }
    if (free_node == -1){
        //Returns NULL since there is NOT ENOUGH FREE MEMORY
    }else{
        //Gets the chunk size of the required level
        int level_chunk_size = alloc->chunk_size << (alloc->levels - level - 1);

        //Sets as taken the buddy requested and  his ancestors 
        bitmap_set_bit(alloc->bit_map, free_node, 1);
        set_ancestors(alloc, free_node);
        

        //Gets the address in memory corresponding to the free block
        block = (void*)alloc->actual_memory + ((free_node - first_node)*level_chunk_size);
    }
    
    return block;
}



//initialize a buddy allocator using "levels" levels for buddies
int buddy_init(char* buffer, int buffer_size, int levels, buddy_allocator* alloc, int min_chunk_size,
                char* memory, int memory_size){

    int err = 0;

    //checks is buffer size is enough to store the bitmap and minimum chunk size is greater than memory size
    if(buffer_size <= bitmap_structure_size(levels) || min_chunk_size > memory_size){
        return 1;
    }

    //checks if given levels number in input is over the limit
    if (levels > MAX_LEVELS){
        levels = MAX_LEVELS;
        err = -1;
    } 

    //checks if the memory size is enough for the given minimum chunk's size given
    //(1 << (levels - 1)) is the number of leaves of size min_chunk_size
    if (memory_size < min_chunk_size * (1 << (levels - 1))) return 1;

    if (memory_size != min_chunk_size * (1 << (levels - 1))){
        if (err == -1){
            err = -3;
        }else{
            //Memory input not correct
            err = -2;
        }
    }

    //initializing buddy_allocator fields
    alloc->actual_memory = memory;
    alloc->memory_size = memory_size;
    alloc->levels = levels;
    alloc->chunk_size = min_chunk_size;
    bitmap*  bit_map =(bitmap*) buffer;
    //the bits are stored in the same buffer right after the bitmap structure
    bitmap_init(bit_map, total_nodes(levels) - 1, ((uint8_t*)buffer) + bitmap_structure_size(levels));
    alloc->bit_map = bit_map;

    return err;
}


//Sets as free (bit to 0) the block and proceeds to recursively free the parent if buddy is free (bit already set to 0)
void recursive_merge(buddy_allocator* alloc, int block){
    bitmap_set_bit(alloc->bit_map, block, 0);
    int block_buddy = buddy(block);
    //i.e block is the root
    if (block == 0) return;
    if (!bitmap_bit(alloc->bit_map, block_buddy)) recursive_merge(alloc, parent(block));
}


void buddy_free(buddy_allocator* alloc, void* mem){

    if (mem == NULL) return;

    //distance in bytes between memory start and mem
    int distance = (char*)mem - alloc->actual_memory;

    //mem is not a valid address
    if (distance % alloc->chunk_size != 0) return;

    //Gets the node offset of "mem" from "start" expressed like the number of lowest level's chunks separing the two
    int node_offset = distance / alloc->chunk_size;

    //Gets the index of the first node of the last level
    int first_node = first_of_level(alloc->levels-1);

    //Gets the last level node's index that represents the start of mem
    int mem_node = first_node + node_offset;

    recursive_merge(alloc, mem_node);

}

