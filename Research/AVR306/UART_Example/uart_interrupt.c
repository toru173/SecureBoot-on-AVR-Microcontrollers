/**
 * \file
 *
 * \brief UART Interrupt example
 *
 * Copyright (C) 2016 Atmel Corporation. All rights reserved.
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
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#if __GNUC__
#include "avr/io.h"
#include "avr/interrupt.h"
#else
#include "ioavr.h"
#include <ina90.h>
#endif

#define UBRR_VAL	51
/* UART Buffer Defines */
#define UART_RX_BUFFER_SIZE 8     /* 2,4,8,16,32,64,128 or 256 bytes */
#define UART_TX_BUFFER_SIZE 8
#define UART_RX_BUFFER_MASK (UART_RX_BUFFER_SIZE - 1)

#if (UART_RX_BUFFER_SIZE & UART_RX_BUFFER_MASK)
	#error RX buffer size is not a power of 2
#endif

#define UART_TX_BUFFER_MASK (UART_TX_BUFFER_SIZE - 1)
#if (UART_TX_BUFFER_SIZE & UART_TX_BUFFER_MASK)
	#error TX buffer size is not a power of 2
#endif

/* Static Variables */
static unsigned char UART_RxBuf[UART_RX_BUFFER_SIZE];
static volatile unsigned char UART_RxHead;
static volatile unsigned char UART_RxTail;
static unsigned char UART_TxBuf[UART_TX_BUFFER_SIZE];
static volatile unsigned char UART_TxHead;
static volatile unsigned char UART_TxTail;

/* Prototypes */
void InitUART(unsigned char ubrr_val);
unsigned char ReceiveByte(void);
void TransmitByte(unsigned char data);

int main(void)
{
	/* Set the baudrate to 9800 bps using internal 8MHz RC Oscillator */
	InitUART(UBRR_VAL);
	
	#if __GNUC__
		sei();
	#else
		_SEI();
	#endif

	for(;;) {
		/* Echo the received character */
		TransmitByte(ReceiveByte());
	}
	return 0;
}

/* Initialize UART */
void InitUART(unsigned char ubrr_val)
{
	unsigned char x;

	/* Set the baud rate */
	UBRRH = (unsigned char)(ubrr_val>>8);
	UBRRL = (unsigned char)ubrr_val;
	/* Enable UART receiver and transmitter */
	UCSRB = ((1<<RXEN) | (1<<TXEN) | (1<<RXCIE));

	/* Flush receive buffer */
	x = 0; 			    

	UART_RxTail = x;
	UART_RxHead = x;
	UART_TxTail = x;
	UART_TxHead = x;
}

/* Interrupt handlers */
#if __GNUC__
ISR(USART_RX_vect)
#else
#pragma vector=USART_RXC_vect
__interrupt void UART_RX_interrupt(void)
#endif
{
	unsigned char data;
	unsigned char tmphead;
	
	/* Read the received data */
	data = UDR;                 
	/* Calculate buffer index */
	tmphead = (UART_RxHead + 1) & UART_RX_BUFFER_MASK;
	/* Store new index */
	UART_RxHead = tmphead;      

	if (tmphead == UART_RxTail) {
		/* ERROR! Receive buffer overflow */
	}
	/* Store received data in buffer */
	UART_RxBuf[tmphead] = data;
}

#if __GNUC__
ISR(USART_UDRE_vect)
#else
#pragma vector=USART_UDRE_vect
__interrupt void UART_TX_interrupt(void)
#endif

{
	unsigned char tmptail;

	/* Check if all data is transmitted */
	if (UART_TxHead != UART_TxTail) {
		/* Calculate buffer index */
		tmptail = ( UART_TxTail + 1 ) & UART_TX_BUFFER_MASK;
		/* Store new index */
		UART_TxTail = tmptail;      
		/* Start transmission */
		UDR = UART_TxBuf[tmptail];
	} else {
		/* Disable UDRE interrupt */
		UCSRB &= ~(1<<UDRIE);        
	}
}

unsigned char ReceiveByte(void)
{
	unsigned char tmptail;
	
	/* Wait for incoming data */
	while (UART_RxHead == UART_RxTail);
	/* Calculate buffer index */
	tmptail = (UART_RxTail + 1) & UART_RX_BUFFER_MASK;
	/* Store new index */
	UART_RxTail = tmptail;
	             
	/* Return data */
	return UART_RxBuf[tmptail];
}

void TransmitByte(unsigned char data)
{
	unsigned char tmphead;
	
	/* Calculate buffer index */
	tmphead = (UART_TxHead + 1) & UART_TX_BUFFER_MASK;
	/* Wait for free space in buffer */
	while (tmphead == UART_TxTail);
	/* Store data in buffer */
	UART_TxBuf[tmphead] = data;
	/* Store new index */
	UART_TxHead = tmphead;
	/* Enable UDRE interrupt */
	UCSRB |= (1<<UDRIE);
}

