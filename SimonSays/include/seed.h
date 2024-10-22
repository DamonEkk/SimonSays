#include <stdint.h>

extern volatile uint8_t sequence;

void Set_seed(uint32_t seed);

uint8_t Get_sequence(uint16_t cycles);

uint8_t Screen_sequence(void);

void Clear_press(void);

uint8_t Compare_results(uint8_t screen_side, uint8_t user_result, uint16_t current_sequence);