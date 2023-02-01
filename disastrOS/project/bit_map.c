#include <assert.h>
#include "bit_map.h"
#include <stdio.h>
#include <math.h>


//returns the buddy
int buddy(int i){
  return (i&0x01) ? i + 1 : i - 1;
}

//return the right child of i
int right(int i){
  return 2 * i + 2;
}

//return the left child of i
int left(int i){
  return 2 * i + 1;
}

//returns the parent of i
int parent(int i){
  assert(i != 0);
  return floor((i-1) / 2);
}

//returns the number of bytes to store bits booleans
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
int BitMap_bit(const BitMap* bit_map, int bit_num){
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
 
 
int main(){
  BitMap struttura;
  uint8_t array[] = { (uint8_t)3 };
  BitMap_init(&struttura, 8, array);

  BitMap_setBit(&struttura, 7, 1);

  for (int i = 0; i < 8; i++){
    printf("numero: %d", array[0]);
    printf("bit %d: %d\n ", i, BitMap_bit(&struttura, i));
  }

  printf("buddy di 7: %d\n", buddy(7));
  
  return 0;
}