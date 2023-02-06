#include <assert.h>
#include "bit_map.h"
#include <stdio.h>
#include <math.h>


/* returns the number of bytes needed to store the bits */
int bitmap_get_bytes(int bits){

  return (bits >> 3) + ((bits%8)!=0);
  
}

/* initializes a bitmap on an external array */
void bitmap_init(bitmap* bit_map, int num_bits, uint8_t* buffer){

  bit_map->buffer=buffer;

  bit_map->num_bits=num_bits;

  bit_map->buffer_size=bitmap_get_bytes(num_bits);


 //initializing all bits to ensure they are set to 0
  for (int i = 0; i <= num_bits; i++){
    bitmap_set_bit(bit_map, i, 0);
  }

}

/* sets to "status" the bit bit_num */
void bitmap_set_bit(bitmap* bit_map, int bit_num, int status){

  // getting the byte number "dividing" by 8
  int byte_num=bit_num>>3;

  assert(byte_num<bit_map->buffer_size);

  /*getting the requested bit in that byte (need only the last 3 bit) 
     bbb (3bit) are enough to indicate every single bit of the 8 bits in that byte */
  int bit_in_byte=bit_num&0x07;

  //saving in mask the value 1 in "the bit_in_byte position"
  int mask = 1 << bit_in_byte;

  if (status) {

    //status is 1 therefore we use bit on bit OR using mask (which has the wanted bit as 1)
    bit_map->buffer[byte_num] |= (mask);

  } else {

    /*status is 0 therefore we use bit on bit AND using NOT mask (it has the wanted bit as 0 and the others as 1).
     In this way we modify the requested bit protecting the others */
    bit_map->buffer[byte_num] &= ~(mask);

  }
}

/* inspects the "bit_num" bit in the bitmap and returns its value */
int bitmap_bit(const bitmap* bit_map, int bit_num){

  //getting the byte number "dividing" b
  int byte_num=bit_num>>3; 

  assert(byte_num<bit_map->buffer_size);

  /* getting the requested bit in that byte (need only the last 3 bit) 
   bbb (3bit) are enough to indicate every single bit of the 8 bits in that byte */
  int bit_in_byte=bit_num&0x07;

  //saving in mask the value 1 in the "bit_in_byte" position
  int mask = 1 << bit_in_byte;

  //masking the byte so now in the bit_in_byte position there is value we wanted to inspect
  int masked_byte = bit_map->buffer[byte_num] & mask;

  //shifting the inspected bit from the bit_in_byte position to the first position
  int requested_bit = masked_byte >> bit_in_byte;
  
  return requested_bit;
}
 
 