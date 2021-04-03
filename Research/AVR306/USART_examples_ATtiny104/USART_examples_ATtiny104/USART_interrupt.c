/**
 * \file
 *
 * \brief USART Interrupt example
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
 
#if __GNUC__
#include "avr/io.h"
#include "avr/interrupt.h"
#else
#include "ioavr.h"
#endif

volatile unsigned char received_data;
void USART0_Init( unsigned int baudrate );
void USART0_Transmit( unsigned char data );
volatile int flag = 0;

/* Main - a simple test program*/
void main( void )
{
	/* Set the baudrate to 2400 bps using a 1 MHz  */
	USART0_Init(25 );   

    /* Enable interrupts => enable UART interrupts */
	sei();          

	while(1){
				
		if(flag){
			
			flag = 0;
			USART0_Transmit(received_data);
		}
	}
}

/* Initialize USART */
void USART0_Init( unsigned int baudrate )
{
	/* Set the baud rate */
	UBRRH = (unsigned char) (baudrate>>8);
	UBRRL = (unsigned char) baudrate;
	
	UCSRB = ( ( 1 << RXCIE ) | ( 1 << RXEN ) | ( 1 << TXEN ) );
	
	/* Set frame format: 8 data 2stop */
	UCSRC = (1<<USBS)|(1<<UCSZ0)|(1<<UCSZ1);              
	
}

/* Interrupt handlers */

ISR (USART_RXC_vect) 
{
	received_data = UDR;
	flag = 1;	
}


void USART0_Transmit( unsigned char data )
{
	UDR = data;
}

