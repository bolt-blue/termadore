BUILD ?= debug

TOPDIR := ..
INCLDIR := ${TOPDIR}/src
RENDERLIB := ${INCLDIR}/libtermadore.a
MATHLIB := -lm

CFLAGS_debug = -g -O0
CFLAGS_release = -O3 -DNDEBUG
CFLAGS := $(CFLAGS_$(BUILD)) -Wall ${CFLAGS}

RM = rm -f
