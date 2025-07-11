#define F_CPU 8000000UL
#define BAUD_NUM 51

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "usart.h"
#include "led_blink.h"

#define TOTAL_PARAM 4

int parameters[] = {0, 0, 0, 0};


void initiate(){
    USART_init();
    led_init();
}


void extract_parameters(char * cmd){
    cmd++;
    parameters[0] = strtol(cmd, &cmd, 10);
    parameters[1] = strtol(cmd, &cmd, 10);
    parameters[2] = strtol(cmd, &cmd, 10);
    parameters[3] = strtol(cmd, &cmd, 10);
}


void main(){
    initiate();
    char cmd[20];
    led_on();
    _delay_ms(2000);
    led_off();
    USART_receive(cmd);
    USART_send_char(ENDING_CHAR);
    
    while(1){
        led_on();
        USART_receive(cmd);
        extract_parameters(cmd);
        rapid_blink_led(50, 50);
        // USART_send(cmd);
        itoa(parameters[0], cmd, 10);
        USART_send(cmd);
        itoa(parameters[1], cmd, 10);
        USART_send(cmd);
        itoa(parameters[2], cmd, 10);
        USART_send(cmd);
        itoa(parameters[3], cmd, 10);
        USART_send(cmd);
        led_off();
    }
}