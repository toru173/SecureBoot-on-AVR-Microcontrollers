/* Copyright (c) 2006, Emile van der Laan
      All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */

#ifndef _SHA_H_
#define _SHA_H_

/* Secure Hash Algorithm */
/* Useful defines & typedefs */
#include <avr/pgmspace.h>

#define SHA_BLOCKSIZE		64
#define SHA_DIGESTSIZE		20

typedef struct {
	unsigned long Data[16];		/* SHA data buffer */
	unsigned long Digest[5];		/* message digest */
	unsigned long Count_lo, Count_hi;	/* 64-bit bit count */
	int Local;			/* unprocessed amount in data */
} SHA_INFO;

extern SHA_INFO Sha_Info;

void Sha_Init();
void Sha_Update(unsigned char *, unsigned int);
void Sha_Update_P(UPGM_P , unsigned int);
void Sha_Final();

#endif /* _SHA_H_ */
