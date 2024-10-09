#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "spi.h"
#include "uart.h"

volatile uint8_t condition = 0;
volatile uint8_t pb_debounced_state = 0xFF;

uint8_t segment1A = 0b00111110;
uint8_t segment1B = 0b01101011;
uint8_t clear = 0b11111111;

//frequencies in hz
volatile uint16_t e_high = 313;
volatile uint16_t c_sharp = 263;
volatile uint16_t a_norm = 418;
volatile uint16_t e_low = 157;


void timer_init(void){
    cli();
    TCB1.CTRLB = TCB_CNTMODE_INT_gc; // Configure TCB1 in periodic interrupt mode
    TCB1.CCMP = 16667;               // Set interval for 5 ms (16667 clocks @ 3.333 MHz)
    TCB1.INTCTRL = TCB_CAPT_bm;      // CAPT interrupt enable
    TCB1.CTRLA = TCB_ENABLE_bm;      // Enable
    sei();
}

void button_timer_init(void){
    cli();
    TCA0.SINGLE.PER = 16667;
    TCA0.SINGLE.CMP0 = 16667;
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0_bm; // Set interval for 5 ms       
    sei();
}

    //3.33mhz 418 hz
    //1/418 = 0.00239 = 2.39 / 300 = 0.00796
    // duty cycle needs to be 50%

    // PORTB_DIRSET = PIN1_bm;
    // TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_CMP1EN_bm;
    // TCA0.SINGLE.PER = 0;
    // TCA0_SINGLE_CMP0 = 0; // buzzer set to 0
    // TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;
    // sei();

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

ISR(TCA0_CMP0_vect){
    uart_puts("DETECTED");
    static uint8_t countA = 0;
    static uint8_t countB = 0; 
    
    uint8_t pb_sample = PORTA.IN;

    uint8_t pb_changed = pb_sample ^ pb_debounced_state;
    countB = (countB ^ countA) & pb_changed;
    countA = ~countA & pb_changed;

    pb_debounced_state ^= (countB & countA) | (pb_changed & pb_debounced_state);

    TCA0.SINGLE.INTFLAGS = TCA0_SINGLE_CMP0;
}

