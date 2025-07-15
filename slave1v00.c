#define F_CPU 8000000UL
#define BAUD_NUM 51

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "headers/i2c.h"
#include "headers/pca9685.h"
#include "usart.h"
#include "led_blink.h"

#define TOTAL_PARAM 4
#define TOTAL_ANGLE 6
#define ADDR 0x00
#define FREQUENCY 50

int parameters[] = {0, 0, 0, 0};
volatile float angles[] = {-90, -90, -90, -90, -90, -90};
volatile float contants[] = {0.25, 0.25, 0.25, 0.25, 0.25, 0.25};


void initiate(){
    USART_init();
    led_init();
    pca9685_init(ADDR, FREQUENCY);
}


void extract_parameters(char * cmd){
    // cmd++;
    // parameters[0] = strtol(cmd, &cmd, 10);
    // parameters[1] = strtol(cmd, &cmd, 10);
    // parameters[2] = strtol(cmd, &cmd, 10);
    // parameters[3] = strtol(cmd, &cmd, 10);
    parameters[0] = +2;
    parameters[1] = +2;
    parameters[2] = +2;
    parameters[3] = +2;
}


void imply_command(){
    if(parameters[0]!=0){
        angles[0] += ((parameters[0]>0)? 1 : -1) * contants[0];
        pca9685_servo(0, angles[0]);
    }
    if(parameters[1]!=0){
        angles[1] += ((parameters[1]>0)? 1 : -1) * contants[1];
        pca9685_servo(1, angles[1]);
    }
    if(parameters[2]!=0){
        angles[2] += ((parameters[2]>0)? 1 : -1) * contants[2];
        pca9685_servo(2, angles[2]);
    }
}


void set_angles(){
    for(int i=0; i<TOTAL_ANGLE; i++){
        pca9685_servo(i, angles[i]);
    }
}


void main(){
    initiate();
    char cmd[20];
    led_on();
    set_angles();
    _delay_ms(2000);
    led_off();
    // if (UCSRA & (1 << FE)) {
    //     rapid_blink_led(50, 50);
    // }
    // USART_receive(cmd);
    USART_receive_char();
    _delay_ms(50);
    USART_send_char(ENDING_CHAR);
    _delay_ms(50);
    
    while(1){
        // USART_receive(cmd);
        USART_receive_char();
        led_on();
        extract_parameters(cmd);
        // rapid_blink_led(50, 50);
        // USART_send(cmd);
        // itoa(parameters[0], cmd, 10);
        // USART_send(cmd);
        // itoa(parameters[1], cmd, 10);
        // USART_send(cmd);
        // itoa(parameters[2], cmd, 10);
        // USART_send(cmd);
        // itoa(parameters[3], cmd, 10);
        // USART_send(cmd);
        
        imply_command();
        led_off();
        USART_send_char(ENDING_CHAR);
    }
}

#include "headers/i2c.c"
#include "headers/pca9685.c"
