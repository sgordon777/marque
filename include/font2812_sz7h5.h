#include <Arduino.h>

#define x (0x7)
#define o (0x00)
#define FONT_HEIGHT (7)

uint8_t font_wid[256] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 0x00 - 0x0f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,1,3,5,5,5,5,1,3,3,5,3,1,3,1,5, // 32-47: space, !, ", #, $, %, &, ', (, ), *, +, ,, -, ., /
  5,3,5,5,5,5,5,5,5,5,1,1,3,3,3,5, // 48-57 = 0-9, 58-63(:,;,<,=,>,?)
  5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5, // @, A-O
  5,5,5,5,5,5,5,5,5,5,5,3,5,3,5,5, // P-Z,[,\],^,_,
  2,5,4,4,4,5,4,4,5,3,3,4,3,5,5,4, // tic, a-o
  4,4,4,4,3,5,5,5,5,5,5,0,0,0,0,0, // p-z

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

  o,o,o,o,o,o,o, // 32 (space)

  x,x,x,x,x,o,x, // 33 (!)

  x,x,o,o,o,o,o, // 34 (")") 
  o,o,o,o,o,o,o,
  x,x,o,o,o,o,o, 

  o,o,x,o,x,o,o, // 35
  x,x,x,x,x,x,x,
  o,o,x,o,x,o,o,
  x,x,x,x,x,x,x,
  o,o,x,o,x,o,o,

  o,x,x,o,o,o,x, // 36 ($)
  x,o,o,x,o,o,x,
  x,x,x,x,x,x,x,
  x,o,o,x,o,o,x,
  x,o,o,o,x,x,o,

  o,x,x,o,o,o,x, // 37 (%)
  o,x,x,o,x,x,o,
  o,o,o,x,x,o,o,
  o,x,x,o,o,x,x,
  x,o,o,o,o,x,x, 

  o,x,x,o,x,x,o, // 38 (&)
  x,o,o,x,o,o,x,
  x,o,o,x,o,o,x,
  o,x,x,o,x,o,x,
  o,o,o,o,o,x,x,

  x,x,o,o,o,o,o, // 39 (')
  
  o,x,x,x,x,x,o, // 40 (()
  x,x,o,o,o,x,x,
  x,o,o,o,o,o,x,

  x,o,o,o,o,o,x, // 41 ())
  x,x,o,o,o,x,x,
  o,x,x,x,x,x,o, 

  o,o,x,o,x,o,x, // 42 (*)
  o,o,o,x,x,x,o,
  o,o,x,x,x,x,x,
  o,o,o,x,x,x,o,
  o,o,x,o,x,o,x,

  o,o,o,x,o,o,o, // 43 (+)
  o,o,x,x,x,o,o,
  o,o,o,x,o,o,o,

  o,o,o,o,o,o,x, // 44 (,)

  o,o,o,x,o,o,o, // 45 (-)
  o,o,o,x,o,o,o,
  o,o,o,x,o,o,o,

  o,o,o,o,o,o,x, // 46 (.)

  o,o,o,o,o,o,x, // 47 (%)
  o,o,o,o,x,x,o,
  o,o,o,x,x,o,o,
  o,x,x,o,o,o,o,
  x,o,o,o,o,o,o,

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

  o,o,x,o,o,x,o, // 58 (:)

  o,o,x,o,o,x,x, // 59 (;)

  o,o,o,x,o,o,o, // 60 (<)
  o,o,x,o,x,o,o,
  o,x,o,o,o,x,o,

  o,o,x,o,x,o,o, // 61 (=)
  o,o,x,o,x,o,o,
  o,o,x,o,x,o,o,

  o,x,o,o,o,x,o, // 62 (>)
  o,o,x,o,x,o,o,
  o,o,o,x,o,o,o, 

  x,o,o,o,o,o,o, // 63 (?)
  x,o,o,x,x,o,x,
  x,o,o,x,o,o,o,
  x,o,o,x,o,o,o,
  o,x,x,o,o,o,o,
  
  o,x,x,x,x,x,o, // 64 (@)
  x,o,x,x,x,o,x,
  x,x,o,o,x,x,x,
  x,o,x,x,x,o,x,
  o,x,x,x,x,x,o,

  o,o,o,x,x,x,x, // 65 (A)
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

  x,o,o,o,o,x,x,  // 90 (Z)
  x,o,o,o,x,x,x,
  x,o,o,x,x,o,x,
  x,o,x,x,o,o,x,
  x,x,x,o,o,o,x,


  x,x,x,x,x,x,x,  // 91-96 ([ \ ] ^ _ `)
  x,o,o,o,o,o,x,
  x,o,o,o,o,o,x,

  x,o,o,o,o,o,o, 
  o,x,x,o,o,o,o,
  o,o,o,x,x,o,o,
  o,o,o,o,x,x,o,
  o,o,o,o,o,o,x, 
  
  x,o,o,o,o,o,x,
  x,o,o,o,o,o,x,
  x,x,x,x,x,x,x,
  
  o,o,x,o,o,o,o, 
  o,x,o,o,o,o,o,
  x,o,o,o,o,o,o,
  o,x,o,o,o,o,o,
  o,o,x,o,o,o,o,

  o,o,o,o,o,o,x, 
  o,o,o,o,o,o,x,
  o,o,o,o,o,o,x,
  o,o,o,o,o,o,x,
  o,o,o,o,o,o,x,

  o,x,o,o,o,o,o,
  x,o,o,o,o,o,o,


