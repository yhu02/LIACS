# Compile all the test programs for CA 2020 assignment 2

# The x86 target will compile x86 versions of the main test files so you
# can check what the output is supposed to be

TARGETS = basic.bin hello.bin comp.bin brainfuck.bin \
	hello.s comp.s roman.s

SRC_DIR = src_modified/
BIN_DIR =
CRT = $(SRC_DIR)minicrt-riscv.s
HEADERS = $(wildcard $(SRC_DIR)*.h)

BIN_TARGETS = $(patsubst %.bin,$(BIN_DIR)%.bin,$(TARGETS))

CC = riscv64-unknown-elf-gcc
CFLAGS_COMMON = -Wall -nostdlib -fno-builtin -nodefaultlibs
CFLAGS = -march=rv64imafd $(CFLAGS_COMMON)


.phony: all clean x86

all:	$(TARGETS)


#
# The targets for the different levels
# The .s files are generated for level 3, in which case stalls need to
# be inserted manually.
#

# Level 0: basic.bin
basic.bin:	$(SRC_DIR)basic.s $(HEADERS)
		$(CC) $(CFLAGS) -o $@ $<

basic.s:	$(SRC_DIR)basic.c $(HEADERS)
		$(CC) $(CFLAGS) -S -o $@ $<

# Level 1: hello.bin, compiled with -O1
hello.bin:	$(SRC_DIR)hello.s $(HEADERS)
		$(CC) $(CFLAGS) -O1 -o $@ $< $(CRT)

hello.s:	$(SRC_DIR)hello.c $(HEADERS)
		$(CC) $(CFLAGS) -O1 -S -o $@ $<

# Level 2: comp.bin, compiled with -O2
comp.bin:	$(SRC_DIR)comp.s $(SRC_DIR)roman.s $(HEADERS)
		$(CC) $(CFLAGS) -O2 -o $@ $< $(SRC_DIR)roman.s $(CRT)

comp.s:		$(SRC_DIR)comp.c $(HEADERS)
		$(CC) $(CFLAGS) -O2 -S -o $@ $<

roman.s:	$(SRC_DIR)roman.c $(HEADERS)
		$(CC) $(CFLAGS) -O2 -S -o $@ $<

# Level 5: brainfuck.bin, compiled with -O2
brainfuck.bin:	$(SRC_DIR)brainfuck.s $(HEADERS)
		$(CC) $(CFLAGS) -O2 -o $@ $< $(CRT)


clean:
		rm -f $(BIN_TARGETS)
		rm -fr $(X86_DIR)

#
# x86 support
#
# Compile x86 binaries for all the test programs to see the correct output
#

X86_CC = gcc
X86_CFLAGS = -Wall -O2

# Subdirectory for output
X86_DIR = x86out
# Targets for everything but basic (can't compile RISC-V asm as x86)
X86_TARGETS = $(patsubst %.bin,$(X86_DIR)/%,$(filter-out basic.bin,$(TARGETS)))

x86: $(X86_DIR) $(X86_TARGETS)

$(X86_DIR):
		mkdir -p $(X86_DIR)

# The three examples that require roman.c
$(X86_DIR)/comp: $(SRC_DIR)comp.c $(SRC_DIR)roman.c $(HEADERS)
		$(X86_CC) $(X86_CFLAGS) -o $@ $< $(SRC_DIR)roman.c

# Generic rule for the rest
$(X86_DIR)/%: $(SRC_DIR)%.c $(HEADERS)
		$(X86_CC) $(X86_CFLAGS) -o $@ $<
