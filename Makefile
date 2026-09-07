CC=gcc
CFLAGS=-Wall -Wextra -g -Iinclude -Ivendor
LIBS = -lcurl

VENDOR=$(wildcard vendor/*/*.c)
VENDOR_OBJ=$(VENDOR:vendor/%/%.c=build/%.o)

# APP
SRC=$(wildcard src/*.c)
C_FILES=$(SRC) $(VENDOR)

SRC_OBJ=$(SRC:src/%.c=build/%.o)
OBJ_FILES=$(filter-out %prime_number_generator.o, $(SRC_OBJ)) $(VENDOR_OBJ)

APP_OUT=build/app

all: $(APP_OUT)

$(APP_OUT): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(APP_OUT) $(LIBS)

# TEST
TESTS=$(wildcard tests/*.c)
TESTS_C_FILES=src/logger.c src/compute.c src/http_time_service.c src/main.c src/time_conversion.c $(TESTS) $(VENDOR)

TESTS_OBJ=$(TESTS:tests/%.c=build/%.o)
TESTS_OBJ_FILES=build/logger.o build/compute.o build/http_time_service.o build/main.o build/time_conversion.o $(TESTS_OBJ) $(VENDOR_OBJ)

TESTS_OUT=build/test

test: $(TESTS_OUT)

$(TESTS_OUT): $(TESTS_OBJ_FILES)
	$(CC) $(CFLAGS) $(TESTS_OBJ_FILES) -o $(TESTS_OUT) $(LIBS)

# Prime Number Generator
PNG_FILES=src/primeNumberGenerator.c

PNG_OBJ_FILES=build/primeNumberGenerator.o

PNG_OUT=build/png

png: $(PNG_OUT)

$(PNG_OUT): $(PNG_OBJ_FILES)
	$(CC) $(CFLAGS) $(PNG_OBJ_FILES) -o $(PNG_OUT)

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: tests/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: vendor/%/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build