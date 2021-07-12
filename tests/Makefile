include ../Makefile.inc

CFLAGS += ${MATHLIB} -I${INCLDIR}

TESTSRC = $(wildcard *.c)
TESTS = $(patsubst %.c, %, $(TESTSRC))

all : $(TESTS)

.PHONY : clean
clean :
	${RM} ${TESTS} *.o

${TESTS} : ${RENDERLIB} ${MATHLIB}
