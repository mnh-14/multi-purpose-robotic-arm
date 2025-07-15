#ifndef _LEDS_H_
#define _LEDS_H_

#include <avr/io.h>
#include <util/delay.h>


/// @brief initializes the Any pin as led output pin
/// @param pin the desired pin, Example: "A0", "C0"
void led_init(char * pin){
    switch (pin[0]) {
    case 'A':
        DDRA |= (1 << (pin[1]-'0'));
        break;
    case 'B':
        DDRB |= (1 << (pin[1]-'0'));
        break;
    case 'C':
        DDRC |= (1 << (pin[1]-'0'));
        break;
    case 'D':
        DDRD |= (1 << (pin[1]-'0'));
        break;
    default:
        break;
    }
}



/// @brief Turns the LED connected to pin, on
/// @param pin the desired pin, Example: "A0", "C0"
void led_on(char * pin){
    switch (pin[0]) {
    case 'A':
        PORTA |= (1 << (pin[1]-'0'));
        break;
    case 'B':
        PORTB |= (1 << (pin[1]-'0'));
        break;
    case 'C':
        PORTC |= (1 << (pin[1]-'0'));
        break;
    case 'D':
        PORTD |= (1 << (pin[1]-'0'));
        break;
    default:
        break;
    }
}

/// @brief Turns the LED connected to pin, on
/// @param pin the desired pin, Example: "A0", "C0"
void led_off(char * pin){
    switch (pin[0]) {
    case 'A':
        PORTA &= ~(1 << (pin[1]-'0'));
        break;
    case 'B':
        PORTB &= ~(1 << (pin[1]-'0'));
        break;
    case 'C':
        PORTC &= ~(1 << (pin[1]-'0'));
        break;
    case 'D':
        PORTD &= ~(1 << (pin[1]-'0'));
        break;
    default:
        break;
    }
}

void led_blink(char* pin, int times){
    static int count = 0;
    if(count < times){
        count ++;
        return;
    }
    count = 0;
    switch (pin[0]) {
        case 'A':
            PORTA ^= (1 << (pin[1]-'0'));
            break;
        case 'B':
            PORTB ^= (1 << (pin[1]-'0'));
            break;
        case 'C':
            PORTC ^= (1 << (pin[1]-'0'));
            break;
        case 'D':
            PORTD ^= (1 << (pin[1]-'0'));
            break;
        default:
            break;
    }
}

/// @brief Blink rapidly with specified delay. NOTE: It will delay the machine
/// @param pin len on which pin. Example: "A0", "C0" etc
/// @param blink_delay amount to delay
/// @param times How many time do you want to blink
void rapid_blink_led(char* pin, int blink_delay, int times){
    int d = blink_delay;
    while (times--) {
        while(d--) _delay_ms(1);
        d = blink_delay;
        led_blink(pin, 0);
    }
}

#endif