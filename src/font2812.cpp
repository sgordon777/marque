#include <font2812.h>
#include <font2812_sz7h4.h>

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
  font_pos[33] = font_pos[32] + FONT_HEIGHT*font_wid[32]; // !
  font_pos[42] = font_pos[33] + FONT_HEIGHT*font_wid[33]; // *
  font_pos[63] = font_pos[42] + FONT_HEIGHT*font_wid[42]; // ?




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

void font_xfer(CRGB* src, CRGB* dst, unsigned xres, unsigned yres, unsigned srcoffs, unsigned srclen)
{
  for (unsigned i=0; i<xres; ++i)
  {
    unsigned dstbase = i*yres;
    unsigned srcbase = (i*yres + srcoffs) % srclen;
    for (unsigned j=0; j<yres; ++j)
    {
        CRGB pix = src[srcbase + j];
        if (dstbase & 8)
        {
          dst[dstbase + yres - 1 - j] = pix;
        }
        else
        {
          dst[dstbase + j] = pix;
        }
    }
  }
}   
