#include "uart.h"
#include "timer.h"
#include "spi.h"
#include "button.h"
#include "high_score.h"
#include "seed.h"
#include "adc.h"

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#define MAX_HZ 20000
#define MIN_HZ 20

typedef enum {
        START,
        GAMEPLAY,
        END,
        FREQUENCY,
        RESET,
        SEED
    } action;

    typedef enum {
        INPUT,
        SUCCESS,
        GENERATE,
        FAIL
    } gameplay_action;

    action state = START;
    gameplay_action game_state = INPUT;
    uint8_t test = 0;
    uint8_t gg = 0b10000000;
    uint8_t middle = 0b11110111;
    uint8_t clock_change;

    uint8_t zero = 0b00001000;
    uint8_t one = 0b01101011;
    uint8_t two = 0b01000100;
    uint8_t three = 0b01000001;
    uint8_t four = 0b00100011;
    uint8_t five = 0b00010001;
    uint8_t six = 0b00010000;
    uint8_t seven = 0b01011011;
    uint8_t eight = 0b00000000;
    uint8_t nine = 0b00000001;



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
 Stops the effects of a button click after one clock cycle has passed or the user has let go of the button.

 @param pin is used to determine which button was clicked
 */
void Stop_button(uint8_t pin){
    uint8_t temp_clock;
    uint8_t bit_unlock = 0;
    time = 0x0;
    temp_clock = clock;
    counting = 1;

    while(1){
        uint8_t sample = pb_debounced_state;

        if (clock != temp_clock){
            bit_unlock |= PIN0_bm;
            counting = 0;
        }

        if (sample &= pin){
            bit_unlock |= PIN1_bm;
            
        }
                    
        if (bit_unlock == 3){
            Clear_press();
            return;
        }
    }
}

/*
 gets the display digit binary value.

 @param num, num you wish to get the digit to.
 @return digit, display binary
*/
uint8_t Get_screen_digit(uint8_t num){
    uint8_t digit;

    switch (num){
        case 0:
            digit = zero;
            break;
        case 1:
            digit = one;
            break;
        case 2:
            digit = two;
            break;
        case 3:
            digit = three;
            break;
        case 4:
            digit = four;
            break;
        case 5:
            digit = five;
            break;
        case 6:
            digit = six;
            break;
        case 7:
            digit = seven;
            break;
        case 8:
            digit = eight;
            break;
        case 9:
            digit = nine;
            break;
        default:
            digit = zero;
            break;
    }
    return digit;
}

/*
 main gameplay loop
*/
void Gameplay_loop(){
    uint8_t sample = pb_debounced_state;
    uint8_t previous_state = pb_debounced_state;
    uint8_t pb_changed;
    uint8_t pb_falling_edge;
    uint8_t pb_rising_edge;
    uint8_t current_count = 1;

    PORTB_OUTSET = PIN5_bm; // sets led to off
    PORTB_DIRSET = PIN5_bm; //set light as output

    while (1){  // Game loop

        previous_state = sample;
        sample = pb_debounced_state;
        pb_changed = sample ^ previous_state; 
        pb_falling_edge = pb_changed & ~sample;
        pb_rising_edge = pb_changed & sample;
        uint8_t left_side = 0;
        
        switch (state){
            case START:
                Screen_sequence(); // plays game results.
                game_state = INPUT;
                state = GAMEPLAY;

            break;

            
            case GAMEPLAY:

            switch (game_state){
                case INPUT:

                    // waits for button press.
                    if (pb_falling_edge & PIN4_bm) {
                        Set_perif(SEGMENT_1, 0, e_high); // sets display and buzzer
                        Stop_button(PIN4_bm); // checks that the button has been activated for x amount of time.

                        if (Compare_results(0, SEGMENT_1, current_count) == 0){ // sends corrosponding button result and checks if that matched simon's result.
                            current_count++; // increases users count.
                            
                            game_state = SUCCESS;
                        } else {
                            game_state = FAIL;
                        }
                    } 

                    else if (pb_falling_edge & PIN5_bm) {
                        Set_perif(SEGMENT_2, 0, c_sharp);
                        Stop_button(PIN5_bm);

                        if (Compare_results(0, SEGMENT_2, current_count) == 0){
                            current_count++;
                            game_state = SUCCESS;
                        } else {
                            game_state = FAIL;
                        }
                    }

                    else if (pb_falling_edge & PIN6_bm) {
                        Set_perif(0, SEGMENT_1, a_norm);
                        Stop_button(PIN6_bm);

                        if (Compare_results(1, SEGMENT_1, current_count) == 0){
                            current_count++;
                            game_state = SUCCESS;
                        } else {
                            game_state = FAIL;
                        }
                    }

                    else if (pb_falling_edge & PIN7_bm){
                        Set_perif(0, SEGMENT_2, e_low);
                        Stop_button(PIN7_bm);
                         
                        if (Compare_results(1, SEGMENT_2, current_count) == 0){
                            current_count++;
                            game_state = SUCCESS;
                        } else {
                            game_state = FAIL;
                        }
                    }
                    break;
                

                case SUCCESS:

                    if (current_count > sequence){
                        Set_perif(gg, gg, 0);
                        Pause();
                        game_state = GENERATE;
                    }
                    else{
                        Pause();
                        game_state = INPUT;
                    }
                    
                    break;


                case GENERATE:
                    sequence++;
                    current_count = 1;
                    Screen_sequence();
                    game_state = INPUT;
                    break;


                case FAIL:
                    Set_perif(middle, middle, 0);

                    uint8_t temp = Get_sequence(sequence, 1);
                    Pause();
                    // leaderboard needs to be displayed here

                    // check if score is over 100 and gets rid of nums over 99.
                    while (1){
                        if (sequence >= 100){
                            sequence -= 100;
                        }
                        break;
                    }

                    // Gets the tenth value and the left over becomes the right value
                    while (1){
                        if (sequence >= 90 && (!(sequence < 10))){
                            left_side++;
                            sequence -= 10;
                        }
                        break;
                    }
                    // left side will be left_side and right side will be sequence

                    // display score
                    if (left_side == 0){
                        Set_perif(0, Get_screen_digit(sequence), 0);
                    }
                    else{
                        Set_perif(Get_screen_digit(left_side), Get_screen_digit(sequence), 0);
                    }

                    // playback timer
                    
                    state = END;
                    break;


                default:
                    current_count = 1;
                    sequence = 1;
                    game_state = INPUT;
                    break;
            }

            break;

            case END:
                Pause();
                current_count = 1;
                sequence = 1;
                game_state = INPUT;
                state = START;
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
    timer_init();
    buzzer_init();
    adc_init();
    uart_init();
    Clear_press();
    spi_init();
    button_timer_init();
    button_init();  
    High_score_init();

    
    Gameplay_loop();

    return 0;
}

