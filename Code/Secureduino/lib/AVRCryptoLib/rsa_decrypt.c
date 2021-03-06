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

// By Emile van der Laan
// www.emsign.nl

#include "AVRCrytolib.h"
#include "rsa_asm.h"

void rsa_decrypt(unsigned int uiKeySize,unsigned char *pucRsa_data ,unsigned int uiExponent,unsigned char   *pucModulo,unsigned char *pucRsa_s,unsigned char *pucRsa_tmp)
{
unsigned char flag;
flag=1;
/* s = 1 */
/* a = a^e%d */
while(uiExponent)
  {
	if(uiExponent&1)
	  {/*   s=(s*a)%d; */
	   if(flag)
		  { 
	  		flag=0;
			memcpy(pucRsa_s,pucRsa_data,uiKeySize);
             /*  modulo(tmp,d,s,A_LEN,D_LEN,S_LEN);*/ /* S = A % D */
		  }
		else
		  {
		  	rsa_mull(uiKeySize,pucRsa_data,pucRsa_s,pucRsa_tmp); /* TMP = A*S   */                  
		  	rsa_modulo(uiKeySize,pucRsa_tmp,pucModulo,pucRsa_s); /* S = TMP % D */
		  }
	  }
	uiExponent>>=1;
	/* a=(a*a)%d   */
	if(uiExponent) /* A is not used is e == 0 */
	{
	  rsa_mull(uiKeySize,pucRsa_data,pucRsa_data,pucRsa_tmp); /* TMP = A * A    */
	  rsa_modulo(uiKeySize,pucRsa_tmp,pucModulo,pucRsa_data); /* A   = TMP % D  */
    }
  }
memcpy(pucRsa_data,pucRsa_s,uiKeySize);
}
