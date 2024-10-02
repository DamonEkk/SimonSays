#include <avr/io.h>

void button_init(void){
    PORTB_PIN4CTRL = PORT_PULLUPEN_bm; 
    PORTB_PIN5CTRL = PORT_PULLUPEN_bm;
    PORTB_PIN6CTRL = PORT_PULLUPEN_bm;
    PORTB_PIN7CTRL = PORT_PULLUPEN_bm;
}