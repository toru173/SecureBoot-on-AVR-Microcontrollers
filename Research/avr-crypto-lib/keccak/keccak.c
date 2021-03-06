/* keecak.c */
/*
    This file is part of the AVR-Crypto-Lib.
    Copyright (C) 2006-2015 Daniel Otte (bg@nerilex.org)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "memxor.h"
#include "rotate64.h"
#include "keccak.h"

#ifdef DEBUG
#  undef DEBUG
#endif

#define DEBUG 0

#if DEBUG
#include "cli.h"

void keccak_dump_state(uint64_t a[5][5]){
	uint8_t i,j;
	for(i=0; i<5; ++i){
		cli_putstr_P(PSTR("\r\n"));
		cli_putc('0'+i);
		cli_putstr_P(PSTR(": "));
		for(j=0; j<5; ++j){
			cli_hexdump_rev(&(a[i][j]), 8);
			cli_putc(' ');
		}
	}
}

void keccak_dump_ctx(keccak_ctx_t *ctx){
	keccak_dump_state(ctx->a);
	cli_putstr_P(PSTR("\r\nDBG: r: "));
	cli_hexdump_rev(&(ctx->r), 2);
	cli_putstr_P(PSTR("\t c: "));
	cli_hexdump_rev(&(ctx->c), 2);
	cli_putstr_P(PSTR("\t d: "));
	cli_hexdump(&(ctx->d), 1);
	cli_putstr_P(PSTR("\t bs: "));
	cli_hexdump(&(ctx->bs), 1);
}

#endif

/*
const uint64_t rc[] PROGMEM = {
       0x0000000000000001LL, 0x0000000000008082LL,
       0x800000000000808ALL, 0x8000000080008000LL,
       0x000000000000808BLL, 0x0000000080000001LL,
       0x8000000080008081LL, 0x8000000000008009LL,
       0x000000000000008ALL, 0x0000000000000088LL,
       0x0000000080008009LL, 0x000000008000000ALL,
       0x000000008000808BLL, 0x800000000000008BLL,
       0x8000000000008089LL, 0x8000000000008003LL,
       0x8000000000008002LL, 0x8000000000000080LL,
       0x000000000000800ALL, 0x800000008000000ALL,
       0x8000000080008081LL, 0x8000000000008080LL,
       0x0000000080000001LL, 0x8000000080008008LL
};
*/

const static uint8_t rc_comp[] PROGMEM = {
		0x01, 0x92, 0xda, 0x70,
		0x9b, 0x21, 0xf1, 0x59,
		0x8a, 0x88, 0x39, 0x2a,
		0xbb, 0xcb, 0xd9, 0x53,
		0x52, 0xc0, 0x1a, 0x6a,
		0xf1, 0xd0, 0x21, 0x78,
};

#define RP_IDX(i, j) ((((2 * j + 3 * i) % 5) * 5 + i) * 8)

uint8_t const rho_pi_idx_table[5][5] PROGMEM = {
        { RP_IDX(0, 0), RP_IDX(0, 1), RP_IDX(0, 2), RP_IDX(0, 3), RP_IDX(0, 4) },
        { RP_IDX(1, 0), RP_IDX(1, 1), RP_IDX(1, 2), RP_IDX(1, 3), RP_IDX(1, 4) },
        { RP_IDX(2, 0), RP_IDX(2, 1), RP_IDX(2, 2), RP_IDX(2, 3), RP_IDX(2, 4) },
        { RP_IDX(3, 0), RP_IDX(3, 1), RP_IDX(3, 2), RP_IDX(3, 3), RP_IDX(3, 4) },
        { RP_IDX(4, 0), RP_IDX(4, 1), RP_IDX(4, 2), RP_IDX(4, 3), RP_IDX(4, 4) }
};

#define ROT_BIT(a) (( (a) <= 4) ? ((a) << 1) : (0x01 | ((8 - (a)) << 1)))
#define ROT_CODE(a) ((((a) / 8 + ((((a) % 8) > 4) ? 1 : 0)) << 4) | ROT_BIT(((a) % 8)))

const uint8_t keccak_rotate_codes[5][5] PROGMEM = {
        { ROT_CODE( 0), ROT_CODE( 1), ROT_CODE(62), ROT_CODE(28), ROT_CODE(27) },
        { ROT_CODE(36), ROT_CODE(44), ROT_CODE( 6), ROT_CODE(55), ROT_CODE(20) },
        { ROT_CODE( 3), ROT_CODE(10), ROT_CODE(43), ROT_CODE(25), ROT_CODE(39) },
        { ROT_CODE(41), ROT_CODE(45), ROT_CODE(15), ROT_CODE(21), ROT_CODE( 8) },
        { ROT_CODE(18), ROT_CODE( 2), ROT_CODE(61), ROT_CODE(56), ROT_CODE(14) }
};


