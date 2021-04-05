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

int BOOTLOADER_SECTION main (void)
{
    uart_init();
    stdout = stdin = stderr = &uart_stdio;
    
    /*
    int i = 10;
    while(i > -1)
    {
        printf("Verifying firmware. Boot in %d", i);
        i--;
        _delay_ms(1000);
    }
    
        
    while (1)
    {
        printf("Hello from the BLS!\n");
        _delay_ms(1000);
    }
    */
    sleep_mode();
}
