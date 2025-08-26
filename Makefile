CC      := gcc
CFLAGS  := -Wall -Wextra -O2 -Wpedantic
LDFLAGS := -lm -fopenmp

SRC     := $(wildcard *.c)
TARGET  := build/convol

.PHONY: build clean run

build: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LDFLAGS)

clean:
	rm -rf build

format:
	clang-format -i $(SRC) *.h

format-check:
	clang-format --dry-run --Werror $(SRC) *.h

lint:
	clang-tidy $(SRC) -- $(CFLAGS)