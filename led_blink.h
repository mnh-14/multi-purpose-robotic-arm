#ifndef _LED_BLINK_H_
#define _LED_BLINK_H_

#ifndef F_CPU
#define F_CPU 1000000
#endif

#include <avr/io.h>
#include <util/delay.h>

#ifndef DDR_LED
#define LED_PORT PORTA
#define LED_PIN PA0
#define DDR_LED DDRA
#endif


/// @brief initializes the led pin as output. Default pin is A0
/// To change into a different pin, define DDR_LED, LED_PORT and LED_PIN
void led_init(){
    DDR_LED |= (1<<LED_PIN);
}


void led_on(){
    LED_PORT |= (1<<LED_PIN);
}


void led_off(){
    LED_PORT &= ~(1<<LED_PIN);
}

void blink_led(int times){
    static int count = 0;
    if(count >= times){
        count = 0;
        LED_PORT ^= (1<<LED_PIN);
    }
    count++;
}


/// @brief Blink rapidly with specified delay. NOTE: It will delay the machine
/// @param blink_delay amount to delay
/// @param times How many time do you want to blink
void rapid_blink_led(int blink_delay, int times){
    int d = blink_delay;
    while (times--) {
        while(d--) _delay_ms(1);
        d = blink_delay;
        blink_led(0);
    }
}


#endif
