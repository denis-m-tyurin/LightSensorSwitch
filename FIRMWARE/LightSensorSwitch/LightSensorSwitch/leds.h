#ifndef LEDS_H
#define LEDS_H

void leds_init();

void leds_red_blink(long duration_us);
void leds_blue_blink(long duration_us);

#define LEDS_SHORT_BLINK 1
#define LEDS_LONG_BLINK 100000

#endif