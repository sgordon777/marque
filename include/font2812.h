#ifndef FONT2812_H
#define FONT2812_H
#include <Arduino.h>
#include <FastLED.h>

#define COLOR_DEFAULT       (256)
#define COLOR_CYCLE         (257)
#define COLOR_ALT2           (258)
#define COLOR_ALT3           (259)
#define COLOR_BACKGROUND    (0)




void font_init(void);
unsigned font_draw(String txt, uint8_t* buf, unsigned bufoffs, unsigned yres, unsigned fgcolor, unsigned p1=0, unsigned p2=0, unsigned p3=0);
void font_xfer(uint8_t* src, CRGB* dst, unsigned xres, unsigned yres, unsigned srcoffs, unsigned srclen);


#endif // FONT2812_H
