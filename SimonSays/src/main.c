#include "uart.h"
#include "timer.h"
#include "spi.h"
#include "button.h"

#include <stdio.h>
#include <stdint.h>

#define MAX_HZ 20000
#define MIN_HZ 20

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

    void increaseFrequency(void){
        if ((e_high >> 1 > MAX_HZ) && (c_sharp >> 1 > MAX_HZ) && (a_norm >> 1 > MAX_HZ) && (e_low >> 1 > MAX_HZ)){
            c_sharp >>= 1;
            a_norm >>= 1; 
            e_low >>= 1;
        }
    }

    void decreaseFrequency(void){
        if ((e_high << 1 < MIN_HZ) && (c_sharp << 1 < MIN_HZ) && (a_norm << 1 < MIN_HZ) && (e_low << 1 < MIN_HZ)){
            c_sharp <<= 1;
            a_norm <<= 1; 
            e_low <<= 1;
        }
    }

    return 0;
}