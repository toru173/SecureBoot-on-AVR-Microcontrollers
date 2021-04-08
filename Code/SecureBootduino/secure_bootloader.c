/*
 * Secure bootloader checks signature of firmware and provides for other utilities
 */

//#define DEBUG

#include <stdlib.h>

#ifndef DEBUG
#define USERAWSTRINGS //for UART comms. Saves code space
#endif

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>

#ifdef DEBUG
#include <stdio.h> // Handy for debugging but bloats code significantly
#endif

#include "mcu_defs.h"
#include "secure_bootloader.h"

// Set up MCU
#define F_CPU CLOCK
#define BAUD UART_BAUD
#include <util/setbaud.h>
#include <util/delay.h>

// Set up USART
#include "uart.h"

// Monitor
#include "monitor.h"

//AVRNaCl Encryption Library
#include "avrnacl.h"
#include "randombytes.h"

#define STATE_VEC_BYTES 64
#define BLOCK_SIZE_BYTES 128


/*
 * This demonstrate how to use the avr_mcu_section.h file
 * The macro adds a section to the ELF file with useful
 * information for the simulator
 */
#include "avr_mcu_section.h"
AVR_MCU(F_CPU, MCU);

#ifdef DEBUG
FILE uart_stdio = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);
#endif

#define MAXTEST_BYTES (1024 + BLOCK_SIZE_BYTES)

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
        
        
        long long i;
        long long j;
        
        char checksum[STATE_VEC_BYTES * 2 + 1];

        unsigned char h[STATE_VEC_BYTES+32];
        unsigned char h2[STATE_VEC_BYTES+32];
        unsigned char m[MAXTEST_BYTES+32];
        unsigned char m2[MAXTEST_BYTES+32];

        *h  += 16;
        *h2 += 16;
        *m  += 16;
        *m2 += 16;

        for (i = 0;i < MAXTEST_BYTES;++i)
        {
          crypto_uint16 hlen = STATE_VEC_BYTES;
          crypto_uint16 mlen = i;
          for (j = -16;j < 0;++j) h[j] = random();
          for (j = hlen;j < hlen + 16;++j) h[j] = random();
          for (j = -16;j < hlen + 16;++j) h2[j] = h[j];
          for (j = -16;j < 0;++j) m[j] = random();
          for (j = mlen;j < mlen + 16;++j) m[j] = random();
          for (j = -16;j < mlen + 16;++j) m2[j] = m[j];
          if (crypto_hashblocks_sha512(h,m,mlen) != mlen % BLOCK_SIZE_BYTES) my_printf("crypto_hashblocks returns wrong remaining length\n");
          for (j = -16;j < mlen + 16;++j) if (m2[j] != m[j]) my_printf("crypto_hashblocks writes to input\n");
          for (j = -16;j < 0;++j) if (h2[j] != h[j]) my_printf("crypto_hashblocks writes before output\n");
          for (j = hlen;j < hlen + 16;++j) if (h2[j] != h[j]) my_printf("crypto_hashblocks writes after output\n");
          for (j = 0;j < hlen;++j) m2[j] = h2[j];
          if (crypto_hashblocks_sha512(h2,m2,mlen) != mlen % BLOCK_SIZE_BYTES) my_printf("crypto_hashblocks returns wrong remaining length\n");
          if (crypto_hashblocks_sha512(m2,m2,mlen) != mlen % BLOCK_SIZE_BYTES) my_printf("crypto_hashblocks returns wrong remaining length\n");
          for (j = 0;j < hlen;++j) if (m2[j] != h2[j]) my_printf("crypto_hashblocks does not handle overlap\n");
          for (j = 0;j < mlen;++j) m[j] ^= h[j % hlen];
          m[mlen] = h[0];
        }
        if (crypto_hashblocks_sha512(h,m,MAXTEST_BYTES) != MAXTEST_BYTES % BLOCK_SIZE_BYTES) my_printf("crypto_hashblocks returns wrong remaining length\n");

        for (i = 0;i < STATE_VEC_BYTES;++i) {
          checksum[2 * i] = "0123456789abcdef"[15 & (h[i] >> 4)];
          checksum[2 * i + 1] = "0123456789abcdef"[15 & h[i]];
        }
        checksum[2 * i] = 0;

        my_printf("Checksum: ");
        my_printf(checksum);
        my_printf("\n");

        *h  -= 16;
        *h2 -= 16;
        *m  -= 16;
        *m2 -= 16;

        free(h);
        free(h2);
        free(m);
        free(m2);
        
        
        /*
        unsigned char hash[STATE_VEC_BYTES];
        unsigned char block[BLOCK_SIZE_BYTES];
        
        for (int i = 0; i < BLOCK_SIZE_BYTES; i++)
            block[i] = 0;

        my_printf("\n");
        my_printf(bytetohex(sizeof(block)));
        my_printf("\nHashing beginning. Success should be zero: ");
        my_printf(bytetohex((uint8_t) crypto_hashblocks_sha512(hash, block, 0)));
        my_printf("\nhashing finished! Complete hash:\n");
        
        for (int i = 0; i < 64; i++)
            my_printf(bytetohex(hash[i]));
        
        my_printf("\n");
        */
        
    }
    _delay_ms(1000);
    run_firmware();
}
