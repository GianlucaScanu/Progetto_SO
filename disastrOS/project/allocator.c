#include <assert.h>
#include "bit_map.h"
#include <stdio.h>
#include <math.h>
#include "allocator.h"



char * memory;
int bitmap_size;


int bitmap_structure_size(int levels){
    int bits_number = (1 << (levels - 1));
    //Adding the size of the buffer of 2^(levels - 1) bits
    int size = bits_number * sizeof(uint8_t);
    //Adding the size of the struct remaining fields'
    size += 2 * sizeof(int);
}

int buddy_init(char* buffer){
    int buffer_size = 1000000 * sizeof(char);


    //TROVA MODO PER DETERMINARE IL NUMERO DI LIVELLI CHE VUOI

    return buffer_size - bitmap_structure_size(MAX_LEVELS);


    //GESTISCI LA MEMORIA RIMANENTE (NON STRUTTURE) E RESTITUISCILA IN BLOCCHI GRAZIE ALLA BITMAP CREATA CON I LIVELLI
}

int main(){
    char buffer[1000000];
    int size = buddy_init(buffer);
    printf("la size della bitmap è: %d\n", size);
    return 0;
}