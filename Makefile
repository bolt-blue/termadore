BUILD ?= debug

CFLAGS_debug = -g -O0
CFLAGS_release = -O3 -DNDEBUG
CFLAGS := $(CFLAGS_$(BUILD)) -Wall
LDLIBS := -lm

OBJ := screen.o

.PHONY: all
all: $(OBJ) demo test

.PHONY: $(basename $(OBJ))
$(basename $(OBJ)): $(OBJ)

$(OBJ): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

DEMOS = demo_basic demo_ripple demo_torus
.PHONY: demo
demo: $(DEMOS)

$(DEMOS): $(OBJ) $(addsuffix .c, $(DEMOS))
	$(CC) $(CFLAGS) $(OBJ) $@.c -o $@ $(LDLIBS)

TESTDIR = tests
TESTSRC = $(wildcard $(TESTDIR)/*.c)
TESTS = $(patsubst $(TESTDIR)/%.c, $(TESTDIR)/%, $(TESTSRC))
.PHONY: test
test: $(TESTS)

test-%: $(TESTDIR)/%
	# noop

$(TESTS): $(OBJ)
	echo $(TESTS)
	$(CC) $(CFLAGS) $(OBJ) $@.c -o $@

.PHONY: clean
clean:
	rm -f $(TESTS)
	rm -f $(DEMOS)
	rm -f $(OBJ)