static inline
void keccak_round(uint64_t *a, uint8_t rci){
	uint64_t b[5][5];
	uint8_t i, j;
	union {
			uint64_t v64;
			uint8_t v8[8];
		} t;
    const uint8_t *rot_code = (const uint8_t*)keccak_rotate_codes;
    const uint8_t *idx_idx = (const uint8_t*)rho_pi_idx_table;
    uint64_t *a_tmp = (uint64_t*)a;
	/* theta */
	for(i = 0; i < 5; ++i){
		b[i][0] = a[i] ^ a[5 + i] ^ a[10 + i] ^ a[15 + i] ^ a[20 + i];
	}
 	for(i = 0; i < 5; ++i){
		t.v64 = b[(4 + i) % 5][0] ^ rotate64_1bit_left(b[(i + 1) % 5][0]);
		for(j = 0; j < 5; ++j){
			a[j * 5 + i] ^= t.v64;
		}
	}
#if DEBUG
	cli_putstr_P(PSTR("\r\nAfter theta:"));
	keccak_dump_state(a);
#endif
	/* rho & pi */
    for(i = 0; i < 25; ++i){
            *((uint64_t*)(((uint8_t*)b) + pgm_read_byte(idx_idx++))) =
                rotate64left_code(*a_tmp++, pgm_read_byte(rot_code++));

    }
#if DEBUG
	cli_putstr_P(PSTR("\r\n--- after rho & pi ---"));
	keccak_dump_state(a);
#endif
	/* chi */
	for(i = 0; i < 5; ++i){
		for(j = 0; j < 5; ++j){
			a[j * 5 + i] =  b[j][i] ^ ((~(b[j][(i + 1) % 5])) & (b[j][(i + 2) % 5]));
		}
	}

#if DEBUG
	cli_putstr_P(PSTR("\r\nAfter chi:"));
	keccak_dump_state(a);
#endif
	/* iota */

//	memcpy_P(&t, &(rc_comp[rci]), 8);
	t.v64 = 0;
	t.v8[0] = pgm_read_byte(&(rc_comp[rci]));
	if(t.v8[0] & 0x40){
		t.v8[7] = 0x80;
	}
	if(t.v8[0] & 0x20){
		t.v8[3] = 0x80;
	}
	if(t.v8[0] & 0x10){
		t.v8[1] = 0x80;
	}
	t.v8[0] &= 0x8F;

	a[0] ^= t.v64;
#if DEBUG
	cli_putstr_P(PSTR("\r\nAfter iota:"));
	keccak_dump_state(a);
#endif
}

void keccak_f1600(void *a){
	uint8_t i = 0;
	do {
#if DEBUG
		cli_putstr_P(PSTR("\r\n\r\n--- Round "));
		cli_hexdump(&i, 1);
		cli_putstr_P(PSTR(" ---"));
#endif
		keccak_round((uint64_t*)a, i);
	} while (++i < 24);
}

void keccak_nextBlock(keccak_ctx_t *ctx, const void *block){
	memxor(ctx->a, block, ctx->bs);
	keccak_f1600(ctx->a);
}

void keccak_lastBlock(keccak_ctx_t *ctx, const void *block, uint16_t length_b){
    uint8_t length_B;
    uint8_t t;
    while(length_b >= ctx->r){
        keccak_nextBlock(ctx, block);
        block = (uint8_t*)block + ctx->bs;
        length_b -=  ctx->r;
    }
    length_B = length_b / 8;
    memxor(ctx->a, block, length_B);
    /* append 1 */
    if(length_b & 7){
        /* we have some single bits */
        t = ((uint8_t*)block)[length_B] >> (8 - (length_b & 7));
        t |= 0x01 << (length_b & 7);;
    }else{
        t = 0x01;
    }
    ctx->a[length_B] ^= t;
    if(length_b == ctx->r - 1){
        keccak_f1600(ctx->a);
    }
    ctx->a[ctx->bs - 1] ^= 0x80;
    keccak_f1600(ctx->a);
}

void keccak_ctx2hash(void *dest, uint16_t length_b, keccak_ctx_t *ctx){
	while(length_b >= ctx->r){
		memcpy(dest, ctx->a, ctx->bs);
		dest = (uint8_t*)dest + ctx->bs;
		length_b -= ctx->r;
		keccak_f1600(ctx->a);
	}
	memcpy(dest, ctx->a, (length_b+7)/8);
}

void keccak224_ctx2hash(void *dest, keccak_ctx_t *ctx){
	keccak_ctx2hash(dest, 224, ctx);
}

void keccak256_ctx2hash(void *dest, keccak_ctx_t *ctx){
	keccak_ctx2hash(dest, 256, ctx);
}

void keccak384_ctx2hash(void *dest, keccak_ctx_t *ctx){
	keccak_ctx2hash(dest, 384, ctx);
}

void keccak512_ctx2hash(void *dest, keccak_ctx_t *ctx){
	keccak_ctx2hash(dest, 512, ctx);
}

/*
  1. SHA3-224: ???Keccak[r = 1152, c =  448, d = 28]???224
  2. SHA3-256: ???Keccak[r = 1088, c =  512, d = 32]???256
  3. SHA3-384: ???Keccak[r =  832, c =  768, d = 48]???384
  4. SHA3-512: ???Keccak[r =  576, c = 1024, d = 64]???512
*/
void keccak_init(uint16_t r, keccak_ctx_t *ctx){
	memset(ctx->a, 0x00, 5 * 5 * 8);
	ctx->r = r;
	ctx->bs = (uint8_t)(r / 8);
}

void keccak224_init(keccak_ctx_t *ctx){
	keccak_init(1152, ctx);
}

void keccak256_init(keccak_ctx_t *ctx){
	keccak_init(1088, ctx);
}

void keccak384_init(keccak_ctx_t *ctx){
	keccak_init( 832, ctx);
}

void keccak512_init(keccak_ctx_t *ctx){
	keccak_init( 576, ctx);
}
