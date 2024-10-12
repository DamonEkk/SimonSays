#include <stdint.h>

extern volatile uint32_t e_high;
extern volatile uint32_t c_sharp;
extern volatile uint32_t a_norm;
extern volatile uint32_t e_low;
extern volatile uint8_t pb_debounced_state;
extern uint8_t SEGMENT_1;
extern uint8_t SEGMENT_2;

void timer_init(void);
void button_timer_init(void);
void buzzer_init(void);

void Set_left_digit(uint8_t digit);

void Set_right_digit(uint8_t digit);

void setBuzzer(uint32_t note);