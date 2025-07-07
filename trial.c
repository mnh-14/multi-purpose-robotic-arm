#define F_CPU 8000000UL
#define BAUD_NUM 51
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usart.h"
#include "led_blink.h"


int main(){
    led_init();
    USART_init();
    USART_init_interrupt();
    sei();
    while(1){
        USART_send("Hello");
        blink_led(0);
        _delay_ms(500);
    }

}