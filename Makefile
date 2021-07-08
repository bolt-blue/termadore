BUILD ?= debug

CFLAGS_debug = -g -O0
CFLAGS_release = -O3 -DNDEBUG
CFLAGS := $(CFLAGS_$(BUILD)) -Wall
LDLIBS := -lm

SRCDIR := src
SRCS := $(wildcard $(SRCDIR)/*.c)

INCDIRS := $(SRCDIR)
INC := $(addprefix -I,$(INCDIRS))

OBJ := $(patsubst $(SRCDIR)/%.c,$(SRCDIR)/%.o, $(SRCS))

.PHONY: all
all: $(OBJ) demo test

.PHONY: $(notdir $(basename $(OBJ)))
$(notdir $(basename $(OBJ))): $(OBJ)

$(OBJ): %.o: %.c %.h
	$(CC) -c $(CFLAGS) $< -o $@

DEMODIR = demos
DEMOSRC = $(wildcard $(DEMODIR)/*.c)
DEMOS = $(patsubst $(DEMODIR)/%.c,$(DEMODIR)/%, $(DEMOSRC))
.PHONY: demo
demo: $(DEMOS)

demo_%: $(DEMODIR)/%
	

TESTDIR = tests
TESTSRC = $(wildcard $(TESTDIR)/*.c)
TESTS = $(patsubst $(TESTDIR)/%.c,$(TESTDIR)/%, $(TESTSRC))
.PHONY: test
test: $(TESTS)

test_%: $(TESTDIR)/%
	

%: %.c $(OBJ)
	$(CC) $(CFLAGS) $(INC) $(OBJ) $< -o $@ $(LDLIBS)

.PHONY: clean
clean:
	rm -f $(TESTS)
	rm -f $(DEMOS)
	rm -f $(OBJ)
