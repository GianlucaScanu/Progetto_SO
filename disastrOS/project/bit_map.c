#include <assert.h>
#include "bit_map.h"
#include <stdio.h>
#include <math.h>


//returns the number of bytes needed to store the bits
int bitmap_get_bytes(int bits){
  return (bits >> 3) + ((bits%8)!=0);
}

// initializes a bitmap on an external array
void bitmap_init(bitmap* bit_map, int num_bits, uint8_t* buffer){
  bit_map->buffer=buffer;
  bit_map->num_bits=num_bits;
  bit_map->buffer_size=bitmap_get_bytes(num_bits);
}

// sets a the bit bit_num in the bitmap
// status= 0 or 1
void bitmap_set_bit(bitmap* bit_map, int bit_num, int status){
  // getting the byte number "dividing" by 8
  int byte_num=bit_num>>3;
  assert(byte_num<bit_map->buffer_size);
  // getting the requested bit in that byte (need only the last 3 bit) 
  //-> bbb (3bit) are enough to indicate every single bit of the 8 in that byte
  int bit_in_byte=bit_num&0x07;
  // saving in mask 1 in the bit_in_byte position
  int mask = 1 << bit_in_byte;
  if (status) {
    //status == 1 -> bit on bit or using mask (it has the wanted bit as 1)
    bit_map->buffer[byte_num] |= (mask);
  } else {
    //status == 0 -> bit on bit and using NOT mask (it has the wanted bit as 0 and the others as 1)
    // in this way we modify the requested bit protectin the others
    bit_map->buffer[byte_num] &= ~(mask);
  }
}

// inspects the status of the bit bit_num
int bitmap_bit(const bitmap* bit_map, int bit_num){
  // getting the byte number "dividing" b
  int byte_num=bit_num>>3; 
  assert(byte_num<bit_map->buffer_size);
  // getting the requested bit in that byte (need only the last 3 bit) 
  //-> bbb (3bit) are enough to indicate every single bit of the 8 in that byte
  int bit_in_byte=bit_num&0x07;
  // saving in mask 1 in the bit_in_byte position
  int mask = 1 << bit_in_byte;
  // masking the byte -> now in the bit_in_byte position there is value we wanted to inspect
  int masked_byte = bit_map->buffer[byte_num] & mask;
  //shifting the inspected bit from the bit_in_byte position to the first position
  int requested_bit = masked_byte >> bit_in_byte;
  return requested_bit;
}
 
 