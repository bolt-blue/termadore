BUILD ?= debug

CFLAGS_debug = -g -O0
CFLAGS_release = -O3 -DNDEBUG
CFLAGS := $(CFLAGS_$(BUILD)) -Wall
LDLIBS := -lm

OBJ := screen.o

.PHONY: all
all: $(OBJ) demo

.PHONY: $(basename $(OBJ))
$(basename $(OBJ)): $(OBJ)

$(OBJ): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

DEMOS = demo_screen demo_ripple
demo: $(DEMOS)

$(DEMOS): $(OBJ) $(addsuffix .c, $(DEMOS))
	$(CC) $(CFLAGS) $(OBJ) $@.c -o $@ $(LDLIBS)

.PHONY: clean
clean:
	rm -f $(DEMOS)
	rm -f $(OBJ)
