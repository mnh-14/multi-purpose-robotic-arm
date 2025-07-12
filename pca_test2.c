#define F_CPU 8000000UL
#define F_SCL 100000UL

#include <avr/io.h>
#include <util/delay.h>
#include "led_blink.h"

void i2c_init(void) {
    TWSR = 0x00; // Prescaler = 1
    TWBR = ((F_CPU / F_SCL) - 16) / 2; // Should be 2
}

void i2c_start(void) {
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
}

void i2c_stop(void) {
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
}

void i2c_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
}

int main(void) {
    i2c_init();
    led_init();
    _delay_ms(500);

    while (1) {
        i2c_start();
        i2c_write(0x40); // PCF8574 Write Address (A0–A2 = GND)
        i2c_write(0xF0); // Set P0-P3 High, P4-P7 Low
        i2c_stop();
        _delay_ms(1000);
        led_on();

        i2c_start();
        i2c_write(0x40);
        i2c_write(0x0F); // Set P0-P3 Low, P4-P7 High
        i2c_stop();
        _delay_ms(1000);
        led_off();
    }
}