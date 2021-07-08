include ../defaults.mk

ifeq ($(MAKELEVEL), 0)
SRCDIR := ../src
OBJDIR := ../obj
OBJ := $(wildcard $(OBJDIR)/*.o)

INC := $(SRCDIR)
INCDIRS := $(addprefix -I,$(INC))
endif

CFLAGS += $(INCDIRS)

DEMOSRC = $(wildcard *.c)
DEMOS = $(patsubst %.c, %, $(DEMOSRC))

all: $(DEMOS)

%: %.c $(OBJ)
ifndef OBJ
	@echo "Required object(s) missing; run make in parent directory"
	@exit 1
endif
	$(CC) $(CFLAGS) $(OBJ) $< -o $@ $(LDLIBS)

demo_%: %
	@# noop

.PHONY: clean
clean:
ifdef ($(TESTS)
	rm $(DEMOS)
endif
