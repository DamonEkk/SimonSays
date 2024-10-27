#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "spi.h"
#include "uart.h"
#include "adc.h"
#include "seed.h"

volatile uint8_t condition = 0;
volatile uint8_t pb_debounced_state = 0xFF;
volatile uint16_t time = 0;
volatile uint8_t counting = 0;
uint8_t first;
uint8_t second;

void timer_init(void){
    cli();
    TCB1.CTRLB = TCB_CNTMODE_INT_gc; // Configure TCB1 in periodic interrupt mode
    TCB1.CCMP = 52083;               
    TCB1.INTCTRL = TCB_CAPT_bm;      // CAPT interrupt enable
    TCB1.CTRLA = TCB_ENABLE_bm;      // Enable
    sei();
    
}

void button_timer_init(void){
    cli();
    TCB0.CCMP = 16667;
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;
    TCB0.INTCTRL = TCB_CAPT_bm;
    TCB0.CTRLA = TCB_ENABLE_bm;   // Enable
    sei();
}

void buzzer_init(void){
    PORTB_DIRSET = PIN0_bm;
    cli();
    TCA0.SINGLE.PERBUF = 0;
    TCA0.SINGLE.CMP0BUF = 0;
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;       
    sei();
}

ISR(TCB1_INT_vect){
    uint16_t potentiometer = ADC0_RESULT;

    uint16_t scaled = 16 + (((uint32_t) potentiometer * (128-16)) >> 8);
    if (counting == 1){
        time++;
        if (time == (scaled >> 1)){ // 200 = 1 sec (0.25-2seconds) 50-400
            clock ^= 0x1;
            time = 0;
            uart_puts("triggered interrupt\n");
        }
    }
    TCB1.INTFLAGS = TCB_CAPT_bm; // clear flag
 
}

void Pause(void){
    uint8_t temp_clock = clock;
    counting = 1;
    while (clock == temp_clock);
    counting = 0;
    time = 0x00;
}

ISR(TCB0_INT_vect){

    second = second_digit;
    first = first_digit;
    static uint8_t countA = 0;
    static uint8_t countB = 0; 
    
    uint8_t pb_sample = PORTA.IN;

    uint8_t pb_changed = pb_sample ^ pb_debounced_state;
    countB = (countB ^ countA) & pb_changed;
    countA = ~countA & pb_changed;

    pb_debounced_state ^= (countB & countA) | (pb_changed & pb_debounced_state);


    if (condition % 2 == 0){
        spi_write(first);
        condition++;
    }
    else {
        spi_write(second);
        condition++;
    }

    TCB0.INTFLAGS = TCB_CAPT_bm;
}


