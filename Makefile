include defaults.mk

SRCDIR := $(CURDIR)/src
OBJDIR := $(CURDIR)/obj
DEMODIR := $(CURDIR)/demos
TESTDIR := $(CURDIR)/tests

INC := $(SRCDIR)
INCDIRS := $(addprefix -I,$(INC))

CFLAGS += $(INCDIRS)

.PHONY: all
all: $(OBJ) demo test

OBJ := $(addprefix $(OBJDIR)/,$(LIBNAME).o)

.PHONY: $(LIBNAME)
$(LIBNAME): $(OBJ)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

# TODO: Can the export be done only for the demo and test rules?
export OBJ
export INCDIRS

.PHONY: demo test
demo: $(OBJ)
	cd $(DEMODIR) && $(MAKE)
demo_%: $(OBJ)
	cd $(DEMODIR) && $(MAKE) $@
test: $(OBJ)
	cd $(TESTDIR) && $(MAKE)
test-%: $(OBJ)
	cd $(TESTDIR) && $(MAKE) $@

.PHONY: clean clean-test clean-demo
clean: clean-test clean-demo
	rm -rf $(OBJDIR)

clean-demo:
	cd $(DEMODIR) && $(MAKE) clean
clean-test:
	cd $(TESTDIR) && $(MAKE) clean
