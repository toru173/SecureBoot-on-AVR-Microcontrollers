/* bigint_io.h */
/*
    This file is part of the ARM-Crypto-Lib.
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

#ifndef BIGINT_IO_H_
#define BIGINT_IO_H_

#include "bigint2.h"

void    bigint_print_hex(const bigint_t *a);
uint8_t bigint_read_hex_echo(bigint_t *a, bigint_length_t length);

#endif /* BIGINT_IO_H_ */
