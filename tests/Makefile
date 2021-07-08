include ../defaults.mk

ifeq ($(MAKELEVEL), 0)
SRCDIR := ../src
OBJDIR := ../obj
OBJ := $(wildcard $(OBJDIR)/*.o)

INC := $(SRCDIR)
INCDIRS := $(addprefix -I,$(INC))
endif

CFLAGS += $(INCDIRS)

TESTSRC = $(wildcard *.c)
TESTS = $(patsubst %.c, %, $(TESTSRC))

all: $(TESTS)

%: %.c $(OBJ)
ifndef OBJ
	@echo "Required object(s) missing; run make in parent directory"
	@exit 1
endif
	$(CC) $(CFLAGS) $(OBJ) $< -o $@ $(LDLIBS)

test-%: %
	@# noop

.PHONY: clean
clean:
ifdef ($(TESTS)
	rm $(TESTS)
endif
