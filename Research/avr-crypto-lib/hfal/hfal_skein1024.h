/* hfal_skein1024.h */
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
/**
 * \file     hfal_skein1024.h
 * \email    bg@nerilex.org
 * \author   Daniel Otte 
 * \date     2009-03-13
 * \license  GPLv3 or later
 * 
 */

#ifndef HFAL_SKEIN1024_H_
#define HFAL_SKEIN1024_H_

#include <avr/pgmspace.h>
#include "hashfunction_descriptor.h"

extern const hfdesc_t skein1024_128_desc;
extern const hfdesc_t skein1024_160_desc;
extern const hfdesc_t skein1024_224_desc;
extern const hfdesc_t skein1024_256_desc;
extern const hfdesc_t skein1024_384_desc;
extern const hfdesc_t skein1024_512_desc;
extern const hfdesc_t skein1024_1024_desc;

#endif /* HFAL_SHA1024_H_ */
