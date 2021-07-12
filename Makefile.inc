BUILD ?= debug

TOPDIR := ..
INCLDIR := ${TOPDIR}/src
RENDERLIB := ${INCLDIR}/screen.o
MATHLIB := -lm

CFLAGS_debug = -g -O0
CFLAGS_release = -O3 -DNDEBUG
CFLAGS := $(CFLAGS_$(BUILD)) -Wall

RM = rm -f
