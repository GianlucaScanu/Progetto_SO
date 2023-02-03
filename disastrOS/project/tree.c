#include "tree.h"


int total_nodes(int levels){
    return (1 << (levels)) - 1;
}

int first_of_level(int level){
    return (1 << level) - 1;
}