#
# rv64-emu -- Simple 64-bit RISC-V simulator
#
# Copyright (C) 2017  Leiden University, The Netherlands.
#

# Note on the compile command: we explicitly specify addresses for the
# text and data segments: some of our tests rely on it and the RISC-V
# toolchain seems to sometimes change these.

# Compile command for unit tests for compressed instructions
c.%.bin:	c.%.s
		riscv64-unknown-elf-gcc -Ttext=0x10000 -Tdata=0x11100 \
			-Wall -march=rv64imafdc -O0 \
			-nostdlib -fno-builtin -nodefaultlibs -o $@ $<

# Compile command for unit tests for regular instructions. In this case
# we explicitly specify the RISC-V architecture without 'c' extension
# to suppress the generation of compressed instructions.
%.bin:		%.s
		riscv64-unknown-elf-gcc -Ttext=0x10000 -Tdata=0x11100 \
			-Wall -march=rv64imafd -O0 \
			-nostdlib -fno-builtin -nodefaultlibs -o $@ $<
