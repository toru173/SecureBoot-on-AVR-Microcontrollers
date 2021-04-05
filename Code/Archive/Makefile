# From https://www.pololu.com/docs/0J61/6.3
MCU=atmega2560
CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os
LDFLAGS=-Wl,-gc-sections -Wl,-relax
CC=avr-gcc
SIM=simavr
TARGET=hello_world_uart
OBJECT_FILES=$(TARGET).o
CLOCK=8000000

all: $(TARGET).hex

clean:
	rm -f *.o *.hex *.obj *.hex

%.hex: %.obj
	avr-objcopy -R .eeprom -O ihex $< $@

%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

sim: $(TARGET).hex
	export SIMAVR_UART_XTERM=1
	$(SIM) -f $(CLOCK) -m $(MCU) -vvv $(TARGET).hex

program: $(TARGET).hex
	avrdude -p $(MCU) -c avr109 -P $(PORT) -U flash:w:$(TARGET).hex
