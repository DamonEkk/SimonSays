#include <avr/io.h>

#include <stdio.h>
#include <stdint.h>

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
        unique_seed = student_id;
    }

    for (uint8_t i; i < cycles; i++){
        bit = unique_seed & PIN0_bm;
        if (bit == 1){
            unique_seed ^= mask;
        }
        
        step = unique_seed & (PIN0_bm & PIN1_bm);
    }
    
}


