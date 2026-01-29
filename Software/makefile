CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
SIZE=arm-none-eabi-size

JLINK_PATH ?= "C:\Program Files\SEGGER\JLink_V896"

PROJECT_ROOT=.

INCLUDE_PATHS=\
	-I$(PROJECT_ROOT)/app/Include \
	-I$(PROJECT_ROOT)/bsp/Include \
	-I$(PROJECT_ROOT)/stm32f030/core/Include \
	-I$(PROJECT_ROOT)/stm32f030/peripherals/Include \
	-I$(PROJECT_ROOT)/stm32f030/peripherals/Include/Legacy \

CFLAGS=$(INCLUDE_PATHS) -Wall -O1 -mcpu=cortex-m0 -mthumb -g3 -ffunction-sections -fdata-sections
CFLAGS+=-DSTM32F030x6 -DUSE_HAL_DRIVER -DDEBUG

LDFLAGS=-mcpu=cortex-m0 -mthumb -Wl,--gc-sections -Wl,-Map=temperature_sensor.map -T$(PROJECT_ROOT)/bsp/Loader/STM32F030F4PX_FLASH.ld

SOURCES=$(wildcard ./stm32f030/peripherals/Source/*.c) \
		$(wildcard ./stm32f030/core/Source/*.c) \
		$(wildcard ./bsp/Source/*.c) \
		$(wildcard ./app/Source/*.c)
OBJECTS=$(SOURCES:.c=.o)

STARTUP_FILE=stm32f030/core/Startup/startup_stm32f030f4px.s

all: temperature_sensor.elf
	$(SIZE) temperature_sensor.elf

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

temperature_sensor.elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(STARTUP_FILE) $^ -o $@

clean:
	rm -rf $(OBJECTS) temperature_sensor.elf temperature_sensor.map

JLINK_DEVICE=STM32F030F4
JLINK_SPEED=4000

program: temperature_sensor.elf
	"$(JLINK_PATH)\jrun.exe" -device $(JLINK_DEVICE) -speed $(JLINK_SPEED) -if SWD temperature_sensor.elf

erase:
	"$(JLINK_PATH)\jrun.exe" -device $(JLINK_DEVICE) -speed $(JLINK_SPEED) -if SWD -erase

.PHONY: program erase