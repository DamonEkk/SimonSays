#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>

#include "timer.h"
#include "uart.h"

uint32_t student_id = 0x12197718;
uint32_t unique_seed = 0x0;
uint32_t mask = 0xE2024CAB;

uint8_t CLEAR = 0b11111111;

//frequencies in hz
volatile uint32_t e_high = 10650; //313
volatile uint32_t c_sharp = 12674; // 263
volatile uint32_t a_norm = 7974; // 418
volatile uint32_t e_low = 21231; // 157
volatile uint8_t first_digit = 0b01111111; 
volatile uint8_t second_digit = 0b1111111;

uint8_t SEGMENT_1 = 0b00111110;
uint8_t SEGMENT_2 = 0b01101011;
uint8_t current_sequence = 1;
uint8_t screen_image;


volatile uint8_t sequence = 1;


/*
    Sets unique seed.

    @param seed unique seed
*/
void Set_seed(uint32_t seed){
    unique_seed = seed;
}

/*
    Sets the left display

    @param digit digit that will be displayed
*/
void Set_left_digit(uint8_t digit){
    first_digit = (digit ^ PIN7_bm);

    if (digit == SEGMENT_1 || digit == SEGMENT_2){
        second_digit = 0b1111111;
    }
}

/*
    Sets the right display

    @param digit digit that will be displayed

*/
void Set_right_digit(uint8_t digit){
    second_digit = digit;
    if (digit == SEGMENT_1 || digit == SEGMENT_2){
        first_digit = 0b01111111; 
    }
}

/*
    sets the buzzer frequency.

    @param note sets buzzer sound
*/
void Set_buzzer(uint32_t note){
    
    if (note == 0){ // shifting cmp0buf if note == 0 could cause weird errors.
        TCA0.SINGLE.CMP0BUF = note;
        return;
    }

    TCA0.SINGLE.PERBUF = note; // sets note
    TCA0.SINGLE.CMP0BUF = note >> 1; // 50% duty
}

/*
sets all displays and buzzer

@param left display side
@param right display side
@param note buzzer sound
*/
void Set_perif(uint8_t left, uint8_t right, uint32_t note){
    

    if (left != 0){
        Set_left_digit(left);
    }
    
    if (right!= 0){ // 0 is used as an ignore value instead of off.
        Set_right_digit(right);
    }

    Set_buzzer(note);
    
}

/*
    rotates the seed into a sudo random sequence cycles amount of times and
    returns current value.

    @param cycles x amount of times seed it "randomised"
    @param fail used to determine if the seed should be saved upon fail
    @return returns last 2 values after randomised
*/
uint8_t Get_sequence(uint16_t cycles, uint8_t fail){
    uint8_t bit;
    uint8_t step;

    unique_seed = student_id;

    for (uint8_t i = 0; i < cycles; i++){
        bit = unique_seed & PIN0_bm; // gets last bit
        unique_seed ^= 0b10000000000000000000000000000000; // rotates bit to the front.
        unique_seed >>= 1; 

        // might need to add bit to the front
        if (bit == 1){
            unique_seed ^= mask;
        }
        
        step = unique_seed & 0x3;
    }

    if (fail == 1){ // used to set the seed once a fail state is activated.
        student_id = unique_seed;
    }

    return step;  
}

/*
    Clears screen and buzzer
*/
void Clear_press(void){
    Set_perif(CLEAR, CLEAR, 0);
}

typedef enum{
    VALUE,
    SHOWING,
    HIDING,
    END
} screening;

screening screening_state = VALUE;

/*
    Counts sequence amount of times and requests the current value's randomised number 
    displays that value to screen and continues until max sequence num.
*/
uint8_t Screen_sequence(void){
    
        switch (screening_state){
            case VALUE:
                //uart_puts("state VALUE\n");

                screen_image = Get_sequence(current_sequence, 0);
                
                if (screen_image == 0){
                    Set_perif(SEGMENT_1, 0, e_high);
                    
                }
                else if (screen_image == 1){
                    Set_perif(SEGMENT_2, 0, c_sharp);
                    
                }
                else if (screen_image == 2){
                    Set_perif(0, SEGMENT_1, a_norm);
                    
                }
                else if (screen_image == 3){
                    Set_perif(0, SEGMENT_2, e_low);
                    
                }

                time2 = 0;
                screening_state = SHOWING;
                return 0;

                break;

            case SHOWING:
            //uart_puts("state SHOWING\n");
                if (time2 == (250 >> 1)){
                    Clear_press();
                    screening_state = HIDING;
                }
                return 0;
                break;
            case HIDING:
            //uart_puts("state HIDING\n");
                if (time2 == (250)){
                    screening_state = END;
                }
                return 0;
                break;
            case END:
            //uart_puts("state END\n");
                if (current_sequence < sequence){
                    current_sequence++;
                    screening_state = VALUE;
                }
                else{
                    current_sequence = 1;
                    return 1;
                }
                return 0;
                break;
            default:
                screening_state = VALUE;
                break;
        }
    }

 
 /*
    checks the users result compared to the value displayed on screen.
    if user succeeds state is updated.
    if user fails state is updated to fail state.

    @param screen_side gets the value's screen side
    @param user_result the value the user entered.
    @param current_sequence generates x position in the sudo random string to compare to user input
    @return true or false determines if the user failed or succeeded.
 */
uint8_t Compare_results(uint8_t screen_side, uint8_t user_result, uint16_t current_sequence){
    uint8_t compare_val;
    uint8_t correct_sequence = Get_sequence(current_sequence, 0);
    screening_state = VALUE;

    if (screen_side == 0){ // checks left side values first
    
        if (user_result == SEGMENT_1){ // then checks sequence
            compare_val = 0;
            
        }
        else{
            compare_val = 1;
            
        }
    }
    else if (screen_side == 1){
        if (user_result == SEGMENT_1){
            compare_val = 2;
            
        }
        else{
            compare_val = 3;
            
        }
    }

    if (compare_val == correct_sequence){
        return 0;
    } 
    // sequence didnt match
    return 1;

}
