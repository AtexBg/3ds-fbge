CC = arm-linux-gnueabi-gcc
ARCH = -march=armv6
CFLAGS = -static -Iinclude
#Uncomment this line to add debug warnings
#CFLAGS_MORE = -Wall -O2
SRC = $(wildcard src/*.c) $(wildcard *.c)
OBJ = $(SRC:.c=.o)

OUTPUT = fbge
OUTPUT_DIR = build
TARGET = $(OUTPUT_DIR)/$(OUTPUT)

all: $(TARGET)
	@echo "[MAKE] $(OUTPUT) compiled for ARMv6 architecture if no compiler errors shown."

$(TARGET): $(OBJ)
	@mkdir -p $(OUTPUT_DIR)
	@rm -f $(OUTPUT_DIR)/previous
	@mv -f $(TARGET) $(OUTPUT_DIR)/previous 2>/dev/null || true
	$(CC) $(ARCH) $(CFLAGS) $(CFLAGS_MORE) -o $(TARGET) $^ 

src/%.o: src/%.c
	$(CC) $(ARCH) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o src/*.o $(OUTPUT_DIR)/*

rebuild: clean all
