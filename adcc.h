#ifndef _ADCC_H_
#define _ADCC_H_ 1

#include <avr/io.h>

void ADC_single_channel_init(){
    ADMUX |= ((1<<ADLAR) | (1<<REFS1));
    ADCSRA |= ((1<<ADPS1) | (1<<ADEN));
}

void ADC_interrup_init(){
    ADCSRA |= (1<<ADIE);
}

/// @brief Read data from a certain channel (1-7)
/// @param channel specified channel, only 1-7 are acceptable
/// @return value from the adc
int ADC_read_channel(uint8_t channel){
    if(channel <= 0) return 0;
    if(channel > 7) return 0;
    ADMUX &= 0b11100000; // Clearing the last 5 bits
    ADMUX |= (channel & 0b00011111);
    ADCSRA |= (1<<ADSC);
    while(ADCSRA & (1<<ADSC));
    int value = ADCH;
    return value;
}


/// @brief Reads the adc from specified channel. Does not block the the MCU execution, uses interrupt
/// @param channel the channel from where to read 
void ADC_read_channel_background(uint8_t channel){
    if(channel <= 0) return;
    if(channel > 7) return;
    ADMUX |= channel;
    ADCSRA |= (1<<ADSC);
}

#endif