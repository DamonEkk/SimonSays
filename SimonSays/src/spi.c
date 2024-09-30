#include <avr/io.h>
#include <avr/interrupt.h>


void spi_init(void){
    
    cli();
    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT1_gc;
    PORTC.DIR |= (PIN0_bm | PIN2_bm | PIN3_bm ); // clock mosi ss enable
    PORTA_DIR  |= PIN1_bm;
    SPI0.CTRLA = SPI_MASTER_bm;
    SPI0_INTCTRL |= SPI_IE_bm; //enable interrupt 
    SPI0.CTRLB = SPI_SSD_bm; //unbuffed mode
    PORTC.OUTCLR = PIN3_bm; //set ss to low
    SPI0.CTRLA |= SPI_ENABLE_bm;
    sei();
}

// Setter
void spi_write(uint8_t data){
    SPI0.DATA = data;
    
    
}

// Getter
uint8_t spi_read(void){
    return SPI0.DATA;
}

ISR(SPI0_INT_vect){
    PORTA.OUTCLR = PIN1_bm; //Rising edge
    PORTA.OUTSET = PIN1_bm;
    SPI0_INTFLAGS = SPI_IF_bm | SPI_WRCOL_bm;
}

