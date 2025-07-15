#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "../led_blink.h"

int main(){
    led_init();
    while (1) {
        blink_led(0);
        _delay_ms(1000);
    }
    
}