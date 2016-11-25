CC=gcc
CFLAGS=-lm -Wall -Wno-unused-but-set-variable -g
SOURCES=links.c main.c
BIN=dlx

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(BIN)

clean:
	-@rm -rf $(BIN) core 2> /dev/null || true
