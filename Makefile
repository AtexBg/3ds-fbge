CC = arm-linux-gnueabi-gcc
ARCH = -march=armv6k
CFLAGS = -static -Iinclude -O3 -funroll-loops -Wall -Wextra
SRC = $(wildcard src/*.c) $(wildcard *.c)
OBJ = $(SRC:.c=.o)

OUTPUT = fbge
OUTPUT_DIR = build
TARGET = $(OUTPUT_DIR)/$(OUTPUT)

MAKEFLAGS += -j$(shell nproc) #compiles way faster with that

all: $(TARGET)
	@printf "[\033[32mMAKE\033[0m] \033[33m$(OUTPUT)\033[0m compiled for ARMv6 architecture if no compiler errors shown.\n"

$(TARGET): $(OBJ)
	@mkdir -p $(OUTPUT_DIR)
	@rm -f $(OUTPUT_DIR)/previous
	@printf "\033[32mBacking-up previous binary...\033[0m\n"
	@mv -f $(TARGET) $(OUTPUT_DIR)/previous 2>/dev/null || true
	@printf "[\033[32mLD\033[0m] Linking into $(TARGET) binary...\033[0m\n"
	@$(CC) $(ARCH) $(CFLAGS) -o $(TARGET) $^ 

src/%.o: src/%.c
	@printf "Compiling file \033[36m$<\033[0m...\n"
	@$(CC) $(ARCH) $(CFLAGS) -c $< -o $@ > /dev/null

clean:
	@printf "\033[33mCleaning compiled files...\033[0m\n"
	@rm -f *.o src/*.o $(OUTPUT_DIR)/*

rebuild:
	@make clean
	@make
