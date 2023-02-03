#pragma once
#include <stdint.h>

// simple bit array
typedef struct  {
  uint8_t *buffer; 
  int buffer_size;
  int num_bits; 
} bitmap;

// returns the number of bytes to store bits booleans
int bitmap_get_bytes(int bits);

// initializes a bitmap on an external array
void bitmap_init(bitmap* bit_map, int num_bits, uint8_t* buffer);

// sets a the bit bit_num in the bitmap
// status= 0 or 1
void bitmap_set_bit(bitmap* bit_map, int bit_num, int status);

// inspects the status of the bit bit_num
int bitmap_bit(const bitmap* bit_map, int bit_num);
