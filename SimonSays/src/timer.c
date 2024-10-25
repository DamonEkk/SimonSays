#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "spi.h"
#include "uart.h"
#include "adc.h"

volatile uint8_t condition = 0;
volatile uint8_t pb_debounced_state = 0xFF;

uint8_t CLEAR = 0b11111111;

//frequencies in hz
volatile uint32_t e_high = 10650; //313
volatile uint32_t c_sharp = 12674; // 263
volatile uint32_t a_norm = 7974; // 418
volatile uint32_t e_low = 21231; // 157
volatile uint8_t firstDigit = 0b01111111; 
volatile uint8_t secondDigit = 0b1111111;
volatile uint8_t clock = 0;
volatile uint16_t time = 0;

uint8_t SEGMENT_1 = 0b00111110;
uint8_t SEGMENT_2 = 0b01101011;



void timer_init(void){
    cli();
    TCB1.CTRLB = TCB_CNTMODE_INT_gc; // Configure TCB1 in periodic interrupt mode
    TCB1.CCMP = 16667;               // Set interval for 5 ms (16667 clocks @ 3.333 MHz)
    TCB1.INTCTRL = TCB_CAPT_bm;      // CAPT interrupt enable
    TCB1.CTRLA = TCB_ENABLE_bm;      // Enable
    sei();
}

void button_timer_init(void){
    cli();
    TCB0.CCMP = 16667;
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;
    TCB0.INTCTRL = TCB_CAPT_bm;
    TCB0.CTRLA = TCB_ENABLE_bm;   // Enable
    sei();
}

void buzzer_init(void){
    PORTB_DIRSET = PIN0_bm;
    uart_puts("buzzer active");
    cli();
    TCA0.SINGLE.PERBUF = 0;
    TCA0.SINGLE.CMP0BUF = 0;
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP0EN_bm;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;       
    sei();
}

void Set_perif(uint8_t left, uint8_t right, uint32_t note){
    if (left != 0){
        Set_left_digit(left);
    }
    
    if (right!= 0){
        Set_right_digit(right);
    }
    
    if (note != 0){
        Set_buzzer(note);
    }
    
}

void Set_left_digit(uint8_t digit){
    firstDigit = (digit ^ PIN7_bm);

    if (digit == SEGMENT_1 || digit == SEGMENT_2){
        secondDigit = 0b1111111;
    }
}

void Set_right_digit(uint8_t digit){
    secondDigit = digit;
    if (digit == SEGMENT_1 || digit == SEGMENT_2){
        firstDigit = 0b01111111; 
    }
}

void Set_buzzer(uint32_t note){
    if (note == 0){
        TCA0.SINGLE.PERBUF = note;
        TCA0.SINGLE.CMP0BUF = note;
        return;
    }

    TCA0.SINGLE.PERBUF = note;
    TCA0.SINGLE.CMP0BUF = note >> 1;
}

ISR(TCB1_INT_vect){
    uint16_t potentiometer = 255 - ADC0_RESULT;

    if (potentiometer != 0){
        potentiometer++;
    }

    uint16_t scaled = 50 + (((uint32_t) potentiometer * (400-50)) >> 8);

     
    time++;
    if (time == scaled){ // 200 = 1 sec (0.25-2seconds) 50-400
        //uart_puts("One second passed\n");

        // clock that cycles 1 and 0
        clock ^= 0x1;
        time = 0;
    }
 
     
    if (condition % 2 == 0){
        spi_write(firstDigit);
        condition++;
    }
    else {
        spi_write(secondDigit);
        condition++;
    }
    
    TCB1.INTFLAGS = TCB_CAPT_bm; // clear flag
}

ISR(TCB0_INT_vect){
    static uint8_t countA = 0;
    static uint8_t countB = 0; 
    
    uint8_t pb_sample = PORTA.IN;

    uint8_t pb_changed = pb_sample ^ pb_debounced_state;
    countB = (countB ^ countA) & pb_changed;
    countA = ~countA & pb_changed;

    pb_debounced_state ^= (countB & countA) | (pb_changed & pb_debounced_state);

    TCB0.INTFLAGS = TCB_CAPT_bm;
}

