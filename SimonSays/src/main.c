#include "uart.h"
#include "timer.h"
#include "spi.h"
#include "button.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_HZ 20000
#define MIN_HZ 20
#define SCORE_ROWS 5

uint8_t sequence = 1;
uint8_t xy = 18; 
char *high_scores[SCORE_ROWS][2];
char *name = "john";
char *score = "10";

void increaseFrequency(void){
        if ((e_high >> 1 < MAX_HZ) && (c_sharp >> 1 < MAX_HZ) && (a_norm >> 1 < MAX_HZ) && (e_low >> 1 < MAX_HZ)){
            c_sharp >>= 1;
            a_norm >>= 1; 
            e_low >>= 1;
        }
    }

    void decreaseFrequency(void){
        if ((e_high << 1 > MIN_HZ) && (c_sharp << 1 > MIN_HZ) && (a_norm << 1 > MIN_HZ) && (e_low << 1 > MIN_HZ)){
            c_sharp <<= 1;
            a_norm <<= 1; 
            e_low <<= 1;
        }
    }

    void Place_score(int row){
        high_scores[row][0] = name;
        high_scores[row][1] = score;
    }

    void Override_score(char *new_score, uint8_t position){
        
    }

    void Check_score(void){
        for (uint8_t i = 0; i < SCORE_ROWS; i++){
            if (high_scores[i][0] == NULL){
                Place_score(i);
                printf("%s %s\n", high_scores[i][0], high_scores[i][1]);
                break;
            }
            
            else if (atoi(score) > atoi(high_scores[i][1])){
                Override_score(score, i);
            }
            
        
        }
    }


int main(void){ 
    // setup
    uart_init();
    spi_init();
    timer_init();
    button_init();  

    for (uint8_t i = 0; i < SCORE_ROWS; i++){
        for (uint8_t k = 0; k < 2; k++){
            high_scores[i][k] = NULL;
        }
    }

    // function list table 3
    typedef enum {
        START,
        GAMEPLAY,
        END,
        FREQUENCY,
        RESET,
        SEED
    } action;

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

