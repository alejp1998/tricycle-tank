# Tricycle Tank — Makefile (Raspberry Pi + wiringPi)
#
# Targets:
#   make            build the game binary (Debug/piTankGo_1)
#   make debug      build with -g -O0 -DDEBUG (DPRINTF enabled)
#   make run        build and run with sudo
#   make check      syntax-check every translation unit on any host
#                   (uses tests/stubs wiringPi headers — no Pi needed)
#   make test       build + run the FSM unit tests (hardware-free)
#   make clean      remove the build directory

CC      ?= gcc
CFLAGS  ?= -Wall -Wextra -std=gnu99 -O2
LDFLAGS ?= -lwiringPi -lpthread -lrt -lm

SRCS := piTankGo_1.c fsm.c player.c ruedas.c torreta.c tmr.c xbox360.c
OBJS := $(SRCS:%.c=Debug/%.o)
TARGET := Debug/piTankGo_1

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

Debug/%.o: %.c
	@mkdir -p Debug
	$(CC) $(CFLAGS) -c -o $@ $<

debug: CFLAGS = -Wall -Wextra -std=gnu99 -O0 -g -DDEBUG
debug: all

run: all
	sudo ./$(TARGET)

# Host-side syntax check with the wiringPi stubs (no Raspberry Pi required)
# -Wno-unused-parameter: FSM callbacks receive `this` even when they don't use it
check:
	@echo "== syntax-checking all sources with wiringPi stubs =="
	$(CC) $(CFLAGS) -Wno-unused-parameter -Itests/stubs -fsyntax-only $(SRCS)
	@echo "OK"

# FSM unit tests (fsm.c / tmr.c are pure C, no wiringPi)
test:
	@echo "== building and running FSM unit tests =="
	$(CC) $(CFLAGS) -Wno-unused-parameter -I. -o /tmp/tricycle_test_fsm tests/test_fsm.c fsm.c tmr.c -lrt
	/tmp/tricycle_test_fsm

clean:
	rm -rf Debug

.PHONY: all debug run check test clean
