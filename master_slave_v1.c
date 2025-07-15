#define F_CPU 8000000UL
#define BAUD_NUM 51

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
// #include <stdio.h>
// #include <stdlib.h>
#include "headers/i2c.h"
#include "headers/pca9685.h"
#include "usart.h"
#include "led_blink.h"
#include "adcc.h"


#define IGNORE_PRECISION_BITS 1
#define TOTAL_PARAM 4
#define TOTAL_ANGLE 6
#define ADDR 0x00
#define FREQUENCY 50
// #define OFFSET(x) (int)((x >> 4) - 7.5)
#define OFFSET(x) (int)((x >> 5) - 3.5)

volatile uint8_t receiver_ready = 0x00;
int dr=0, dtheta1=0, dtheta2=0, dphi=0;
int parameters[4] = {0, 0, 0, 0};
volatile float angles[] = {-90, 0, -90, -90, -90, -90};
volatile float contants[] = {0.25, 0.25, 0.25, 0.25, 0.25, 0.25};




void set_angles(){
    for(int i=0; i<TOTAL_ANGLE; i++){
        if(angles[i] > 0 && angles[i]>MAX_ANGLE) angles[i] = MAX_ANGLE;
        if(angles[i] < 0 && angles[i]< -1*MAX_ANGLE) angles[i] = -1 * MAX_ANGLE;
        pca9685_servo(i, angles[i]);
    }
}

float calc_angle(float angle, float da, float multiplier){
    angle += ((da > 0)? 1 : -1) * multiplier;
    if(angle > 0 && angle>MAX_ANGLE) angle = MAX_ANGLE;
    else if(angle < 0 && angle < -1*MAX_ANGLE) angle = -1 * MAX_ANGLE;
    return angle;
}

void imply_command(){
    if(parameters[0]!=0){
        angles[0] = calc_angle(angles[0], parameters[0], contants[0]);
        pca9685_servo(0, angles[0]);
    }
    if(parameters[1]!=0){
        angles[1] = calc_angle(angles[1], parameters[1], contants[1]);
        pca9685_servo(1, angles[1]);
    }
    if(parameters[2]!=0){
        angles[2] = calc_angle(angles[2], parameters[2], contants[2]);
        pca9685_servo(2, angles[2]);
    }
    if(parameters[3]!=0){
        angles[3] = calc_angle(angles[3], parameters[3], contants[3]);
        pca9685_servo(3, angles[3]);
    }
}


void initialization(){
    led_init();
    pca9685_init(ADDR, FREQUENCY);
    // USART_init();
    // USART_init_interrupt();
    ADC_single_channel_init();
}


// ISR(USART_RXC_vect){
//     receiver_ready = 0x01;
//     char ch = UDR;
// }

// void set_next_move(char * movement){
//     // sprintf(movement, "m %d %d %d %d", dr, dtheta1, dtheta2, dphi);
//     *movement++ = 'm';
//     *movement++ = ' ';
//     for(int i=0; i<TOTAL_PARAM; i++){
//         itoa(parameters[i], movement, 10);
//         while(*movement) movement++;
//         *movement++ = ' ';
//     }
//     *(movement-1) = '\0';
// }


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
    // char command[20];
    // sei();
    led_on();
    set_angles();
    _delay_ms(1000);
    led_off();
    _delay_ms(1000);
    led_on();
    _delay_ms(1000);
    // USART_send("Waiting for confirmation");
    // char ch;
    // while(!receiver_ready){ // Wait until receiver is ready
    //     // blink_led(1);
    //     _delay_ms(60);
    // }
    // ch = USART_receive_char();
    // led_off();
    
    while(1){
        // _delay_ms(50);
        // blink_led(1);
        led_on();
        _delay_ms(5);
        // ch = USART_receive_char();
        // if(receiver_ready != 0x01){
        //     _delay_ms(5);
        //     blink_led(50);
        //     continue;
        // }
        receiver_ready = 0x00;
        led_off();
        compile_joystic_input();
        imply_command();
        // set_next_move(command);
        // rapid_blink_led(50, 100);
        // USART_send(command);
        led_off();
    }
}


#include "headers/i2c.c"
#include "headers/pca9685.c"