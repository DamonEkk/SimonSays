#include <stdint.h>

extern volatile uint8_t sequence;

void Set_seed(uint32_t seed);

uint8_t Get_sequence(uint16_t cycles);

uint8_t Screen_sequence(void);

void Clear_press(void);