#include "uart.h"
#include "timer.h"
#include "spi.h"
#include "button.h"
#include "high_score.h"

#include <stdio.h>
#include <stdint.h>

#define MAX_HZ 20000
#define MIN_HZ 20


//TODO
// Button configuration
// make 2darrayInit
// seed
// clock

uint8_t sequence = 1;
uint8_t xy = 18; 


/*
Increases frequency by one octave and validates that hz stays in audible range.
shifts bits once for multiplying by 2
*/
void increaseFrequency(void){
    if ((e_high >> 1 < MAX_HZ) && (c_sharp >> 1 < MAX_HZ) && (a_norm >> 1 < MAX_HZ) && (e_low >> 1 < MAX_HZ)){
        c_sharp >>= 1;
        a_norm >>= 1; 
        e_low >>= 1;
    }
}

/*
Decreases frequency by one octave and validates that hz stays in audible range.
shifts bits once for dividing by 2
*/
void decreaseFrequency(void){
    if ((e_high << 1 > MIN_HZ) && (c_sharp << 1 > MIN_HZ) && (a_norm << 1 > MIN_HZ) && (e_low << 1 > MIN_HZ)){
        c_sharp <<= 1;
        a_norm <<= 1; 
        e_low <<= 1;
    }
}

/*
Prints leaderboard out and exludes null values
*/
void Print_leaderboard(void){
    for (uint8_t i = 0; i < SCORE_ROWS; i++){
        // prints each row that is not null
        if (!(high_scores[i][0] == NULL)){
            printf("%s %s\n", high_scores[i][0], high_scores[i][1]);
        }
    }
}


int main(void){ 
    // setup
    uart_init();
    spi_init();
    timer_init();
    button_init();  
    High_score_init();
    

    // function list table 3
    typedef enum {
        START,
        GAMEPLAY,
        END,
        FREQUENCY,
        RESET,
        SEED
    } action;

    typedef enum {
        GENERATE,
        INPUT,
        PLAY,
        SUCCESS,
        FAIL
    } gameplay_action;

    action state = START;
    uint8_t test = 0;

    while (1){  // Game loop

    switch (state){
        case START:
        break;

        case GAMEPLAY:
        break;

        case END:
        break;

        case FREQUENCY:
        if (test == 1){
            increaseFrequency();
            test = 0;
            state = GAMEPLAY;
        }
        else if (test == 2){
            decreaseFrequency();
            test = 0;
            state = GAMEPLAY;
        }
        break;

        case RESET:
        break;

        case SEED:
        break;

        default:
        state = START;
        break;
    }
        

    }

    return 0;
}

