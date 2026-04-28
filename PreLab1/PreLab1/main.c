/*
 * PreLab1.c
 *
 * Created: 21/04/2026
 * Author: Juan Rodrigo Donis
 * Description: Envía el carácter K por comunicación serial.
 */
/****************************************/
// Encabezado (Libraries)

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

/****************************************/
// Function prototypes

void UART0_Init(void);
void UART0_TxChar(char c);

/****************************************/
// Main Function

int main(void)
{
    UART0_Init();

    while (1)
    {
        UART0_TxChar('K');
        _delay_ms(1000);
    }
}

/****************************************/
// NON-Interrupt subroutines

void UART0_Init(void)
{
    uint16_t ubrr = 103;   // 9600 baudios con F_CPU = 16 MHz

    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;

    UCSR0A = 0x00;
    UCSR0B = (1 << TXEN0);                         // habilita transmisión
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);       // 8 bits, sin paridad, 1 stop
}

void UART0_TxChar(char c)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}