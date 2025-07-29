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
#define GRIP_LIMIT 200
#define PICK_POS_LIMIT 2
// #define OFFSET(x) (int)((x >> 4) - 7.5)
#define OFFSET(x) (int)((x >> 5) - 3.5)

volatile uint8_t receiver_ready = 0x00;
int dr=0, dtheta1=0, dtheta2=0, dphi=0;
// 0-> r++, 1->left/right, 2->up/down, 3->grip/ungrip
int parameters[4] = {0, 0, 0, 0};
uint32_t angle_values[] = {ANGLE_0, ANGLE_90, ANGLE_90, ANGLE_90, ANGLE_180, ANGLE_0};
enum delta multipliers[] = { O_1, O_1, O_1, O_1, O_25, O_25 };

enum states {searching_ground, stablizing, searching_object, pick_positioning, grabbing, picking, done};
enum states curr_state = searching_ground;

void find_ground();
void stablize_above_ground();
void find_object();
void take_position_to_pick();
void grab_the_object();
void pickup_n_move();

void state_change_indicate(){
    led_on();
    _delay_ms(1000);
    led_off();
}


void ir_pin_init(){
    DDRA &= ~((1 << PA1) | (1 << PA2) | (1 << PA3));
    PORTA |= ((1 << PA1) | (1 << PA2) | (1 << PA3));
}


uint8_t left_ir(){
    return (PINA & (1<<PA1))==0x00;
}
uint8_t right_ir(){
    return (PINA & (1<<PA3))==0x00;

}
uint8_t center_ir(){
    return (PINA & (1<<PA2))==0x00;
}


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
void increase_r(){
    angle_values[1] = calc_angle_value(angle_values[1], -parameters[0], multipliers[1]);
    angle_values[2] = calc_angle_value(angle_values[2], parameters[0], multipliers[2]);
    angle_values[3] = calc_angle_value(angle_values[3], parameters[0], multipliers[3]);
    pca9685_servo_with_value(1, angle_values[1]);
    pca9685_servo_with_value(2, angle_values[2]);
    pca9685_servo_with_value(3, angle_values[3]);
}

void imply_command(){
    if(parameters[0]!=0){
        blink_led(50);
        increase_r();
    }
    if(parameters[1]!=0){
        blink_led(50);
        angle_values[0] = calc_angle_value(angle_values[0], parameters[1], multipliers[0]);
        pca9685_servo_with_value(0, angle_values[0]);
    }
    if(parameters[2]!=0){
        blink_led(50);
        angle_values[1] = calc_angle_value(angle_values[1], parameters[2], multipliers[1]);
        pca9685_servo_with_value(1, angle_values[1]);
    }
    if(parameters[3]!=0){
        blink_led(50);
        angle_values[5] = calc_angle_value(angle_values[5], parameters[3], multipliers[5]);
        pca9685_servo_with_value(5, angle_values[5]);
    }
}


void initialization(){
    led_init();
    pca9685_init_50(ADDR);
    ir_pin_init();
    // USART_init();
    // USART_init_interrupt();
    // ADC_single_channel_init();

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


// void compile_joystic_input(){
//     // dr = ADC_read_channel(1) >> IGNORE_PRECISION_BITS;
//     // dtheta1 = ADC_read_channel(2) >> IGNORE_PRECISION_BITS;
//     // dtheta2 = ADC_read_channel(3) >> IGNORE_PRECISION_BITS;
//     // dphi = ADC_read_channel(4) >> IGNORE_PRECISION_BITS;
//     blink_led(100);
//     parameters[0] = OFFSET(ADC_read_channel(0x01));
//     parameters[1] = OFFSET(ADC_read_channel(0x02));
//     parameters[2] = OFFSET(ADC_read_channel(0x03));
//     parameters[3] = OFFSET(ADC_read_channel(0x04));
//     blink_led(100);
// }


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
    for(int i=0; i<15; i++){
        parameters[0] = +2;
        imply_command();
        _delay_ms(5);
    }
    parameters[0] = 0;
    
    
    while(1){
        _delay_ms(100);
        switch(curr_state){
            case searching_ground:
                find_ground(); break;
            case stablizing:
                stablize_above_ground(); break;
            case searching_object:
                find_object(); break;
            case pick_positioning:
                take_position_to_pick(); break;
            case grabbing:
                grab_the_object(); break;
            case picking:
                pickup_n_move(); break;
            default:
                // Nothing
        }
        state_change_indicate();
    }
}


void find_ground(){
    while(1){
        blink_led(10);
        parameters[2] = +2;
        imply_command();
        _delay_ms(5);
        if(left_ir()==0x01 && center_ir()==0x01 && right_ir()==0x01){
            parameters[2] = 0;
            curr_state = stablizing;
            return;
        }
    }
}

void stablize_above_ground(){
    while(1){
        blink_led(10);
        parameters[2] = -2;
        imply_command();
        _delay_ms(5);
        if(left_ir()==0x00 && center_ir()==0x00 && right_ir()==0x00){
            parameters[2] = 0;
            curr_state = searching_object;
            return;
        }
    }    
}

void find_object(){
    uint8_t d = +2;
    while(1){
        blink_led(10);
        parameters[1] = d;
        imply_command();
        _delay_ms(5);
        if(left_ir()==0x00 && center_ir()==0x01 && right_ir()==0x00){
            parameters[1] = 0;
            curr_state = pick_positioning;
            return;
        }
        // if(angle_values[0] <= ANGLE_0 || angle_values[0] >= ANGLE_180){
        //     d = -d;
        //     parameters[0] = -2;
        //     parameters[1] = d;
        //     imply_command();
        //     _delay_ms(5);
        //     imply_command();
        //     _delay_ms(5);
        //     imply_command();
        //     _delay_ms(5);
        //     parameters[0] = 0;
        // }
    }

}

void take_position_to_pick(){
    uint8_t picking_count = 0;
    while(1){
        blink_led(10);
        parameters[2] = +2;
        imply_command();
        _delay_ms(5);
        picking_count++;
        if(left_ir()==0x01 && center_ir()==0x01 && right_ir()==0x01){
            imply_command();
            imply_command();
            imply_command();
            // if(picking_count > PICK_POS_LIMIT){
            parameters[2] = 0;
            curr_state = grabbing;
            return;
            // }
        }
    }
}

void grab_the_object(){
    uint8_t grip_count = 0;
    while(1){
        blink_led(10);
        parameters[3] = +2;
        imply_command();
        _delay_ms(5);
        grip_count++;
        if(grip_count > GRIP_LIMIT){
            parameters[3] = 0;
            curr_state = picking;
            return;
        }
    }
}

void pickup_n_move(){
    uint16_t counts = 0;
    uint16_t pick_up_point = 500;
    while(1){
        blink_led(10);
        if(counts > pick_up_point) break;
        counts++;
        parameters[2] = -2;
        imply_command();
        _delay_ms(5);
    }
    parameters[2] = 0;
    while(1){
        blink_led(10);
        parameters[1] = +2;
        imply_command();
        _delay_ms(5);
        if(angle_values[0] <= ANGLE_0 || angle_values[0] >= ANGLE_180) break;
    }
    parameters[1] = 0;
    while(1){
        blink_led(10);
        parameters[3] = -2;
        imply_command();
        _delay_ms(5);
        if(angle_values[5] <= ANGLE_0 || angle_values[5] >= ANGLE_180) break;
    }
    
    parameters[3] = 0;
    curr_state = done;
    return;
}


#include "headers/i2c.c"
#include "headers/pca9685.c"
