#include <avr/io.h>
#include <avr/interrupt.h>
#include "spi.h"
#include "uart.h"

uint8_t volatile condition = 0;

void timer_init(void)
{
    cli();
    TCB1.CTRLB = TCB_CNTMODE_INT_gc; // Configure TCB1 in periodic interrupt mode
    TCB1.CCMP = 16667;               // Set interval for 5 ms (16667 clocks @ 3.333 MHz)
    TCB1.INTCTRL = TCB_CAPT_bm;      // CAPT interrupt enable
    TCB1.CTRLA = TCB_ENABLE_bm;      // Enable
    sei();
}

ISR(TCB1_INT_vect)
{
    uint8_t firstDigit = 0b11101011; // 1
    uint8_t secondDigit = 0b01000100; 
     
    if (condition % 2 == 0){
        spi_write(firstDigit);
        condition++;
    }
    else {
        spi_write(secondDigit);
        condition++;
    }
    
    
    TCB1.INTFLAGS = TCB_CAPT_bm; // clear flag
}

