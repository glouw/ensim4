ENGINE ?= ENGINE_8_CYL

ALLOWED_ENGINES := ENGINE_3_CYL ENGINE_8_CYL

ifeq (,$(filter $(ENGINE),$(ALLOWED_ENGINES)))
$(error Engines available: '$(ALLOWED_ENGINES)')
endif

CC = clang

CFLAGS = -std=c23 -O3 -ffast-math -march=native
WFLAGS = -Wall -Wextra -Wpedantic
LDFLAGS = -lm -lSDL3
DFLAGS = -DENSIM4_MONITOR_REFRESH_RATE_HZ=60

ifeq ($(MAKECMDGOALS),sanitize)
CFLAGS += -fsanitize=address,undefined -g
endif

ifeq ($(MAKECMDGOALS),simd)
CFLAGS += -Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize
SIMD_FILTER = 2>&1
endif

ifeq ($(MAKECMDGOALS),visualize)
DFLAGS += -DENSIM4_VISUALIZE
endif

ifeq ($(MAKECMDGOALS),perf)
DFLAGS += -DENSIM4_PERF
endif
PERF_ARGS=cpu-cycles,instructions,L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses,branch-instructions,branch-misses,dTLB-loads,dTLB-load-misses,dTLB-stores,dTLB-store-misses,iTLB-loads,iTLB-load-misses

BIN = ensim4
SRC = src/main.c

all:
	$(CC) $(CFLAGS) $(WFLAGS) $(DFLAGS) $(SRC) $(LDFLAGS) -D$(ENGINE) -o $(BIN) $(SIMD_FILTER)
	@echo -e "\033[0;32m"
	@echo -e "Remember to set your monitor refresh rate in the Makefile"
	@echo -e "\033[0m"

vroom: all
	@./$(BIN)

sanitize: all
	@./$(BIN)

size: all
	@objdump -d -M intel $(BIN) > $(BIN).asm
	@size $(BIN)

simd: all

visualize: all
	@./$(BIN)

perf: all
	@git log -1 --pretty=format:"%H %an %ad %s" >> perf.txt;
	@perf stat -e $(PERF_ARGS) ./$(BIN) &>> perf.txt

check:
	@for eng in $(ALLOWED_ENGINES); do $(MAKE) ENGINE=$$eng; done

clean:
	rm -f visualize/*.txt
	rm -f $(BIN) $(BIN).asm
	rm -f perf.txt
