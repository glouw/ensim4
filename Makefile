CC = gcc
CFLAGS = -std=c23 -O3 -march=native
WFLAGS = -Wall -Wextra -Wpedantic
LDFLAGS = -lm -lSDL3
BIN = ensim4
SRC = src/main.c

all: $(BIN)

$(BIN): $(SRC) $(GCH)
	$(CC) $(CFLAGS) $(WFLAGS) $(SRC) $(LDFLAGS) -o $@
	objdump -d -M intel $@ > $@.asm

perf: all
	perf stat -e cycles,instructions,cache-references,cache-misses,branches,branch-misses ./$(BIN)

clean:
	rm -f visualize/*.txt
	rm -f $(BIN) $(BIN).asm
