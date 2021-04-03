/**
 * \file
 *
 * \brief USART Polling example
 *
 * Copyright (C) 2016 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#if __GNUC__
#include "avr/io.h"
#else
#include "ioavr.h"
#endif

#define UBRR_VAL	51
/* Prototypes */
void USART1_Init(unsigned int ubrr_val);
unsigned char USART1_Receive(void);
void USART1_Transmit(unsigned char data);


int main(void)
{
	/* Set the baudrate to 9600 bps using 8MHz internal RC oscillator */
	USART1_Init(UBRR_VAL); 

	for(;;) {
		/* Echo the received character */
		USART1_Transmit(USART1_Receive());
	}
	return 0;
}

/* Initialize UART */
void USART1_Init(unsigned int ubrr_val)
{
	/* Set the baud rate */
	UBRR1H = (unsigned char) (ubrr_val>>8);
	UBRR1L = (unsigned char) ubrr_val;
	
	/* Enable UART receiver and transmitter */
	UCSR1B = ((1<<RXEN)|(1<<TXEN));
	
	/* For devices in which UBRRH/UCSRC shares the same location
	* eg; ATmega16, URSEL should be written to 1 when writing UCSRC
	* 
	*/
	/* Set frame format: 8 data 2stop */
	UCSR1C = (1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);
}


unsigned char USART1_Receive(void)
{
	/* Wait for incomming data */
	while (!(UCSR1A & (1<<RXC)))
	;
	/* Return the data */
	return UDR1;
}

void USART1_Transmit(unsigned char data)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR1A & (1<<UDRE)))
	;
	/* Start transmission */
	UDR1 = data;
}
