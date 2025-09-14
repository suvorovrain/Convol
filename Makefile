CC      := gcc
CFLAGS  := -Wall -Wextra -O2 -Wpedantic
LDFLAGS := -lm -fopenmp

SRC := $(shell find src -name '*.c') $(wildcard *.c)
HDR := $(shell find src -name '*.c') $(wildcard *.h)
TARGET  := build/convol

.PHONY: build clean run

build: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LDFLAGS)

clean:
	rm -rf build

fmt:
	clang-format -i $(SRC) $(HDR)

fmt-check:
	clang-format --dry-run --Werror $(SRC) $(HDR)

lint:
	clang-tidy $(SRC) -- $(CFLAGS)
	