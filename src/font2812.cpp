#include <font2812.h>
#include <font2812_sz7h5.h>


CRGB pallet[] = {
  CRGB(0,0,0),              // 0=black
  CRGB(255,0,0),            // 1=red
  CRGB(0,255,0),            // 2=green
  CRGB(0,0,255),            // 3=blue
  CRGB(255,255,0),          // 4=yellow
  CRGB(0,255,255),          // 5=cyan
  CRGB(255,0,255),          // 6=magenta
  CRGB(255,255,255)         // 7=white        
};

#define NUM_COLOR       (8)

unsigned font_char(uint8_t a, uint8_t* bufoffs, unsigned bufpos, unsigned yres, unsigned fgcolor);

void font_init(void)
{
  int i;

  font_pos[32] = 0;
  for (i=33; i<(123); ++i)  // space to 'z'
  {
    font_pos[i] = font_pos[i-1] + FONT_HEIGHT*font_wid[i-1];
  }

  font_pos[176] = font_pos[122] + FONT_HEIGHT*font_wid[122]; // °

}

// render text onto a buffer of height yres (which can be different from font height)
//
// txt : text to render
// buf : buffer to render to
// bufoffs: offset
// yres : height in pixels of bitmap
//
//
unsigned font_draw(String txt, uint8_t* buf, unsigned bufoffs, unsigned yres, unsigned fgcolor, unsigned p1, unsigned p2, unsigned p3)
{
  const char* cstr = txt.c_str();
  unsigned color = fgcolor;

  if (fgcolor == COLOR_CYCLE)
    color = p1;
  else if (fgcolor == COLOR_ALT2)
    color = p1;
  if (fgcolor == COLOR_ALT3)
    color = p1;

  for (int i=0; i<txt.length(); ++i)
  {

    // render the font
    uint8_t a = cstr[i];
    int offs = font_char(a, buf, bufoffs,  yres, color) + yres; // 1 pixel space
    bufoffs += offs;
    
    if (fgcolor == COLOR_CYCLE)
    {
      if (color == p2) color = p1;
      color = color + 1;
    }
    if (fgcolor == COLOR_ALT2)
    {
        if (color == p1) color = p2;
        else if (color == p2) color = p1;
    }
    if (fgcolor == COLOR_ALT3)
    {
        if (color == p1) color = p2;
        else if (color == p2) color = p3;
        else if (color == p3) color = p1;
    }
    if (color >= NUM_COLOR) color = 1;

  }
  return bufoffs;
}




unsigned font_char(uint8_t a, uint8_t* buf, unsigned bufpos, unsigned yres, unsigned fgcolor)
{
  unsigned w = 5;
  unsigned h = FONT_HEIGHT;
  unsigned i,j,k;

  w = font_wid[a];
  k = font_pos[a];
  for (i=0; i<w; i++)
  {
    for (j=0; j<h; ++j)
    {
        unsigned pix = font_bmp[k++];

        // 0 : background, don't ever override
        // 1-255 : pallet colors, override with fgcolor unless fgcolor is COLOR_DEFAULT
        if (pix != 0)
        {
            if (fgcolor != COLOR_DEFAULT)
            {
                pix = fgcolor;
            }
        }
        buf[i*yres + j + bufpos] = pix;
    }
  }

  return w*yres;
}

// copy buffer to buffer
//
// src: source buffer
// dst: dest buffer
// xres: horiz resolution of dest bitmap
// yres: horiz resolution of dest and source bitmap
// srcoffs: start offset 

void font_xfer(uint8_t* src, CRGB* dst, unsigned xres, unsigned yres, unsigned srcoffs, unsigned srclen)
{
  for (unsigned i=0; i<xres; ++i)
  {
    unsigned dstbase = i*yres;
    unsigned srcbase = (i*yres + srcoffs) % srclen;
    for (unsigned j=0; j<yres; ++j)
    {
        uint8_t pix = src[srcbase + j];
        if (dstbase & 8)
        {
          dst[dstbase + yres - 1 - j] = pallet[pix];
        }
        else
        {
          dst[dstbase + j] = pallet[pix];
        }
    }
  }
}   
