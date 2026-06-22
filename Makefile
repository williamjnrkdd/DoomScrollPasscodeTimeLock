CC=gcc
CFLAGS=-Wall -Wextra -g

SRC=$(wildcard src/*.c)
OBJ=$(SRC:src/%.c=build/%.o)

OUT=build/app

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(OBJ) -o $(OUT)

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build