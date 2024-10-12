#include <stdio.h>
#include <stdint.h>

uint32_t student_id = 12197718;
uint32_t unique_seed = 0;

void Start(void){
    if (unique_seed == 0){
        unique_seed = student_id;
    }

    
}