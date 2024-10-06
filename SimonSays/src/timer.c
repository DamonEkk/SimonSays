#include <avr/io.h>
#include <avr/interrupt.h>
#include "spi.h"
#include "uart.h"

uint8_t volatile condition = 0;

uint8_t segment1A = 0b00111110;
uint8_t segment1B = 0b01101011;


void timer_init(void)
{
    cli();
    TCB1.CTRLB = TCB_CNTMODE_INT_gc; // Configure TCB1 in periodic interrupt mode
    TCB1.CCMP = 16667;               // Set interval for 5 ms (16667 clocks @ 3.333 MHz)
    TCB1.INTCTRL = TCB_CAPT_bm;      // CAPT interrupt enable
    TCB1.CTRLA = TCB_ENABLE_bm;      // Enable

    PORTB_DIRSET = PIN1_bm;
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_CMP1EN_bm;
    TCA0.SINGLE.PER = 0;
    TCA0_SINGLE_CMP0 = 0; // buzzer set to 0
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;
    sei();
}
 // write to 7 seg
ISR(TCB1_INT_vect)
{
    uint8_t firstDigit = (segment1A | PIN7_bm); // 1 use pin7_7 to change to left side of screen
    uint8_t secondDigit = segment1B; 
     
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

