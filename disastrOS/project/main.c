
#include "bit_map.h"
#include <stdio.h>
#include "allocator.h"
#include "tree.h"
#include "string.h"

//buddy struct allocated externally: we insert only the bitmap structures in the buffer

int main(){
    char buffer[1000000];
    buddy_allocator alloc;
    char memory[1024*1024];
    int error = buddy_init(buffer, 1000000, 10, &alloc, 2048, memory, 1024*1024);
    printf("If Error is 0 the buddy allocator's initialization has been completed without problems.\n");
    printf("If Error is -1 the buddy allocator's initialization has been completed but the number of levels was decreased to the maximum value of 16.\n");
    printf("If Error is -2 the buddy allocator's initialization has NOT been completed regularly since memory and block values were inconsistent.\n");
    printf("Error is: %d\n\n", error);



    printf("Asking all the blocks of the lowest level and printing some of them..\n");

    void* block_i;
    for (int i = 0; i < ((&alloc)->memory_size/2048); i++){
        block_i = buddy_malloc(&alloc, 2048);
        if (i%16 == 0){
            printf("Using i = %d should be: %p, ", i , &(memory[0])+(2048*i));
            printf("It is: %p\n", block_i);
        }
    }

    printf("Now the memory should be empty, so the next malloc should return null.\n");
    int is_null = (buddy_malloc(&alloc, 2048) == NULL);
    printf("Prints TRUE if the next malloc returned null: %s\n", is_null ? "TRUE" : "FALSE");
    




    /*
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
    printf("\n");*/
    return 0;
}