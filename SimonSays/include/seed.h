#include <stdint.h>

extern volatile uint8_t sequence = 1;

void Set_seed(uint32_t seed);

uint8_t Get_sequence(uint16_t cycles);

uint8_t Screen_sequence(void);