CC=gcc
CFLAGS=-lm -Wall -Wextra --std=c11
SOURCES=links.c main.c set_gen.c
BIN=dlx

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(BIN)

clean:
	-@rm -rf $(BIN) core 2> /dev/null || true
