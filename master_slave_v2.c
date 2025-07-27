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
uint32_t angle_values[] = {ANGLE_0, ANGLE_90, ANGLE_0, ANGLE_0, ANGLE_0, ANGLE_0};
enum delta multipliers[] = { O_1, O_1, O_25, O_25, O_25, O_25 };


void set_angles(){
    for(int i=0; i<TOTAL_ANGLE; i++){
        pca9685_servo_with_value(i, angle_values[i]);
    }
}

uint32_t calc_angle_value(uint32_t angle_value, int da, enum delta multiplier){
    angle_value += ((da > 0)? 1 : -1) * delta_value(multiplier);
    if(angle_value>ANGLE_180) angle_value = ANGLE_180;
    else if(angle_value < ANGLE_0) angle_value = ANGLE_0;
    return angle_value;
}

// uint32_t calc_angle_value(uint32_t angle_value, int da, )

void imply_command(){
    if(parameters[0]!=0){
        angle_values[0] = calc_angle_value(angle_values[0], parameters[0], multipliers[0]);
        pca9685_servo_with_value(0, angle_values[0]);
    }
    if(parameters[1]!=0){
        angle_values[1] = calc_angle_value(angle_values[1], parameters[1], multipliers[1]);
        pca9685_servo_with_value(1, angle_values[1]);
    }
    if(parameters[2]!=0){
        angle_values[2] = calc_angle_value(angle_values[2], parameters[2], multipliers[2]);
        pca9685_servo_with_value(2, angle_values[2]);
    }
    if(parameters[3]!=0){
        angle_values[3] = calc_angle_value(angle_values[3], parameters[3], multipliers[3]);
        pca9685_servo_with_value(3, angle_values[3]);
    }
}


void initialization(){
    led_init();
    pca9685_init_50(ADDR);
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