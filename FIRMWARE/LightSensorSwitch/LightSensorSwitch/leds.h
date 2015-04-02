#ifndef LEDS_H
#define LEDS_H

void leds_init();

void leds_red_blink(long duration_us);
void leds_blue_blink(long duration_us);

void leds_red_flash(long duration_ms);
void leds_blue_flash(long duration_ms);

void leds_set_red(int led_state);
void leds_set_blue(int led_state);

#define LEDS_SHORT_BLINK 50
#define LEDS_LONG_BLINK 1000
#define LED_STATE_ON 1
#define LED_STATE_OFF 0
#endif