// lower

  o,o,o,o,o,x,o, // 97 (a)
  o,o,x,o,x,o,x,
  o,o,x,o,x,o,x,
  o,o,o,x,x,x,x,
  o,o,o,o,o,o,x,

  o,x,x,x,x,x,x,
  o,o,o,x,o,o,x,
  o,o,o,x,o,o,x,
  o,o,o,o,x,x,o,

  o,o,o,x,x,x,o,
  o,o,x,o,o,o,x,
  o,o,x,o,o,o,x,
  o,o,x,o,o,o,x,
  
  o,o,o,o,x,x,o,
  o,o,o,x,o,o,x,
  o,o,o,x,o,o,x,
  o,x,x,x,x,x,o,

  o,o,o,x,x,x,o,
  o,o,x,o,x,o,x,
  o,o,x,o,x,o,x,
  o,o,x,o,x,o,x,
  o,o,o,x,x,o,o,

  o,o,o,x,o,o,o,
  o,x,x,x,x,x,x,
  x,o,o,x,o,o,o,
  x,x,o,o,o,o,o,
  
  o,o,x,x,o,o,o,
  o,x,o,o,x,o,x,
  o,x,o,o,x,o,x,
  o,o,x,x,x,x,o,

  x,x,x,x,x,x,x,
  o,o,o,x,o,o,o,
  o,o,x,o,o,o,o,
  o,o,x,o,o,o,o,
  o,o,o,x,x,x,x,

  o,o,x,o,o,o,x,
  x,o,x,x,x,x,x,
  o,o,o,o,o,o,x,

  o,o,o,o,o,x,o,
  o,o,o,o,o,o,x,
  x,o,x,x,x,x,x,    //j

  o,x,x,x,x,x,x,
  o,o,o,x,x,o,o,
  o,o,x,o,o,x,o,
  o,x,o,o,o,o,x,

  x,o,o,o,o,o,x,
  x,x,x,x,x,x,x,
  o,o,o,o,o,o,x,

  o,o,x,x,x,x,x,
  o,o,x,o,o,o,o,
  o,o,o,x,x,o,o,
  o,o,x,o,o,o,o,
  o,o,x,x,x,x,x,

  o,o,x,x,x,x,x, 
  o,o,o,x,o,o,o,
  o,o,x,o,o,o,o,
  o,o,x,o,o,o,o,
  o,o,o,x,x,x,x,

  o,o,o,x,x,x,o, // o
  o,o,x,o,o,o,x,
  o,o,x,o,o,o,x,
  o,o,o,x,x,x,o,

  o,o,o,x,x,x,x,
  o,o,x,o,o,x,o,
  o,o,x,o,o,x,o,
  o,o,o,x,x,o,o,

  o,o,o,x,x,o,o,
  o,o,x,o,o,x,o,
  o,o,x,o,o,x,o,
  o,o,o,x,x,x,x,

  o,o,x,x,x,x,x,    // r
  o,o,o,x,o,o,o,
  o,o,x,o,o,o,o,
  o,o,x,o,o,o,o,

  o,o,o,x,o,o,x,
  o,o,x,o,x,o,x,
  o,o,x,o,x,o,x,
  o,o,x,o,o,x,o,

  o,o,x,o,o,o,o,
  x,x,x,x,x,x,x,
  o,o,x,o,o,o,x,

  o,o,x,x,x,x,o,
  o,o,o,o,o,o,x,
  o,o,o,o,o,o,x,
  o,o,x,x,x,x,x,
  o,o,o,o,o,o,x,

  o,o,x,x,x,o,o,
  o,o,o,o,x,x,o,
  o,o,o,o,o,x,x,
  o,o,o,o,x,x,o,
  o,o,x,x,x,o,o,

  o,o,x,x,x,x,x, // w
  o,o,o,o,o,x,o,
  o,o,o,x,x,o,o,
  o,o,o,o,o,x,o,
  o,o,x,x,x,x,x,

  o,o,x,o,o,o,x,
  o,o,o,x,o,x,o,
  o,o,o,o,x,o,o,
  o,o,o,x,o,x,o,
  o,o,x,o,o,o,x,

  o,o,x,x,o,o,x,
  o,o,o,x,x,o,x,
  o,o,o,o,x,x,x,
  o,o,o,x,x,o,o,
  o,o,x,x,o,o,o,
  
  o,o,x,o,o,x,x,
  o,o,x,o,x,x,x,
  o,o,x,o,x,o,x,
  o,o,x,x,x,o,x,
  o,o,x,x,o,o,x,    
  
  o,x,x,x,o,o,o, // b0 (°)  
  o,x,o,x,o,o,o,
  o,x,x,x,o,o,o,

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

