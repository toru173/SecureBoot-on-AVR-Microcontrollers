/* scal-nessie.h */
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

#ifndef SCALNESSIE_H_
#define SCALNESSIE_H_

#include <stdint.h>
#include <streamcipher_descriptor.h>

void scal_nessie_set_estream(uint8_t v);
uint8_t scal_nessie_get_estream(void);

void scal_nessie_stream_run(const scdesc_t *desc, uint16_t keysize_b, uint16_t ivsize_b);
void scal_nessie_run(const scdesc_t *desc);

#endif /* SCALNESSIE_H_ */
