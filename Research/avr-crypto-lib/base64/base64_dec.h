/* base64_dec.h */
/*
 *   This file is part of the AVR-Crypto-Lib.
 *   Copyright (C) 2006-2015 Daniel Otte (bg@nerilex.org)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BASE64_DEC_H_
#define BASE64_DEC_H_

#include <stdint.h>

int base64_binlength(char *str, uint8_t strict);
int base64dec(void *dest, const char *b64str, uint8_t strict);

#endif /*BASE64_DEC_H_*/
