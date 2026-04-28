/*
 * PreLab2.c
 *
 * Created: 21/04/2026
 * Author: Juan Rodrigo Donis
 * Description: Recibe un caracter desde la terminal serial
 *              y lo muestra en 8 LEDs usando D2-D9.
 */
/****************************************/
// Encabezado (Libraries)

#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>

/****************************************/
// Function prototypes

void UART0_Init(void);
char UART0_ReadChar(void);
void LEDs_Init(void);
void MostrarByte(uint8_t valor);

/****************************************/
// Main Function

int main(void)
{
    uint8_t datoRecibido = 0;

    UART0_Init();
    LEDs_Init();

    while (1)
    {
        datoRecibido = (uint8_t)UART0_ReadChar();
        MostrarByte(datoRecibido);
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
    UCSR0B = (1 << RXEN0);                         // habilita recepción
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);       // 8 bits, sin paridad, 1 stop
}

char UART0_ReadChar(void)
{
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

void LEDs_Init(void)
{
    // D2-D7 => PD2-PD7 como salida
    DDRD |= 0b11111100;

    // D8-D9 => PB0-PB1 como salida
    DDRB |= 0b00000011;

    // Todo inicia apagado
    PORTD &= 0b00000011;
    PORTB &= 0b11111100;
}

void MostrarByte(uint8_t valor)
{
    // Bits 0-5 hacia D2-D7
    PORTD = (PORTD & 0b00000011) | ((valor & 0x3F) << 2);

    // Bits 6-7 hacia D8-D9
    PORTB = (PORTB & 0b11111100) | ((valor >> 6) & 0x03);
}