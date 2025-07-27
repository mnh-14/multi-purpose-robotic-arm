#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "../leds.h"

int main(){
    led_init("B0");
    while (1) {
        led_blink("B0", 10);
        _delay_ms(50);
    }
}