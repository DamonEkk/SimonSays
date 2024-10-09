#include <stdint.h>

extern volatile uint16_t e_high;
extern volatile uint16_t c_sharp;
extern volatile uint16_t a_norm;
extern volatile uint16_t e_low;
extern volatile uint8_t pb_debounced_state;

void timer_init(void);
void button_timer_init(void);
