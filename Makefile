BUILD="DEBUG"

CFLAGS_DEBUG = -g -O0
CFLAGS = $(CFLAGS_DEBUG) -Wall

.PHONY: all
all: test

TESTS = test_screen.test test_ripple.test
test: $(TESTS)

%.test: %.c
	$(CC) $(CFLAGS) $< -o $* -lm

.PHONY: clean
clean:
	rm $(TESTS)
