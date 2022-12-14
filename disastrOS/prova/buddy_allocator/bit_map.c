#include <assert.h>
#include "bit_map.h"
#include <stdio.h>

// returns the number of bytes to store bits booleans
int BitMap_getBytes(int bits){
  return bits/8 + (bits%8)!=0;
}

// initializes a bitmap on an external array
void BitMap_init(BitMap* bit_map, int num_bits, uint8_t* buffer){
  bit_map->buffer=buffer;
  bit_map->num_bits=num_bits;
  bit_map->buffer_size=BitMap_getBytes(num_bits);
}

// sets a the bit bit_num in the bitmap
// status= 0 or 1
void BitMap_setBit(BitMap* bit_map, int bit_num, int status){
  // get byte
  int byte_num=bit_num>>3;
  assert(byte_num<bit_map->buffer_size);
  int bit_in_byte=bit_num&0x07;
  int mask = 1 << bit_in_byte;
  if (status) {
    bit_map->buffer[byte_num] |= (mask);
  } else {
    bit_map->buffer[byte_num] &= ~(mask);
  }
}
/*
// inspects the status of the bit bit_num
int BitMap_bit(const BitMap* bit_map, int bit_num){
  int byte_num=bit_num>>3; 
  assert(byte_num<bit_map->buffer_size);
  int bit_in_byte=byte_num&0x03;
  return (bit_map->buffer[byte_num] & (1<<bit_in_byte))!=0;
}
*/
// inspects the status of the bit bit_num
int BitMap_bit(const BitMap* bit_map, int bit_num){
  int byte_num=bit_num>>3; 
  assert(byte_num<bit_map->buffer_size);
  int bit_in_byte=bit_num&0x07;
  int mask = 1 << bit_in_byte;
  int masked_byte = bit_map->buffer[byte_num] & mask;
  int requested_bit = masked_byte >> bit_in_byte;
  return requested_bit;
  //return (bit_map->buffer[byte_num] & (1<<bit_in_byte))!=0;
}
 
 
int main(){
  BitMap struttura;
  uint8_t array[] = { (uint8_t)7 };
  BitMap_init(&struttura, 8, array);

  BitMap_setBit(&struttura, 7, 1);

  for (int i = 0; i < 8; i++){
    printf("numero: %d", array[0]);
    printf("bit %d: %d\n ", i, BitMap_bit(&struttura, i));
  }
  
  return 0;
}