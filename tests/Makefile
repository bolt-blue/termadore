include ../Makefile.inc

CFLAGS += ${MATHLIB}

# Or just list test names?
TESTSRC = $(wildcard *.c)
TESTS = $(patsubst %.c, %, $(TESTSRC))

all: $(TESTS)

.PHONY : clean
clean :
	${RM} ${TESTS} *.o

${TESTS} : ${RENDERLIB} ${MATHLIB}
