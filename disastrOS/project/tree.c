#include "tree.h"
#include <math.h>
#include "assert.h"

/* returns the total number of nodes in the tree */
int total_nodes(int levels){

    return (1 << (levels)) - 1;

}

/* returns the level the buddy is in */
int get_level(int i){

  return floor(log2(i-1));

}

/* returns the index of the level's first node */
int first_of_level(int level){

    return (1 << level) - 1;

}

/* returns the number of nodes in a level */
int nodes_in_level(int level){

    return 1 << level;

}

/* returns the buddy of i*/
int buddy(int i){

  return (i&0x01) ? i + 1 : i - 1;

}

/* returns the right child of i */
int right(int i){

  return 2 * i + 2;

}

/* returns the left child of i */
int left(int i){

  return 2 * i + 1;

}

/* returns the parent of i */
int parent(int i){

  return floor((i-1) / 2);
  
}

