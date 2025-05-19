CC = gcc
CFLAGS = -std=c23 -Ofast -march=native
WFLAGS = -Wall -Wextra
LDFLAGS = -lm -lSDL3
BIN = ensim4
SRC = src/main.c

all:
	$(CC) $(CFLAGS) $(WFLAGS) $(SRC) $(LDFLAGS) -o $(BIN)
	objdump -d -M intel $(BIN) > ensim4.asm

perf: all
	perf stat -e cycles,instructions,cache-references,cache-misses,branches,branch-misses ./$(BIN)

clean:
	rm -f visualize/*.txt
	rm -f $(BIN)
	rm -f $(BIN).asm
