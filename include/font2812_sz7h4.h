#include <Arduino.h>


#define x (0xff)
#define o (0x00)
#define FONT_HEIGHT (7)

uint8_t font_wid[256] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 0x00 - 0x0f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,1,0,0,0,0,0,0,0,0,5,0,0,0,0,0, // 32(space), 33(!), 42(*)
  5,3,5,5,5,5,5,5,5,5,0,0,0,0,0,5, // 63(?)
  0,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
  5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,
  0,5,4,4,4,4,3,4,4,1,2,3,1,5,4,4, //o
  4,4,4,4,3,4,5,5,4,5,5,0,0,0,0,0, // 0x70 - 0x7f

  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 

};

uint16_t font_pos[256];

uint8_t font_bmp[] = {

  o,o,o,x,x,x,x, //
  o,x,x,x,x,o,o,
  x,x,o,o,x,o,o,
  o,x,x,x,x,o,o,
  o,o,o,x,x,x,x,

  x,x,x,x,x,x,x,
  x,o,o,x,o,o,x,
  x,o,o,x,o,o,x,
  x,x,o,x,o,x,x,
  o,x,x,o,x,x,o,

  o,x,x,x,x,x,o,
  x,o,o,o,o,o,x,
  x,o,o,o,o,o,x,
  x,o,o,o,o,o,x,
  o,x,o,o,o,x,o,

  x,x,x,x,x,x,x,
  x,o,o,o,o,o,x,
  x,o,o,o,o,o,x,
  x,x,o,o,o,x,x,
  o,x,x,x,x,x,o,

  x,x,x,x,x,x,x,
  x,o,o,x,o,o,x,
  x,o,o,x,o,o,x,
  x,o,o,x,o,o,x,
  x,x,o,o,o,x,x,

  x,x,x,x,x,x,x,
  x,o,o,x,o,o,o,
  x,o,o,x,o,o,o,
  x,o,o,x,o,o,o,
  x,o,o,x,o,o,o,

  x,x,x,x,x,x,x,
  x,o,o,o,o,o,x,
  x,o,o,x,o,o,x,
  x,o,o,x,o,o,x,
  x,o,o,x,x,x,x,
  
  x,x,x,x,x,x,x,
  o,o,o,x,o,o,o,
  o,o,o,x,o,o,o,
  o,o,o,x,o,o,o,
  x,x,x,x,x,x,x,

  x,o,o,o,o,o,x, 
  x,o,o,o,o,o,x,
  x,x,x,x,x,x,x,
  x,o,o,o,o,o,x, 
  x,o,o,o,o,o,x, 

  x,o,o,o,o,o,x, 
  x,o,o,o,o,o,x,
  x,o,o,o,o,x,x,
  x,x,x,x,x,x,o,
  x,o,o,o,o,o,o, 

  x,x,x,x,x,x,x,
  o,o,o,x,o,o,o, 
  o,o,x,o,x,o,o,
  o,x,o,o,o,x,o,
  x,o,o,o,o,o,x,

  x,x,x,x,x,x,x,  // L
  o,o,o,o,o,o,x, 
  o,o,o,o,o,o,x, 
  o,o,o,o,o,o,x, 
  o,o,o,o,o,o,x, 

  x,x,x,x,x,x,x,
  x,x,o,o,o,o,o, 
  o,o,x,x,o,o,o,
  x,x,o,o,o,o,o, 
  x,x,x,x,x,x,x,

  x,x,x,x,x,x,x,
  x,x,o,o,o,o,o,
  o,o,x,x,x,o,o,
  o,o,o,o,o,x,x,
  x,x,x,x,x,x,x,

  o,x,x,x,x,x,o,
  x,x,o,o,o,x,x,
  x,o,o,o,o,o,x,
  x,x,o,o,o,x,x,
  o,x,x,x,x,x,o,

  x,x,x,x,x,x,x,  // p
  x,o,o,x,o,o,o,
  x,o,o,x,o,o,o,
  x,o,o,x,o,o,o,
  o,x,x,o,o,o,o,

  o,x,x,x,x,o,o,
  x,x,o,o,x,x,o,
  x,o,o,o,o,x,o,
  x,x,o,o,x,x,x,
  o,x,x,x,x,o,x,

  x,x,x,x,x,x,x,  // R
  x,o,o,x,o,o,o,
  x,o,o,x,x,o,o,
  x,o,o,x,o,x,o,
  o,x,x,o,o,o,x,

  o,x,x,o,o,o,x,
  x,o,o,x,o,o,x,
  x,o,o,x,o,o,x,
  x,o,o,x,o,o,x,
  x,o,o,o,x,x,o,

  x,o,o,o,o,o,o,  // T
  x,o,o,o,o,o,o,
  x,x,x,x,x,x,x,
  x,o,o,o,o,o,o,
  x,o,o,o,o,o,o,

  x,x,x,x,x,x,o, 
  o,o,o,o,o,o,x,
  o,o,o,o,o,o,x,
  o,o,o,o,o,o,x,
  x,x,x,x,x,x,o, 

  x,x,x,x,o,o,o,
  o,o,o,x,x,x,o,
  o,o,o,o,o,x,x,
  o,o,o,x,x,x,o,
  x,x,x,x,o,o,o,

  x,x,x,x,x,x,x,
  o,o,o,o,o,x,x,
  o,o,o,x,x,o,o,
  o,o,o,o,o,x,x,
  x,x,x,x,x,x,x,

  x,x,o,o,o,x,x,
  o,x,x,o,x,x,o,
  o,o,x,x,x,o,o,
  o,x,x,o,x,x,o,
  x,x,o,o,o,x,x,

  x,x,x,o,o,o,o,
  o,o,x,x,o,o,o,
  o,o,o,x,x,x,x,
  o,o,x,x,o,o,o,
  x,x,x,o,o,o,o,

  x,o,o,o,o,x,x,  // Z
  x,o,o,o,x,x,x,
  x,o,o,x,x,o,x,
  x,o,x,x,o,o,x,
  x,x,x,o,o,o,x,

  
  o,o,o,o,x,x,o, // 910
  o,o,o,x,o,o,x,
  o,o,o,x,o,o,x,
  o,o,o,x,x,x,x,
  o,o,o,o,o,o,x,

  x,x,x,x,x,x,x,
  o,o,o,x,o,o,x,
  o,o,o,x,o,o,x,
  o,o,o,o,x,x,o,

  o,o,o,o,x,x,o,
  o,o,o,x,o,o,x,
  o,o,o,x,o,o,x,
  o,o,o,x,o,o,x,
  
  o,o,o,o,x,x,o,
  o,o,o,x,o,o,x,
  o,o,o,x,o,o,x,
  x,x,x,x,x,x,x,

#if 0  
  o,o,o,o,x,x,o,
  o,o,o,x,o,o,x,
  o,o,o,x,x,o,x,
  o,o,o,x,x,o,x,
#else
  o,o,o,x,x,x,o,
  o,o,x,o,x,o,x,
  o,o,x,o,x,o,x,
  o,o,x,x,x,o,x,
#endif

  o,o,o,o,x,o,o,
  o,x,x,x,x,x,x,
  o,x,o,o,x,o,o,
  
  o,o,x,x,o,o,x,
  o,x,o,o,x,o,x,
  o,x,o,o,x,o,x,
  o,o,x,x,x,x,o,

  o,x,x,x,x,x,x,
  o,o,o,x,o,o,o,
  o,o,o,x,o,o,o,
  o,o,o,x,x,x,x,

  o,x,o,x,x,x,x,

  o,o,o,o,o,o,x,    //j
  o,x,o,x,x,x,x, 
  
  o,x,x,x,x,x,x,
  o,o,o,o,x,o,o,
  o,o,x,x,o,x,x,

  o,x,x,x,x,x,x,

  o,o,o,x,x,x,x,
  o,o,o,x,o,o,o,
  o,o,o,o,x,x,o,
  o,o,o,x,o,o,o,
  o,o,o,x,x,x,x,

  o,o,x,x,x,x,x,
  o,o,o,x,o,o,o,
  o,o,o,x,o,o,o,
  o,o,o,x,x,x,x,

  o,o,o,o,x,x,o,
  o,o,o,x,o,o,x,
  o,o,o,x,o,o,x,
  o,o,o,o,x,x,o,

  o,o,x,x,x,x,x,
  o,x,o,o,x,o,o,
  o,x,o,o,x,o,o,
  o,o,x,x,o,o,o,

  o,o,x,x,o,o,o,
  o,x,o,o,x,o,o,
  o,x,o,o,x,o,o,
  o,o,x,x,x,x,x,

  o,o,o,x,x,x,x,    // r
  o,o,o,o,x,o,o,
  o,o,o,x,o,o,o,
  o,o,o,x,o,o,o,

  o,o,o,x,o,o,x,
  o,o,x,o,x,o,x,
  o,o,x,o,x,o,x,
  o,o,x,o,o,x,o,

  o,o,o,x,o,o,o,
  o,o,x,x,x,x,x,
  o,o,o,x,o,o,o,

  o,o,o,x,x,x,o,
  o,o,o,o,o,o,x,
  o,o,o,o,o,o,x,
  o,o,o,x,x,x,o,

  o,o,o,x,x,o,o,
  o,o,o,o,o,x,o,
  o,o,o,o,o,o,x,
  o,o,o,o,o,x,o,
  o,o,o,x,x,o,o,

  o,o,o,x,x,o,o,
  o,o,o,o,o,x,x,
  o,o,o,x,x,o,o,
  o,o,o,o,o,x,x,
  o,o,o,x,x,o,o,

  o,o,o,x,o,o,x,
  o,o,o,o,x,x,o,
  o,o,o,o,x,x,o,
  o,o,o,x,o,o,x,

  o,o,x,x,o,o,o,
  o,o,o,x,x,o,x,
  o,o,o,o,x,x,x,
  o,o,o,x,x,o,o,
  o,o,x,x,o,o,o,

  o,o,o,x,o,o,x, // z
  o,o,o,x,o,x,x,
  o,o,o,x,x,x,x,
  o,o,o,x,x,o,x,
  o,o,o,x,o,o,x,
  
  o,o,o,o,o,o,o, // 32 (space)

  x,x,x,x,x,o,x, // 33 (!)

  o,o,x,o,x,o,x, // 42 (*)
  o,o,o,x,x,x,o,
  o,o,x,x,x,x,x,
  o,o,o,x,x,x,o,
  o,o,x,o,x,o,x,

  o,x,x,x,x,x,o, // 48 (0)
  x,o,o,o,o,o,x,
  x,o,x,x,x,o,x,
  x,o,o,o,o,o,x,
  o,x,x,x,x,x,o,

  x,x,o,o,o,o,x,
  x,x,x,x,x,x,x,
  o,o,o,o,o,o,x,

  o,x,x,o,o,x,x,
  x,x,o,o,x,x,x,
  x,o,o,o,x,o,x,
  x,o,o,x,o,o,x,
  o,x,x,o,o,o,x,
  
  o,x,o,o,o,x,o,
  x,x,o,o,o,o,x,
  x,o,o,x,o,o,x,
  x,o,o,x,o,o,x,
  o,x,x,x,x,x,o,

  x,x,x,x,o,o,o, 
  o,o,o,x,o,o,o,
  o,o,o,x,o,o,o,
  o,o,o,x,o,o,o,
  x,x,x,x,x,x,x,

  x,x,x,x,o,o,x, 
  x,o,o,x,o,o,x,
  x,o,o,x,o,o,x,
  x,o,o,x,o,o,x,
  x,o,o,o,x,x,o,

  o,x,x,x,x,x,x,
  x,o,o,x,o,o,x,
  x,o,o,x,o,o,x,
  x,o,o,x,o,o,x,
  o,o,o,x,x,x,o,

  x,x,o,o,o,o,o,
  x,o,o,o,o,o,o,
  x,o,o,o,x,x,x,
  x,o,x,x,o,o,o,  
  o,x,o,o,o,o,o,

  o,x,x,x,x,x,o,
  x,o,o,x,o,o,x,
  x,o,o,x,o,o,x,
  x,o,o,x,o,o,x,
  o,x,x,x,x,x,o,

  o,x,x,x,x,o,o, // 57(9)
  x,o,o,o,x,o,o,
  x,o,o,o,x,o,o,
  x,o,o,o,x,o,o,
  o,x,x,x,x,x,x,

  x,o,o,o,o,o,o, // 63 (?)
  x,o,o,x,x,o,x,
  x,o,o,x,o,o,o,
  x,o,o,x,o,o,o,
  o,x,x,o,o,o,o,

};
