#pragma once
#include <stdint.h>

// simple bit array
typedef struct  {
  uint8_t *buffer; 
  int buffer_size;
  int num_bits; 
} bitmap;

/* returns the number of bytes needed to store bits booleans */
int bitmap_get_bytes(int bits);

/* initializes a bitmap on an external array */
void bitmap_init(bitmap* bit_map, int num_bits, uint8_t* buffer);

/* sets to "status" the bit bit_num */
void bitmap_set_bit(bitmap* bit_map, int bit_num, int status);

/* inspects the "bit_num" bit in the bitmap and returns its value */
int bitmap_bit(const bitmap* bit_map, int bit_num);
