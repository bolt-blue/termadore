SRCDIR := src
DEMODIR := demos
TESTDIR := tests

all :
	@${MAKE} -C ${SRCDIR}
	@${MAKE} -C ${DEMODIR}
	@${MAKE} -C ${TESTDIR}

.PHONY : clean
clean :
	@${MAKE} -C ${SRCDIR} clean
	@${MAKE} -C ${DEMODIR} clean
	@${MAKE} -C ${TESTDIR} clean
