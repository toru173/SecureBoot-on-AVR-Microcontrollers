#
# 	Copyright 2008, 2009 Michel Pollet <buserror@gmail.com>
#
#	This file is part of simavr.
#
#	simavr is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	simavr is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with simavr.  If not, see <http://www.gnu.org/licenses/>.

# From https://www.pololu.com/docs/0J61/6.3
MCU=atmega328p
AVR_CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os
AVR_LDFLAGS=-Wl,-gc-sections -Wl,-relax
AVR_CC=avr-gcc

target = secureduino
payload = secure_payload
bootloader = secure_bootloader
bootloader_addr = 0x3C00 //
simavr = ./simavr

IPATH = .
IPATH += ${simavr}/examples/parts
IPATH += ${simavr}/include
IPATH += ${simavr}/simavr/sim

VPATH = .
VPATH += ${simavr}/examples/parts

LDFLAGS += -lpthread -lutil

all: obj payload bootloader ${target}

include ${simavr}/Makefile.common

board = ${OBJ}/${target}.elf

${board} : ${OBJ}/uart_pty.o
${board} : ${OBJ}/${target}.o

${target}: ${board}
	@echo $@ done

payload:
	${AVR_CC} ${AVR_CFLAGS} ${AVR_LDFLAGS} ${payload}.c -o ${payload}.elf
	avr-objcopy -R .eeprom -O ihex ${payload}.elf ${payload}.hex
	rm -rf ${payload}.elf

bootloader:
	${AVR_CC} ${AVR_CFLAGS} ${AVR_LDFLAGS} --section-start=.bootloader=$(bootloader_addr) ${bootloader}.c -o ${bootloader}.elf
	avr-objcopy -R .eeprom -O ihex ${bootloader}.elf ${bootloader}.hex
	rm -rf ${bootloader}.elf

simulate:
	export SIMAVR_UART_XTERM=1
	${OBJ}/${target}.elf -v ${payload}.hex

clean: clean-${OBJ}
	rm -rf *.a *.axf ${target} *.vcd *.hex *.o
