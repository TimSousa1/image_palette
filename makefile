SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .c .h .o

CC := gcc
CFLAGS = -O3

ifdef DEBUG
	CFLAGS = -ggdb -D DEBUG
endif

CFLAGS += -Wall -Wextra

SRC := $(shell find . -name '*.c')
OBJ := $(SRC:%.c=%.o)
DEPS := $(shell find . -name '*.h')
LIBS := $(shell pkg-config --libs raylib) -lm
OUT_NAME := km

%.o : %.c $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS) $(LIBS)

$(OUT_NAME) : $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -r *.o $(OUT_NAME)
