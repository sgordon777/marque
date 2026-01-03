#ifndef FONT2812_H
#define FONT2812_H
#include <Arduino.h>
#include <FastLED.h>


void font_init(void);
void font_draw(String txt, CRGB* buf, unsigned offs, unsigned yres);
void font_xfer(CRGB* src, CRGB* dst, unsigned xres, unsigned yres, unsigned srcoffs, unsigned srclen);


#endif // FONT2812_H
