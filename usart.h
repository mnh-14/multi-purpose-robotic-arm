#ifndef _USART_H_
#define _USART_H_ 1

#ifndef BAUD_NUM
#define BAUD_NUM 51
#endif

#ifndef ENDING_CHAR
#define ENDING_CHAR '$'
#endif

#include <avr/io.h>


void USART_init(){
    // Set baud rate: UBRR = (F_CPU / (16 * BAUD)) - 1
    // Assuming F_CPU = 8 MHz and Baud rate = 9600
    uint16_t ubrr_value = BAUD_NUM; // for 9600 bps

    // Set baud rate
    UBRRH = (ubrr_value >> 8); // higher 8 bits
    UBRRL = ubrr_value;        // lower 8 bits

    // Enable receiver and sender
    UCSRB |= (1 << RXEN);
    UCSRB |= (1 << TXEN);

    // Set frame format: 8 data, 1 stop bit, no parity
    UCSRC |= (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
}

void USART_init_interrupt(){
    // todo: set the interrupt up
    UCSRB |= (1<<RXCIE);
}


char USART_receive_char() {
    // Wait until data is received
    while (!(UCSRA & (1 << RXC)));

    // Get and return received data from buffer
    return UDR;
}

void USART_receive(char *buffer) {
    char ch;
    uint8_t i = 0;

    // Keep receiving until newline
    while(1){
        ch = USART_receive_char();
        if(ch == ENDING_CHAR) break;
        buffer[i++] = ch;
    }

    buffer[i] = '\0'; // Null terminate
}

void USART_send_char(char c){
    while (!(UCSRA & (1 << UDRE))); // Wait until buffer is empty
    UDR = c;
}

void USART_send(char *data) {
    while (*data) {
        USART_send_char(*data);
        data++;
    }
    USART_send_char(ENDING_CHAR);
}


#endif