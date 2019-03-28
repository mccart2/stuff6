/*
 * Assignment 3
 *
 * Created: 3/27/2019 8:56:33 PM
 * Author : Tom McCarroll
 */ 





#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

                                         // Function Declarations
void USART_init( unsigned int ubrr );
void USART_tx_string( char *data );

volatile int Count;
char outs[60];
int n;
float integer;
char str[] = "Is this thing on?";
char empty[] = " ";

int main(void)
{
    Count  = 0;
    TIMSK0 |= (1 << TOIE0);
    sei ();
    TCCR0A = 0x00;
    TCCR0B = 0x05;
    USART_init(BAUD_PRESCALLER);                                // Initialize the USART
    USART_tx_string("All systems nominal\r\n");                          // we're alive!
    while(1);
}
/* INIT USART (RS-232)  */
void USART_init( unsigned int ubrr )
{
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << TXEN0);     // Enable RX, TX & RX interrupt
    UCSR0C = (3 << UCSZ00);    //asynchronous 8 N 1
}
/* SEND A STRING TO THE RS-232 */
void USART_tx_string( char *data )
{
    while ((*data != '\0'))
    {
        while (!(UCSR0A & (1 <<UDRE0)));
        UDR0 = *data;
        data++;
    }
}
ISR (TIMER0_OVF_vect)
{
    while (Count < 61)
    {
        if ((TIFR0 & 0x01) == 1)
        {
            TIFR0 = 0x01;
            Count++;
        }
    }
    if (Count > 60)
    {
        
        
        
        n = rand();
        integer = 4;
        dtostrf(integer, 2, 2, outs);
        USART_tx_string(outs);
        USART_tx_string(empty);
        snprintf(outs, sizeof(outs), "%3d\r\n" , n);
        USART_tx_string(outs);
        USART_tx_string(empty);
        USART_tx_string(str);
        USART_tx_string(empty);
        
        
        Count = 0;
    }
}
