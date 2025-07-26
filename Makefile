CC = clang

CFLAGS = -std=c23 -O3 -ffast-math -march=native

ifeq (0,1)
CFLAGS += -Rpass=loop-vectorize
endif

ifeq (0,1)
CFLAGS+= -fsanitize=address,undefined -g
endif

WFLAGS = -Wall -Wextra -Wpedantic
LDFLAGS = -lm -lSDL3
BIN = ensim4
SRC = src/main.c

MONITOR_REFRESH_RATE_HZ = 60

all:
	$(CC) $(CFLAGS) $(WFLAGS) $(SRC) $(LDFLAGS) -DENSIM4_MONITOR_REFRESH_RATE_HZ=$(MONITOR_REFRESH_RATE_HZ) -o $(BIN)
	@echo -e "\033[0;32m"
	@echo -e "Remember to set your monitor refresh rate in the Makefile"
	@echo -e "\033[0m"

check: all
	objdump -d -M intel $(BIN) > $(BIN).asm
	size $(BIN)

vroom: all
	./$(BIN)

perf: all
	perf stat -e\
cpu-cycles,instructions,\
L1-dcache-loads,L1-dcache-load-misses,\
LLC-loads,LLC-load-misses,\
branch-instructions,branch-misses,\
dTLB-loads,dTLB-load-misses,\
dTLB-stores,dTLB-store-misses,\
iTLB-loads,iTLB-load-misses \
./$(BIN) 360 \
&>>perf.txt

clean:
	rm -f visualize/*.txt
	rm -f $(BIN) $(BIN).asm
	rm -f perf.txt
