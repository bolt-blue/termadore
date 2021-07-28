include ../Makefile.inc

${RENDERLIB} : screen.o drawutils.o tdsignal.o
	ar -rc libtermadore.a $?

%.o: %.c
	${CC} ${CFLAGS} -c $^

.PHONY : clean
clean :
	${RM} *.o
