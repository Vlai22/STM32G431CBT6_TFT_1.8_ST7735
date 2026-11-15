# Тулчейн (исправлена опечатка eadi -> eabi)
GCC = arm-none-eabi-gcc
G++ = arm-none-eabi-g++

LIB = -I./inc \
      -ID:/Projects/Embedded/SCR_LIBS/stm32g4xx-hal-driver-1.2.7/Inc/ \
      -ID:/Projects/Embedded/SCR_LIBS/cmsis-device-g4-1.2.6/Include/ \
	  -ID:/Projects/Embedded/CMSIS/CMSIS_6/CMSIS/Core/Include/

# Разделяем флаги: общие для железа и специфичные для C++
MCU_FLAGS = -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb -DSTM32G431xx -O0
CXX_FLAGS = -std=c++20 -fno-exceptions

LDFLAGS   = -T STM32G431F8P6_FLASH.ld --specs=nosys.specs

# Ищем исходники
SRCS_CPP = $(wildcard src/*.cpp)
SRCS_C   = $(wildcard src/*.c)
SRCS_ASM = $(wildcard src/*.s)

# Превращаем в объектные файлы (исправлена ошибка с SRCS_S -> SRCS_ASM)
OBJS = $(SRCS_CPP:.cpp=.o) $(SRCS_C:.c=.o) $(SRCS_ASM:.s=.o)

all: tft.bin

# Шаг 3: Конвертация в BIN
tft.bin: tft.elf
	arm-none-eabi-objcopy -O binary $< $@

# Шаг 2: Линковка
tft.elf: $(OBJS)
	$(G++) $(OBJS) $(MCU_FLAGS) $(CXX_FLAGS) $(LDFLAGS) -o $@

# Шаг 1: Компиляция C++ файлов (исправлена опечатка %o -> %.o)
%.o: %.cpp
	$(G++) $(MCU_FLAGS) $(CXX_FLAGS) $(LIB) -c $< -o $@

# Компиляция Си и Ассемблера (без флагов C++)
%.o: %.c
	$(GCC) $(MCU_FLAGS) $(LIB) -c $< -o $@
%.o: %.s
	$(GCC) $(MCU_FLAGS) $(LIB) -c $< -o $@

clean:
	rm -f $(shell find . -name "*.o") *.elf *.bin