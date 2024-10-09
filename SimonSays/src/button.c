#include <avr/io.h>
#include "timer.h"
#include "uart.h"

// volatile uint8_t pb_debounced_state = 0xFF;

void button_init(void){
    PORTA_PIN4CTRL = PORT_PULLUPEN_bm; 
    PORTA_PIN5CTRL = PORT_PULLUPEN_bm;
    PORTA_PIN6CTRL = PORT_PULLUPEN_bm;
    PORTA_PIN7CTRL = PORT_PULLUPEN_bm;
}

