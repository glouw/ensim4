CC = clang

CFLAGS = -std=c23 -O3 -ffast-math -march=native
WFLAGS = -Wall -Wextra -Wpedantic
LDFLAGS = -lm -lSDL3
DFLAGS = -DENSIM4_MONITOR_REFRESH_RATE_HZ=60

ifeq (0,1)
CFLAGS += -Rpass=loop-vectorize
endif

ifeq (0,1)
CFLAGS+= -fsanitize=address,undefined -g
endif

ifeq ($(MAKECMDGOALS),visualize)
DFLAGS += -DENSIM4_VISUALIZE
endif

ifeq ($(MAKECMDGOALS),perf)
DFLAGS += -DENSIM4_PERF
endif

BIN = ensim4
SRC = src/main.c

all:
	$(CC) $(CFLAGS) $(WFLAGS) $(DFLAGS) $(SRC) $(LDFLAGS) -o $(BIN)
	@echo -e "\033[0;32m"
	@echo -e "Remember to set your monitor refresh rate in the Makefile"
	@echo -e "\033[0m"

vroom: all
	@./$(BIN)

size: all
	@objdump -d -M intel $(BIN) > $(BIN).asm
	@size $(BIN)

visualize: all
	@./$(BIN)

perf: all
	@git log -1 --pretty=format:"%H %an %ad %s" >> perf.txt;
	@perf stat -e\
cpu-cycles,instructions,\
L1-dcache-loads,L1-dcache-load-misses,\
LLC-loads,LLC-load-misses,\
branch-instructions,branch-misses,\
dTLB-loads,dTLB-load-misses,\
dTLB-stores,dTLB-store-misses,\
iTLB-loads,iTLB-load-misses \
./$(BIN) \
&>> perf.txt

clean:
	rm -f visualize/*.txt
	rm -f $(BIN) $(BIN).asm
	rm -f perf.txt
