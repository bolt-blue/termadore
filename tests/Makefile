include ../Makefile.inc

CFLAGS += ${MATHLIB}

TESTSRC = $(wildcard *.c)
TESTS = $(patsubst %.c, %, $(TESTSRC))

all : $(TESTS)

.PHONY : clean
clean :
	${RM} ${TESTS} *.o

${TESTS} : ${RENDERLIB} ${MATHLIB}
