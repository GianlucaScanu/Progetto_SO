#include <assert.h>
#include "bit_map.h"
#include <stdio.h>
#include <math.h>
#include "allocator.h"
#include "tree.h"
#include "string.h"



int main(){
    char buffer[1000000];
    //buddy struct allocated externally: we insert only the bitmap structures in the buffer
    buddy_allocator alloc;
    char memory[16*16];
    int err = buddy_init(buffer, 1000000, 3, &alloc, 64, memory, 16*16);
    //Using int to pass the size we must respect the maximum value
    char* block = (char*)buddy_malloc(&alloc, 128);
    char* block1 = (char*)buddy_malloc(&alloc, 64);
    
    for (int i = 0; i <= (&alloc)->bit_map->num_bits; i++){
        printf(" %d    ",  bitmap_bit((&alloc)->bit_map, i));
    }
    printf("\n");
    strcpy(block, "some string");
    printf("%s\n", &block[0]);
    buddy_free(&alloc,(void*)block1);


    for (int i = 0; i <= (&alloc)->bit_map->num_bits; i++){
        printf(" %d", bitmap_bit((&alloc)->bit_map, i));
    }
    printf("\n");
    return 0;
}