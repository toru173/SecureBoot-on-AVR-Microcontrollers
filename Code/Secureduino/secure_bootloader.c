/*
 * Secure bootloader checks signature of firmware and provides for other utilities
 */

//#define DEBUG

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>

#ifdef DEBUG
#include <stdio.h> // Handy for debugging but bloats code significantly
#endif

//#include "mcu_defs.h"
#include "secure_bootloader.h"

// Set up MCU
#define F_CPU 20000000
#define BAUD 9600
#include <util/setbaud.h>
#include <util/delay.h>

// Set up USART
#include "uart.h"

// Monitor
#include "monitor.h"

//AVRNaCl Encryption Library
//#include "avrnacl.h"

//AVRCryptoLib Encryption Library
#include "AVRCrytolib.h"

#define PK_BYTES 32
#define STATE_VEC_BYTES 64
#define BLOCK_SIZE_BYTES 128

/*
 * This demonstrate how to use the avr_mcu_section.h file
 * The macro adds a section to the ELF file with useful
 * information for the simulator
 */
#include "avr_mcu_section.h"
AVR_MCU(F_CPU, "atmega2560");

#ifdef DEBUG
FILE uart_stdio = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);
#endif

#define RSA_MAX_LEN (1024/8)

unsigned char cryptdata[RSA_MAX_LEN];
unsigned char public_key[RSA_MAX_LEN];
unsigned char private_key[RSA_MAX_LEN];
unsigned int  public_exponent;

unsigned char rsa_tmp[3*RSA_MAX_LEN];
#define rsa_s (rsa_tmp+(2*RSA_MAX_LEN))

// Change to another 1024 bit key
const unsigned char public1024e3[] PROGMEM={
0xB1,0x92,0x97,0xD1,0xC9,0x0F,0x4A,0xE9,
0x7C,0x1C,0x61,0xDA,0xCA,0x82,0x9E,0x37,
0xCB,0x85,0x36,0xCB,0xB1,0xB9,0xEF,0x8D,
0xEE,0x5D,0x43,0x11,0x14,0x95,0x73,0x7A,
0xD1,0x55,0x62,0x12,0xB2,0x14,0x7C,0x4C,
0x21,0x0A,0xA0,0x1D,0x23,0x2B,0x79,0xDF,
0x8E,0x21,0xD6,0x7E,0xF3,0x3D,0xD3,0x6A,
0x43,0x53,0x7C,0x84,0xD9,0x6E,0x60,0xEF,
0x85,0x35,0xF8,0xBD,0x02,0x2B,0xEB,0xB3,
0x68,0x2F,0x84,0xF9,0xA3,0x58,0x13,0x1C,
0x8D,0xE4,0xD6,0xAE,0x56,0x17,0xED,0x28,
0x65,0x2A,0x15,0x05,0x54,0x83,0xF9,0x7F,
0x18,0x77,0x00,0x9A,0x2D,0x05,0x99,0x20,
0x55,0xBD,0x59,0x54,0x5A,0xBF,0x9E,0x05,
0x10,0x72,0x47,0xF8,0x6E,0x03,0xDB,0xE2,
0x3A,0x55,0x41,0x6B,0x4B,0xF0,0xEE,0x67};

/* just random data for testeting */
const unsigned char CONSTANT_DATA[] PROGMEM=
{
    0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x5A,0x59,0xE9,0x71,0x1A,0xCB,0x01,0x11,0xDF,0x92,0x8E,0xF4,0x7B,0xAD,0xD8,0x69,
    0xD5,0x8F,0x8C,0xAD,0xC2,0xEB,0xAD,0xCC,0x01,0xB9,0xB2,0x36,0x42,0xF4,0xA0,0x3A,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0x00,0xff,
    0x01,0x0F,0x00,0x00,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x5A,0x59,0xE9,0x71,0x1A,0xCB,0x01,0x11,0xDF,0x92,0x8E,0xF4,0x7B,0xAD,0xD8,0xBB,
    0xD5,0x8F,0x8C,0xAD,0xC2,0xEB,0xAD,0xCC,0x01,0xB9,0xB2,0x36,0x42,0xF4,0xA0,0x3A,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0x23,0xAA,
    0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x5A,0x59,0xE9,0x71,0x1A,0xCB,0x01,0x11,0xDF,0x92,0x8E,0xF4,0x7B,0xAD,0xD8,0x69,
    0xD5,0x8F,0x8C,0xAD,0xC2,0xEB,0xAD,0xCC,0x01,0xB9,0xB2,0x36,0x42,0xF4,0xA0,0x3A,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0x00,0xff,
    0x01,0x0F,0x00,0x00,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x5A,0x59,0xE9,0x71,0x1A,0xCB,0x01,0x11,0xDF,0x92,0x8E,0xF4,0x7B,0xAD,0xD8,0xBB,
    0xD5,0x8F,0x8C,0xAD,0xC2,0xEB,0xAD,0xCC,0x01,0xB9,0xB2,0x36,0x42,0xF4,0xA0,0x3A,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0x23,0xAA
};

int main (void)
{
    uart_init();
    
    #ifdef DEBUG
    stdout = stdin = stderr = &uart_stdio;
    #endif
        
    char c = uart_getrawchar(); // Wait for input before continuing
    
    if (c == 'm')
    {
        // Enter monitor

        raw_printf("\n");
        raw_printf("\nHashing beginning. Success should be zero: ");
        // raw_printf(bytetohex((uint8_t) crypto_hashblocks_sha512(hash, block, sizeof(block))));
        raw_printf("\nhashing finished! Complete hash:\n");

        raw_printf("\n");
        
        raw_printf("\nSigning beginning. Success should be zero: ");
        /* (ciphertext^public_exponent)%public_key = plaintext */
        public_exponent=3;
        memcpy_P(public_key    ,public1024e3        ,sizeof(public1024e3));
        rsa_decrypt(sizeof(public1024e3),cryptdata,public_exponent,public_key,rsa_s,rsa_tmp);
        if (memcmp_P(cryptdata,CONSTANT_DATA,sizeof(public1024e3)))
        {
            raw_printf("0\n");
        }
        raw_printf("\nSigning finished!\n");
    }
    //_delay_ms(1000);
    while(1); // hang here while debugging
    run_firmware();
}
