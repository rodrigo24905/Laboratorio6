/*
 * Lab6_UART_Completo.c
 *
 * Created: 21/04/2026
 * Author: Juan Rodrigo Donis
 * Description:
 * Este programa hace dos cosas al mismo tiempo:
 * 1. Envía "Hola mundo" cada 1 segundo por UART.
 * 2. Si recibe un carácter desde la terminal, lo muestra en 8 LEDs.
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
void cadena(char txt[]);
uint8_t UART0_HayDato(void);
char UART0_RxChar(void);

void LEDs_Init(void);
void MostrarByte(uint8_t valor);

/****************************************/
// Main Function

int main(void)
{
    uint8_t datoRecibido = 0;
    uint8_t contador10ms = 0;

    UART0_Init();
    LEDs_Init();

    while (1)
    {
        // Aquí revisa si llegó un carácter desde la terminal
        if (UART0_HayDato())
        {
            datoRecibido = (uint8_t)UART0_RxChar();
            MostrarByte(datoRecibido);
        }

           _delay_ms(10);
        contador10ms++;

        // Cada 100 vueltas de 10 ms, ya pasó 1 segundo
        if (contador10ms >= 100)
        {
            cadena("Hola mundo\r\n");
            contador10ms = 0;
        }
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
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);          // Habilita RX y TX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);        // 8 bits, sin paridad, 1 stop
}

void UART0_TxChar(char c)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void cadena(char txt[])
{
    uint8_t i = 0;

    while (txt[i] != '\0')
    {
        UART0_TxChar(txt[i]);
        i++;
    }
}

uint8_t UART0_HayDato(void)
{
    if (UCSR0A & (1 << RXC0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

char UART0_RxChar(void)
{
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

void LEDs_Init(void)
{
    // D2-D7 = PD2-PD7
    DDRD |= 0b11111100;

    // D8-D9 = PB0-PB1
    DDRB |= 0b00000011;

    // Todo inicia apagado
    PORTD &= 0b00000011;
    PORTB &= 0b11111100;
}

void MostrarByte(uint8_t valor)
{
    // Bits 0 a 5 en D2-D7
    PORTD = (PORTD & 0b00000011) | ((valor & 0x3F) << 2);

    // Bits 6 y 7 en D8-D9
    PORTB = (PORTB & 0b11111100) | ((valor >> 6) & 0x03);
}