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

//AVRCryptoLib Encryption Library
#include "AVRCrytolib.h"

//#define PK_BYTES 32
//#define STATE_VEC_BYTES 64
//#define BLOCK_SIZE_BYTES 128

/*
 * This demonstrate how to use the avr_mcu_section.h file
 * The macro adds a section to the ELF file with useful
 * information for the simulator
 */
#include "avr_mcu_section.h"
AVR_MCU(F_CPU, "atmega328p");

#define ROM_TOP 28544

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

void get512block(uint8_t *buffer, uint16_t baseaddress)
{
    for (int i = 0; i < 64; i++)
    {
        buffer[i] = pgm_read_byte(baseaddress + i);
    }
}

void get1024block(uint8_t *buffer, uint16_t baseaddress)
{
    for (int i = 0; i < 128; i++)
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
    
    if (c)
    {
        raw_printf("\n");

        // Enter monitor

        uint8_t hash_block[64];
        uint8_t signature_block[128];

        get512block(hash_block, 0);
        
        raw_printf("Hashing beginning:\n");
        

        Sha_Init();
        
        for (int i = 0; i < ROM_TOP; i+= 64)
        {
            get512block(hash_block, i);
            Sha_Update(hash_block, sizeof(hash_block));
            raw_printf(". ");
        }
        
        Sha_Final();
        
        raw_printf("\nHashing finished!\n");
        
        raw_printf("Final Hash:\n\n");
        
        uint8_t hash[20];

        for (int i = 0; i < 5; i ++)
        {
            unsigned long word = Sha_Info.Digest[i];
            hash[(i * 4) + 0] = (word & 0xFF000000) >> 24;
            hash[(i * 4) + 1] = (word & 0x00FF0000) >> 16;
            hash[(i * 4) + 2] = (word & 0x0000FF00) >> 8;
            hash[(i * 4) + 3] = (word & 0x000000FF);
        }
        
        for (int i = 0; i < sizeof(hash); i++)
        {
            raw_printf(bytetohex(hash[i]));
        }

        raw_printf("\n");
        
        raw_printf("\nChecking Signature...\n\n");
        
        get1024block(signature_block, ROM_TOP);
        
        rsa_decrypt(sizeof(public_key), signature_block, public_exponent, public_key, rsa_s, rsa_tmp);
        
        for (int i = 108; i < sizeof(signature_block); i++)
        {
            raw_printf(bytetohex(signature_block[i]));
        }
        
        raw_printf("\n");
        
        for (int i = sizeof(signature_block); i > 108; i--)
        {
            if (signature_block[i] != hash[i - 108])
            {
                raw_printf("Invalid firmware signature. Hanging here...\n");
                while(1);
            }
        }
        
        raw_printf("Firmware signature is valid. Proceeding to boot payload\n");
        run_firmware();
    }
}
