BIN = ensim4
SRC = src/main.c
CFLAGS = -std=c23 -Wall -Wextra -O3 -march=native -lm -lSDL3
CC = gcc

all:
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)
	objdump -d -M intel $(BIN) > ensim4.asm

clean:
	rm visualize/*.txt
	rm $(BIN)
