#define F_CPU 8000000UL
#define BAUD_NUM 51

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"
#include "led_blink.h"
#include "adcc.h"


#define IGNORE_PRECISION_BITS 1
#define TOTAL_PARAM 4
// #define OFFSET(x) (int)((x >> 4) - 7.5)
#define OFFSET(x) (int)((x >> 5) - 3.5)

volatile uint8_t receiver_ready = 0x00;
int dr=0, dtheta1=0, dtheta2=0, dphi=0;
int parameters[4] = {0, 0, 0, 0};

void initialization(){
    led_init();
    USART_init();
    // USART_init_interrupt();
    ADC_single_channel_init();
}


// ISR(USART_RXC_vect){
//     receiver_ready = 0x01;
//     char ch = UDR;
// }

void set_next_move(char * movement){
    // sprintf(movement, "m %d %d %d %d", dr, dtheta1, dtheta2, dphi);
    *movement++ = 'm';
    *movement++ = ' ';
    for(int i=0; i<TOTAL_PARAM; i++){
        itoa(parameters[i], movement, 10);
        while(*movement) movement++;
        *movement++ = ' ';
    }
    *(movement-1) = '\0';
}


void compile_joystic_input(){
    // dr = ADC_read_channel(1) >> IGNORE_PRECISION_BITS;
    // dtheta1 = ADC_read_channel(2) >> IGNORE_PRECISION_BITS;
    // dtheta2 = ADC_read_channel(3) >> IGNORE_PRECISION_BITS;
    // dphi = ADC_read_channel(4) >> IGNORE_PRECISION_BITS;
    blink_led(100);
    parameters[0] = OFFSET(ADC_read_channel(0x01));
    parameters[1] = OFFSET(ADC_read_channel(0x02));
    parameters[2] = OFFSET(ADC_read_channel(0x03));
    parameters[3] = OFFSET(ADC_read_channel(0x04));
    blink_led(100);
}


int main(){
    initialization();
    char command[20];
    // sei();
    led_on();
    _delay_ms(1000);
    led_off();
    _delay_ms(1000);
    led_on();
    _delay_ms(1000);
    led_off();
    _delay_ms(1000);
    USART_send("Waiting for confirmation");
    _delay_ms(50);
    led_on();
    char ch;
    // while(!receiver_ready){ // Wait until receiver is ready
    //     // blink_led(1);
    //     _delay_ms(60);
    // }
    ch = USART_receive_char();
    led_off();
    
    while(1){
        // _delay_ms(50);
        // blink_led(1);
        led_on();
        ch = USART_receive_char();
        // if(receiver_ready != 0x01){
        //     _delay_ms(5);
        //     blink_led(50);
        //     continue;
        // }
        receiver_ready = 0x00;
        led_off();
        _delay_ms(10);
        compile_joystic_input();
        set_next_move(command);
        // rapid_blink_led(50, 100);
        led_on();
        USART_send(command);
        led_off();
    }
}
