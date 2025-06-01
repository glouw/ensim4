CC = gcc
CFLAGS = -std=c23 -Ofast -march=native
ifeq (0,1)
CFLAGS+= -fsanitize=address,undefined -g
endif
WFLAGS = -Wall -Wextra -Wpedantic
LDFLAGS = -lm -lSDL3
BIN = ensim4
SRC = src/main.c

all:
	$(CC) $(CFLAGS) $(WFLAGS) $(SRC) $(LDFLAGS) -o $(BIN)
	objdump -d -M intel $(BIN) > $(BIN).asm
	size $(BIN)

perf: all
	perf stat -e\
cpu-cycles,instructions,\
L1-dcache-loads,L1-dcache-load-misses,\
LLC-loads,LLC-load-misses,\
branch-instructions,branch-misses,\
dTLB-loads,dTLB-load-misses,\
dTLB-stores,dTLB-store-misses,\
iTLB-loads,iTLB-load-misses,\
alignment-faults,\
context-switches,\
alignment-faults \
./$(BIN) 360

  #branch-instructions OR branches                    [Hardware event]
  #branch-misses                                      [Hardware event]
  #bus-cycles                                         [Hardware event]
  #cache-misses                                       [Hardware event]
  #cache-references                                   [Hardware event]
  #cpu-cycles OR cycles                               [Hardware event]
  #instructions                                       [Hardware event]
  #ref-cycles                                         [Hardware event]
  #alignment-faults                                   [Software event]
  #bpf-output                                         [Software event]
  #cgroup-switches                                    [Software event]
  #context-switches OR cs                             [Software event]
  #cpu-clock                                          [Software event]
  #cpu-migrations OR migrations                       [Software event]
  #dummy                                              [Software event]
  #emulation-faults                                   [Software event]
  #major-faults                                       [Software event]
  #minor-faults                                       [Software event]
  #page-faults OR faults                              [Software event]
  #task-clock                                         [Software event]


clean:
	rm -f visualize/*.txt
	rm -f $(BIN) $(BIN).asm
