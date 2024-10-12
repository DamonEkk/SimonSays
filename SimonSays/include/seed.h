#include <stdint.h>

extern volatile uint8_t sequence = 1;

void Set_seed(uint32_t seed);

void Start(void);