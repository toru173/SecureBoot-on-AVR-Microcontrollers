/* bcal_seed.h */
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
 * \file     bcal_seed.h
 * \email    bg@nerilex.org
 * \author   Daniel Otte 
 * \date     2009-01-09
 * \license  GPLv3 or later
 * 
 */

#include <avr/pgmspace.h>
#include "blockcipher_descriptor.h"
#include "seed.h"
#include "keysize_descriptor.h"

extern const bcdesc_t seed_desc;
