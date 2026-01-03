#include <font2812.h>
#include <font2812_sz7.h>

unsigned font_char(uint8_t a, CRGB* buf, unsigned bufpos, unsigned yres);

void font_init(void)
{
  int i;

  font_pos[65] = 0;
  for (i=66; i<(65+26); ++i)
  {
    font_pos[i] = font_pos[i-1] + FONT_HEIGHT*font_wid[i-1];
  }

  font_pos[97] = (font_pos[65+25] + FONT_HEIGHT*font_wid[65+25]);
  for (i=98; i<(97+26); ++i)
  {
    font_pos[i] = font_pos[i-1] + FONT_HEIGHT*font_wid[i-1];
  }
  font_pos[32] = font_pos[97+25] + FONT_HEIGHT*font_wid[97+25];
}

// render text onto a buffer of height yres (which can be different from font height)
//
// txt : text to render
// buf : buffer to render to
// bufoffs: offset
// yres : height in pixels of bitmap
//
//
void font_draw(String txt, CRGB* buf, unsigned bufoffs, unsigned yres)
{
  const char* cstr = txt.c_str();

  for (int i=0; i<txt.length(); ++i)
  {
    uint8_t a = cstr[i];
    // render the font
    int offs = font_char(a, buf, bufoffs,  yres) + yres; // 1 pixel space
    bufoffs += offs;
  }
}


unsigned font_char(uint8_t a, CRGB* buf, unsigned bufpos, unsigned yres)
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
//        unsigned base = bufpos + i*yres;
//        if (base & 8)
//        {
//          buf[i*yres + yres - 1 - j + bufpos] = pix | (pix<<8) | (pix<<16);
//        }
//        else
        {
          buf[i*yres + j + bufpos] = pix | (pix<<8) | (pix<<16);
        }
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

void font_xfer(CRGB* src, CRGB* dst, unsigned xres, unsigned yres, unsigned srcoffs)
{
  for (unsigned i=0; i<xres; ++i)
  {
    unsigned base = i*yres;
    for (unsigned j=0; j<yres; ++j)
    {
        CRGB pix = src[i*yres + j + srcoffs];
        if (base & 8)
        {
          dst[i*yres + yres - 1 - j] = pix;
        }
        else
        {
          dst[i*yres + j] = pix;
        }
    }
  }
}   
