
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
        printf("If Error is -3 the buddy's allocator's initialization has NOT been completed regularly since memory and block values were inconsistent and the number of levels was decreased to the maximum value of 16.\n");
        printf("Error is: %d\n\n", error);



        printf("Asking all the blocks of the lowest level and printing some of them..\n");

        void* block_i;

        for (int i = 0; i < ((&alloc1)->memory_size/2048); i++){
            block_i = buddy_malloc(&alloc1, 2048);
            if (i%16 == 0){
                printf("Using i = %d should be: %p, ", i , &(memory[0])+(2048*i));
                printf("it is: %p\n", block_i);
            }
            //Commenta per vedere la memoria empty
            //buddy_free(&alloc1, block_i);
        }

        printf("\n");

        printf("Now the memory should be empty, so the next malloc should return null.\n");
        int is_null = (buddy_malloc(&alloc1, 2048) == NULL);
        printf("Prints TRUE if the next malloc returned null: %s\n", is_null ? "TRUE" : "FALSE");

        printf("\n");

        printf("Using a block to store a string accessing it...\n");
        strcpy(block_i, "Some string");
        printf("Prints the string in the block: %s\n", (char*)block_i);

        printf("Test 1 ended successfully\n");
        printf("\n");

    }

    else if (!strcmp(argv[1], "test2")){
        char buffer[1000000];
        buddy_allocator alloc1;
        char memory[1024*1024];
        int error = buddy_init(buffer, 1000000, 10, &alloc1, 1024, memory, 1024*1024);
        printf("If Error is 0 the buddy allocator's initialization has been completed without problems.\n");
        printf("If Error is -1 the buddy allocator's initialization has been completed but the number of levels was decreased to the maximum value of 16.\n");
        printf("If Error is -2 the buddy allocator's initialization has NOT been completed regularly since memory and block values were inconsistent.\n");
        printf("If Error is -3 the buddy's allocator's initialization has NOT been completed regularly since memory and block values were inconsistent and the number of levels was decreased to the maximum value of 16.\n");
        printf("Error is: %d\n\n", error);


        char new_buffer[1000000];
        buddy_allocator alloc2;
        char new_memory[1024*1024];
        int new_error = buddy_init(new_buffer, 1000000, 10, &alloc2, 2048, new_memory, 1024*1024);
        printf("Error is: %d\n\n", new_error);


        printf("Trying some malloc...\n");
        printf("Malloc of size 55555\n");
        void * first_malloc = buddy_malloc(&alloc2, 55555);
        printf("Malloc of size 1000\n");
        void * second_malloc = buddy_malloc(&alloc2, 1000);
        //Not enough memory
        printf("Malloc of size 7777777 (There should be not enough memory)\n");
        void * third_malloc = buddy_malloc(&alloc2, 7777777);
        printf("Malloc of size 600000\n");
        void * fourth_malloc = buddy_malloc(&alloc2, 600000);
        printf("Freeing them in another order...\n");
        
        buddy_free(&alloc2, second_malloc);
        buddy_free(&alloc2, first_malloc);
        buddy_free(&alloc2, third_malloc);

        printf("Test 2 ended successfully\n");
        printf("\n");
    }

    else if (!strcmp(argv[1], "test3")){

            printf("TODO");
    }



    return 0;
}