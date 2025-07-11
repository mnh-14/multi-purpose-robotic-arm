#define F_CPU 8000000UL
#define BAUD_NUM 51

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "usart.h"
#include "led_blink.h"


volatile uint8_t receiver_ready = 0x00;
int dr, dtheta1, dtheta2, dphi;

void initialization(){
    led_init();
    USART_init();
    USART_init_interrupt();
}


ISR(USART_RXC_vect){
    receiver_ready = 0x01;
}

void set_next_move(char * movement){
    sprintf(movement, "m %d %d %d %d", dr, dtheta1, dtheta2, dphi);
}

int main(){
    initialization();
    char command[20];
    sei();
    blink_led(0);
    _delay_ms(1000);
    blink_led(0);
    USART_send("Waiting for confirmation");
    while(!receiver_ready){ // Wait until receiver is ready
        blink_led(1);
        _delay_ms(60);
    }
    dr = 2; dtheta1 = -1; dtheta2 = 3; dphi = 8;
    
    while(1){
        _delay_ms(300);
        blink_led(0);
        if(!receiver_ready) continue;
        receiver_ready = 0x00;
        rapid_blink_led(10, 15);
        set_next_move(command);
        USART_send(command);        
    }
}