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

target = secureduino
payload = secure_payload
payload_size := $$(stat -c '%s' ${payload}.hex)
trailing_bytes = 13
bootloader = secure_bootloader
MCU=atmega328p
bootloader_addr = 0x7000 # Start of BLS on the Atmega2560 in BYTES
simavr = ./simavr
includedir = ./include

INCLUDE_FILES = $(wildcard ${includedir}/*.c)

# From https://www.pololu.com/docs/0J61/6.3
AVR_CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os
AVR_LDFLAGS=-Wl,-gc-sections -Wl,-relax -I${includedir}
AVR_BOOTLOADER_LDFLAGS=-Wl,--section-start=.text=${bootloader_addr}
AVR_CC=avr-gcc

IPATH = .
IPATH += ${simavr}/examples/parts
IPATH += ${simavr}/include
IPATH += ${simavr}/simavr/sim

VPATH = .
VPATH += ${includedir}
VPATH += ${simavr}/examples/parts

LDFLAGS += -lpthread -lutil

all: obj binary ${target}

include ${simavr}/Makefile.common

board = ${OBJ}/${target}.elf

${board} : ${OBJ}/uart_pty.o
${board} : ${OBJ}/${target}.o

${target}: ${board}
	@echo $@ done

payload:
	@echo ${INCLUDE_FILES}
	@${AVR_CC} ${AVR_CFLAGS} ${AVR_LDFLAGS} ${payload}.c ${INCLUDE_FILES} -o ${payload}.elf
	@avr-objcopy -R .eeprom -O ihex ${payload}.elf ${payload}.hex
	avr-size ${payload}.hex

bootloader:
	@${AVR_CC} ${AVR_CFLAGS} ${AVR_LDFLAGS} ${AVR_BOOTLOADER_LDFLAGS} ${bootloader}.c ${INCLUDE_FILES} -o ${bootloader}.elf
	@avr-objcopy -j .text -j .data -O ihex ${bootloader}.elf ${bootloader}.hex
	avr-size ${bootloader}.hex
	
# Hex merging from https://www.avrfreaks.net/forum/merging-bootloader-hex-file-and-application-hex-file-one
# simduino doesn't like an Intel Hex file with multiple segments (it will only load the first)
# Below hack to get everything into a single .hex file with one segment. Leave BIN behind for debug

binary: payload bootloader
	@cat ${payload}.hex | awk '/^:00000001FF/ == 0' > binary.hex
	@cat ${bootloader}.hex >> binary.hex
	@avr-objcopy --gap-fill 0xFF -I ihex -O binary binary.hex binary.bin
	@avr-objcopy -I binary -O ihex binary.bin binary.hex
	
debug:
	@export SIMAVR_UART_XTERM=1
	${OBJ}/${target}.elf -d binary.hex

simulate:
	@export SIMAVR_UART_XTERM=1
	${OBJ}/${target}.elf -v binary.hex

clean: clean-${OBJ}
	rm -rf *.a *.axf ${target} *.vcd *.hex *.o *.bin *.elf
