#include <stdint.h>

extern volatile uint16_t time;
extern volatile uint8_t counting;

void timer_init(void);
void button_timer_init(void);
void buzzer_init(void);

uint32_t get_elapsed_time(void);

void reset_timer(void);

void Pause(void);

