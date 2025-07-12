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
    int angle = 0;
    pca9685_servo(0, angle);
    while(1){
        for(;angle<=90; angle++){
            pca9685_servo(0, angle);
            blink_led(15);
            _delay_ms(20);
        }
        for(;angle>=-90; angle--){
            pca9685_servo(0, angle);
            blink_led(15);
            _delay_ms(20);
        }
        // _delay_ms(500);
        // pca9685_servo(1, 0);
    }
    
}





// The c files are now included
#include "headers/i2c.c"
#include "headers/pca9685.c"