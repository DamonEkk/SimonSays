#include "uart.h"
#include "timer.h"
#include "spi.h"
#include "button.h"

#include <stdio.h>
#include <stdint.h>

uint8_t sequence = 1;
uint8_t xy = 18; 
char *high_scores[5][5];


int main(void){ 
    // setup
    uart_init();
    spi_init();
    timer_init();
    button_init();  

    // function list table 3
    typedef enum {
        GAMEPLAY,
        FREQUENCY,
        RESET,
        SEED
    } action;

    while (1){  // Game loop
        

    }

    return 0;
}