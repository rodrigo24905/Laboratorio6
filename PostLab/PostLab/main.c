/*
 * PostLab_UART.c
 *
 * Created: 28/04/2026
 * Author: Juan Rodrigo Donis Alvizures
 * Description:
 * Muestra un menu por UART:
 * 1. Leer potenciómetro
 * 2. Enviar ASCII
 * Luego vuelve a mostrar el menú.
 */
/****************************************/
// Encabezado (Libraries)

#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>

/****************************************/
// Function prototypes

void UART0_Init(void);
void UART0_TxChar(char c);
void cadena(char txt[]);
char UART0_RxChar(void);
char UART0_RxCharNoEnter(void);

void ADC0_Init(void);
uint16_t ADC0_Read(void);

void LEDs_Init(void);
void MostrarByte(uint8_t valor);

void UART0_TxUInt16(uint16_t valor);
void MostrarMenu(void);

/****************************************/
// Main Function

int main(void)
{
    char opcion;
    char caracter;
    uint16_t valorPot;

    UART0_Init();
    ADC0_Init();
    LEDs_Init();

    while (1)
    {
        MostrarMenu();
        opcion = UART0_RxCharNoEnter();

        UART0_TxChar(opcion);
        cadena("\r\n");

        switch (opcion)
        {
            case '1':
                valorPot = ADC0_Read();
                cadena("Valor del potenciometro: ");
                UART0_TxUInt16(valorPot);
                cadena("\r\n\r\n");
                break;

            case '2':
                cadena("Ingrese un caracter: ");
                caracter = UART0_RxCharNoEnter();
                UART0_TxChar(caracter);
                cadena("\r\n");

                MostrarByte((uint8_t)caracter);

                cadena("ASCII decimal: ");
                UART0_TxUInt16((uint8_t)caracter);
                cadena("\r\n\r\n");
                break;

            default:
                cadena("Opcion invalida\r\n\r\n");
                break;
        }
    }
}

/****************************************/
// NON-Interrupt subroutines

void UART0_Init(void)
{
    uint16_t ubrr = 103;

    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;

    UCSR0A = 0x00;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
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

char UART0_RxChar(void)
{
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

char UART0_RxCharNoEnter(void)
{
    char c;

    do
    {
        c = UART0_RxChar();
    }
    while (c == '\r' || c == '\n');

    return c;
}

void ADC0_Init(void)
{
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t ADC0_Read(void)
{
    ADMUX = (ADMUX & 0xF0) | 0x00;
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

void LEDs_Init(void)
{
    DDRD |= 0b11111100;
    DDRB |= 0b00000011;

    PORTD &= 0b00000011;
    PORTB &= 0b11111100;
}

void MostrarByte(uint8_t valor)
{
    PORTD = (PORTD & 0b00000011) | ((valor & 0x3F) << 2);
    PORTB = (PORTB & 0b11111100) | ((valor >> 6) & 0x03);
}

void UART0_TxUInt16(uint16_t valor)
{
    char buffer[6];
    uint8_t i = 0;
    uint8_t j;

    if (valor == 0)
    {
        UART0_TxChar('0');
        return;
    }

    while (valor > 0)
    {
        buffer[i] = (valor % 10) + '0';
        valor = valor / 10;
        i++;
    }

    for (j = i; j > 0; j--)
    {
        UART0_TxChar(buffer[j - 1]);
    }
}

void MostrarMenu(void)
{
    cadena("Menu UART\r\n");
    cadena("1. Leer Potenciometro\r\n");
    cadena("2. Enviar ASCII\r\n");
    cadena("Seleccione una opcion: ");
}