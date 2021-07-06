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

DEMODIR = demos
DEMOSRC = $(wildcard $(DEMODIR)/*.c)
DEMOS = $(patsubst $(DEMODIR)/%.c, $(DEMODIR)/%, $(DEMOSRC))
.PHONY: demo
demo: $(DEMOS)

demo_%: $(DEMODIR)/%
	# noop

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
