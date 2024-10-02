#include "uart.h"
#include "timer.h"
#include "spi.h"

#include <stdio.h>
#include <stdint.h>

uint8_t sequence = 1;


int main(void){ 
    // setup
    uart_init();
    spi_init();
    timer_init();

    while (1){  // Game loop
        

    }

    return 0;
}