CC = gcc
OPT = -O1
CFLAGS = -Wall -Wextra -std=c99 $(OPT) -I include/ -DLOG_USE_COLOR 
LDFLAGS = -Wl,--stack,33554432

ifeq ($(OS), Windows_NT)
	MKDIR = @mkdir
	RM = @rmdir /s /q
	BIN = out.exe
else # Linux
	MKDIR = @mkdir -p
	RM = @rm -r
	BIN = out
endif

BIN_DIR = bin
OBJ_DIR = obj

OBJS = main.o huffman.o

all: clean build run

clean:
	-$(RM) $(BIN_DIR)
	-$(RM) $(OBJ_DIR)

build: create_dir $(BIN_DIR)/$(BIN)

create_dir:
	$(MKDIR) $(BIN_DIR)
	$(MKDIR) $(OBJ_DIR)

# Create executable.
$(BIN_DIR)/$(BIN): $(addprefix $(OBJ_DIR)/,$(OBJS))
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Create object files.
$(OBJ_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $^

run:
	./$(BIN_DIR)/$(BIN)
