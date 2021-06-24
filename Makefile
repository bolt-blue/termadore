BUILD="DEBUG"

CFLAGS_DEBUG = -g -O0
CFLAGS = $(CFLAGS_DEBUG) -Wall

.PHONY: all
all: test

TESTS = test_screen.test
test: $(TESTS)

%.test: %.c
	$(CC) $(CFLAGS) $< -o $*

.PHONY: clean
clean:
	rm $(TESTS)
