#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

/*
 * This demonstrate how to use the avr_mcu_section.h file
 * The macro adds a section to the ELF file with useful
 * information for the simulator
 */
#include "include/avr_mcu_section.h"
AVR_MCU(F_CPU, "atmega2580");

/*
 * This small section tells simavr to generate a VCD trace dump with changes to these
 * registers.
 */
const struct avr_mmcu_vcd_trace_t _mytrace[]  _MMCU_ = {
    { AVR_MCU_VCD_SYMBOL("PORTB"), .what = (void*)&PORTB, },
};

int main (void)
{
    DDRB |= _BV(DDB0);
    
    int count = 0;
    
    while(count < 10)
    {
        PORTB ^= _BV(PB0);
        _delay_ms(500);
        count++;
    }
    
    // this quits the simulator, since interupts are off
    // this is a "feature" that allows running tests cases and exit
    sleep_cpu();
}
