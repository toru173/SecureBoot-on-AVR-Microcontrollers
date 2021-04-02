/**
 * \file
 *
 * \brief USART driver
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
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

#include "usart_io.h"

/**
 * \defgroup usart_driver_group USART driver
 *
 * The USART driver is used to send or receive characters using USART0.
 *
 * @{
 */

/** \brief Function to get a char from the USART if it is ready.
 *
 * \retval UDR0     Returns USART0 data register
 */
uint8_t usart0_getchar(void)
{
	// Wait for incoming data
	while (!(UCSR0A & (1 << RXC0)))
		;

	// Return the read data
	return UDR0;
}

/** \brief Function to transmit a char using USART0 if it is ready.
 *
 * \param c     Variable used to transmit a character using USART0
 *
 */
#if defined(__GNUC__)
int usart0_putchar(char c, FILE *stream)
{
	UCSR0A |= (1 << TXC0);
	if (c == '\n')
		usart0_putchar('\r', stream);
	// Wait for empty transmit buffer
	while (!(UCSR1A & (1 << UDRE1)))
		;
	// Start transmission
	UDR0 = c;
	// Wait for transmit complete
	while (!(UCSR0A & (1 << TXC0)))
		;
	// Clear TXC flag
	UCSR0A |= 1 << TXC0;

	return 0;
}
#elif defined(__IAR_SYSTEMS_ICC__)
int fputc(int c, FILE *f)
{
	// Wait for empty transmit buffer
	while (!(UCSR0A & (1 << UDRE0)))
		;
	// Put data into buffer, sends the data
	UDR0 = (char)c;
	// Wait for transmit complete
	while (!(UCSR0A & (1 << TXC0)))
		;
	// Clear TXC flag
	UCSR0A |= 1 << TXC0;

	return c;
}
#else
#error Unsupported compiler
#endif
//! @}
