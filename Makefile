BUILD ?= debug

CFLAGS_debug = -g -O0
CFLAGS_release = -O3 -DNDEBUG
CFLAGS := $(CFLAGS_$(BUILD)) -Wall
LDLIBS := -lm

OBJ := screen.o

.PHONY: all
all: $(OBJ) test

.PHONY: $(basename $(OBJ))
$(basename $(OBJ)): $(OBJ)

$(OBJ): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

TESTS = test_screen test_ripple
test: $(TESTS)

$(TESTS): $(OBJ) $(addsuffix .c, $(TESTS))
	$(CC) $(CFLAGS) $(OBJ) $@.c -o $@ $(LDLIBS)

.PHONY: clean
clean:
	rm -f $(TESTS)
	rm -f $(OBJ)
