
#include "bit_map.h"
#include <stdio.h>
#include "allocator.h"
#include "tree.h"
#include "string.h"

//buddy struct allocated externally: we insert only the bitmap structures in the buffer

int main(int argc, char **argv){
  
    if (argc != 2){
        printf("Call main using one of the three arguments: test1, test2, test3.\n");
        return 0;
    }

    if (!strcmp(argv[1], "test1")){

        char buffer[1000000];
        buddy_allocator alloc1;
        char memory[1024*1024];
        int error = buddy_init(buffer, 1000000, 10, &alloc1, 2048, memory, 1024*1024);
        printf("If Error is 0 the buddy allocator's initialization has been completed without problems.\n");
        printf("If Error is -1 the buddy allocator's initialization has been completed but the number of levels was decreased to the maximum value of 16.\n");
        printf("If Error is -2 the buddy allocator's initialization has NOT been completed regularly since memory and block values were inconsistent.\n");
        printf("Error is: %d\n\n", error);



        printf("Asking all the blocks of the lowest level and printing some of them..\n");

        void* block_i;
        for (int i = 0; i < ((&alloc1)->memory_size/2048); i++){
            block_i = buddy_malloc(&alloc1, 2048);
            if (i%16 == 0){
                printf("Using i = %d should be: %p, ", i , &(memory[0])+(2048*i));
                printf("it is: %p\n", block_i);
            }
        }

        printf("\n");

        printf("Now the memory should be empty, so the next malloc should return null.\n");
        int is_null = (buddy_malloc(&alloc1, 2048) == NULL);
        printf("Prints TRUE if the next malloc returned null: %s\n", is_null ? "TRUE" : "FALSE");

        printf("\n");

        printf("Using a block to store a string accessing it...\n");
        strcpy(block_i, "Some string");
        printf("Prints the string in the block: %s\n", (char*)block_i);

    }

    else if (!strcmp(argv[1], "test2")){

            printf("FARE TEST 2 CON FREE");
    }

    else if (!strcmp(argv[1], "test3")){

            printf("TODO");
    }



    return 0;
}