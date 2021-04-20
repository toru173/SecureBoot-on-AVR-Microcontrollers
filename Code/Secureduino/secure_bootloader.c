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
AVR_MCU(F_CPU, "atmega328p");

#ifdef DEBUG
FILE uart_stdio = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);
#endif

//AVRCryptoLib vars
#define RSA_MAX_LEN (128) // 1024 bites = 128 bytes

unsigned char cryptdata[RSA_MAX_LEN];
unsigned int  public_exponent = 3; // Valid options in OpenSSL are 3 and 65537

unsigned char rsa_tmp[3 * RSA_MAX_LEN];
#define rsa_s (rsa_tmp + (2 * RSA_MAX_LEN))

// Generated using OpenSSL
unsigned char public_key[]  =
{
    0xc9, 0x08, 0x12, 0xc5, 0xe4, 0x2c, 0x63, 0xc0, 0xe6, 0x88, 0xbb, 0xfa,
    0x88, 0x48, 0xa8, 0xf1, 0x87, 0x82, 0x5d, 0xfb, 0x85, 0xa1, 0x89, 0x4e,
    0xf1, 0xa1, 0xb0, 0x7a, 0x2e, 0x20, 0x65, 0x63, 0xdb, 0xb3, 0xf0, 0xc8,
    0xc6, 0xd1, 0x50, 0xbe, 0xa6, 0xb1, 0xb1, 0x8e, 0x31, 0x41, 0xfe, 0x7c,
    0x03, 0x0c, 0xcf, 0xad, 0x5a, 0xfc, 0x44, 0x8e, 0xf2, 0x06, 0xd8, 0xef,
    0x89, 0x74, 0x0c, 0x77, 0x4c, 0x2d, 0x55, 0x93, 0x93, 0x7e, 0xd4, 0xaf,
    0x84, 0x13, 0x4f, 0x32, 0x04, 0xf3, 0x44, 0x3f, 0x62, 0x55, 0xaa, 0x34,
    0xe2, 0xa2, 0xbc, 0x3e, 0xb5, 0xe2, 0x9d, 0xed, 0x1f, 0xb5, 0xe5, 0x77,
    0xfd, 0x84, 0x55, 0x4d, 0x7c, 0xef, 0xc4, 0x8b, 0x46, 0x5c, 0x4d, 0xc0,
    0x76, 0x49, 0xe1, 0x11, 0x89, 0x48, 0x4c, 0x2e, 0xfb, 0xc8, 0x98, 0x92,
    0xe0, 0x1a, 0x60, 0x78, 0xf1, 0xe2, 0xbb, 0x3f
};

// Encrypted test data on Mac using OpenSSL
const unsigned char test_data[] PROGMEM =
{
    0x5d, 0xb8, 0x33, 0xc8, 0x90, 0x6e, 0x98, 0x66, 0x5c, 0xf2, 0x81, 0x9b,
    0xdc, 0x26, 0x16, 0xa2, 0x64, 0x86, 0xe9, 0x84, 0xb7, 0x32, 0xbb, 0x29,
    0x87, 0x81, 0xd4, 0x1b, 0x81, 0x9e, 0x73, 0x71, 0xb3, 0x02, 0x7a, 0x8c,
    0xda, 0x78, 0xf0, 0xf6, 0x2d, 0xa4, 0x1d, 0x16, 0xab, 0xe0, 0x23, 0x75,
    0x30, 0xc7, 0xc0, 0xb8, 0x90, 0x21, 0xf6, 0xe9, 0xed, 0x78, 0xf4, 0xc5,
    0x2a, 0xab, 0x7b, 0x48, 0x2e, 0x22, 0xaa, 0x16, 0xd8, 0x1d, 0x04, 0x65,
    0x30, 0x55, 0x21, 0x64, 0x4f, 0x15, 0x0d, 0xed, 0x81, 0xb0, 0x4f, 0x9b,
    0x16, 0x35, 0x88, 0xbf, 0x48, 0x61, 0xb4, 0x1e, 0x24, 0x25, 0x76, 0x83,
    0xd9, 0xbd, 0x8c, 0xbd, 0xcb, 0xc9, 0xa5, 0xb1, 0xc5, 0xc3, 0x05, 0x21,
    0x93, 0x0c, 0xa9, 0x6d, 0x10, 0xd0, 0xb6, 0x66, 0x6c, 0x06, 0x52, 0xc2,
    0x1b, 0x85, 0x61, 0xba, 0x3e, 0x87, 0x43, 0x5a
  };

unsigned char test_bin[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

void get512block(uint8_t *buffer, uint16_t baseaddress)
{
    for (int i = 0; i < sizeof(buffer); i++)
    {
        buffer[i] = pgm_read_byte(baseaddress + i);
    }
}


int main (void)
{
    uart_init();
    
    #ifdef DEBUG
    stdout = stdin = stderr = &uart_stdio;
    #endif
        
    char c = uart_getrawchar(); // Wait for input before continuing
    
    if (c == 'm')
    {
        raw_printf("\n");

        // Enter monitor

        uint8_t *buffer[64];
        
        get512block(*buffer, 0);
        
        for (int i = i; i < sizeof(buffer); i++)
        {
            raw_printf(bytetohex((uint8_t) &buffer[i]));
        }
        
        
        raw_printf("\n");
        raw_printf("\nHashing beginning:\n");
        

        Sha_Init();
        Sha_Update(test_bin, sizeof(test_bin));
        Sha_Update(test_bin, sizeof(test_bin));
        Sha_Final();
        
        for (int i = 0; i < 5; i ++)
        {
            unsigned long word = Sha_Info.Digest[i];
            raw_printf(bytetohex((word & 0xFF000000) >> 24));
            raw_printf(bytetohex((word & 0x00FF0000) >> 16));
            raw_printf(bytetohex((word & 0x0000FF00) >> 8));
            raw_printf(bytetohex(word & 0x000000FF));
            raw_printf(" ");
        }
        /*
        for (int i = i; i < sizeof(hash); i++)
        {
            raw_printf(bytetohex((uint8_t) hash[i]));
        }
         
         */
        
        raw_printf("\nhashing finished!\n");

        raw_printf("\n");
        
        raw_printf("\nSigning beginning...\n");
        
        memcpy_P(cryptdata, test_data, sizeof(test_data));
        
        rsa_decrypt(sizeof(public_key), cryptdata, public_exponent, public_key, rsa_s, rsa_tmp);
        
        for (int i = i; i < sizeof(cryptdata); i++)
        {
            if(cryptdata[i] != 0x00)
                uart_putrawchar(cryptdata[i]);
        }
        
        raw_printf("\n");
        
        raw_printf("\nSigning finished!\n");
    }
    //_delay_ms(1000);
    while(1); // hang here while debugging
    run_firmware();
}
