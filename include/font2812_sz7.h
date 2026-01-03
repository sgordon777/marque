#include <Arduino.h>


#define x (0xff)
#define o (0x00)
#define FONT_HEIGHT (7)

uint8_t font_wid[128] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
  5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,
  0,5,4,4,4,4,3,4,3,1,2,3,1,5,4,4, //o
  4,4,4,4,3,4,3,5,4,3,3,0,0,0,0,0
};

uint16_t font_pos[128];

uint8_t font_bmp[] = {

  o,o,o,o,x,x,x,
  o,o,x,x,x,o,o,
  x,x,x,o,x,o,o,
  o,o,x,x,x,o,o,
  o,o,o,o,x,x,x,

  x,x,x,x,x,x,x,
  x,o,o,x,o,o,x,
  x,o,o,x,o,o,x,
  x,x,o,x,o,x,x,
  o,x,x,x,x,x,o,

  x,x,x,x,x,x,x,
  x,o,o,o,o,o,x,
  x,o,o,o,o,o,x,
  x,o,o,o,o,o,x,
  x,o,o,o,o,o,x,

  x,x,x,x,x,x,x,
  x,o,o,o,o,o,x,
  x,o,o,o,o,o,x,
  x,x,o,o,o,x,x,
  o,x,x,x,x,x,o,

  x,x,x,x,x,x,x,
  x,o,o,x,o,o,x,
  x,o,o,x,o,o,x,
  x,o,o,x,o,o,x,
  x,o,o,x,o,o,x,

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

  o,o,o,o,o,o,x, 
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
  o,o,x,x,x,o,o,
  x,x,o,o,o,o,o, 
  x,x,x,x,x,x,x,

  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,

  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,

  x,x,x,x,x,x,x,  // p
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,

  x,x,x,x,x,x,x, 
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,

  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,

  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,

  x,x,x,x,x,x,x,  // T
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,

  x,x,x,x,x,x,x, 
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,

  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,

  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,

  x,x,x,x,x,x,x,  // x
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,

  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,

  x,x,x,x,x,x,x,  // Z
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,

  
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

  o,o,o,o,x,x,o,
  o,o,o,x,o,o,x,
  o,o,o,x,x,o,x,
  o,o,o,x,x,o,x,

  o,o,o,o,x,o,o,
  o,x,x,x,x,x,x,
  o,x,o,o,x,o,o,
  
  o,o,x,x,o,o,x,
  o,x,o,o,x,o,x,
  o,x,o,o,x,o,x,
  o,o,x,x,x,x,o,

  o,x,x,x,x,x,x,
  o,o,o,x,x,o,o,
  o,o,o,x,x,x,x,

  o,x,o,x,x,x,x,

  o,x,o,x,x,x,x,
  o,o,o,o,o,o,x,

  o,x,x,x,x,x,x,
  o,o,o,o,x,o,o,
  o,o,x,x,x,x,x,

  o,x,x,x,x,x,x,

  o,o,x,x,x,x,x,
  o,o,o,x,o,o,o,
  o,o,o,o,x,x,o,
  o,o,o,x,o,o,o,
  o,o,x,x,x,x,x,

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

  o,o,o,x,x,x,x,            // r
  o,o,o,o,x,o,o,
  o,o,o,x,o,o,o,
  o,o,o,x,o,o,o,

  o,o,o,x,o,o,x,
  o,o,x,o,x,o,x,
  o,o,x,o,x,o,x,
  o,o,x,o,o,x,o,

  o,o,o,o,x,o,o,
  o,o,x,x,x,x,x,
  o,o,o,o,x,o,o,

  o,o,o,x,x,x,o,
  o,o,o,o,o,o,x,
  o,o,o,o,o,o,x,
  o,o,o,x,x,x,o,

  o,o,x,x,x,o,o,
  o,o,o,o,o,x,x,
  o,o,x,x,x,o,o,

  o,o,o,x,x,o,o,
  o,o,o,o,o,x,x,
  o,o,o,x,x,o,o,
  o,o,o,o,o,x,x,
  o,o,o,x,x,o,o,

  o,o,x,o,o,o,x,
  o,o,o,x,x,x,o,
  o,o,o,x,x,x,o,
  o,o,x,o,o,o,x,

  o,o,x,x,o,o,o,
  o,o,o,o,x,x,x,
  o,o,x,x,o,o,o,

  o,o,x,o,o,x,x,    // z
  o,o,x,o,x,o,x,
  o,o,x,x,o,o,x,
  
  o,o,o,o,o,o,o,
  o,o,o,o,o,o,o,
  o,o,o,o,o,o,o,
  o,o,o,o,o,o,o,
  o,o,o,o,o,o,o,

  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,
  x,x,x,x,x,x,x,

};
