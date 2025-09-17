CC      := gcc
CFLAGS  := -Wall -Wextra -O2 -Wpedantic
LDFLAGS := -lm -fopenmp

SRC := $(shell find src -name '*.c' ! -name 'main.c')
APP_SRC := $(SRC) src/main.c
HDR := $(shell find src -name '*.h')
TARGET  := build/convol

TEST_SRC := $(shell find tests/unit -name '*.c')
TEST_BIN := build/tests

.PHONY: build clean fmt fmt-check lint test

build: $(TARGET)

$(TARGET): $(APP_SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(TEST_BIN): $(SRC) $(TEST_SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $(SRC) $(TEST_SRC) -o $@ $(LDFLAGS) -lcmocka -pthread

test: $(TEST_BIN)
	@./$(TEST_BIN)

clean:
	rm -rf build

fmt:
	clang-format -i $(SRC) $(APP_SRC) $(HDR) $(TEST_SRC)

fmt-check:
	clang-format --dry-run --Werror $(SRC) $(APP_SRC) $(HDR) $(TEST_SRC)

lint:
	clang-tidy $(SRC) $(APP_SRC) $(TEST_SRC) -- $(CFLAGS)

	