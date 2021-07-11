include ../Makefile.inc

CFLAGS += ${MATHLIB}

# Or just list demo names?
DEMOSRC = $(wildcard *.c)
DEMOS = $(patsubst %.c, %, $(DEMOSRC))

all: $(DEMOS)

.PHONY : clean
clean :
	${RM} ${DEMOS} *.o

${DEMOS} : ${RENDERLIB} ${MATHLIB}
