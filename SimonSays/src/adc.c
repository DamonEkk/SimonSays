#include <avr/io.h>

void adc_init(void){
    ADC0_CTRLA = ADC_ENABLE_bm; // Enable ADC

    ADC0.CTRLB = ADC_PRESC_DIV2_gc; // prescaler set to 2

    ADC0_CTRLC = (4 << ADC_TIMEBASE_gp) | ADC_REFSEL_VDD_gc; // 4 CLK_PER | VDD

    ADC0.CTRLE = 64; // duration

    ADC0.CTRLF = ADC_FREERUN_bm;

    ADC0.MUXPOS = ADC_MUXPOS_AIN2_gc;

    ADC0.COMMAND = ADC_MODE_SINGLE_8BIT_gc | ADC_START_IMMEDIATE_gc; // Set to 8bit 
}