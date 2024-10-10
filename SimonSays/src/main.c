#include "uart.h"
#include "timer.h"
#include "spi.h"
#include "button.h"
#include "high_score.h"

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#define MAX_HZ 20000
#define MIN_HZ 20



//TODO
// Button configuration
// make 2darrayInit
// seed
// clock

uint8_t sequence = 1;
uint8_t xy = 18; 

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
    gameplay_action game_state = GENERATE;
    uint8_t test = 0;


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
            uart_puts(high_scores[i][0]);
            uart_puts(" ");
            uart_puts(high_scores[i][1]);
            uart_puts("\n");
        }
    }
}

/*
 main gameplay loop
*/
void Gameplay_loop(){
    uint8_t sample = pb_debounced_state;
    uint8_t previous_state = pb_debounced_state;
    uint8_t pb_changed;
    uint8_t pb_falling_edge;

    PORTB_OUTSET = PIN5_bm; // sets led to off
    PORTB_DIRSET = PIN5_bm; //set light as output

    while (1){  // Game loop

    
    previous_state = sample;
    sample = pb_debounced_state;
    pb_changed = sample ^ previous_state; 
    pb_falling_edge = pb_changed & previous_state;


    switch (state){
        case START:
            if (pb_falling_edge & PIN4_bm) {
                Set_left_digit(SEGMENT_1);
            } 
            else if (pb_falling_edge & PIN5_bm) {
                Set_left_digit(SEGMENT_2);
            }
            else if (pb_falling_edge & PIN6_bm) {
                Set_right_digit(SEGMENT_1);
            }
            else if (pb_falling_edge & PIN7_bm){
                Set_right_digit(SEGMENT_2);
            }

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

}

int main(void){ 
    // setup
    uart_init();
    spi_init();
    timer_init();
    button_timer_init();
    button_init();  
    High_score_init();
    Gameplay_loop();

    return 0;
}

