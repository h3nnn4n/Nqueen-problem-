CC=gcc
CFLAGS=-lm -Wall -Wextra --std=c11 -Ofast -fomit-frame-pointer -faggressive-loop-optimizations -fdelete-null-pointer-checks -free -fsched-critical-path-heuristic -mtune=native -march=native
#CFLAGS=-lm -Wall -Wextra --std=c11 -Og -g
SOURCES=links.c main.c set_gen.c
EXECUTABLE=dlx

OBJECTS=$(SOURCES:.c=.o)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -c -o $@

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
