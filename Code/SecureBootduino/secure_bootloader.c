/*
 * Secure bootloader checks signature of firmware and provides for other utilities
 */

#define F_CPU 16000000UL // 16MHz oscillator is what is used in the simulator
#define BAUD 9600 // Could be faster if desired. This is just a PoC

#include <avr/io.h>
#include <avr/boot.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <util/setbaud.h>

#include <stdio.h>

#include "uart.h"
#include "secure_bootloader.h"

/*
 * This demonstrate how to use the avr_mcu_section.h file
 * The macro adds a section to the ELF file with useful
 * information for the simulator
 */
#include "avr_mcu_section.h"
AVR_MCU(F_CPU, "atmega328p");

FILE uart_stdio = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

/* Function Prototypes (taken from optiboot.c*/
/* The main function is in init9, which removes the interrupt vector table */
/* we don't need. It is also 'naked', which means the compiler does not    */
/* generate any entry or exit code itself. */
//int main(void) __attribute__ ((naked)) __attribute__ ((section (".init9")));

BOOTLOADER_SECTION int main (void)
{
    //*    some chips dont set the stack properly (from STK500v2 Bootloader)
    asm volatile ( ".set __stack, %0" :: "i" (RAMEND) );
    asm volatile ( "ldi    16, %0" :: "i" (RAMEND >> 8) );
    asm volatile ( "out %0,16" :: "i" (AVR_STACK_POINTER_HI_ADDR) );
    asm volatile ( "ldi    16, %0" :: "i" (RAMEND & 0x0ff) );
    asm volatile ( "out %0,16" :: "i" (AVR_STACK_POINTER_LO_ADDR) );
    
    uart_init();
    stdout = stdin = stderr = &uart_stdio;
    
    while (1)
    {
        UDR0 = 0x55;
        // printf("Hello from the BLS!");
        // _delay_ms(1000);
    }
}
