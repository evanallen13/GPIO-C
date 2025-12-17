CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11
LDFLAGS := -lgpiod

SRC := $(shell find . -name "*.c")
BIN := build/main

.PHONY: all run clean

all: $(BIN)

$(BIN): $(SRC)
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(BIN) $(LDFLAGS)

run: $(BIN)
	./$(BIN)

clean:
	rm -rf build
