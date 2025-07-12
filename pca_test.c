#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "headers/i2c.h"
#include "headers/pca9685.h"
#include "led_blink.h"


int main(){
    pca9685_init(0x00, 50);
    led_init();
    _delay_ms(500);
    pca9685_servo(0, 0);
    while(1){
        _delay_ms(500);
        blink_led(0);
        // _delay_ms(500);
        // pca9685_servo(1, 0);
    }
    
}





// The c files are now included
#include "headers/i2c.c"
#include "headers/pca9685.c"