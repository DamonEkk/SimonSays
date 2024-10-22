#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>

#include "timer.h"
#include "uart.h"

uint32_t student_id = 0x12197718;
uint32_t unique_seed = 0x0;
uint32_t mask = 0xE2024CAB;



volatile uint8_t sequence = 1;

void Set_seed(uint32_t seed){
    unique_seed = seed;
}

uint8_t Get_sequence(uint16_t cycles){
    uint8_t bit;
    uint8_t step;
    if (unique_seed == 0){
        uart_puts("seed set to student id");
        unique_seed = student_id;
    }

    for (uint8_t i = 0; i < cycles; i++){
        bit = unique_seed & PIN0_bm;
        unique_seed >>= 1;
        // might need to add bit to the front
        if (bit == 1){
            unique_seed ^= mask;
        }
        
        step = unique_seed & 0x3;
    }

    return step;  
}

void Clear_press(void){
    Set_left_digit(CLEAR);
    Set_right_digit(CLEAR);
    Set_buzzer(0);
}

void Screen_sequence(void){
    for (int i = 0; i < sequence; i++){ 
        time = 0;
        uint8_t Screen_image = Get_sequence(i);
        uint8_t clock_change = clock;
        
        if (Screen_image == 0){
            Set_left_digit(SEGMENT_1);
            Set_buzzer(e_high);
            uart_puts("0\n");
        }
        else if (Screen_image == 1){
            Set_left_digit(SEGMENT_2);
            Set_buzzer(c_sharp);
            uart_puts("1\n");
        }
        else if (Screen_image == 2){
            Set_right_digit(SEGMENT_1);
            Set_buzzer(a_norm);
            uart_puts("2\n");
        }
        else if (Screen_image == 3){
            Set_right_digit(SEGMENT_2);
            Set_buzzer(e_low);
            uart_puts("3\n");
        }

        while (clock == clock_change);
        clock_change = clock;
        Clear_press();
        while (clock == clock_change);
        
    }
    Clear_press();
}
 // screen side will be 0 for left and 1 for right
uint8_t Compare_results(uint8_t screen_side, uint8_t user_result, uint16_t current_sequence){
    uint8_t compare_val;
    uint8_t correct_sequence = Get_sequence(current_sequence);

    if (screen_side == 0){
        if (user_result == SEGMENT_1){
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
