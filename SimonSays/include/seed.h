#include <stdint.h>

extern volatile uint8_t sequence;
extern volatile uint32_t e_high;
extern volatile uint32_t c_sharp;
extern volatile uint32_t a_norm;
extern volatile uint32_t e_low;
extern volatile uint8_t pb_debounced_state;
extern volatile uint8_t clock;
extern volatile uint8_t first_digit; 
extern volatile uint8_t second_digit;
extern uint8_t SEGMENT_1;
extern uint8_t SEGMENT_2;
extern uint8_t CLEAR;


void Set_seed(uint32_t seed);

uint8_t Get_sequence(uint16_t cycles, uint8_t fail);

uint8_t Screen_sequence(void);

void Clear_press(void);

uint8_t Compare_results(uint8_t screen_side, uint8_t user_result, uint16_t current_sequence);

void Set_perif(uint8_t left, uint8_t right, uint32_t note);

void Set_left_digit(uint8_t digit);

void Set_right_digit(uint8_t digit);

void Set_buzzer(uint32_t note);