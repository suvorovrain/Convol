CC      := gcc
CFLAGS  := -Wall -Wextra -O2 -Wpedantic
LDFLAGS := -lm

SRC     := src/main.c src/utils/io.c src/utils/parse.c src/algo/algo.c src/algo/linear_convolution.c src/filters/filters.c
TARGET  := build/main

.PHONY: build clean run

build: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LDFLAGS)

clean:
	rm -rf build
