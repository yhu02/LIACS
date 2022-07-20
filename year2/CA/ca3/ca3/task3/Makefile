#
# Skeleton code for use with Computer Architecture 2021 assignment 3,
# LIACS, Leiden University.
#

CC = gcc-7

# Do not touch the "-f" options, these are here to help you not being
# misled by the compiler.
CFLAGS = -O3 -g -Wall -std=gnu99 -I../common \
	-fno-tree-vectorize -fno-unroll-loops -fno-inline
LDFLAGS = -lm
PNGFLAGS = `pkg-config --cflags --libs libpng`

# Comment these lines to disable the timing code. (macOS does not
# provide clock_gettime().
CFLAGS += -DENABLE_TIMING
LDFLAGS += -lrt

# TODO: add names of your targets here
TARGETS = \
		grayscale\
		grayscale_2\
		grayscale_4\
		grayscale_6\
		grayscale_8

all:		$(TARGETS)

%:		%.c ../common/image.c ../common/timing.c
		$(CC) $(CFLAGS) -o $@ $^ $(PNGFLAGS) $(LDFLAGS)

clean:
		rm -f $(TARGETS)
