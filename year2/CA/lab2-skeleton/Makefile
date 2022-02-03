#
# rv64-emu -- Simple 64-bit RISC-V simulator
#
# Copyright (C) 2016,2019  Leiden University, The Netherlands.
#

CXX = c++

CXXFLAGS = -std=c++17 -Wall -Weffc++ -g -Og -Wno-unused-result
LDFLAGS =

OBJECTS = \
	hazard-detection-unit.o\
	control-unit.o\
	alu.o \
	config-file.o \
	elf-file.o \
	inst-decoder.o \
	inst-formatter.o \
	main.o \
	memory.o \
	memory-bus.o \
	memory-control.o \
	processor.o \
	serial.o \
	stages.o \
	sys-status.o \
	testing.o

OBJECTS_FB = framebuffer.o

HEADERS = \
	hazard-detection-unit.h\
	control-unit.h\
	alu.h \
	arch.h \
	config-file.h \
	elf-file.h \
	inst-decoder.h \
	memory.h \
	memory-bus.h \
	memory-control.h \
	memory-interface.h \
	mux.h \
	processor.h \
	reg-file.h \
	serial.h \
	stages.h \
	sys-status.h \
	testing.h

HEADERS_FB = framebuffer.h


ifdef ENABLE_FRAMEBUFFER
OBJECTS += $(OBJECTS_FB)
HEADERS += $(HEADERS_FB)

# For when the SDL2 package was installed normally
CXXFLAGS += -DENABLE_FRAMEBUFFER `pkg-config --cflags sdl2`
LDFLAGS  +=`pkg-config --libs sdl2`
endif


all:    	rv64-emu

rv64-emu:	$(OBJECTS)
		$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

%.o:		%.cc $(HEADERS)
		$(CXX) $(CXXFLAGS) -c $<

clean:
		rm -f rv64-emu
		rm -f $(OBJECTS) $(OBJECTS_FB)

check:		rv64-emu
		./test_instructions.py
