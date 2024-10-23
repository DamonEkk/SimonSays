#include "uart.h"

#include <avr/io.h>
#include <stdint.h>

void uart_init(void)
{
    // Output enable USART0 TXD (PB2)
    PORTB.DIRSET = PIN2_bm;
    USART0.BAUD = 1389;                           // 9600 baud @ 3.333 MHz
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm; // Enable Tx/Rx

    // enable input
    // USART1.BAUD = 1389;
    // USART1.CTRLA = USART_RXCIE_bm;                           // 9600 baud @ 3.333 MHz
    // USART1.CTRLB = USART_RXEN_bm | USART_TXEN_bm; // Enable Tx/Rx
}

char uart_getc(void)
{
    while (!(USART0.STATUS & USART_RXCIF_bm))
        ; // Wait for data
    return USART0.RXDATAL;
}

void uart_putc(char c)
{
    while (!(USART0.STATUS & USART_DREIF_bm))
        ; // Wait for TXDATA empty
    USART0.TXDATAL = c;
}

void uart_puts(char* c){ // sends each character to uart_puts()
    while (*c  != '\0'){
        uart_putc(*c);
        c++;

    }
}

