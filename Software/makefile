CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
SIZE=arm-none-eabi-size

PROJECT_ROOT=.
BUILD_DIR=$(PROJECT_ROOT)/_Build

INCLUDE_PATHS=\
	-I$(PROJECT_ROOT)/app/Include \
	-I$(PROJECT_ROOT)/bsp/Include \
	-I$(PROJECT_ROOT)/stm32f030/core/Include \
	-I$(PROJECT_ROOT)/stm32f030/peripherals/Include \
	-I$(PROJECT_ROOT)/stm32f030/peripherals/Include/Legacy \

SOURCES=\
	$(wildcard ./app/Source/*.c) \
	$(wildcard ./bsp/Source/*.c) \
	$(wildcard ./stm32f030/core/Source/*.c) \
	$(wildcard ./stm32f030/peripherals/Source/*.c)

OBJECTS_PATH=_Build

CFLAGS=$(INCLUDE_PATHS) -Wall -Os -mcpu=cortex-m0 -mthumb -g3 -ffunction-sections -fdata-sections -DSTM32F030x6 -DUSE_HAL_DRIVER -DDEBUG
LDFLAGS=-mcpu=cortex-m0 -mthumb -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/temperature_sensor.map -T$(PROJECT_ROOT)/bsp/Loader/STM32F030F4PX_FLASH.ld

OBJECTS=$(patsubst %.c,$(OBJECTS_PATH)/%.o,$(SOURCES))

STARTUP_FILE=stm32f030\core\Startup\startup_stm32f030f4px.s


# When make or make all is called, first create the build folder if it does ot exist, then build all .o files in the Build folder, finally link them to crteate the final .elf and .map files
all: $(BUILD_DIR) $(OBJECTS) $(BUILD_DIR)/temperature_sensor.elf

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/temperature_sensor.elf: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(STARTUP_FILE) -o $@
	$(SIZE) $@

# Compile each .c file to .o file in the Build folder
$(OBJECTS_PATH)/%.o: %.c
	@if not exist "$(dir $@)" mkdir "$(subst /,\,$(dir $@))"
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(subst /,\,$(BUILD_DIR))"
	@mkdir "$(subst /,\,$(BUILD_DIR))"
