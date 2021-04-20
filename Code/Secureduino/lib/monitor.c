/*
 * Header files for monitor comms
 */

#include <avr/io.h>
#include <avr/pgmspace.h>

//#include "mcu_defs.h"
#include "monitor.h"
#include "uart.h"

char *bytetohex(uint8_t byte)
{
    char *c = "\0\0";
    uint8_t nybble0 = (byte >> 4) & 0x0F;
    uint8_t nybble1 = byte& 0x0F;
    if (nybble0 > 0x09)
        nybble0 += 0x37; // A-F
    else
        nybble0 += 0x30; // 0-9

    if (nybble1 > 0x09)
        nybble1 += 0x37; // A-F
    else
        nybble1 += 0x30; // 0-9
    c[0] = nybble0;
    c[1] = nybble1;
    return c;
}

// Finish me later
uint8_t hextobyte(char hex[2])
{
    uint8_t byte = 0x00;
    return byte;
}

void dumpROM()
{
    // Assume ROM < 32K for now, as testing with Atmega328p
    
    uint16_t  flashpointer = 0x0000;
    
    while (flashpointer < 0x8000)
    {
        for (int word = 0; word < 8; word++)
        {
            raw_printf(bytetohex(pgm_read_byte(flashpointer++)));
            raw_printf(bytetohex(pgm_read_byte(flashpointer++)));
            raw_printf(" ");
        }
        raw_printf("\n");
    }
}
