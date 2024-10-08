#include <avr/io.h>
#include "timer.h"

volatile uint8_t pb_debounced_state;

void button_init(void){
    PORTB_PIN4CTRL = PORT_PULLUPEN_bm; 
    PORTB_PIN5CTRL = PORT_PULLUPEN_bm;
    PORTB_PIN6CTRL = PORT_PULLUPEN_bm;
    PORTB_PIN7CTRL = PORT_PULLUPEN_bm;
}

ISR(TCB0_INT_vect){
    static uint8_t countA = 0;
    static uint8_t countB = 0; 
    
    uint8_t pb_sample = PORTA.IN;

    uint8_t pb_changed = pb_sample ^ pb_debounced_state;
    countB = (countB ^ countA) & pb_changed;
    countA = ~countA & pb_changed;

    pb_debounced_state ^= (countB & countA) | (pb_changed & pb_debounced_state);

    TCB0.INTFLAGS = TCB_CAPT_bm; // Acknowledge interrupt
}