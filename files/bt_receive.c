#include <avr/io.h>

void bt_receive_init() {
    // Set baud rate: UBRR = (F_CPU / (16 * BAUD)) - 1
    // Assuming F_CPU = 8 MHz and Baud rate = 9600
    uint16_t ubrr_value = 51; // for 9600 bps

    // Set baud rate
    UBRRH = (ubrr_value >> 8); // higher 8 bits
    UBRRL = ubrr_value;        // lower 8 bits

    // Enable receiver
    UCSRB |= (1 << RXEN);

    // Set frame format: 8 data, 1 stop bit, no parity
    UCSRC |= (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
}

char bt_receive_char() {
    // Wait until data is received
    while (!(UCSRA & (1 << RXC)));

    // Get and return received data from buffer
    return UDR;
}

void bt_receive(char *buffer) {
    char ch;
    uint8_t i = 0;

    // Keep receiving until newline
    do {
        ch = bt_receive_char();
        buffer[i++] = ch;
    } while (ch != '\n');

    buffer[i] = '\0'; // Null terminate
}


void bt_send_init() {
    // Set same baud rate (e.g., 9600 bps)
    uint16_t ubrr_value = 51;

    UBRRH = (ubrr_value >> 8);
    UBRRL = ubrr_value;

    // Enable transmitter
    UCSRB |= (1 << TXEN);

    // Set frame format: 8 data, 1 stop bit, no parity
    UCSRC |= (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
}

void bt_send(char *data) {
    while (*data) {
        while (!(UCSRA & (1 << UDRE))); // Wait until buffer is empty
        UDR = *data++;
    }
}


#include <avr/io.h>
#include <util/delay.h>

int main() {
    char received[50];

    bt_receive_init();
    bt_send_init();

    while (1) {
        bt_receive(received); // Wait for input from Bluetooth
        bt_send(received);    // Echo it back
    }

    return 0;
}
